#include <mysql.h>
#include <string>
#include <iostream>
#include "cashier.h"
#include "cooker.h"
#include "customer.h"
#include "waiter.h"
#include "boss.h"
#include "administrator.h"

using namespace std;

void connectToDatabase(MYSQL *&conn);

int login(MYSQL *conn, const string &username, const string &password);

int main() {
    MYSQL *conn;
    connectToDatabase(conn);

    char choice;
    string username, password;
    int role = -1;

    while (true) {
        cout << "\n==================== ��������ϵͳ ====================\n";
        cout << "1. ����Ա\n";
        cout << "2. ��ʦ\n";
        cout << "3. �˿�\n";
        cout << "4. ����Ա\n";
        cout << "5. �ϰ�\n";
        cout << "6. ����Ա\n";
        cout << "0. �˳�\n";
        cout << "��ѡ�����: ";
        cin >> choice;

        if (choice == '3') {
            run_customer();
            continue;
        }

        while (role == -1) {
            cout << "�������û���: ";
            cin >> username;
            cout << "����������: ";
            cin >> password;
            role = login(conn, username, password);
        }

        switch (choice) {
            case '1':
                if (role >= 1) run_cashier();
                else cout << "Ȩ�޲��㣬�޷�ִ�д˲�����\n";
                break;
            case '2':
                if (role >= 1) run_cooker();
                else cout << "Ȩ�޲��㣬�޷�ִ�д˲�����\n";
                break;
            case '4':
                if (role >= 1) run_waiter();
                else cout << "Ȩ�޲��㣬�޷�ִ�д˲�����\n";
                break;
            case '5':
                if (role >= 2) run_boss();
                else cout << "Ȩ�޲��㣬�޷�ִ�д˲�����\n";
                break;
            case '6':
                if (role >= 2) run_administrator();
                else cout << "Ȩ�޲��㣬�޷�ִ�д˲�����\n";
                break;
            case '0':
                cout << "�������˳�\n";
                mysql_close(conn);
                return 0;
            default:
                cout << "��Ч��ѡ�������ԡ�\n";
        }

        role = -1; // Reset role for next login
    }

    mysql_close(conn);
    return 0;
}

void connectToDatabase(MYSQL *&conn) {
    const char *server = "localhost";
    const char *user = "root";
    const char *password = "123456789"; // MySQL ����
    const char *database = "restaurant"; // Corrected database name
    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        cerr << "MySQL ����ʧ��: " << mysql_error(conn) << endl;
        exit(1);
    }
}

int login(MYSQL *conn, const string &username, const string &password) {
    string query = "SELECT role FROM users WHERE username = '" + username + "' AND password = '" + password + "'";
    if (mysql_query(conn, query.c_str())) {
        cerr << "��¼��ѯʧ��: " << mysql_error(conn) << endl;
        return -1;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (res == NULL) {
        cerr << "��ȡ��ѯ���ʧ��: " << mysql_error(conn) << endl;
        return -1;
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    if (row == NULL) {
        cout << "�û������������.\n";
        mysql_free_result(res);
        return -1;
    }

    int role = stoi(row[0]);
    mysql_free_result(res);
    return role;
}