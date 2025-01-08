// src/cashier.cpp
#include <mysql.h>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

void connectToDatabase_cashier(MYSQL *&conn); // 连接到数据库
void updateTableStatus_cashier(MYSQL *conn, int table_id, int status); // 更新桌号状态
void settleBill(MYSQL *conn, int table_id); // 结算小票
void dailyStatistics_cashier(MYSQL *conn); // 统计当天结算
void dailyDishStatistics_cashier(MYSQL *conn); // 统计当天待做和已做菜品

void run_cashier() {
    MYSQL *conn;
    connectToDatabase_cashier(conn);

    char choice;
    while (true) {
        cout << "\n==================== 收银员端 ====================\n";
        cout << "1. 结算小票\n";
        cout << "2. 统计当天结算\n";
        cout << "3. 统计当天待做和已做菜品\n";
        cout << "0. 退出\n";
        cout << "请选择操作: ";
        cin >> choice;

        switch (choice) {
            case '1': {
                int table_id;
                cout << "请输入桌号ID: ";
                cin >> table_id;
                settleBill(conn, table_id);
                break;
            }
            case '2':
                dailyStatistics_cashier(conn);
                break;
            case '3':
                dailyDishStatistics_cashier(conn);
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

// 连接到数据库
void connectToDatabase_cashier(MYSQL *&conn) {
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

// 更新桌号状态
void updateTableStatus_cashier(MYSQL *conn, int table_id, int status) {
    string query = "UPDATE tables SET status = " + to_string(status) + " WHERE table_id = " + to_string(table_id);
    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL 查询失败: " << mysql_error(conn) << endl;
    }
}

// 结算小票
void settleBill(MYSQL *conn, int table_id) {
    string query = "SELECT dish_name, price, quantity, total_price FROM orders WHERE table_id = " + to_string(table_id)
                   + " AND status = 2";
    if (mysql_query(conn, query.c_str())) {
        cerr << "查询订单失败: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    double total_price = 0;
    cout << "\n==================== 结算小票 ====================\n";
    cout << "桌号: " << table_id << endl;
    cout << left << setw(20) << "菜品名称" << setw(10) << "单价" << setw(10) << "数量" << setw(10) << "总价" << endl;
    cout << "-------------------------------------------------\n";
    while ((row = mysql_fetch_row(res))) {
        string dish_name = row[0] ? row[0] : "NULL";
        double price = row[1] ? stod(row[1]) : 0;
        int quantity = row[2] ? stoi(row[2]) : 0;
        double item_total_price = row[3] ? stod(row[3]) : 0;
        total_price += item_total_price;

        cout << left << setw(20) << dish_name << setw(10) << price << setw(10) << quantity << setw(10) << item_total_price << endl;
    }
    cout << "-------------------------------------------------\n";

    if (total_price < 500) {
        int num_people;
        cout << "消费金额不足500元，请输入用餐人数: ";
        cin >> num_people;
        double table_fee = num_people * 5;
        total_price += table_fee;
        cout << "收取餐位费: " << table_fee << " 元\n";
    }

    cout << "订单总金额: " << total_price << " 元\n";
    cout << "===============================================\n";

    mysql_free_result(res);

    // 更新桌号状态为0
    updateTableStatus_cashier(conn, table_id, 0);
}

// 统计当天结算
void dailyStatistics_cashier(MYSQL *conn) {
    // 查询已结算订单的总金额和桌号
    string query = "SELECT table_id, SUM(total_price) FROM orders WHERE status = 2 GROUP BY table_id";
    if (mysql_query(conn, query.c_str())) {
        cerr << "统计查询失败: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    cout << "\n当天已结算订单:\n";
    cout << left << setw(10) << "桌号" << setw(10) << "总金额" << endl;
    cout << "----------------------\n";
    while ((row = mysql_fetch_row(res))) {
        cout << left << setw(10) << (row[0] ? row[0] : "NULL") << setw(10) << (row[1] ? row[1] : "NULL") << " 元" <<
                endl;
    }
    cout << "======================\n";

    mysql_free_result(res);
}

// 统计当天待做和已做菜品
void dailyDishStatistics_cashier(MYSQL *conn) {
    string query = "SELECT table_id, dish_name, status, quantity FROM orders WHERE status IN (0, 1) ORDER BY table_id";
    if (mysql_query(conn, query.c_str())) {
        cerr << "统计查询失败: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    cout << "\n当天待做和已做菜品统计:\n";
    cout << left << setw(10) << "桌号" << setw(20) << "菜品名称" << setw(10) << "状态" << setw(10) << "数量" << endl;
    cout << "-------------------------------------------------\n";
    while ((row = mysql_fetch_row(res))) {
        string status;
        if (row[2]) {
            int statusInt = stoi(row[2]);
            switch (statusInt) {
                case 0: status = "待做";
                    break;
                case 1: status = "已做";
                    break;
                default: status = "未知";
                    break;
            }
        } else {
            status = "NULL";
        }
        cout << left << setw(10) << (row[0] ? row[0] : "NULL") << setw(20) << (row[1] ? row[1] : "NULL") << setw(10) <<
                status << setw(10) << (row[3] ? row[3] : "NULL") << endl;
    }
    cout << "======================\n";

    mysql_free_result(res);
}
