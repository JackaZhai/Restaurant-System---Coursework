#include <mysql.h>
#include <iostream>
#include <cstdlib>

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    // ��ʼ��MySQL����
    conn = mysql_init(NULL);

    // ����ʼ���Ƿ�ɹ�
    if (conn == NULL) {
        std::cerr << "mysql_init() failed" << std::endl;
        exit(1);
    }

    // �������ݿ�
    if (mysql_real_connect(conn, "localhost", "root", "123456789",
            "database_name", 0, NULL, 0) == NULL) {
        std::cerr << "mysql_real_connect() failed. Error: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        exit(1);
            }

    // ִ��SQL��ѯ
    if (mysql_query(conn, "SELECT * FROM table_name")) {
        std::cerr << "SELECT * error: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        exit(1);
    }

    // ��ȡ��ѯ���
    res = mysql_store_result(conn);
    if (res == NULL) {
        std::cerr << "mysql_store_result() failed. Error: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        exit(1);
    }

    // �����ѯ���
    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            std::cout << (row[i] ? row[i] : "NULL") << " ";
        }
        std::cout << std::endl;
    }

    // �ͷŲ�ѯ���
    mysql_free_result(res);

    // �ر����ݿ�����
    mysql_close(conn);

    return 0;
}
