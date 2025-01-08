#ifndef BOSS_H
#define BOSS_H

#include <mysql.h>
#include <string>

void connectToDatabase_boss(MYSQL *&conn);
void displayAllOrders(MYSQL *conn);
void dailyStatistics(MYSQL *conn);
void run_boss();

#endif // BOSS_H