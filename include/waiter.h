#ifndef WAITER_H
#define WAITER_H

#include <mysql.h>
#include <string>

void connectToDatabase_cashier(MYSQL *&conn);
void displayOrders(MYSQL *conn, int table_id);
void updateOrderStatus(MYSQL *conn, int order_id, int status);
void addItemsToOrder(MYSQL *conn, int table_id, int dish_id, int quantity);
void run_waiter();

#endif // WAITER_H