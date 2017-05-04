#include "DatabaseQuery.h"

DatabaseQuery *DatabaseQuery::m_pDatabaseQuery = nullptr;

DatabaseQuery::DatabaseQuery()
{

}

DatabaseQuery::~DatabaseQuery()
{

}

DatabaseQuery *DatabaseQuery::instance()
{
    return m_pDatabaseQuery;
}

void DatabaseQuery::disinstance()
{
    if(m_pDatabaseQuery != nullptr)
    {
        delete m_pDatabaseQuery;
        m_pDatabaseQuery = nullptr;
    }
}
