#include <mysql/mysql.h>
#include <iostream>

using namespace std;

// 连接到数据库
void connectToDatabase(MYSQL *&conn) {
    const char *server = "localhost";
    const char *user = "root";
    const char *password = "123456789"; // MySQL 密码
    const char *database = "restaurant";
    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        cerr << "MySQL 连接失败: " << mysql_error(conn) << endl;
        exit(1);
    }
}

// 初始化表
void initializeTables(MYSQL *conn) {
    // 删除 orders 表的内容
    if (mysql_query(conn, "DELETE FROM orders")) {
        cerr << "删除 orders 表内容失败: " << mysql_error(conn) << endl;
        return;
    } else {
        cout << "orders 表内容已删除.\n";
    }

    // 将 tables 表的状态设置为 0
    if (mysql_query(conn, "UPDATE tables SET status = 0")) {
        cerr << "更新 tables 表状态失败: " << mysql_error(conn) << endl;
        return;
    } else {
        cout << "tables 表状态已更新为 0.\n";
    }
}

int main() {
    MYSQL *conn;
    connectToDatabase(conn);

    initializeTables(conn);

    mysql_close(conn);
    return 0;
}