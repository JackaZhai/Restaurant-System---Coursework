// src/boss.cpp
#include <mysql.h>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

void connectToDatabase_boss(MYSQL *&conn); // ���ӵ����ݿ�
void displayAllOrders(MYSQL *conn); // ��ʾ���ж���
void dailyStatistics(MYSQL *conn); // ͳ�Ƶ������

void run_boss() {
    MYSQL *conn;
    connectToDatabase_boss(conn);

    char choice;

    while (true) {
        cout << "\n==================== �����ϰ�� ====================\n";
        cout << "1. �鿴���ж���\n";
        cout << "2. ͳ�Ʋ�ѯ\n";
        cout << "0. �˳�\n";
        cout << "��ѡ�����: ";
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
                cout << "��Ч��ѡ�������ԡ�\n";
        }
    }

    mysql_close(conn);
}

void connectToDatabase_boss(MYSQL *&conn) {
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

void displayAllOrders(MYSQL *conn) {
    string query = "SELECT order_id, table_id, dish_name, quantity, total_price, status FROM orders";
    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL ��ѯʧ��: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    cout << "\n==================== ���ж��� ====================\n";
    cout << left << setw(10) << "����ID" << setw(10) << "����" << setw(20) << "��Ʒ����" << setw(10) << "����" << setw(10) <<
            "��???" << setw(10) << "״̬" << endl;
    cout << "-------------------------------------------------\n";
    while ((row = mysql_fetch_row(res))) {
        string status;
        if (row[5]) {
            int statusInt = stoi(row[5]);
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
        cerr << "ͳ�Ʋ�ѯʧ��: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);

    if (row && row[0]) {
        cout << "�����ѽ����ܽ��: " << row[0] << " Ԫ\n";
    } else {
        cout << "�������ѽ��㶩����\n";
    }
    mysql_free_result(res);

    query = "SELECT COUNT(DISTINCT table_id) FROM orders WHERE status = 2";
    if (mysql_query(conn, query.c_str())) {
        cerr << "ͳ�Ʋ�ѯʧ��: " << mysql_error(conn) << endl;
        return;
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    if (row && row[0]) {
        cout << "������ʹ����������: " << row[0] << " ��\n";
    } else {
        cout << "��������ʹ�����ӡ�\n";
    }
    mysql_free_result(res);
}
