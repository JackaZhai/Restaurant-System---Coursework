#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <mysql.h>
#include <string>
#include <map>

void connectToDatabase_customer(MYSQL *&conn);
char getch();
void displayWelcomeScreen();
void displayCategories(MYSQL *conn, std::map<int, std::string> &categories);
void displayMenu(MYSQL *conn, const std::string &category);
void displayAvailableTables(MYSQL *conn);
void placeOrder(MYSQL *conn, int table_id, int dish_id, int quantity);
void updateTableStatus(MYSQL *conn, int table_id, int status);
void displayOrderSummary(MYSQL *conn, int table_id);
void run_customer();

#endif // CUSTOMER_H