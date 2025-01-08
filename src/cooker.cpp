// src/cooker.cpp
#include <mysql.h>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

void connectToDatabase_cooker(MYSQL *&conn); // ���ӵ����ݿ�
void displayPendingOrders(MYSQL *conn); // ��ʾ���д�������
void updateOrderStatus_cooker(MYSQL *conn, int order_id, int status); // ���¶���״̬

void run_cooker() {
    MYSQL *conn;
    connectToDatabase_cooker(conn);

    int order_id;
    char choice;
    int status;

    while (true) {
        cout << "\n==================== ��ʦ�� ====================\n";
        cout << "1. �鿴��������\n";
        cout << "2. ���¶���״̬\n";
        cout << "0. �˳�\n";
        cout << "��ѡ�����: ";
        cin >> choice;

        switch (choice) {
            case '1':
                displayPendingOrders(conn);
                break;
            case '2':
                cout << "�����붩��ID: ";
                cin >> order_id;
                cout << "��������״̬ (1-����/2-�ѳ���): ";
                cin >> status;
                if (status == 1 || status == 2) {
                    updateOrderStatus_cooker(conn, order_id, status);
                } else {
                    cout << "��Ч��״̬�������ԡ�\n";
                }
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
void connectToDatabase_cooker(MYSQL *&conn) {
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

// ��ʾ���д�������
void displayPendingOrders(MYSQL *conn) {
    string query = "SELECT order_id, table_id, dish_name, quantity, total_price FROM orders WHERE status = 0";
    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL ��ѯʧ��: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    cout << "\n==================== �������� ====================\n";
    cout << left << setw(10) << "����ID" << setw(10) << "����" << setw(20) << "��Ʒ����" << setw(10) << "����" << setw(10) << "�ܼ�"
            << endl;
    cout << "-------------------------------------------------\n";
    while ((row = mysql_fetch_row(res))) {
        cout << left << setw(10) << (row[0] ? row[0] : "NULL") << setw(10) << (row[1] ? row[1] : "NULL") << setw(20) <<
                (row[2] ? row[2] : "NULL") << setw(10) << (row[3] ? row[3] : "NULL") << setw(10) << (
                    row[4] ? row[4] : "NULL") << endl;
    }
    cout << "=============================================\n";

    mysql_free_result(res);
}

// ���¶���״̬
void updateOrderStatus_cooker(MYSQL *conn, int order_id, int status) {
    string query = "UPDATE orders SET status = " + to_string(status) + " WHERE order_id = " + to_string(order_id);
    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL ��ѯʧ��: " << mysql_error(conn) << endl;
    } else {
        cout << "����״̬���³ɹ�.\n";
    }
}
