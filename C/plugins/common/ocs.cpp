/*
 * Fledge OSI Soft OCS integration.
 *
 * Copyright (c) 2020 Dianomic Systems
 *
 * Released under the Apache 2.0 Licence
 *
 * Author: Stefano Simonelli
 */

#include <string>
#include <vector>
#include <utility>

#include <ocs.h>
#include <string_utils.h>
#include <logger.h>
#include <simple_https.h>
#include <rapidjson/document.h>
#include "rapidjson/error/en.h"

using namespace std;
using namespace rapidjson;

OCS::OCS()
{
}

// Destructor
OCS::~OCS()
{
}

/**
 * Extracts the OCS token from the JSON returned by the OCS api
 */
std::string OCS::extractToken(const string& response)
{
	Document JSon;
	string token;

	ParseResult ok = JSon.Parse(response.c_str());
	if (!ok)
	{
		Logger::getLogger()->error("OCS token extract, invalid json - HTTP response :%s:", response.c_str());
	}
	else
	{
		if (JSon.HasMember("access_token"))
		{
			token = JSon["access_token"].GetString();
		}
	}

	return(token);
}

/**
 * Calls the OCS api to retrieve the authentication token related to the the clientId and clientSecret
 */
std::string OCS::retrieveToken(const string& clientId, const string& clientSecret)
{
	string token;
	string response;
	string payload;

	HttpSender *endPoint;
	vector<pair<string, string>> header;
	int httpCode;

	endPoint = new SimpleHttps(OCS_HOST,
							   TIMEOUT_CONNECT,
							   TIMEOUT_REQUEST,
							   RETRY_SLEEP_TIME,
							   MAX_RETRY);

	header.push_back( std::make_pair("Content-Type", "application/x-www-form-urlencoded"));
	header.push_back( std::make_pair("Accept", " text/plain"));

	payload =  PAYLOAD_RETRIEVE_TOKEN;

	StringReplace(payload, "CLIENT_ID_PLACEHOLDER",        clientId);
	StringReplace(payload, "CLIENT_SECRET_ID_PLACEHOLDER", clientSecret);

	// Anonymous auth
	string authMethod = "a";
	endPoint->setAuthMethod (authMethod);

	try
	{
		httpCode = endPoint->sendRequest("POST",
										 URL_RETRIEVE_TOKEN,
										 header,
										 payload);

		response = endPoint->getHTTPResponse();

		if (httpCode >= 200 && httpCode <= 399)
		{
			token = extractToken(response);
			Logger::getLogger()->debug("OCS authentication token :%s:" ,token.c_str() );
		}
		else
		{
			Logger::getLogger()->warn("Error in retriving the authentication token from OCS - http :%d: :%s: ", httpCode, response.c_str());
		}

	}
	catch (exception &ex)
	{
		Logger::getLogger()->warn("Error in retriving the authentication token from OCS - error :%s: ", ex.what());
	}

	delete endPoint;

	return token;
}