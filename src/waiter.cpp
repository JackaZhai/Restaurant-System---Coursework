// src/waiter.cpp
#include <mysql.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>


using namespace std;

void connectToDatabase_waiter(MYSQL *&conn); // ���ӵ����ݿ�
void displayOrders(MYSQL *conn, int table_id); // ��ʾ����
void updateOrderStatus(MYSQL *conn, int order_id, int status); // ���¶���״̬
void addItemsToOrder(MYSQL *conn, int table_id, int dish_id, int quantity); // ��Ӳ�Ʒ������

void run_waiter() {
    MYSQL *conn;
    connectToDatabase_waiter(conn);

    int table_id, dish_id, quantity, order_id;
    char choice;
    int status;

    while (true) {
        cout << "\n==================== ����Ա�� ====================\n";
        cout << "1. �鿴����\n";
        cout << "2. ���¶���״̬\n";
        cout << "3. ��Ӳ�Ʒ������\n";
        cout << "0. �˳�\n";
        cout << "��ѡ�����: ";
        cin >> choice;

        switch (choice) {
            case '1':
                cout << "����������: ";
                cin >> table_id;
                displayOrders(conn, table_id);
                break;
            case '2':
                cout << "�����붩��ID: ";
                cin >> order_id;
                cout << "��������״̬ (0-����/1-����/2-�ѳ���): ";
                cin >> status;
                updateOrderStatus(conn, order_id, status);
                break;
            case '3':
                cout << "����������: ";
                cin >> table_id;
                cout << "�������ƷID: ";
                cin >> dish_id;
                cout << "����������: ";
                cin >> quantity;
                addItemsToOrder(conn, table_id, dish_id, quantity);
                break;
            case '0':
                mysql_close(conn);
                return;
            default:
                cout << "��Ч��ѡ�������ԡ�\n";
        }
    }

    mysql_close(conn);
}

// ���ӵ����ݿ�
void connectToDatabase_waiter(MYSQL *&conn) {
    const char *server = "localhost";
    const char *user = "root";
    const char *password = "123456789"; // MySQL ����
    const char *database = "restaurant";
    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        cerr << "MySQL ����ʧ��: " << mysql_error(conn) << endl;
        exit(1);
    }
}

// ��ʾ����
void displayOrders(MYSQL *conn, int table_id) {
    string query = "SELECT order_id, dish_name, quantity, total_price, status FROM orders WHERE table_id = " +
                   to_string(table_id);
    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL ��ѯʧ��: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    cout << "\n==================== ���� ====================\n";
    cout << left << setw(10) << "����ID" << setw(20) << "��Ʒ����" << setw(10) << "����" << setw(10) << "�ܼ�" << setw(10) << "״̬"
            << endl;
    cout << "-------------------------------------------------\n";
    while ((row = mysql_fetch_row(res))) {
        string status;
        if (row[4]) {
            try {
                int statusInt = stoi(row[4]);
                switch (statusInt) {
                    case 0: status = "����";
                        break;
                    case 1: status = "����";
                        break;
                    case 2: status = "�ѳ���";
                        break;
                    default: status = "δ֪";
                        break;
                }
            } catch (const std::invalid_argument &e) {
                status = "��Ч";
            }
        } else {
            status = "NULL";
        }
        cout << left << setw(10) << (row[0] ? row[0] : "NULL") << setw(20) << (row[1] ? row[1] : "NULL") << setw(10) <<
                (row[2] ? row[2] : "NULL") << setw(10) << (row[3] ? row[3] : "NULL") << setw(10) << status << endl;
    }
    cout << "=============================================\n";

    mysql_free_result(res);
}

// ���¶���״̬
void updateOrderStatus(MYSQL *conn, int order_id, int status) {
    string query = "UPDATE orders SET status = " + to_string(status) + " WHERE order_id = " + to_string(order_id);
    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL ��ѯʧ��: " << mysql_error(conn) << endl;
    } else {
        cout << "����״̬���³ɹ�.\n";
    }
}

// ��Ӳ�Ʒ������
void addItemsToOrder(MYSQL *conn, int table_id, int dish_id, int quantity) {
    // ��ȡ��Ʒ��Ϣ
    string query = "SELECT name, price FROM dishes WHERE id = " + to_string(dish_id);
    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL ��ѯʧ��: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) {
        cerr << "δ�ҵ���Ʒ ID: " << dish_id << endl;
        mysql_free_result(res);
        return;
    }

    string dish_name = row[0];
    double price = stod(row[1]);
    double total_price = price * quantity;

    mysql_free_result(res);

    // ���붩��
    query = "INSERT INTO orders (table_id, dish_id, dish_name, price, quantity, total_price, status) VALUES (" +
            to_string(table_id) + ", " + to_string(dish_id) + ", '" + dish_name + "', " + to_string(price) + ", " +
            to_string(quantity) + ", " + to_string(total_price) + ", 0) ON DUPLICATE KEY UPDATE quantity = quantity + "
            +
            to_string(quantity) + ", total_price = total_price + " + to_string(total_price);

    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL ��ѯʧ��: " << mysql_error(conn) << endl;
    } else {
        cout << "��Ʒ��ӳɹ�.\n";
    }
}
