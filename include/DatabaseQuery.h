#ifndef DATABASEQUERY_H
#define DATABASEQUERY_H

//单例实现
class DatabaseQuery
{
public:
    static DatabaseQuery *instance();

    static void disinstance();

protected:
    explicit DatabaseQuery();
    ~DatabaseQuery();

private:
    static DatabaseQuery *m_pDatabaseQuery;
};

#endif // DATABASEQUERY_H
