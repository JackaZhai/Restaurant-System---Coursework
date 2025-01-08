#include <mysql.h>
#include <iostream>
#include <cstdlib>

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    // 初始化MySQL对象
    conn = mysql_init(NULL);

    // 检查初始化是否成功
    if (conn == NULL) {
        std::cerr << "mysql_init() failed" << std::endl;
        exit(1);
    }

    // 连接数据库
    if (mysql_real_connect(conn, "localhost", "root", "123456789",
            "database_name", 0, NULL, 0) == NULL) {
        std::cerr << "mysql_real_connect() failed. Error: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        exit(1);
            }

    // 执行SQL查询
    if (mysql_query(conn, "SELECT * FROM table_name")) {
        std::cerr << "SELECT * error: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        exit(1);
    }

    // 获取查询结果
    res = mysql_store_result(conn);
    if (res == NULL) {
        std::cerr << "mysql_store_result() failed. Error: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        exit(1);
    }

    // 处理查询结果
    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            std::cout << (row[i] ? row[i] : "NULL") << " ";
        }
        std::cout << std::endl;
    }

    // 释放查询结果
    mysql_free_result(res);

    // 关闭数据库连接
    mysql_close(conn);

    return 0;
}
