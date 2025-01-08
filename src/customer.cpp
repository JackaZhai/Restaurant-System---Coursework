// src/customer.cpp
#include <mysql.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

using namespace std;

void connectToDatabase_customer(MYSQL *&conn); // ���ӵ����ݿ�
int getch(); // ��ȡ�û�����
void displayWelcomeScreen(); // ��ʾ��ӭ����
void displayCategories(MYSQL *conn, map<int, string> &categories); // ��ʾ��Ʒ����
void displayMenu(MYSQL *conn, const string &category); // ��ʾ�˵�
void displayAvailableTables(MYSQL *conn); // ��ʾ��������
void placeOrder(MYSQL *conn, int table_id, int dish_id, int quantity); // �¶���
void updateTableStatus(MYSQL *conn, int table_id, int status); // ��������״̬
void displayOrderSummary(MYSQL *conn, int table_id); // ��ʾ����ժҪ

void run_customer() {
    MYSQL *conn;
    connectToDatabase_customer(conn);

    int table_id, dish_id, quantity;
    char choice;
    map<int, string> categories;

    while (true) {
        displayWelcomeScreen();
        displayAvailableTables(conn);
        cout << "\n����������: ";
        cin >> table_id;
        updateTableStatus(conn, table_id, 1); // ��������״̬Ϊ��ռ��

        do {
            displayCategories(conn, categories);

            int category_choice;
            cout << "\n��ѡ���Ʒ���� (�� 0 �˳�): ";
            cin >> category_choice;

            if (category_choice == 0) {
                break;
            }

            if (categories.find(category_choice) != categories.end()) {
                displayMenu(conn, categories[category_choice]);

                cout << "�������Ʒ ID ���е㵥: ";
                cin >> dish_id;
                cout << "����������: ";
                cin >> quantity;

                placeOrder(conn, table_id, dish_id, quantity);

                cout << "�Ƿ����ѡ���Ʒ? (y/n): ";
                cin >> choice;
                if (choice == 'n' || choice == 'N') {
                    break;
                }
            } else {
                cout << "��Ч��ѡ�������ԡ�\n";
            }
        } while (true);

        displayOrderSummary(conn, table_id); // ��ʾ����ժҪ

        cout << "�Ƿ�����µ�? (y/n): ";
        cin >> choice;

        if (choice == 'n' || choice == 'N') {
            break;
        }
    }

    mysql_close(conn);
}

// ���ӵ����ݿ�
void connectToDatabase_customer(MYSQL *&conn) {
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

int getch() {
#ifdef _WIN32
    return _getch();
#else
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
#endif
}

// ��ʾ��ӭ����
void displayWelcomeScreen() {
    cout << "��ӭ���٣�" << endl;
    cout << "�����������˵�..." << endl;
    getch(); // �ȴ��û��������
}

// ��ʾ��Ʒ����
void displayCategories(MYSQL *conn, map<int, string> &categories) {
    if (mysql_query(conn, "SELECT DISTINCT category FROM dishes")) {
        cerr << "MySQL ��ѯʧ��: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;
    int index = 1;

    cout << "\n==================== ��Ʒ���� ====================\n";
    while ((row = mysql_fetch_row(res))) {
        cout << index << ". " << (row[0] ? row[0] : "NULL") << endl;
        categories[index] = row[0];
        index++;
    }
    cout << "0. �˳�\n";
    cout << "=============================================\n";

    mysql_free_result(res);
}

// ��ʾ�˵�
void displayMenu(MYSQL *conn, const string &category) {
    string query = "SELECT id, name, category, price FROM dishes WHERE category = '" + category + "'";
    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL ��ѯʧ��: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    cout << "\n=========================== �˵� ===========================\n";
    cout << "ID\t����\t\t\t����\t\t�۸�" << endl;
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

// ��ʾ��������
void displayAvailableTables(MYSQL *conn) {
    if (mysql_query(conn, "SELECT table_id, seats FROM tables WHERE status = 0")) {
        cerr << "MySQL ��ѯʧ��: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    cout << "\n==================== �������� ====================\n";
    cout << left << setw(10) << "����" << setw(10) << "��λ��" << endl;
    cout << "---------------------------------------------\n";
    while ((row = mysql_fetch_row(res))) {
        cout << left << setw(10) << (row[0] ? row[0] : "NULL") << setw(10) << (row[1] ? row[1] : "NULL") << endl;
    }
    cout << "=============================================\n";

    mysql_free_result(res);
}

// �¶���
void placeOrder(MYSQL *conn, int table_id, int dish_id, int quantity) {
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
            to_string(quantity) + ", " + to_string(total_price) +
            ", '0') ON DUPLICATE KEY UPDATE quantity = quantity + " +
            to_string(quantity) + ", total_price = total_price + " + to_string(total_price);

    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL ��ѯʧ��: " << mysql_error(conn) << endl;
    } else {
        cout << "�����ɹ�.\n";
    }
}

// ��������״̬
void updateTableStatus(MYSQL *conn, int table_id, int status) {
    string query = "UPDATE tables SET status = " + to_string(status) + " WHERE table_id = " + to_string(table_id);
    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL ��ѯʧ��: " << mysql_error(conn) << endl;
    }
}

// ���㲢��ʾ����ժҪ
void displayOrderSummary(MYSQL *conn, int table_id) {
    // Retrieve order ID, creation time, and status
    string query = "SELECT order_id, creation_time, status FROM orders WHERE table_id = " + to_string(table_id) +
                   " LIMIT 1";
    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL ��ѯʧ��: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);

    if (!row) {
        cerr << "δ�ҵ�������Ϣ��\n";
        mysql_free_result(res);
        return;
    }

    string order_id = row[0] ? row[0] : "NULL";
    string creation_time = row[1] ? row[1] : "NULL";
    string status;
    if (row[2]) {
        int statusInt = stoi(row[2]);
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

    mysql_free_result(res);

    // Retrieve detailed order summary
    query = "SELECT dish_name, price, SUM(quantity), SUM(total_price) FROM orders WHERE table_id = " +
            to_string(table_id) + " GROUP BY dish_name, price";
    if (mysql_query(conn, query.c_str())) {
        cerr << "MySQL ��ѯʧ��: " << mysql_error(conn) << endl;
        return;
    }

    res = mysql_store_result(conn);

    cout << "\n��ͳɹ���\n";
    cout << "���Ķ�����ϢΪ��\n";
    cout << "������ţ�" << order_id << "\n";
    cout << "��������ʱ�䣺" << creation_time << "\n";
    cout << "��ǰ�����ܼۣ�";
    double total_cost = 0;
    while ((row = mysql_fetch_row(res))) {
        total_cost += row[3] ? stod(row[3]) : 0;
    }
    cout << total_cost << " Ԫ\n";
    cout << "����״̬��" << status << "\n";
    cout << "��ϸ����Ϊ��\n";
    cout << left << setw(20) << "��Ʒ����" << setw(10) << "����" << setw(10) << "����" << setw(10) << "�ܼ�" << endl;
    cout << "-------------------------------------------------\n";
    mysql_data_seek(res, 0); // Reset result pointer to fetch rows again
    while ((row = mysql_fetch_row(res))) {
        cout << left << setw(20) << (row[0] ? row[0] : "NULL") << setw(10) << (row[1] ? row[1] : "NULL") << setw(10) <<
                (row[2] ? row[2] : "NULL") << setw(10) << (row[3] ? row[3] : "NULL") << endl;
    }
    cout << "=============================================\n";

    mysql_free_result(res);
}
