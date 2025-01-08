#include <mysql.h>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

void connectToDatabase_administrator(MYSQL *&conn); // ���ӵ����ݿ�
void initializeTables(MYSQL *conn); // ��ʼ����
void addDish(MYSQL *conn, const string &name, double price); // ��Ӳ�Ʒ
void deleteDish(MYSQL *conn, const string &name); // ɾ����Ʒ
void addDrink(MYSQL *conn, const string &name, double price); // ��Ӿ�ˮ
void deleteDrink(MYSQL *conn, const string &name); // ɾ����ˮ
void addTable(MYSQL *conn, int table_id, int capacity); // ��Ӳ�λ
void deleteTable(MYSQL *conn, int table_id); // ɾ����λ
void createUser(MYSQL *conn, const string &username, const string &password, int role);
void deleteUser(MYSQL *conn, const string &username);

void createUser(MYSQL *conn, const string &username, const string &password, int role) {
    string query = "INSERT INTO users (username, password, role) VALUES ('" + username + "', '" + password + "', " + to_string(role) + ")";
    if (mysql_query(conn, query.c_str())) {
        cerr << "�����û�ʧ��: " << mysql_error(conn) << endl;
    } else {
        cout << "�û������ɹ�.\n";
    }
}

void deleteUser(MYSQL *conn, const string &username) {
    string query = "DELETE FROM users WHERE username = '" + username + "'";
    if (mysql_query(conn, query.c_str())) {
        cerr << "ɾ���û�ʧ��: " << mysql_error(conn) << endl;
    } else {
        cout << "�û�ɾ���ɹ�.\n";
    }
}
void run_administrator() {
    MYSQL *conn;
    connectToDatabase_administrator(conn);

    char choice;
    while (true) {
        cout << "\n==================== ϵͳ����Ա�� ====================\n";
        cout << "1. ϵͳ��ʼ��\n";
        cout << "2. ��Ӳ�Ʒ\n";
        cout << "3. ɾ����Ʒ\n";
        cout << "4. ��Ӿ�ˮ\n";
        cout << "5. ɾ����ˮ\n";
        cout << "6. ��Ӳ�λ\n";
        cout << "7. ɾ����λ\n";
        cout << "8. �����û�\n";
        cout << "9. ɾ���û�\n";
        cout << "0. �˳�\n";
        cout << "��ѡ�����: ";
        cin >> choice;

        switch (choice) {
            case '1':
                initializeTables(conn);
                break;
            case '2': {
                string name;
                double price;
                cout << "�������Ʒ����: ";
                cin >> name;
                cout << "�������Ʒ�۸�: ";
                cin >> price;
                addDish(conn, name, price);
                break;
            }
            case '3': {
                string name;
                cout << "������Ҫɾ���Ĳ�Ʒ����: ";
                cin >> name;
                deleteDish(conn, name);
                break;
            }
            case '4': {
                string name;
                double price;
                cout << "�������ˮ����: ";
                cin >> name;
                cout << "�������ˮ�۸�: ";
                cin >> price;
                addDrink(conn, name, price);
                break;
            }
            case '5': {
                string name;
                cout << "������Ҫɾ���ľ�ˮ����: ";
                cin >> name;
                deleteDrink(conn, name);
                break;
            }
            case '6': {
                int table_id, capacity;
                cout << "�������λID: ";
                cin >> table_id;
                cout << "�������λ����: ";
                cin >> capacity;
                addTable(conn, table_id, capacity);
                break;
            }
            case '7': {
                int table_id;
                cout << "������Ҫɾ���Ĳ�λID: ";
                cin >> table_id;
                deleteTable(conn, table_id);
                break;
            }
            case '8': {
                string username, password;
                int role;
                cout << "���������û���: ";
                cin >> username;
                cout << "������������: ";
                cin >> password;
                cout << "�������ɫ (0: �˿�, 1: ����Ա/��ʦ/����Ա, 2: �ϰ�/����Ա): ";
                cin >> role;
                createUser(conn, username, password, role);
                break;
            }
            case '9': {
                string username;
                cout << "������Ҫɾ�����û���: ";
                cin >> username;
                deleteUser(conn, username);
                break;
            }
            case '0':
                mysql_close(conn);
                return;
            default:
                cout << "��Ч��ѡ�������ԡ�\n";
        }
    }

    mysql_close(conn);
}

void connectToDatabase_administrator(MYSQL *&conn) {
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

// ��ʼ����
void initializeTables(MYSQL *conn) {
    // ɾ�� orders �������
    if (mysql_query(conn, "DELETE FROM orders")) {
        cerr << "ɾ�� orders ������ʧ��: " << mysql_error(conn) << endl;
        return;
    } else {
        cout << "orders ��������ɾ��.\n";
    }

    // ���� orders �������ֵ
    if (mysql_query(conn, "ALTER TABLE orders AUTO_INCREMENT = 0")) {
        cerr << "���� orders ������ֵʧ��: " << mysql_error(conn) << endl;
        return;
    } else {
        cout << "orders ������ֵ������Ϊ 0.\n";
    }

    // �� tables ���״̬����Ϊ 0
    if (mysql_query(conn, "UPDATE tables SET status = 0")) {
        cerr << "���� tables ��״̬ʧ��: " << mysql_error(conn) << endl;
        return;
    } else {
        cout << "tables ��״̬�Ѹ���Ϊ 0.\n";
    }
}

// ��Ӳ�Ʒ
void addDish(MYSQL *conn, const string &name, double price) {
    string query = "INSERT INTO dishes (name, price) VALUES ('" + name + "', " + to_string(price) + ")";
    if (mysql_query(conn, query.c_str())) {
        cerr << "��Ӳ�Ʒʧ��: " << mysql_error(conn) << endl;
    } else {
        cout << "��Ʒ��ӳɹ�.\n";
    }
}

// ɾ����Ʒ
void deleteDish(MYSQL *conn, const string &name) {
    string query = "DELETE FROM dishes WHERE name = '" + name + "'";
    if (mysql_query(conn, query.c_str())) {
        cerr << "ɾ����Ʒʧ��: " << mysql_error(conn) << endl;
    } else {
        cout << "��Ʒɾ���ɹ�.\n";
    }
}

// ��Ӿ�ˮ
void addDrink(MYSQL *conn, const string &name, double price) {
    string query = "INSERT INTO drinks (name, price) VALUES ('" + name + "', " + to_string(price) + ")";
    if (mysql_query(conn, query.c_str())) {
        cerr << "��Ӿ�ˮʧ��: " << mysql_error(conn) << endl;
    } else {
        cout << "��ˮ��ӳɹ�.\n";
    }
}

// ɾ����ˮ
void deleteDrink(MYSQL *conn, const string &name) {
    string query = "DELETE FROM drinks WHERE name = '" + name + "'";
    if (mysql_query(conn, query.c_str())) {
        cerr << "ɾ����ˮʧ��: " << mysql_error(conn) << endl;
    } else {
        cout << "��ˮɾ���ɹ�.\n";
    }
}

// ��Ӳ�λ
void addTable(MYSQL *conn, int table_id, int capacity) {
    string query = "INSERT INTO tables (table_id, capacity, status) VALUES (" + to_string(table_id) + ", " +
                   to_string(capacity) + ", 0)";
    if (mysql_query(conn, query.c_str())) {
        cerr << "��Ӳ�λʧ��: " << mysql_error(conn) << endl;
    } else {
        cout << "��λ��ӳɹ�.\n";
    }
}

// ɾ����λ
void deleteTable(MYSQL *conn, int table_id) {
    string query = "DELETE FROM tables WHERE table_id = " + to_string(table_id);
    if (mysql_query(conn, query.c_str())) {
        cerr << "ɾ����λʧ��: " << mysql_error(conn) << endl;
    } else {
        cout << "��λɾ���ɹ�.\n";
    }
}
