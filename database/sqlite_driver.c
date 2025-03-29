#ifndef sqlite_driver
#define sqlite_driver
#include <sqlite3.h>

void sqlite_connect()
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("test.db", &db);

    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }
    sqlite3_close(db);
}

static int sqlite_callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for (i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void sqlite_create_database()
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;

    /* Open database */
    rc = sqlite3_open("test.db", &db);

    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }
    else
    {
        fprintf(stdout, "Opened database successfully\n");
    }

    /* Create SQL statement */
    sql = "CREATE TABLE GAME_SAVE("
          "CHARACTER_X    INT     NOT NULL,"
          "CHARACTER_Y    INT     NOT NULL,"
          "TIMESTAMP      INT     NOT NULL);";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, sqlite_callback, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Table created successfully\n");
    }
    sqlite3_close(db);
}

void sqlite_select(int (*callback)(void*, int, char**, char**))
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;
    const char *data = "Callback function called";

    /* Open database */
    rc = sqlite3_open("test.db", &db);

    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create SQL statement */
    sql = "SELECT "
        "CHARACTER_X as 'X'," 
        "CHARACTER_Y as 'Y',"
        "TIMESTAMP   as 'T' " 
        "from GAME_SAVE";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_close(db);
}

void sqlite_insert(int x, int y)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;

    /* Open database */
    rc = sqlite3_open("test.db", &db);

    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create SQL statement */
    time_t timestamp = time( NULL );
    sql = (char*) malloc(256);
    sprintf(sql, "INSERT INTO GAME_SAVE (CHARACTER_X,CHARACTER_Y,TIMESTAMP) VALUES (%i,%i,%li); ", x, y, timestamp);
    printf(sql);
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, sqlite_callback, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Records created successfully\n");
    }
    sqlite3_close(db);
    free(sql);
}
#endif