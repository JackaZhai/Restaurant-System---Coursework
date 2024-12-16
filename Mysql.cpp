#include <mysql/mysql.h>
#include <iostream>

using namespace std;

void connectToDatabase() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    const char *server = "localhost";
    const char *user = "root";
    const char *password = "123456789"; // 请替换为你的 MySQL 密码
    const char *database = "restaurant";
    conn = mysql_init(NULL);

    // 连接数据库
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        cerr << "MySQL connection failed: " << mysql_error(conn) << endl;
        return;
    }

    // 执行查询
    if (mysql_query(conn, "SELECT * FROM dishes")) { // 请替换为你的查询语句
        cerr << "MySQL query failed: " << mysql_error(conn) << endl;
        mysql_close(conn);
        return;
    }

    res = mysql_store_result(conn);

    // 输出查询结果
    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            cout << (row[i] ? row[i] : "NULL") << " ";
        }
        cout << endl;
    }

    // 释放结果集
    mysql_free_result(res);
    // 关闭连接
    mysql_close(conn);
}

int main() {
    connectToDatabase();
    return 0;
}