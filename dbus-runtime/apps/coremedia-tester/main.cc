/*
 * main.cc
 *
 *  Created on: Nov 20, 2015
 *      Author: buttonfly
 */


#include <string>
#include "MediaPlayer.h"
#include "webservice.h"
#include "osl.h"
#include "Media.h"
#include "httpservice.h"

using namespace k;

const char *buildDate() {
    return __DATE__ " " __TIME__;
}

int main(int argc, char *argv[]) {
	fprintf(stderr,
        "%s (C)2001-2014, Kaonmedia \n"
    "  Built on %s, BCHP_CHIP \n", argv[0], buildDate());


#define WWW_ROOT  "/tmp/www"
#define SERVICE_PORT   "8089"

	const char *port = (argv[1]) ? argv[1] : SERVICE_PORT;

	k::webservice _web;
	const char *options[] = {
			"listening_port", port,
			"document_root",WWW_ROOT,
			NULL };

	_web.start(options);

	// add plugin
	Media media;
	_web.add((k::httpservice*) &media);
	_web.wait();

	return 0;
}
