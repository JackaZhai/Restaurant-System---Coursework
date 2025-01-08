// src/boss.cpp
#include <mysql.h>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

void connectToDatabase_boss(MYSQL *&conn); // 连接到数据库
void displayAllOrders(MYSQL *conn); // 显示所有订单
void dailyStatistics(MYSQL *conn); // 统计当天结算

void run_boss() {
    MYSQL *conn;
    connectToDatabase_boss(conn);

    char choice;

    while (true) {
        cout << "\n==================== 餐厅老板端 ====================\n";
        cout << "1. 查看所有订单\n";
        cout << "2. 统计查询\n";
        cout << "0. 退出\n";
        cout << "请选择操作: ";
        cin >> choice;

        switch (choice) {
            case '1':
                displayAllOrders(conn);
                break;
            case '2':
                dailyStatistics(conn);
                break;
            case '0':
                mysql_close(conn);
                return;
            default:
                cout << "无效的选择，请重试。\n";
        }
    }

    mysql_close(conn);
}

void connectToDatabase_boss(MYSQL *&conn) {
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

void displayAllOrders(MYSQL *conn) {
    string query = "SELECT order_id, table_id, dish_name, quantity, total_price, status FROM orders";
    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL 查询失败: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    cout << "\n==================== 所有订单 ====================\n";
    cout << left << setw(10) << "订单ID" << setw(10) << "桌号" << setw(20) << "菜品名称" << setw(10) << "数量" << setw(10) <<
            "总???" << setw(10) << "状态" << endl;
    cout << "-------------------------------------------------\n";
    while ((row = mysql_fetch_row(res))) {
        string status;
        if (row[5]) {
            int statusInt = stoi(row[5]);
            switch (statusInt) {
                case 0: status = "待做";
                    break;
                case 1: status = "已做";
                    break;
                case 2: status = "已出餐";
                    break;
                default: status = "未知";
                    break;
            }
        } else {
            status = "NULL";
        }
        cout << left << setw(10) << (row[0] ? row[0] : "NULL") << setw(10) << (row[1] ? row[1] : "NULL") << setw(20) <<
                (row[2] ? row[2] : "NULL") << setw(10) << (row[3] ? row[3] : "NULL") << setw(10) << (
                    row[4] ? row[4] : "NULL") << setw(10) << status << endl;
    }
    cout << "=============================================\n";

    mysql_free_result(res);
}

void dailyStatistics(MYSQL *conn) {
    string query = "SELECT SUM(total_price) FROM orders WHERE status = 2";
    if (mysql_query(conn, query.c_str())) {
        cerr << "统计查询失败: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);

    if (row && row[0]) {
        cout << "当天已结算总金额: " << row[0] << " 元\n";
    } else {
        cout << "当天无已结算订单。\n";
    }
    mysql_free_result(res);

    query = "SELECT COUNT(DISTINCT table_id) FROM orders WHERE status = 2";
    if (mysql_query(conn, query.c_str())) {
        cerr << "统计查询失败: " << mysql_error(conn) << endl;
        return;
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    if (row && row[0]) {
        cout << "当天已使用桌子数量: " << row[0] << " 桌\n";
    } else {
        cout << "当天无已使用桌子。\n";
    }
    mysql_free_result(res);
}
