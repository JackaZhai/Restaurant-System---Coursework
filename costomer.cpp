#include <mysql/mysql.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <termios.h>
#include <unistd.h>

using namespace std;

void connectToDatabase(MYSQL *&conn);// 连接到数据库
char getch();// 获取用户输入
void displayWelcomeScreen();// 显示欢迎界面
void displayCategories(MYSQL *conn, map<int, string> &categories);// 显示菜品种类
void displayMenu(MYSQL *conn, const string &category);// 显示菜单
void displayAvailableTables(MYSQL *conn);// 显示可用桌号
void placeOrder(MYSQL *conn, int table_id, int dish_id, int quantity);// 下订单
void updateTableStatus(MYSQL *conn, int table_id, int status);// 更新桌号状态
void displayOrderSummary(MYSQL *conn, int table_id);// 显示订单摘要


int main() {
    MYSQL *conn;
    connectToDatabase(conn);

    int table_id, dish_id, quantity;
    char choice;
    map<int, string> categories;

    while (true) {
        displayWelcomeScreen();
        displayAvailableTables(conn);
        cout << "\n请输入桌号: ";
        cin >> table_id;
        updateTableStatus(conn, table_id, 1); // 更新桌号状态为已占用

        do {
            displayCategories(conn, categories);

            int category_choice;
            cout << "\n请选择菜品种类 (按 0 退出): ";
            cin >> category_choice;

            if (category_choice == 0) {
                break;
            }

            if (categories.find(category_choice) != categories.end()) {
                displayMenu(conn, categories[category_choice]);

                cout << "请输入菜品 ID 进行点单: ";
                cin >> dish_id;
                cout << "请输入数量: ";
                cin >> quantity;

                placeOrder(conn, table_id, dish_id, quantity);

                cout << "是否继续选择菜品? (y/n): ";
                cin >> choice;
                if (choice == 'n' || choice == 'N') {
                    break;
                }
            } else {
                cout << "无效的选择，请重试。\n";
            }
        } while (true);

        displayOrderSummary(conn, table_id); // 显示订单摘要

        cout << "是否继续下单? (y/n): ";
        cin >> choice;

        if (choice == 'n' || choice == 'N') {
            break;
        }
    }

    mysql_close(conn);
    return 0;
}

// 连接到数据库
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

char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
}

// 显示欢迎界面
void displayWelcomeScreen() {
    cout << "欢迎光临！" << endl;
    cout << "按任意键进入菜单..." << endl;
    getch(); // 等待用户按任意键
}

// 显示菜品种类
void displayCategories(MYSQL *conn, map<int, string> &categories) {
    if (mysql_query(conn, "SELECT DISTINCT category FROM dishes")) {
        cerr << "MySQL 查询失败: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;
    int index = 1;

    cout << "\n==================== 菜品种类 ====================\n";
    while ((row = mysql_fetch_row(res))) {
        cout << index << ". " << (row[0] ? row[0] : "NULL") << endl;
        categories[index] = row[0];
        index++;
    }
    cout << "0. 退出\n";
    cout << "=============================================\n";

    mysql_free_result(res);
}

// 显示菜单
void displayMenu(MYSQL *conn, const string &category) {
    string query = "SELECT id, name, category, price FROM dishes WHERE category = '" + category + "'";
    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL 查询失败: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    cout << "\n=========================== 菜单 ===========================\n";
    cout << "ID\t名称\t\t\t种类\t\t价格" << endl;
    cout << "-------------------------------------------------------------\n";
    while ((row = mysql_fetch_row(res))) {
        cout << (row[0] ? row[0] : "NULL") << "\t"
             << (row[1] ? row[1] : "NULL") << "\t\t"
             << (row[2] ? row[2] : "NULL") << "\t\t"
             << (row[3] ? row[3] : "NULL") << endl;
    }
    cout << "=============================================\n";

    mysql_free_result(res);
}

// 显示可用桌号
void displayAvailableTables(MYSQL *conn) {
    if (mysql_query(conn, "SELECT table_id, seats FROM tables WHERE status = 0")) {
        cerr << "MySQL 查询失败: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    cout << "\n==================== 可用桌号 ====================\n";
    cout << left << setw(10) << "桌号" << setw(10) << "座位数" << endl;
    cout << "---------------------------------------------\n";
    while ((row = mysql_fetch_row(res))) {
        cout << left << setw(10) << (row[0] ? row[0] : "NULL") << setw(10) << (row[1] ? row[1] : "NULL") << endl;
    }
    cout << "=============================================\n";

    mysql_free_result(res);
}

// 下订单
void placeOrder(MYSQL *conn, int table_id, int dish_id, int quantity) {
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
            to_string(quantity) + ", " + to_string(total_price) + ", '0') ON DUPLICATE KEY UPDATE quantity = quantity + " +
            to_string(quantity) + ", total_price = total_price + " + to_string(total_price);

    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL 查询失败: " << mysql_error(conn) << endl;
    } else {
        cout << "订单成功.\n";
    }
}

// 更新桌号状态
void updateTableStatus(MYSQL *conn, int table_id, int status) {
    string query = "UPDATE tables SET status = " + to_string(status) + " WHERE table_id = " + to_string(table_id);
    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL 查询失败: " << mysql_error(conn) << endl;
    }
}

// 计算并显示订单摘要
void displayOrderSummary(MYSQL *conn, int table_id) {
    // Retrieve order ID, creation time, and status
    string query = "SELECT order_id, creation_time, status FROM orders WHERE table_id = " + to_string(table_id) + " LIMIT 1";
    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL 查询失败: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);

    if (!row) {
        cerr << "未找到订单信息。\n";
        mysql_free_result(res);
        return;
    }

    string order_id = row[0] ? row[0] : "NULL";
    string creation_time = row[1] ? row[1] : "NULL";
    string status;
    if (row[2]) {
        int statusInt = stoi(row[2]);
        switch (statusInt) {
            case 0: status = "待做"; break;
            case 1: status = "已做"; break;
            case 2: status = "已出餐"; break;
            default: status = "未知"; break;
        }
    } else {
        status = "NULL";
    }

    mysql_free_result(res);

    // Retrieve detailed order summary
    query = "SELECT dish_name, price, SUM(quantity), SUM(total_price) FROM orders WHERE table_id = " + to_string(table_id) + " GROUP BY dish_name, price";
    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL 查询失败: " << mysql_error(conn) << endl;
        return;
    }

    res = mysql_store_result(conn);

    cout << "\n点餐成功！\n";
    cout << "您的订单信息为：\n";
    cout << "订单编号：" << order_id << "\n";
    cout << "订单创建时间：" << creation_time << "\n";
    cout << "当前订单总价：";
    double total_cost = 0;
    while ((row = mysql_fetch_row(res))) {
        total_cost += row[3] ? stod(row[3]) : 0;
    }
    cout << total_cost << " 元\n";
    cout << "订单状态：" << status << "\n";
    cout << "详细订单为：\n";
    cout << left << setw(20) << "菜品名称" << setw(10) << "单价" << setw(10) << "数量" << setw(10) << "总价" << endl;
    cout << "-------------------------------------------------\n";
    mysql_data_seek(res, 0); // Reset result pointer to fetch rows again
    while ((row = mysql_fetch_row(res))) {
        cout << left << setw(20) << (row[0] ? row[0] : "NULL") << setw(10) << (row[1] ? row[1] : "NULL") << setw(10) << (row[2] ? row[2] : "NULL") << setw(10) << (row[3] ? row[3] : "NULL") << endl;
    }
    cout << "=============================================\n";

    mysql_free_result(res);
}