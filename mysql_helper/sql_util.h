#ifndef __SQLUTIL_H__
#define __SQLUTIL_H__


#include <mysql/mysql.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "logger.h"

using namespace std;

namespace SqlUtil
{

class Field
{
public:
	vector < string > m_name;
	vector < enum_field_types > m_type;

public:
	Field ();
       ~Field ();

       bool IsNum (int num);
       bool IsNum (string num);
       bool IsDate (int num);
       bool IsDate (string num);
       bool IsChar (int num);
       bool IsChar (string num);
       bool IsBlob (int num);
       bool IsBlob (string num);
       int GetField_NO (string field_name);

public:
	DECL_LOGGER(logger);
};

class RecordSet
{
public:
	int m_affected_rows;
	uint64_t m_insert_id;

public:
	bool m_init_field;
	unsigned long *m_lengths;
	int m_field_num;
	Field m_field;
	MYSQL_RES *res;
	MYSQL_ROW row;

public:
	RecordSet ();
	~RecordSet ();

	int FreeResult();

	int InitFields();

	MYSQL_ROW FetchRow();

	int GetField(const string& name, string& value)
	{
		int k=m_field.GetField_NO(name);
		if( k<0 ) return -1;

		value=std::string(row[k], m_lengths[k]);
		return 0;
	}

	template<typename T>
	int GetField(const string& name, T& value)
	{
		string str_value;
		GetField(name, str_value);

		stringstream ss;
		ss << str_value;
		ss >> value;

		return 0;
	}

	int GetAffectedRows();

public:
	DECL_LOGGER(logger);
};


class DataBase
{
public:DataBase ();
       ~DataBase ();

       int Init(const string& host, 
		       int port, 
		       const string& user, 
		       const string& password, 
		       const string& dbname,
		       int retry_times = 3,
               int retry_interval = 1);

       int init_mysql();

private:
       MYSQL * m_Data;

       string m_host;
       string m_user;
       string m_password;
       int m_port;
       string m_dbname;

       bool m_connected;

       int m_retry_times;
       int m_retry_interval;
public:

       MYSQL * GetMysql ();

       int Connect ();
	   int ConnectMysql();
       void DisConnect();
       int ExecuteSql(const char *SQL, uint64_t sql_len,RecordSet& recordset);
	   int ExecuteSql(const std::string& db_name, const char *SQL, uint64_t sql_len,RecordSet& recordset);
	   int MysqlErrno();
       string MysqlError();
       bool ErrDup();

public:
       static string EscapeString(string& in);

public:
	DECL_LOGGER(logger);
};




};

#endif 

