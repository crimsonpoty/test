/*
 * jsonrpc_handler_ex.cpp
 *
 *  Created on: Jun 19, 2013
 *      Author: buttonfly
 */

#include "jsonrpc_handler_ex.h"

namespace k {

CallbackMethodEx::~CallbackMethodEx()
 {
 }

 HandlerEx::HandlerEx()
 {
   /* add a RPC method that list the actual RPC methods contained in the Handler */
   Json::Value root;

   root["description"] = "List the RPC methods available";
   root["parameters"] = Json::Value::null;
   root["returns"] = "Object that contains description of all methods registered";

   AddMethod(new RpcMethodEx<HandlerEx>(*this, &HandlerEx::SystemDescribe, std::string("system.describe"), root));
 }

 HandlerEx::~HandlerEx()
 {
   /* delete all objects from the list */
   for(std::list<CallbackMethodEx*>::const_iterator it = m_methods.begin() ; it != m_methods.end() ; it++)
   {
     delete (*it);
   }
   m_methods.clear();
 }

 void HandlerEx::AddMethod(CallbackMethodEx* method)
 {
   m_methods.push_back(method);
 }

 void HandlerEx::DeleteMethod(const std::string& name)
 {
   /* do not delete system defined method */
   if(name == "system.describe")
   {
     return;
   }

   for(std::list<CallbackMethodEx*>::iterator it = m_methods.begin() ; it != m_methods.end() ; it++)
   {
     if((*it)->GetName() == name)
     {
       delete (*it);
       m_methods.erase(it);
       break;
     }
   }
 }

 bool HandlerEx::SystemDescribe(const Json::Value& msg, Json::Value& response, void* arg0)
 {
   Json::Value methods;
   response["jsonrpc"] = "2.0";
   response["id"] = msg["id"];

   for(std::list<CallbackMethodEx*>::iterator it = m_methods.begin() ; it != m_methods.end() ; it++)
   {
     methods[(*it)->GetName()] = (*it)->GetDescription();
   }

   response["result"] = methods;
   return true;
 }

 std::string HandlerEx::GetString(Json::Value value)
 {
   return m_writer.write(value);
 }

 bool HandlerEx::Check(const Json::Value& root, Json::Value& error)
 {
   Json::Value err;

   /* check the JSON-RPC version => 2.0 */
   if(!root.isObject() || !root.isMember("jsonrpc") || root["jsonrpc"] != "2.0")
   {
     error["id"] = Json::Value::null;
     error["jsonrpc"] = "2.0";

     err["code"] = INVALID_REQUEST;
     err["message"] = "Invalid JSON-RPC request.";
     error["error"] = err;
     return false;
   }

   if(root.isMember("id") && (root["id"].isArray() || root["id"].isObject()))
   {
     error["id"] = Json::Value::null;
     error["jsonrpc"] = "2.0";

     err["code"] = INVALID_REQUEST;
     err["message"] = "Invalid JSON-RPC request.";
     error["error"] = err;
     return false;
   }

   /* extract "method" attribute */
   if(!root.isMember("method") || !root["method"].isString())
   {
     error["id"] = Json::Value::null;
     error["jsonrpc"] = "2.0";

     err["code"] = INVALID_REQUEST;
     err["message"] = "Invalid JSON-RPC request.";
     error["error"] = err;
     return false;
   }

   return true;
 }

 bool HandlerEx::Process(const Json::Value& root, Json::Value& response, void* arg0)
 {
   Json::Value error;
   std::string method;

   if(!Check(root, error))
   {
     response = error;
     return false;
   }

   method = root["method"].asString();

   if(method != "")
   {
     CallbackMethodEx* rpc = Lookup(method);
     if(rpc)
     {
       return rpc->Call(root, response, arg0);
     }
   }

   /* forge an error response */
   response["id"] = root.isMember("id") ? root["id"] : Json::Value::null;
   response["jsonrpc"] = "2.0";

   error["code"] = METHOD_NOT_FOUND;
   error["message"] = "Method not found.";
   response["error"] = error;

   return false;
 }

 bool HandlerEx::Process(const std::string& msg, Json::Value& response, void* arg0)
 {
   Json::Value root;
   Json::Value error;
   bool parsing = false;

   /* parsing */
   parsing = m_reader.parse(msg, root);

   if(!parsing)
   {
     /* request or batched call is not in JSON format */
     response["id"] = Json::Value::null;
     response["jsonrpc"] = "2.0";

     error["code"] = PARSING_ERROR;
     error["message"] = "Parse error.";
     response["error"] = error;
     return false;
   }

   if(root.isArray())
   {
     /* batched call */
     Json::Value::ArrayIndex i = 0;
     Json::Value::ArrayIndex j = 0;

     for(i = 0 ; i < root.size() ; i++)
     {
       Json::Value ret;
       Process(root[i], ret, arg0);

       if(ret != Json::Value::null)
       {
         /* it is not a notification, add to array of responses */
         response[j] = ret;
         j++;
       }
     }
     return true;
   }
   else
   {
     return Process(root, response, arg0);
   }
 }

 bool HandlerEx::Process(const char* msg, Json::Value& response, void* arg0)
 {
   std::string str(msg);

   return Process(str, response, arg0);
 }

 CallbackMethodEx* HandlerEx::Lookup(const std::string& name) const
 {
   for(std::list<CallbackMethodEx*>::const_iterator it = m_methods.begin() ; it != m_methods.end() ; it++)
   {
     if((*it)->GetName() == name)
     {
       return (*it);
     }
   }

   return 0;
 }

} /* namespace k */
