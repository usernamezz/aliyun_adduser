#include <stdio.h>
#include "aos_log.h"
#include "aos_util.h"
#include "aos_string.h"
#include "aos_status.h"
#include "oss_auth.h"
#include "oss_util.h"
#include "oss_api.h"
#include <sqlite3.h>
#include <iostream>
using namespace std;

static int callback(void *res, int argc, char **argv, char **azColName) {
	int i;
	//fprintf(stderr, "%s: ", (const char*)data);
	/*for (i = 0; i<argc; i++) {
	printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}*/
	if (argc == 1) {
		*((int*)res) = atoi(argv[0]);
	}
	printf("\n");
	return 0;
}
void init_options(oss_request_options_t *options) {
	options->config = oss_config_create(options->pool);
	aos_str_set(&options->config->endpoint, "oss-cn-shanghai.aliyuncs.com");
	aos_str_set(&options->config->access_key_id, "LTAIsc3yrMfBdqAQ");
	aos_str_set(&options->config->access_key_secret, "R6YFX2H17TN9QBk6ZnI8mbwYiO9i9e");
	options->config->is_cname = 0;
	options->ctl = aos_http_controller_create(options->pool, 0);
}
void put_object_from_file_test()
{
	//printf("请输入需要上传比对图片的位置");
	char *filename = "C:\\Users\\Zou\\Desktop\\22.jpg";
	//filename = (char*)malloc(sizeof(char));
	char *location;
	location = new char[50];

	//= "C:/Users/Zou/Desktop/1.jpg"
	//scanf("%s", filename);
	//printf("%s", filename);
	aos_pool_t *p = NULL;
	aos_string_t bucket;
	aos_string_t object;
	aos_table_t *headers = NULL;
	aos_table_t *resp_headers = NULL;
	oss_request_options_t *options = NULL;

	aos_status_t *s = NULL;
	aos_string_t file;
	aos_pool_create(&p, NULL);
	/* 创建并初始化options */
	options = oss_request_options_create(p);
	init_options(options);
	/* 初始化参数 */
	headers = aos_table_make(options->pool, 1);
	apr_table_set(headers, "image/jpeg", "image/jpeg");
	aos_str_set(&bucket, "facedatabase");
	//连接数据库获取个数
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	int success;
	char *sql;
	char *sql_str;
	const char* data = "Callback function called";
	/* Open database */
	rc = sqlite3_open("C:\\Users\\Zou\\Desktop\\AliyunAPI\\AliyunAPI\\test.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else {
		fprintf(stderr, "Opened database successfully\n");
	}
	sql = new char[200];
	/*sql_str = new char[200];
	memset(sql_str, '\0', 200);
	memset(sql, '\0', 200);
	sql_str = "insert into userinfo values('qwq2143',25,8,'','male',0);";*/
	//rc = sqlite3_exec(db, sql_str, callback, 0, &zErrMsg);
	/*if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "Records created successfully\n");
	}*/
	sql = "select count(*) from userinfo";
	char ** pResult;
	char *errmsg;
	int nRow, nCol;
	auto res = sqlite3_get_table(db, sql, &pResult, &nRow, &nCol, &errmsg);
	if (res == SQLITE_OK) {
		cout << pResult[nCol] << endl;
		int data = atoi(pResult[nCol]);
		printf("count: %d", data);
		memset(location, '\0', 50);
		sprintf(location, "data/%d.jpg", data + 1);
	}
	sqlite3_close(db);
	//
	aos_str_set(&object, location);
	aos_str_set(&file, filename);
	/* 上传文件 */
	s = oss_put_object_from_file(options, &bucket, &object, &file,
		headers, &resp_headers);
	/* 判断是否上传成功 */
	if (aos_status_is_ok(s)) {
		printf("put object from file succeeded\n");
	}
	else {
		printf("put object from file failed\n");
	}
	/* 释放资源*/
	aos_pool_destroy(p);
}
int main()
{
	if (aos_http_io_initialize(NULL, 0) != AOSE_OK) {
		return -1;
	}        //初始化
	put_object_from_file_test();
}
