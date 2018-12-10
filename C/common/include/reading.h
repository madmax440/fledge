#ifndef _READING_H
#define _READING_H
/*
 * FogLAMP storage client.
 *
 * Copyright (c) 2018 OSisoft, LLC
 *
 * Released under the Apache 2.0 Licence
 *
 * Author: Mark Riddoch, Massimiliano Pinto
 */
#include <datapoint.h>
#include <string>
#include <ctime>
#include <vector>

#define DEFAULT_DATE_TIME_FORMAT      "%Y-%m-%d %H:%M:%S"
#define COMBINED_DATE_STANDARD_FORMAT "%Y-%m-%dT%H:%M:%S"
#define ISO8601_DATE_TIME_FORMAT      "%Y-%m-%d %H:%M:%S +0000"
#define DATE_TIME_BUFFER_LEN          52

/**
 * An asset reading represented as a class.
 *
 * Each asset reading may have multiple datapoints to represent the
 * multiple values that maybe held within a complex asset.
 */
class Reading {
	public:
		Reading(const std::string& asset, Datapoint *value);
		Reading(const std::string& asset, std::vector<Datapoint *> values);
		Reading(const std::string& asset, std::vector<Datapoint *> values, const std::string& ts);
		Reading(const Reading& orig);

		~Reading();
		void				addDatapoint(Datapoint *value);
		std::string			toJSON() const;
		// Return AssetName
		const std::string&              getAssetName() const { return m_asset; };
		// Set AssetName
		void				setAssetName(std::string assetName) { m_asset = assetName; };
		int				getDatapointCount() { return m_values.size(); };
		void				removeAllDatapoints();
		// Return UUID
		const std::string&              getUuid() const { return m_uuid; };
		// Return Reading datapoints
		const std::vector<Datapoint *>	getReadingData() const { return m_values; };
		// Return refrerence to Reading datapoints
		std::vector<Datapoint *>&	getReadingData() { return m_values; };
		unsigned long			getId() const { return m_id; };
		unsigned long			getTimestamp() const { return (unsigned long)m_timestamp.tv_sec; };
		unsigned long			getUserTimestamp() const { return (unsigned long)m_userTimestamp.tv_sec; };
		void				setId(unsigned long id) { m_id = id; };
		void				setUuid(const std::string& uuid) { m_uuid = uuid; };
		void				setTimestamp(unsigned long ts) { m_timestamp.tv_sec = (time_t)ts; };
		void				setUserTimestamp(unsigned long uTs) { m_userTimestamp.tv_sec = (time_t)uTs; };
		void				getUserTimestamp(struct timeval *tm) { *tm = m_userTimestamp; };
		void				getTimestamp(struct timeval *tm) { *tm = m_timestamp; };

		typedef enum dateTimeFormat { FMT_DEFAULT, FMT_STANDARD, FMT_ISO8601 } readingTimeFormat;

		// Return Reading asset time - ts time
		const std::string getAssetDateTime(readingTimeFormat datetimeFmt = FMT_DEFAULT, bool addMs = true) const;
		// Return Reading asset time - user_ts time
		const std::string getAssetDateUserTime(readingTimeFormat datetimeFmt = FMT_DEFAULT, bool addMs = true) const;

	protected:
		Reading() {};
		unsigned long			m_id;
		bool				m_has_id;
		std::string			m_asset;
		struct timeval			m_timestamp;
		struct timeval			m_userTimestamp;
		std::vector<Datapoint *>	m_values;
		std::string			m_uuid;
		// Supported date time formats for 'm_timestamp'
		static std::vector<std::string>	m_dateTypes;
};
#endif

