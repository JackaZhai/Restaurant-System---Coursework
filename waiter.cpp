#include <mysql/mysql.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <termios.h>
#include <unistd.h>

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

// 显示订单
void displayOrders(MYSQL *conn, int table_id) {
    string query = "SELECT order_id, dish_name, quantity, total_price, status FROM orders WHERE table_id = " + to_string(table_id);
    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL 查询失败: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    cout << "\n==================== 订单 ====================\n";
    cout << left << setw(10) << "订单ID" << setw(20) << "菜品名称" << setw(10) << "数量" << setw(10) << "总价" << setw(10) << "状态" << endl;
    cout << "-------------------------------------------------\n";
    while ((row = mysql_fetch_row(res))) {
        string status;
        if (row[4]) {
            try {
                int statusInt = stoi(row[4]);
                switch (statusInt) {
                    case 0: status = "待做"; break;
                    case 1: status = "已做"; break;
                    case 2: status = "已出餐"; break;
                    default: status = "未知"; break;
                }
            } catch (const std::invalid_argument& e) {
                status = "无效";
            }
        } else {
            status = "NULL";
        }
        cout << left << setw(10) << (row[0] ? row[0] : "NULL") << setw(20) << (row[1] ? row[1] : "NULL") << setw(10) << (row[2] ? row[2] : "NULL") << setw(10) << (row[3] ? row[3] : "NULL") << setw(10) << status << endl;
    }
    cout << "=============================================\n";

    mysql_free_result(res);
}

// 更新订单状态
void updateOrderStatus(MYSQL *conn, int order_id, int status) {
    string query = "UPDATE orders SET status = " + to_string(status) + " WHERE order_id = " + to_string(order_id);
    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL 查询失败: " << mysql_error(conn) << endl;
    } else {
        cout << "订单状态更新成功.\n";
    }
}

// 添加菜品到订单
void addItemsToOrder(MYSQL *conn, int table_id, int dish_id, int quantity) {
    // 获取菜品信息
    string query = "SELECT name, price FROM dishes WHERE id = " + to_string(dish_id);
    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL 查询失败: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) {
        cerr << "未找到菜品 ID: " << dish_id << endl;
        mysql_free_result(res);
        return;
    }

    string dish_name = row[0];
    double price = stod(row[1]);
    double total_price = price * quantity;

    mysql_free_result(res);

    // 插入订单
    query = "INSERT INTO orders (table_id, dish_id, dish_name, price, quantity, total_price, status) VALUES (" +
            to_string(table_id) + ", " + to_string(dish_id) + ", '" + dish_name + "', " + to_string(price) + ", " +
            to_string(quantity) + ", " + to_string(total_price) + ", 0) ON DUPLICATE KEY UPDATE quantity = quantity + " +
            to_string(quantity) + ", total_price = total_price + " + to_string(total_price);

    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL 查询失败: " << mysql_error(conn) << endl;
    } else {
        cout << "菜品添加成功.\n";
    }
}

int main() {
    MYSQL *conn;
    connectToDatabase(conn);

    int table_id, dish_id, quantity, order_id;
    char choice;
    int status;

    while (true) {
        cout << "\n==================== 服务员端 ====================\n";
        cout << "1. 查看订单\n";
        cout << "2. 更新订单状态\n";
        cout << "3. 添加菜品到订单\n";
        cout << "0. 退出\n";
        cout << "请选择操作: ";
        cin >> choice;

        switch (choice) {
            case '1':
                cout << "请输入桌号: ";
                cin >> table_id;
                displayOrders(conn, table_id);
                break;
            case '2':
                cout << "请输入订单ID: ";
                cin >> order_id;
                cout << "请输入新状态 (0-待做/1-已做/2-已出餐): ";
                cin >> status;
                updateOrderStatus(conn, order_id, status);
                break;
            case '3':
                cout << "请输入桌号: ";
                cin >> table_id;
                cout << "请输入菜品ID: ";
                cin >> dish_id;
                cout << "请输入数量: ";
                cin >> quantity;
                addItemsToOrder(conn, table_id, dish_id, quantity);
                break;
            case '0':
                mysql_close(conn);
                return 0;
            default:
                cout << "无效的选择，请重试。\n";
        }
    }

    mysql_close(conn);
    return 0;
}