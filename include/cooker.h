#ifndef COOKER_H
#define COOKER_H

#include <mysql.h>
#include <string>

void connectToDatabase_cooker(MYSQL *&conn);
void displayPendingOrders(MYSQL *conn);
void updateOrderStatus(MYSQL *conn, int order_id, int status);
void run_cooker();

#endif // COOKER_H