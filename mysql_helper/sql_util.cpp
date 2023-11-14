#include <assert.h>
#include <cstring>
#include <cstdlib>
#include <mysql/errmsg.h>
#include <mysql/mysqld_error.h>
#include "sql_util.h"

namespace SqlUtil
{
IMPL_LOGGER(Field, logger);
IMPL_LOGGER(RecordSet, logger);
IMPL_LOGGER(DataBase, logger);

/*-------------------------------------------------------Field---------------------------------------------------------*/
Field::Field ()
{
}
Field::~Field ()
{
}
bool Field::IsNum (int num)
{
	if (IS_NUM (m_type[num]))
		return true;
	else
		return false;
}
bool Field::IsNum (string num)
{
	if (IS_NUM (m_type[GetField_NO (num)]))
		return true;
	else
		return false;
}
bool Field::IsDate (int num)
{
	if (FIELD_TYPE_DATE == m_type[num] || FIELD_TYPE_DATETIME == m_type[num])
		return true;
	else
		return false;
}
bool Field::IsDate (string num)
{
	int temp;
	temp = GetField_NO (num);
	if (FIELD_TYPE_DATE == m_type[temp] ||
			FIELD_TYPE_DATETIME == m_type[temp])
		return true;
	else
		return false;
}
bool Field::IsChar (int num)
{
	if (m_type[num] == FIELD_TYPE_STRING ||
			m_type[num] == FIELD_TYPE_VAR_STRING ||
			m_type[num] == FIELD_TYPE_CHAR)
		return true;
	else
		return false;

}
bool Field::IsChar (string num)
{
	int temp;
	temp = this->GetField_NO (num);
	if (m_type[temp] == FIELD_TYPE_STRING ||
			m_type[temp] == FIELD_TYPE_VAR_STRING ||
			m_type[temp] == FIELD_TYPE_CHAR)
		return true;
	else
		return false;
}
bool Field::IsBlob (int num)
{
	if (IS_BLOB (m_type[num]))
		return true;
	else
		return false;
}
bool Field::IsBlob (string num)
{
	if (IS_BLOB (m_type[GetField_NO (num)]))
		return true;
	else
		return false;
}
int Field::GetField_NO (string field_name)
{
	for (unsigned int i = 0; i < m_name.size (); i++)
	{
		if (!m_name[i].compare (field_name))
			return i;

	}
	return -1;
}

/*------------------------------------------RecordSet--------------------------------------------------*/
RecordSet::RecordSet ()
{
	res = NULL;
	row = NULL;
	m_lengths=NULL;

	m_init_field=false;
}
RecordSet::~RecordSet ()
{
	FreeResult();
}

int RecordSet::FreeResult()
{
	if(res!=NULL) 
	{
		mysql_free_result(res);
	}

	res=NULL;
	row=NULL;
	m_lengths=NULL;
	m_init_field=false;

	return 0;
}

int RecordSet::InitFields()
{
	if( m_init_field ) return 0;

	m_field.m_name.clear ();
	m_field.m_type.clear ();

	m_field_num = mysql_num_fields (res);

	MYSQL_FIELD *field;
	for (int x = 0; ( field = mysql_fetch_field (res)) != NULL; x++)
	{
		m_field.m_name.push_back (field->name);
		m_field.m_type.push_back (field->type);
	}

	m_init_field=true;

	return 0;
}

MYSQL_ROW RecordSet::FetchRow()
{
	InitFields();
	row = mysql_fetch_row (res);
	m_lengths = mysql_fetch_lengths(res);
	return row;
}

int RecordSet::GetAffectedRows() 
{
	return m_affected_rows;
}


/*----------------------------------------------------DataBase----------------------------------------------*/
DataBase::DataBase ()
{
	m_Data = NULL;
	m_connected=false;
}
DataBase::~DataBase ()
{
	DisConnect ();
}
int DataBase::Init(const string& host, 
		int port, 
		const string& user, 
		const string& password, 
		const string& dbname,
		int retry_times,
        int retry_interval)
{
	m_host=host;
	m_port=port;
	m_user=user;
	m_password=password;
	m_dbname=dbname;
    m_retry_times=retry_times;
    m_retry_interval=retry_interval;
	return 0;
}

int DataBase::init_mysql()
{
	if( !m_Data )
		m_Data = mysql_init (NULL);

	if(!m_Data)
		return -1;

	return 0;
}

MYSQL *DataBase::GetMysql ()
{
	return m_Data;
}

int DataBase::Connect ()
{
	if(m_connected) 
		return 0;

	if( init_mysql()<0 )
		return -1;

	if ( mysql_real_connect (m_Data, m_host.c_str (),
				m_user.c_str (), m_password.c_str (),
				m_dbname.c_str (), m_port, NULL, 0) )
	{
		m_connected=true;
		if (mysql_select_db (m_Data, m_dbname.c_str ()) != 0)
		{
			DisConnect();
			return -1;
		}

		LOG4CPLUS_TRACE(logger, "mysql_real_connect success. "
                                                    <<"{"<<m_host
                                                    <<", "<<m_user
                                                    <<", "<<m_password
                                                    <<", "<<m_dbname
						<<"}");
	}
	else
	{
		LOG4CPLUS_ERROR(logger, "mysql_real_connect error. "
						<<"{"<<m_host
						<<", "<<m_user
						<<", "<<m_password
						<<", "<<m_dbname
						<<"}");
		m_connected=false;
		mysql_close (m_Data);
		m_Data=NULL;
		
		return -1;
	}
	
	return 0;
}

int DataBase::ConnectMysql()
{
	if(m_connected) 
		return 0;

	if( init_mysql()<0 )
		return -1;

	if ( mysql_real_connect (m_Data, m_host.c_str (),
				m_user.c_str (), m_password.c_str (),
				"", m_port, NULL, 0) )
	{
		m_connected=true;
		LOG4CPLUS_TRACE(logger, "mysql_real_connect success. "
                                                    <<"{"<<m_host
                                                    <<", "<<m_user
                                                    <<", "<<m_password
													<<"}");
	}
	else
	{
		LOG4CPLUS_ERROR(logger, "mysql_real_connect error. "
						<<"{"<<m_host
						<<", "<<m_user
						<<", "<<m_password
						<<"}");
		m_connected=false;
		mysql_close (m_Data);
		m_Data=NULL;
		
		return -1;
	}
	
	return 0;
}

void DataBase::DisConnect()
{
	if( !m_connected ) 
		return;

	mysql_close (m_Data);

	m_Data=NULL;

	m_connected=false;

}

int DataBase::ExecuteSql(const char *SQL, 
		uint64_t sql_len,
		RecordSet& recordset)
{
	LOG4CPLUS_TRACE(logger, SQL);
    
	for(int n = 0; n < m_retry_times; n++)
	{
		if ( Connect()!=0 )
		{
			DisConnect();
			::sleep(m_retry_interval);
			continue;
		}

		int rv = mysql_real_query (m_Data, SQL, sql_len);
		if( rv==0 )
		{
			recordset.m_affected_rows = mysql_affected_rows(m_Data);
			recordset.m_insert_id = mysql_insert_id(m_Data);
			recordset.res = mysql_store_result (m_Data);
			return 0;
		}

		LOG4CPLUS_ERROR( logger, "mysql_read_query error. "
				<<" sql: " << SQL << "."
				<<" retry: "<<n << "."
				<<" errno: "<<mysql_errno(m_Data) << "."
				<<" error: "<<mysql_error(m_Data) << "." );

		switch ( mysql_errno(m_Data) )
		{
			case CR_SERVER_GONE_ERROR:
			case CR_SERVER_LOST:
				DisConnect();
				::sleep(m_retry_interval);
				continue;

			default:
				return -1;
		}
	}

	LOG4CPLUS_ERROR(logger, "mysql_read_query error. "
			<<"retry_times: "<<m_retry_times
			<<"retry_interval: "<<m_retry_interval
			<<"sql: " << SQL)
	return -1;
}

int DataBase::ExecuteSql(const std::string& db_name
				, const char *SQL
				, uint64_t sql_len
				, RecordSet& recordset)
{
	LOG4CPLUS_TRACE(logger, SQL);
    
	for(int n = 0; n < m_retry_times; n++)
	{
		if ( ConnectMysql()!=0 )
		{
			DisConnect();
			::sleep(m_retry_interval);
			continue;
		}

		if (mysql_select_db (m_Data, db_name.c_str ()) != 0)
		{
			LOG4CPLUS_ERROR(logger, "mysql select db error: "<<db_name
				                    <<", errno: "<<mysql_errno(m_Data)
				                    <<", error: "<<mysql_error(m_Data));
			DisConnect();
			::sleep(m_retry_interval);
			continue;
		}

		int rv = mysql_real_query (m_Data, SQL, sql_len);
		if( rv==0 )
		{
			recordset.m_affected_rows = mysql_affected_rows(m_Data);
			recordset.m_insert_id = mysql_insert_id(m_Data);
			recordset.res = mysql_store_result (m_Data);
			return 0;
		}

		LOG4CPLUS_ERROR( logger, "mysql_read_query error. "
				<<" sql: " << SQL << "."
				<<" retry: "<<n << "."
				<<" errno: "<<mysql_errno(m_Data) << "."
				<<" error: "<<mysql_error(m_Data) << "." );

		switch ( mysql_errno(m_Data) )
		{
			case CR_SERVER_GONE_ERROR:
			case CR_SERVER_LOST:
				DisConnect();
				::sleep(m_retry_interval);
				continue;

			default:
				return -1;
		}
	}

	LOG4CPLUS_ERROR(logger, "mysql_read_query error. "
			<<"retry_times: "<<m_retry_times
			<<"retry_interval: "<<m_retry_interval
			<<"sql: " << SQL)
	return -1;
}

int DataBase::MysqlErrno()
{
	return mysql_errno(m_Data);
}

string DataBase::MysqlError()
{
	return mysql_error(m_Data);
}

bool DataBase::ErrDup()
{
	return MysqlErrno()==ER_DUP_ENTRY?true:false;
}

string DataBase::EscapeString(string& in)
{
	string ret;
	ret.resize(in.size()*3);

	uint64_t real_size=mysql_escape_string((char*)ret.c_str(), 
			in.c_str(), 
			in.size());

	ret.resize(real_size);
	return ret;
}

};



