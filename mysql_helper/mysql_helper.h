#ifndef __XDRIVE_COMMENT_MYSQL_HELPER_H__
#define __XDRIVE_COMMENT_MYSQL_HELPER_H__

#include <string>
#include <sstream>
#include <mysql/mysql.h>
#include <mysql/mysqld_error.h>
#include <time.h>
#include "sql_util.h"
#include "sstream"
#include <assert.h>
#include "executor_queue.h"
#include <ext/hash_map>
#include "comment_util.h"
#include "md5.h"


using namespace std;
using namespace SqlUtil;

namespace xdrive
{
extern const std::string TABLE_PREFIX;

class MysqlHelper
{
public:
    MysqlHelper()
    {
    }
    ~MysqlHelper()
    {
        if(m_database)
        {
            delete m_database;
            m_database = NULL;
        }

        if(m_audit_database)
        {
            delete m_audit_database;
            m_audit_database = NULL;
        }
    }

public:
    int init(const std::string& host,
             const std::string& user,
             const std::string& password,
             const std::string& dbname)
    {
        //init xdrive_comment数据库
        m_database = new SqlUtil::DataBase;
        return m_database->Init(host,
                                0,
                                user,
                                password,
                                dbname);
		
    }

    int init_audit(const std::string& host,
             const std::string& user,
             const std::string& password,
             const std::string& dbname)
    {
        //init xdrive_comment_audit数据库
        m_audit_database = new SqlUtil::DataBase;
        return m_audit_database->Init(host,
                                      0,
                                      user,
                                      password,
                                      dbname);
		
    }

public:
	int evaluate_comment_db(uint64_t add_num, uint8_t evaluate_type, comment_info_list_t& comment_list_req, comment_info_list_t& comment_list_resp);
    int add_comment_db(comment_info_list_t& comment_list_req, comment_info_list_t& comment_list_resp);
	int add_comment_sql(comment_info_t& req, string& table_name);
	int add_audit_sql(comment_info_t& req, string& table_name, uint64_t comment_id);
    int del_comment_db(comment_info_list_t& comment_list_req, comment_info_list_t& comment_list_resp);
	int modify_comment_db(comment_info_list_t & comment_list_req,comment_info_list_t & comment_list_resp);
	int audit_comment_db(comment_info_t& comment_info, int& count_flag);
    int load_comment_db(std::string& nodeid, comment_info_list_t& comment_list_resp);
	int load_limit_comment_db(std::string& nodeid,comment_info_list_t& comment_list_resp, int limit_num);
	int load_comment_sortbysupportcount_db(std::string& nodeid, comment_info_list_t& comment_list_resp);
	int load_limit_comment_sortbysupportcount_db(std::string& nodeid,comment_info_list_t& comment_list_resp, int limit_num);
    int load_marquee_comment_db(std::string& nodeid, comment_info_list_t& comment_list_resp);

private:
    std::string get_table_name(const std::string& nodeid) const
    {
        if(nodeid.empty())
        {
            return "";
        }

		char md[33] = {0};
		md5_string((const char*)(nodeid.c_str()), nodeid.length(), md);

        std::string sub_str;
		sub_str.assign(md, 2);

        return TABLE_PREFIX + sub_str;			
    }

	std::string get_audit_table_name() const
    {
        return "comment_audit";
    }

private:
    SqlUtil::DataBase*                          m_database;
    SqlUtil::DataBase*                          m_audit_database;
    DECL_LOGGER(logger);
};

};
#endif

