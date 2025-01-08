// src/cashier.cpp
#include <mysql.h>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

void connectToDatabase_cashier(MYSQL *&conn); // ���ӵ����ݿ�
void updateTableStatus_cashier(MYSQL *conn, int table_id, int status); // ��������״̬
void settleBill(MYSQL *conn, int table_id); // ����СƱ
void dailyStatistics_cashier(MYSQL *conn); // ͳ�Ƶ������
void dailyDishStatistics_cashier(MYSQL *conn); // ͳ�Ƶ��������������Ʒ

void run_cashier() {
    MYSQL *conn;
    connectToDatabase_cashier(conn);

    char choice;
    while (true) {
        cout << "\n==================== ����Ա�� ====================\n";
        cout << "1. ����СƱ\n";
        cout << "2. ͳ�Ƶ������\n";
        cout << "3. ͳ�Ƶ��������������Ʒ\n";
        cout << "0. �˳�\n";
        cout << "��ѡ�����: ";
        cin >> choice;

        switch (choice) {
            case '1': {
                int table_id;
                cout << "����������ID: ";
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
                cout << "��Ч��ѡ�������ԡ�\n";
        }
    }

    mysql_close(conn);
}

// ���ӵ����ݿ�
void connectToDatabase_cashier(MYSQL *&conn) {
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

// ��������״̬
void updateTableStatus_cashier(MYSQL *conn, int table_id, int status) {
    string query = "UPDATE tables SET status = " + to_string(status) + " WHERE table_id = " + to_string(table_id);
    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL ��ѯʧ��: " << mysql_error(conn) << endl;
    }
}

// ����СƱ
void settleBill(MYSQL *conn, int table_id) {
    string query = "SELECT dish_name, price, quantity, total_price FROM orders WHERE table_id = " + to_string(table_id)
                   + " AND status = 2";
    if (mysql_query(conn, query.c_str())) {
        cerr << "��ѯ����ʧ��: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    double total_price = 0;
    cout << "\n==================== ����СƱ ====================\n";
    cout << "����: " << table_id << endl;
    cout << left << setw(20) << "��Ʒ����" << setw(10) << "����" << setw(10) << "����" << setw(10) << "�ܼ�" << endl;
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
        cout << "���ѽ���500Ԫ���������ò�����: ";
        cin >> num_people;
        double table_fee = num_people * 5;
        total_price += table_fee;
        cout << "��ȡ��λ��: " << table_fee << " Ԫ\n";
    }

    cout << "�����ܽ��: " << total_price << " Ԫ\n";
    cout << "===============================================\n";

    mysql_free_result(res);

    // ��������״̬Ϊ0
    updateTableStatus_cashier(conn, table_id, 0);
}

// ͳ�Ƶ������
void dailyStatistics_cashier(MYSQL *conn) {
    // ��ѯ�ѽ��㶩�����ܽ�������
    string query = "SELECT table_id, SUM(total_price) FROM orders WHERE status = 2 GROUP BY table_id";
    if (mysql_query(conn, query.c_str())) {
        cerr << "ͳ�Ʋ�ѯʧ��: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    cout << "\n�����ѽ��㶩��:\n";
    cout << left << setw(10) << "����" << setw(10) << "�ܽ��" << endl;
    cout << "----------------------\n";
    while ((row = mysql_fetch_row(res))) {
        cout << left << setw(10) << (row[0] ? row[0] : "NULL") << setw(10) << (row[1] ? row[1] : "NULL") << " Ԫ" <<
                endl;
    }
    cout << "======================\n";

    mysql_free_result(res);
}

// ͳ�Ƶ��������������Ʒ
void dailyDishStatistics_cashier(MYSQL *conn) {
    string query = "SELECT table_id, dish_name, status, quantity FROM orders WHERE status IN (0, 1) ORDER BY table_id";
    if (mysql_query(conn, query.c_str())) {
        cerr << "ͳ�Ʋ�ѯʧ��: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    cout << "\n���������������Ʒͳ��:\n";
    cout << left << setw(10) << "����" << setw(20) << "��Ʒ����" << setw(10) << "״̬" << setw(10) << "����" << endl;
    cout << "-------------------------------------------------\n";
    while ((row = mysql_fetch_row(res))) {
        string status;
        if (row[2]) {
            int statusInt = stoi(row[2]);
            switch (statusInt) {
                case 0: status = "����";
                    break;
                case 1: status = "����";
                    break;
                default: status = "δ֪";
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
