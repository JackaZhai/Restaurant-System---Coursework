#ifndef CASHIER_H
#define CASHIER_H

#include <mysql.h>
#include <string>

void connectToDatabase_cashier(MYSQL *&conn);
void updateTableStatus(MYSQL *conn, int table_id, int status);
void settleBill(MYSQL *conn, int table_id);
void dailyStatistics(MYSQL *conn);
void dailyDishStatistics(MYSQL *conn);
void run_cashier();

#endif // CASHIER_H