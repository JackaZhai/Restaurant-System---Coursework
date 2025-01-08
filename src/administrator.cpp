#include <mysql.h>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

void connectToDatabase_administrator(MYSQL *&conn); // 连接到数据库
void initializeTables(MYSQL *conn); // 初始化表
void addDish(MYSQL *conn, const string &name, double price); // 添加菜品
void deleteDish(MYSQL *conn, const string &name); // 删除菜品
void addDrink(MYSQL *conn, const string &name, double price); // 添加酒水
void deleteDrink(MYSQL *conn, const string &name); // 删除酒水
void addTable(MYSQL *conn, int table_id, int capacity); // 添加餐位
void deleteTable(MYSQL *conn, int table_id); // 删除餐位
void createUser(MYSQL *conn, const string &username, const string &password, int role);
void deleteUser(MYSQL *conn, const string &username);

void createUser(MYSQL *conn, const string &username, const string &password, int role) {
    string query = "INSERT INTO users (username, password, role) VALUES ('" + username + "', '" + password + "', " + to_string(role) + ")";
    if (mysql_query(conn, query.c_str())) {
        cerr << "创建用户失败: " << mysql_error(conn) << endl;
    } else {
        cout << "用户创建成功.\n";
    }
}

void deleteUser(MYSQL *conn, const string &username) {
    string query = "DELETE FROM users WHERE username = '" + username + "'";
    if (mysql_query(conn, query.c_str())) {
        cerr << "删除用户失败: " << mysql_error(conn) << endl;
    } else {
        cout << "用户删除成功.\n";
    }
}
void run_administrator() {
    MYSQL *conn;
    connectToDatabase_administrator(conn);

    char choice;
    while (true) {
        cout << "\n==================== 系统管理员端 ====================\n";
        cout << "1. 系统初始化\n";
        cout << "2. 添加菜品\n";
        cout << "3. 删除菜品\n";
        cout << "4. 添加酒水\n";
        cout << "5. 删除酒水\n";
        cout << "6. 添加餐位\n";
        cout << "7. 删除餐位\n";
        cout << "8. 创建用户\n";
        cout << "9. 删除用户\n";
        cout << "0. 退出\n";
        cout << "请选择操作: ";
        cin >> choice;

        switch (choice) {
            case '1':
                initializeTables(conn);
                break;
            case '2': {
                string name;
                double price;
                cout << "请输入菜品名称: ";
                cin >> name;
                cout << "请输入菜品价格: ";
                cin >> price;
                addDish(conn, name, price);
                break;
            }
            case '3': {
                string name;
                cout << "请输入要删除的菜品名称: ";
                cin >> name;
                deleteDish(conn, name);
                break;
            }
            case '4': {
                string name;
                double price;
                cout << "请输入酒水名称: ";
                cin >> name;
                cout << "请输入酒水价格: ";
                cin >> price;
                addDrink(conn, name, price);
                break;
            }
            case '5': {
                string name;
                cout << "请输入要删除的酒水名称: ";
                cin >> name;
                deleteDrink(conn, name);
                break;
            }
            case '6': {
                int table_id, capacity;
                cout << "请输入餐位ID: ";
                cin >> table_id;
                cout << "请输入餐位容量: ";
                cin >> capacity;
                addTable(conn, table_id, capacity);
                break;
            }
            case '7': {
                int table_id;
                cout << "请输入要删除的餐位ID: ";
                cin >> table_id;
                deleteTable(conn, table_id);
                break;
            }
            case '8': {
                string username, password;
                int role;
                cout << "请输入新用户名: ";
                cin >> username;
                cout << "请输入新密码: ";
                cin >> password;
                cout << "请输入角色 (0: 顾客, 1: 收银员/厨师/服务员, 2: 老板/管理员): ";
                cin >> role;
                createUser(conn, username, password, role);
                break;
            }
            case '9': {
                string username;
                cout << "请输入要删除的用户名: ";
                cin >> username;
                deleteUser(conn, username);
                break;
            }
            case '0':
                mysql_close(conn);
                return;
            default:
                cout << "无效的选择，请重试。\n";
        }
    }

    mysql_close(conn);
}

void connectToDatabase_administrator(MYSQL *&conn) {
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

    // 重置 orders 表的自增值
    if (mysql_query(conn, "ALTER TABLE orders AUTO_INCREMENT = 0")) {
        cerr << "重置 orders 表自增值失败: " << mysql_error(conn) << endl;
        return;
    } else {
        cout << "orders 表自增值已重置为 0.\n";
    }

    // 将 tables 表的状态设置为 0
    if (mysql_query(conn, "UPDATE tables SET status = 0")) {
        cerr << "更新 tables 表状态失败: " << mysql_error(conn) << endl;
        return;
    } else {
        cout << "tables 表状态已更新为 0.\n";
    }
}

// 添加菜品
void addDish(MYSQL *conn, const string &name, double price) {
    string query = "INSERT INTO dishes (name, price) VALUES ('" + name + "', " + to_string(price) + ")";
    if (mysql_query(conn, query.c_str())) {
        cerr << "添加菜品失败: " << mysql_error(conn) << endl;
    } else {
        cout << "菜品添加成功.\n";
    }
}

// 删除菜品
void deleteDish(MYSQL *conn, const string &name) {
    string query = "DELETE FROM dishes WHERE name = '" + name + "'";
    if (mysql_query(conn, query.c_str())) {
        cerr << "删除菜品失败: " << mysql_error(conn) << endl;
    } else {
        cout << "菜品删除成功.\n";
    }
}

// 添加酒水
void addDrink(MYSQL *conn, const string &name, double price) {
    string query = "INSERT INTO drinks (name, price) VALUES ('" + name + "', " + to_string(price) + ")";
    if (mysql_query(conn, query.c_str())) {
        cerr << "添加酒水失败: " << mysql_error(conn) << endl;
    } else {
        cout << "酒水添加成功.\n";
    }
}

// 删除酒水
void deleteDrink(MYSQL *conn, const string &name) {
    string query = "DELETE FROM drinks WHERE name = '" + name + "'";
    if (mysql_query(conn, query.c_str())) {
        cerr << "删除酒水失败: " << mysql_error(conn) << endl;
    } else {
        cout << "酒水删除成功.\n";
    }
}

// 添加餐位
void addTable(MYSQL *conn, int table_id, int capacity) {
    string query = "INSERT INTO tables (table_id, capacity, status) VALUES (" + to_string(table_id) + ", " +
                   to_string(capacity) + ", 0)";
    if (mysql_query(conn, query.c_str())) {
        cerr << "添加餐位失败: " << mysql_error(conn) << endl;
    } else {
        cout << "餐位添加成功.\n";
    }
}

// 删除餐位
void deleteTable(MYSQL *conn, int table_id) {
    string query = "DELETE FROM tables WHERE table_id = " + to_string(table_id);
    if (mysql_query(conn, query.c_str())) {
        cerr << "删除餐位失败: " << mysql_error(conn) << endl;
    } else {
        cout << "餐位删除成功.\n";
    }
}
