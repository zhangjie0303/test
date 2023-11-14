#include "global_var.h"
#include "mysql_helper.h"
#include "executor_queue.h"
#include "comment_properties.h"

using namespace std;

#define GET_UINT8_FIELD(record, field, value)  do { \
	int i_value = 0; \
	record.GetField(field, i_value); \
	value = (uint8_t)i_value; \
} while (0)

namespace xdrive
{
IMPL_LOGGER(MysqlHelper, logger);

const std::string TABLE_PREFIX="comment_info_";

int MysqlHelper::evaluate_comment_db(uint64_t add_num, uint8_t evaluate_type, comment_info_list_t& comment_list_req, comment_info_list_t& comment_list_resp)
{
    								
    int rv;
	if(comment_list_req.size() != 1)
    {
        return ExecutorThreadRequestType::E_WRONG_PARAMETER;
    } 
	
    string table_name = get_table_name(comment_list_req[0]._topicid);

    std::ostringstream ss;
	ss.str("");
    ss << "select * from " << table_name
	   << " where id=" << comment_list_req[0]._id;
	
    SqlUtil::RecordSet comment_record_another;
    rv=m_database->ExecuteSql(ss.str().c_str(), ss.str().length(), comment_record_another);
	if(rv < 0)
	{
		LOG4CPLUS_ERROR(logger, "ExecuteSql sql failed!, sql: " << ss.str());
		return ExecutorThreadRequestType::E_DB; 
	}
    comment_info_t& cinfo_resp = comment_list_req[0];
    if(comment_record_another.FetchRow())
	{
        comment_record_another.GetField("id", cinfo_resp._id);
        comment_record_another.GetField("topicid", cinfo_resp._topicid);
        comment_record_another.GetField("userid", cinfo_resp._userid);
        comment_record_another.GetField("userid_replyto", cinfo_resp._userid_replyto);
		GET_UINT8_FIELD(comment_record_another, "flag", cinfo_resp._flag);
		GET_UINT8_FIELD(comment_record_another, "audit", cinfo_resp._audit);
        comment_record_another.GetField("trample_count", cinfo_resp._trample_count);
        comment_record_another.GetField("content", cinfo_resp._content);
	    GET_UINT8_FIELD(comment_record_another, "comment_type", cinfo_resp._comment_type);
        string stamp_time;
        comment_record_another.GetField("comment_time", stamp_time);
	    cinfo_resp._insert_time = CommentUtil::get_ut_from_time(stamp_time);
        comment_record_another.GetField("support_count", cinfo_resp._support_count);
        comment_record_another.GetField("comment_type", cinfo_resp._comment_type);
        string property_map;
        comment_record_another.GetField("property_map",property_map);
        CommentProperties::parse(property_map, cinfo_resp._property_map);


	}

	LOG4CPLUS_DEBUG(logger, "trample_count: " << cinfo_resp._trample_count << ", support count: " << cinfo_resp._support_count);


    ss.str("");   
    if(evaluate_type == T_EVALUATE_TRAMPLE){
           ss << "update " << table_name
               << " set trample_count = trample_count + " << add_num
               << " where id=" << comment_list_req[0]._id
               << " and topicid='" << comment_list_req[0]._topicid << "'"
               << " and userid=" << comment_list_req[0]._userid;
    }
    else if(evaluate_type == T_EVALUATE_SUPPORT)
   {
	    ss << "update " << table_name
               << " set support_count = support_count + " << add_num 
               << " where id=" << comment_list_req[0]._id
               << " and topicid='" << comment_list_req[0]._topicid << "'"
               << " and userid=" << comment_list_req[0]._userid; 
    }	
	

    //comment_list_resp.push_back(comment_list_req[0]);
    SqlUtil::RecordSet comment_record;
    rv=m_database->ExecuteSql(ss.str().c_str(), ss.str().length(), comment_record);
 
    if(rv != 0)
    {
       return ExecutorThreadRequestType::E_DB;
    }


    if(evaluate_type == T_EVALUATE_SUPPORT)
	{
		cinfo_resp._support_count+=add_num;
	}
	else
	{
		cinfo_resp._trample_count+=add_num;
	}
    comment_list_resp.push_back(cinfo_resp);
    
    return ExecutorThreadRequestType::E_OK;
}

int MysqlHelper::add_comment_db(comment_info_list_t& comment_list_req,comment_info_list_t & comment_list_resp)
{
    if(comment_list_req.size() < 1)
    {
        return ExecutorThreadRequestType::E_WRONG_PARAMETER;
    }
	
    string table_name = get_table_name(comment_list_req[0]._topicid);
    int comment_info_success_num = 0;
	int comment_audit_success_num = 0;
	int last_select_success_num = 0;
    for(uint32_t i = 0; i < comment_list_req.size(); ++i)
    {
		if(add_comment_sql(comment_list_req[i], table_name) == 0)
		{
			comment_info_success_num++;
		}

		std::ostringstream ss;
		SqlUtil::RecordSet comment_record_select_all;
		ss.str("");
		ss << "select * from "<<table_name<<" where id=(select last_insert_id())";
		if(m_database->ExecuteSql(ss.str().c_str(), ss.str().length(), comment_record_select_all) == 0)
		{
			last_select_success_num++;
		}
		assert(comment_record_select_all.FetchRow() != NULL);

		comment_info_t ci;
        comment_record_select_all.GetField("id", ci._id);
        comment_record_select_all.GetField("topicid", ci._topicid);
        comment_record_select_all.GetField("userid", ci._userid);
        comment_record_select_all.GetField("userid_replyto", ci._userid_replyto);
		GET_UINT8_FIELD(comment_record_select_all, "flag", ci._flag);
		GET_UINT8_FIELD(comment_record_select_all, "audit", ci._audit);
        comment_record_select_all.GetField("content", ci._content);
	    comment_record_select_all.GetField("comment_ip", ci._comment_ip);
        comment_record_select_all.GetField("support_count", ci._support_count);
        comment_record_select_all.GetField("trample_count", ci._trample_count);
		string stamp_time;
        comment_record_select_all.GetField("comment_time", stamp_time);
	    ci._insert_time = CommentUtil::get_ut_from_time(stamp_time);
        GET_UINT8_FIELD(comment_record_select_all, "comment_type", ci._comment_type);
        std::string property_map;
        comment_record_select_all.GetField("property_map", property_map);
        CommentProperties::parse(property_map, ci._property_map);
        comment_list_resp.push_back(ci);
        
		string audit_table_name = get_audit_table_name();
		if(add_audit_sql(comment_list_req[i], audit_table_name, ci._id) == 0)
		{
			comment_audit_success_num++;
		}
    }

    if(comment_info_success_num == 0 || comment_audit_success_num == 0 
		|| last_select_success_num == 0)
    {
        return ExecutorThreadRequestType::E_DB;
    }

    return ExecutorThreadRequestType::E_OK;
}

int MysqlHelper::add_comment_sql(comment_info_t& req, string& table_name)
{
    CommentProperties cp(req._property_map);
    std::string property_map = cp.to_string();
	std::ostringstream ss;
    ss.str("");
    ss << "insert into " << table_name
        << " set topicid='" << req._topicid << "'"
        << ", userid=" << req._userid
        << ", userid_replyto=" << req._userid_replyto
        << ", content='" << DataBase::EscapeString(req._content) << "'"
        << ", comment_ip='" << req._comment_ip << "'"
        << ", support_count=" << req._support_count<< ""
        << ", trample_count=" << req._trample_count << ""
        << ", comment_time=now()" 
        << ", comment_type=" << (int)req._comment_type
        << ", property_map='" << DataBase::EscapeString(property_map) << "'";
    
    SqlUtil::RecordSet comment_record;
    return m_database->ExecuteSql(ss.str().c_str(), ss.str().length(), comment_record);
}

int MysqlHelper::add_audit_sql(comment_info_t& req, string& table_name, uint64_t comment_id)
{
	std::ostringstream ss;
    ss.str("");
    ss << "insert into " << table_name
	 << " set comment_id=" << comment_id
        << ", topicid='" << req._topicid << "'"
        << ", userid=" << req._userid
        << ", userid_replyto=" << req._userid_replyto
        << ", content='" << DataBase::EscapeString(req._content) << "'"
        << ", comment_ip='" << req._comment_ip << "'"
        << ", comment_time=now()";
    
    SqlUtil::RecordSet comment_record;
    return m_audit_database->ExecuteSql(ss.str().c_str(), ss.str().length(), comment_record);
}

int MysqlHelper::del_comment_db(comment_info_list_t & comment_list_req,comment_info_list_t & comment_list_resp)
{
    int rv;
    std::ostringstream ss;
    if(comment_list_req.size() < 1)
    {
        return ExecutorThreadRequestType::E_WRONG_PARAMETER;
    }
    string table_name = get_table_name(comment_list_req[0]._topicid);
    int success_num = 0;
    for(uint32_t i = 0; i < comment_list_req.size(); ++i)
    {
        ss.str("");
        ss << "update " << table_name
            << " set flag=1"
            << " where id=" << comment_list_req[i]._id
            << " and topicid='" << comment_list_req[i]._topicid << "'"
            << " and userid=" << comment_list_req[i]._userid;

        SqlUtil::RecordSet comment_record;
        rv=m_database->ExecuteSql(ss.str().c_str(), ss.str().length(), comment_record);
        if(rv == 0)
        {
            success_num++;
        }
        
        SqlUtil::RecordSet query_record;
		ss.str("");
		ss << "select * from "<<table_name
           <<" where id=" << comment_list_req[i]._id;
		m_database->ExecuteSql(ss.str().c_str(), ss.str().length(), query_record);
		
		if(query_record.FetchRow() == NULL)
		{
            continue;
		}
		comment_info_t ci;
        query_record.GetField("id", ci._id);
        query_record.GetField("topicid", ci._topicid);
        query_record.GetField("userid", ci._userid);
        query_record.GetField("userid_replyto", ci._userid_replyto);
		GET_UINT8_FIELD(query_record, "flag", ci._flag);
		GET_UINT8_FIELD(query_record, "audit", ci._audit);
        query_record.GetField("content", ci._content);
	    query_record.GetField("comment_ip", ci._comment_ip);
        query_record.GetField("support_count", ci._support_count);
        query_record.GetField("trample_count", ci._trample_count);
		string stamp_time;
        query_record.GetField("comment_time", stamp_time);
	    ci._insert_time = CommentUtil::get_ut_from_time(stamp_time);
        GET_UINT8_FIELD(query_record, "comment_type", ci._comment_type);
        std::string property_map;
        query_record.GetField("property_map", property_map);
        CommentProperties::parse(property_map, ci._property_map);

        comment_list_resp.push_back(ci);
    }

    if(success_num == 0)
    {
        return ExecutorThreadRequestType::E_DB;
    }

    return ExecutorThreadRequestType::
    E_OK;
}

int MysqlHelper::modify_comment_db(comment_info_list_t & comment_list_req,comment_info_list_t & comment_list_resp)
{
    int rv;
    std::ostringstream ss;
    if(comment_list_req.size() < 1)
    {
        return ExecutorThreadRequestType::E_WRONG_PARAMETER;
    }
    string table_name = get_table_name(comment_list_req[0]._topicid);
    int success_num = 0;
    for(uint32_t i = 0; i < comment_list_req.size(); ++i)
    {
        ss.str("");
        ss << "update " << table_name
            << " set content='" << DataBase::EscapeString(comment_list_req[i]._content) << "'"
            << " where id=" << comment_list_req[i]._id
            << " and topicid='" << comment_list_req[i]._topicid << "'";
        
        SqlUtil::RecordSet comment_record;
        rv=m_database->ExecuteSql(ss.str().c_str(), ss.str().length(), comment_record);
        if(rv == 0)
        {
            success_num++;
        }     
    }

    if(success_num == 0)
    {
        return ExecutorThreadRequestType::E_DB;
    }

    return ExecutorThreadRequestType::E_OK;
}

int MysqlHelper::audit_comment_db(comment_info_t& comment_info, int& count_flag)
{
    int rv;
    std::ostringstream ss;
    
    int success_num = 0;
	int select_success_num = 0;

	string table_name = get_table_name(comment_info._topicid);
	ss.str("");
	ss << "select id, audit from "<<table_name<<" where id="<<comment_info._id;
	SqlUtil::RecordSet select_record;
    rv=m_database->ExecuteSql(ss.str().c_str(), ss.str().length(), select_record);
    if(rv == 0)
    {
        select_success_num++;
    }
	if(!select_record.FetchRow())
	{
		return ExecutorThreadRequestType::E_COMMENT_NOT_EXIST;
	}

	uint8_t audit = 0;
	GET_UINT8_FIELD(select_record, "audit", audit);
	LOG4CPLUS_DEBUG(logger, "MysqlHelper::"<<__FUNCTION__<<", old audit="<<int(audit)
		<<", new audit="<<int(comment_info._audit));

	assert(comment_info._audit == E_AUDIT_NOT 
		|| comment_info._audit == E_AUDIT_PASS 
		|| comment_info._audit == E_AUDIT_FAIL);
	
	if(audit == comment_info._audit)
	{
		// 要改变的值与DB里的值一致，不需要更新DB
		LOG4CPLUS_DEBUG(logger, "MysqlHelper::"<<__FUNCTION__<<", not modify!");
		count_flag  = COUNT_FLAG_NOT_MODIFY;
		return ExecutorThreadRequestType::E_OK;
	}

	if(audit == E_AUDIT_FAIL)
	{
		if(comment_info._audit == E_AUDIT_NOT || comment_info._audit == E_AUDIT_PASS)
			count_flag = COUNT_FLAG_INCREASE;
		else
			count_flag  = COUNT_FLAG_NOT_MODIFY;
	}
	else
	{
		if(comment_info._audit == E_AUDIT_FAIL)
			count_flag = COUNT_FLAG_DECREASE;
		else
			count_flag = COUNT_FLAG_NOT_MODIFY;
	}
	
    ss.str("");
    ss << "update " << table_name
        << " set audit=" << int(comment_info._audit)
        << " where id=" << comment_info._id
        << " and topicid='" << comment_info._topicid << "'";
    
    SqlUtil::RecordSet comment_record;
    rv=m_database->ExecuteSql(ss.str().c_str(), ss.str().length(), comment_record);
    if(rv == 0)
    {
        success_num++;
    }

    if(success_num == 0 || select_success_num == 0)
    {
        return ExecutorThreadRequestType::E_DB;
    }

    return ExecutorThreadRequestType::E_OK;
}

int MysqlHelper::load_comment_db(string& nodeid,comment_info_list_t & comment_list_resp)
{
    int rv;
    std::ostringstream ss;
    string table_name = get_table_name(nodeid);
    ss<<"select * from "<<table_name
      <<" where topicid='"<< nodeid <<"'"
      <<" and flag=0 and (audit=0 or audit=1) and comment_type=0 order by comment_time DESC";
    SqlUtil::RecordSet comment_record;
    rv=m_database->ExecuteSql(ss.str().c_str(), ss.str().length(), comment_record);
    if(rv<0)
    {
        return ExecutorThreadRequestType::E_DB;
    }

    while(comment_record.FetchRow())
    {
        comment_info_t ci;

        comment_record.GetField("id", ci._id);
        comment_record.GetField("topicid", ci._topicid);
        comment_record.GetField("userid", ci._userid);
        comment_record.GetField("userid_replyto", ci._userid_replyto);
		GET_UINT8_FIELD(comment_record, "flag", ci._flag);
		GET_UINT8_FIELD(comment_record, "audit", ci._audit);
        comment_record.GetField("content", ci._content);
	    comment_record.GetField("comment_ip", ci._comment_ip);
        GET_UINT8_FIELD(comment_record, "comment_type", ci._comment_type);

        string property_map;
        comment_record.GetField("property_map",property_map);
        CommentProperties::parse(property_map, ci._property_map);

        string stamp_time;
        comment_record.GetField("comment_time", stamp_time);
	    ci._insert_time = CommentUtil::get_ut_from_time(stamp_time);

        comment_record.GetField("support_count", ci._support_count);
	    comment_record.GetField("trample_count", ci._trample_count);

        comment_list_resp.push_back(ci);
    }

    return ExecutorThreadRequestType::E_OK;
}

int MysqlHelper::load_marquee_comment_db(std::string& nodeid, comment_info_list_t& comment_list_resp)
{
    int rv;
    std::ostringstream ss;
    string table_name = get_table_name(nodeid);
    ss<<"select * from "<<table_name
      <<" where topicid='"<< nodeid <<"'"
      <<" and flag=0 and (audit=0 or audit=1) and comment_type=1 order by comment_time DESC";
    SqlUtil::RecordSet comment_record;
    rv=m_database->ExecuteSql(ss.str().c_str(), ss.str().length(), comment_record);
    if(rv<0)
    {
        return ExecutorThreadRequestType::E_DB;
    }

    while(comment_record.FetchRow())
    {
        comment_info_t ci;

        comment_record.GetField("id", ci._id);
        comment_record.GetField("topicid", ci._topicid);
        comment_record.GetField("userid", ci._userid);
        comment_record.GetField("userid_replyto", ci._userid_replyto);
		GET_UINT8_FIELD(comment_record, "flag", ci._flag);
		GET_UINT8_FIELD(comment_record, "audit", ci._audit);
        comment_record.GetField("content", ci._content);
	    comment_record.GetField("comment_ip", ci._comment_ip);
        GET_UINT8_FIELD(comment_record, "comment_type", ci._comment_type);

        string property_map;
        comment_record.GetField("property_map",property_map);
        CommentProperties::parse(property_map, ci._property_map);

        string stamp_time;
        comment_record.GetField("comment_time", stamp_time);
	    ci._insert_time = CommentUtil::get_ut_from_time(stamp_time);
        comment_record.GetField("support_count", ci._support_count);
	    comment_record.GetField("trample_count", ci._trample_count);
        comment_list_resp.push_back(ci);
    }

    return ExecutorThreadRequestType::E_OK;    
}

int MysqlHelper::load_limit_comment_db(string& nodeid,comment_info_list_t & comment_list_resp, int limit_num)
{
    int rv;
    std::ostringstream ss;
    string table_name = get_table_name(nodeid);
    ss<<"select * from "<<table_name
      <<" where topicid='"<< nodeid <<"'"
      <<" and flag=0 and (audit=0 or audit=1) and comment_type=0 order by comment_time DESC limit "
      <<limit_num;
    SqlUtil::RecordSet comment_record;
    rv=m_database->ExecuteSql(ss.str().c_str(), ss.str().length(), comment_record);
    if(rv<0)
    {
        return ExecutorThreadRequestType::E_DB;
    }

    while(comment_record.FetchRow())
    {
        comment_info_t ci;

        comment_record.GetField("id", ci._id);
        comment_record.GetField("topicid", ci._topicid);
        comment_record.GetField("userid", ci._userid);
        comment_record.GetField("userid_replyto", ci._userid_replyto);
		GET_UINT8_FIELD(comment_record, "flag", ci._flag);
		GET_UINT8_FIELD(comment_record, "audit", ci._audit);
        comment_record.GetField("content", ci._content);
	    comment_record.GetField("comment_ip", ci._comment_ip);
        GET_UINT8_FIELD(comment_record, "comment_type", ci._comment_type);

        string property_map;
        comment_record.GetField("property_map",property_map);
        CommentProperties::parse(property_map, ci._property_map);

		string stamp_time;
        comment_record.GetField("comment_time", stamp_time);
	    ci._insert_time = CommentUtil::get_ut_from_time(stamp_time);
	    comment_record.GetField("support_count", ci._support_count);
	    comment_record.GetField("trample_count", ci._trample_count);
		
        comment_list_resp.push_back(ci);
    }

    return ExecutorThreadRequestType::E_OK;
}

int MysqlHelper::load_comment_sortbysupportcount_db(std::string& nodeid, comment_info_list_t& comment_list_resp)
{
    int rv;
    std::ostringstream ss;
    string table_name = get_table_name(nodeid);
    ss<<"select * from "<<table_name
      <<" where topicid='"<< nodeid <<"'"
      <<" and flag=0 and (audit=0 or audit=1) and comment_type=0 order by support_count DESC,comment_time DESC";
    SqlUtil::RecordSet comment_record;
    rv=m_database->ExecuteSql(ss.str().c_str(), ss.str().length(), comment_record);
    if(rv<0)
    {
        return ExecutorThreadRequestType::E_DB;
    }

    while(comment_record.FetchRow())
    {
        comment_info_t ci;

        comment_record.GetField("id", ci._id);
        comment_record.GetField("topicid", ci._topicid);
        comment_record.GetField("userid", ci._userid);
        comment_record.GetField("userid_replyto", ci._userid_replyto);
		GET_UINT8_FIELD(comment_record, "flag", ci._flag);
		GET_UINT8_FIELD(comment_record, "audit", ci._audit);
        comment_record.GetField("content", ci._content);
		comment_record.GetField("comment_ip", ci._comment_ip);
        GET_UINT8_FIELD(comment_record, "comment_type", ci._comment_type);

        string property_map;
        comment_record.GetField("property_map",property_map);
        CommentProperties::parse(property_map, ci._property_map);

        string stamp_time;
        comment_record.GetField("comment_time", stamp_time);
		ci._insert_time = CommentUtil::get_ut_from_time(stamp_time);

        comment_record.GetField("support_count", ci._support_count);
	    comment_record.GetField("trample_count", ci._trample_count);

        comment_list_resp.push_back(ci);
    }
    return ExecutorThreadRequestType::E_OK;
}

int MysqlHelper::load_limit_comment_sortbysupportcount_db(std::string& nodeid,comment_info_list_t& comment_list_resp, int limit_num)
{
    int rv;
    std::ostringstream ss;
    string table_name = get_table_name(nodeid);
    ss<<"select * from "<<table_name
      <<" where topicid='"<< nodeid <<"'"
      <<" and flag=0 and (audit=0 or audit=1) and comment_type=0 order by support_count DESC,comment_time DESC limit "
      <<limit_num;
    SqlUtil::RecordSet comment_record;
    rv=m_database->ExecuteSql(ss.str().c_str(), ss.str().length(), comment_record);
    if(rv<0)
    {
        return ExecutorThreadRequestType::E_DB;
    }

    while(comment_record.FetchRow())
    {
        comment_info_t ci;

        comment_record.GetField("id", ci._id);
        comment_record.GetField("topicid", ci._topicid);
        comment_record.GetField("userid", ci._userid);
        comment_record.GetField("userid_replyto", ci._userid_replyto);
		GET_UINT8_FIELD(comment_record, "flag", ci._flag);
		GET_UINT8_FIELD(comment_record, "audit", ci._audit);
        comment_record.GetField("content", ci._content);
	    comment_record.GetField("comment_ip", ci._comment_ip);
        GET_UINT8_FIELD(comment_record, "comment_type", ci._comment_type);

        string property_map;
        comment_record.GetField("property_map",property_map);
        CommentProperties::parse(property_map, ci._property_map);

        string stamp_time;
        comment_record.GetField("comment_time", stamp_time);
	    ci._insert_time = CommentUtil::get_ut_from_time(stamp_time);
	    comment_record.GetField("support_count", ci._support_count);
	    comment_record.GetField("trample_count", ci._trample_count);
		
        comment_list_resp.push_back(ci);
    }

    return ExecutorThreadRequestType::E_OK;
}
}

