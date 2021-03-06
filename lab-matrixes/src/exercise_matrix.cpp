#include <iomanip>
#include <iostream>
#include "../matrix-lib/matrix.h"

using namespace std;

void printLine() {
    cout << endl;
    for (int i = 0; i < 48; i++) {
        cout << "#";
    }
    cout << endl;
}

void printLineError(const char *msg) {
    cout << msg;
}

void matrixTests() {
    try {
        cout << "Czytam macierz m1 z pliku: " << endl;
        Matrix m1 = Matrix("m1.txt", "G:/School/PK ROK 2/JiPP/jipp-labs-pk/lab-matrixes");
        m1.print();

        printLine();

        cout << "Wykonuje: cout << m1:" << endl;

        cout << m1;

        printLine();

        cout << "Czytam macierz m2 z pliku: " << endl;
        Matrix m2 = Matrix("m2.txt", "G:/School/PK ROK 2/JiPP/jipp-labs-pk/lab-matrixes");
        m2.print();

        printLine();

        cout << "Set m1[2][2] as m1[2][2]" << endl;
        m1.set(2, 2, m1.get(2, 2));
        cout << "M1[2][2] = " << m1.get(2, 2) << endl;

        printLine();

        cout << "Get m1[2][2]" << endl;
        cout << "M1[2][2] = " << m1.get(2, 2) << endl;

        printLine();

        cout << "Wykonuje: m1 + m2:" << endl;
        Matrix temp = m1 + m2;
        temp.print();

        printLine();

        cout << "Wykonuje: m1 - m2:" << endl;
        temp = m1 - m2;
        temp.print();

        printLine();

        cout << "Wykonuje: m1 * m2:" << endl;
        temp = m1 * m2;
        temp.print();

        printLine();

        cout << "Wykonuje: m1 * m2 == m1:" << endl;
        cout <<(m1*m2 == m1);
        printLine();

        cout << "Wykonuje: m1 == m1:" << endl;
        cout <<(m1 == m1);
        printLine();

        cout << "Zapisuje m1 * m2 do pliki m3.txt" << endl;
        temp.store("m3.txt", ".");

        cout << "-- Koniec Demonstracji  --" << endl << endl;


    }
    catch (matrix_error &e) {
        printLineError(e.what());
    }
}

sqlite3 *connect() {
    sqlite3 *db;

    int resultCode = sqlite3_open("db.sqlite", &db);
    if (resultCode) {
        cerr << "Can't open database: " << endl << sqlite3_errmsg(db) << endl;
        exit(1);
    } else {
        cout << "Opened database successfully" << endl;
    }

    return db;
}

void createTable(sqlite3 *db) {
    const char *sql;
    char **errMsg = nullptr;
    int resultCode;

    sql = "CREATE TABLE IF NOT EXISTS matrices(\n"
          "    matrix_name VARCHAR(128) PRIMARY KEY,\n"
          "    n INTEGER NOT NULL,\n"
          "    m INTEGER NOT NULL,\n"
          "    data TEXT NOT NULL\n"
          ");";

//    sql = "CREATE TABLE matrices2(INTEGER);";
    resultCode = sqlite3_exec(db, sql, nullptr, nullptr, errMsg);

    if (resultCode != SQLITE_OK) {
        cerr << "Error creating table:" << endl << errMsg << endl;
        exit(1);
    }
}

void clearTable(sqlite3 *db) {
    const char *sql;
    char **errMsg = nullptr;
    int rc;

    sql = "DELETE FROM matrices";
    rc = sqlite3_exec(db, sql, nullptr, nullptr, errMsg);

    if (rc != SQLITE_OK) {
        cerr << "Error clearing table \"matrices\":" << endl << errMsg << endl;
        exit(1);
    }
}

void databaseTest(sqlite3 *db) {
    try {
        cout << "Testing SQLITE" << endl;

        Matrix testMatrix(2, 5);
        testMatrix.set(0, 0, 1);
        testMatrix.set(0, 1, 2);
        testMatrix.set(0, 2, 3);
        testMatrix.set(0, 3, 4);
        testMatrix.set(0, 4, 5);
        testMatrix.set(1, 0, 6);
        testMatrix.set(1, 1, 7);
        testMatrix.set(1, 2, 8);
        testMatrix.set(1, 3, 9);
        testMatrix.set(1, 4, 10);

        testMatrix.print();

        testMatrix.store(db, "testmatrix");

        Matrix downloadedMatrix(db, "testmatrix");
        downloadedMatrix.print();


    }
    catch (matrix_error &e) {
        printLineError(e.what());
    }

}

int main() {

    // join DB
    sqlite3 *db = connect();

    // create table in DB
    createTable(db);

    // clear table if something left
    clearTable(db);

    // test some DB operations
    databaseTest(db);

    //close connection

    sqlite3_close(db);

    // test some matrices operations
    matrixTests();
    return 0;
}
