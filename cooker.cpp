#include <mysql/mysql.h>
#include <iostream>
#include <iomanip>
#include <string>

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

// 显示所有待做订单
void displayPendingOrders(MYSQL *conn) {
    string query = "SELECT order_id, table_id, dish_name, quantity, total_price FROM orders WHERE status = 0";
    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL 查询失败: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    cout << "\n==================== 待做订单 ====================\n";
    cout << left << setw(10) << "订单ID" << setw(10) << "桌号" << setw(20) << "菜品名称" << setw(10) << "数量" << setw(10) << "总价" << endl;
    cout << "-------------------------------------------------\n";
    while ((row = mysql_fetch_row(res))) {
        cout << left << setw(10) << (row[0] ? row[0] : "NULL") << setw(10) << (row[1] ? row[1] : "NULL") << setw(20) << (row[2] ? row[2] : "NULL") << setw(10) << (row[3] ? row[3] : "NULL") << setw(10) << (row[4] ? row[4] : "NULL") << endl;
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

int main() {
    MYSQL *conn;
    connectToDatabase(conn);

    int order_id;
    char choice;
    int status;

    while (true) {
        cout << "\n==================== 厨师端 ====================\n";
        cout << "1. 查看待做订单\n";
        cout << "2. 更新订单状态\n";
        cout << "0. 退出\n";
        cout << "请选择操作: ";
        cin >> choice;

        switch (choice) {
            case '1':
                displayPendingOrders(conn);
                break;
            case '2':
                cout << "请输入订单ID: ";
                cin >> order_id;
                cout << "请输入新状态 (1-已做/2-已出餐): ";
                cin >> status;
                if (status == 1 || status == 2) {
                    updateOrderStatus(conn, order_id, status);
                } else {
                    cout << "无效的状态，请重试。\n";
                }
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