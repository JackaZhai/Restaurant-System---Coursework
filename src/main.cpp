#include <mysql.h>
#include <string>
#include <iostream>
#include "cashier.h"
#include "cooker.h"
#include "customer.h"
#include "waiter.h"
#include "boss.h"
#include "administrator.h"

using namespace std;

void connectToDatabase(MYSQL *&conn);

int login(MYSQL *conn, const string &username, const string &password);

int main() {
    MYSQL *conn;
    connectToDatabase(conn);

    char choice;
    string username, password;
    int role = -1;

    while (true) {
        cout << "\n==================== 餐厅管理系统 ====================\n";
        cout << "1. 收银员\n";
        cout << "2. 厨师\n";
        cout << "3. 顾客\n";
        cout << "4. 服务员\n";
        cout << "5. 老板\n";
        cout << "6. 管理员\n";
        cout << "0. 退出\n";
        cout << "请选择操作: ";
        cin >> choice;

        if (choice == '3') {
            run_customer();
            continue;
        }

        while (role == -1) {
            cout << "请输入用户名: ";
            cin >> username;
            cout << "请输入密码: ";
            cin >> password;
            role = login(conn, username, password);
        }

        switch (choice) {
            case '1':
                if (role >= 1) run_cashier();
                else cout << "权限不足，无法执行此操作。\n";
                break;
            case '2':
                if (role >= 1) run_cooker();
                else cout << "权限不足，无法执行此操作。\n";
                break;
            case '4':
                if (role >= 1) run_waiter();
                else cout << "权限不足，无法执行此操作。\n";
                break;
            case '5':
                if (role >= 2) run_boss();
                else cout << "权限不足，无法执行此操作。\n";
                break;
            case '6':
                if (role >= 2) run_administrator();
                else cout << "权限不足，无法执行此操作。\n";
                break;
            case '0':
                cout << "程序已退出\n";
                mysql_close(conn);
                return 0;
            default:
                cout << "无效的选择，请重试。\n";
        }

        role = -1; // Reset role for next login
    }

    mysql_close(conn);
    return 0;
}

void connectToDatabase(MYSQL *&conn) {
    const char *server = "localhost";
    const char *user = "root";
    const char *password = "123456789"; // MySQL 密码
    const char *database = "restaurant"; // Corrected database name
    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        cerr << "MySQL 连接失败: " << mysql_error(conn) << endl;
        exit(1);
    }
}

int login(MYSQL *conn, const string &username, const string &password) {
    string query = "SELECT role FROM users WHERE username = '" + username + "' AND password = '" + password + "'";
    if (mysql_query(conn, query.c_str())) {
        cerr << "登录查询失败: " << mysql_error(conn) << endl;
        return -1;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (res == NULL) {
        cerr << "获取查询结果失败: " << mysql_error(conn) << endl;
        return -1;
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    if (row == NULL) {
        cout << "用户名或密码错误.\n";
        mysql_free_result(res);
        return -1;
    }

    int role = stoi(row[0]);
    mysql_free_result(res);
    return role;
}