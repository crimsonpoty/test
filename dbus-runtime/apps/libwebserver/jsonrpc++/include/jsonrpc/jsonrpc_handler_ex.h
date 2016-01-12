/*
 * jsonrpc_handler_ex.h
 *
 *  Created on: Jun 19, 2013
 *      Author: buttonfly
 */

#ifndef JSONRPC_HANDLER_EX_H_
#define JSONRPC_HANDLER_EX_H_

#include <cstdio>

#include <string>
#include <list>

#include <json/json.h>

#include "jsonrpc_common.h"

using namespace Json::Rpc;

namespace k {

class CallbackMethodEx {
public:

	virtual ~CallbackMethodEx();

	/**
	 * \param arg connection handle to precess asynchronously.
	 */
	virtual bool Call(const Json::Value& msg, Json::Value& response, void* arg) = 0;

	virtual std::string GetName() const = 0;

	virtual Json::Value GetDescription() const = 0;

};


template<class T> class RpcMethodEx : public CallbackMethodEx
{
  public:
    /**
     * \typedef Method
     * \brief T method signature.
     */
    typedef bool (T::*MethodEx)(const Json::Value& msg, Json::Value& response, void* arg0);

    /**
     * \brief Constructor.
     * \param obj object
     * \param method class method
     * \param name symbolic name (i.e. system.describe)
     * \param description method description (in JSON format)
     */
    RpcMethodEx(T& obj, MethodEx method, const std::string& name, const Json::Value description = Json::Value::null)
    {
      m_obj = &obj;
      m_name = name;
      m_method = method;
      m_description = description;
    }

    /**
     * \brief Call the method.
     * \param msg JSON-RPC request or notification
     * \param response response produced (may be Json::Value::null)
     * \return true if message has been correctly processed, false otherwise
     * \note JSON-RPC's notification method MUST set response to Json::Value::null
     */
    virtual bool Call(const Json::Value& msg, Json::Value& response, void* arg0)
    {
      return (m_obj->*m_method)(msg, response, arg0);
    }

    /**
     * \brief Get the name of the methods (optional).
     * \return name of the method as std::string
     */
    virtual std::string GetName() const
    {
      return m_name;
    }

    /**
     * \brief Get the description of the methods (optional).
     * \return description
     */
    virtual Json::Value GetDescription() const
    {
      return m_description;
    }

  private:
    /**
     * \brief Object pointer.
     */
    T* m_obj;

    /**
     * \brief Method of T class.
     */
    MethodEx m_method;

    /**
     * \brief Symbolic name.
     */
    std::string m_name;

    /**
     * \brief JSON-formated description of the RPC method.
     */
    Json::Value m_description;
};



class HandlerEx
{
  public:
    /**
     * \brief Constructor.
     */
    HandlerEx();

    /**
     * \brief Destructor.
     */
    virtual ~HandlerEx();

    /**
     * \brief Add a new RPC method.
     * \param method RPC method to add (MUST be dynamically allocated using new)
     * \note Json::Rpc::Handler object takes care of freeing method memory.\n
     * The way of calling this method is:
     * <code>
     * handler.AddMethod(new RpcMethod<MyClass>(...));
     * </code>
     * \warning The "method" parameter MUST be dynamically allocated (using new).
     */
    void AddMethod(CallbackMethodEx* method);

    /**
     * \brief Remote a RPC method.
     * \param name name of the RPC method
     */
    void DeleteMethod(const std::string& name);

    /**
     * \brief Process a JSON-RPC message.
     * \param msg JSON-RPC message as std::string
     * \param response JSON-RPC response (could be Json::Value::null)
     * \return true if the request has been correctly processed, false otherwise
     * (may be caused by parsed error, ...)
     * \note in case msg is a notification, response is equal to Json::Value::null
     * and the return value is true.
     */
    bool Process(const std::string& msg, Json::Value& response, void* arg0);

    /**
     * \brief Process a JSON-RPC message.
     * \param msg JSON-RPC message as C-String
     * \param response JSON-RPC response (could be Json::Value::null)
     * \return true if the request has been correctly processed, false otherwise (may be
     * caused by parsed error, ...)
     * \note in case msg is a notification, response is equal to Json::Value::null
     * and the return value is true.
     */
    bool Process(const char* msg, Json::Value& response, void* arg0);

    /**
     * \brief RPC method that get all the RPC methods and their description.
     * \param msg request
     * \param response response
     * \return true if processed correctly, false otherwise
     */
    bool SystemDescribe(const Json::Value& msg, Json::Value& response, void* arg0);

    /**
     * \brief Get a std::string representation of Json::Value.
     * \param value JSON message
     * \return string representation
     */
    std::string GetString(Json::Value value);

  private:
    /**
     * \brief JSON reader.
     */
    Json::Reader m_reader;

    /**
     * \brief JSON writer.
     */
    Json::FastWriter m_writer;

    /**
     * \brief List of RPC methods.
     */
    std::list<CallbackMethodEx*> m_methods;

    /**
     * \brief Find CallbackMethod by name.
     * \param name name of the CallbackMethod
     * \return a CallbackMethod pointer if found, 0 otherwise
     */
    CallbackMethodEx* Lookup(const std::string& name) const;

    /**
     * \brief Check if the message is a valid JSON object one.
     * \param root message to check validity
     * \param error complete JSON-RPC error message if method failed
     * \return true if the message is a JSON one, false otherwise
     */
    bool Check(const Json::Value& root, Json::Value& error);

    /**
     * \brief Process a JSON-RPC object message.
     * \param root JSON-RPC message as Json::Value
     * \param response JSON-RPC response that will be filled in this method
     * \return true if the request has been correctly processed, false otherwise
     * (may be caused by parsed error, ...)
     * \note In case msg is a notification, response is equal to Json::Value::null
     * and the return value is true.
     */
    bool Process(const Json::Value& root, Json::Value& response, void* arg0);
};









} /* namespace k */
#endif /* JSONRPC_HANDLER_EX_H_ */

