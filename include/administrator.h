#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include <mysql.h>
#include <string>

void connectToDatabase_administrator(MYSQL *&conn);
void initializeTables(MYSQL *conn);
void addDish(MYSQL *conn, const std::string &name, double price);
void deleteDish(MYSQL *conn, const std::string &name);
void addDrink(MYSQL *conn, const std::string &name, double price);
void deleteDrink(MYSQL *conn, const std::string &name);
void addTable(MYSQL *conn, int table_id, int capacity);
void deleteTable(MYSQL *conn, int table_id);
void run_administrator();

#endif // ADMINISTRATOR_H