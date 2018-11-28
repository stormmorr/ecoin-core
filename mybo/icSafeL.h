/*

	icSafeL - SafeL Database Layer
	Copyright OSIREM (C) 2007
	www.bitolyl.com/osirem  svn://62.44.82.8/unification

	This source is proprietary, and cannot be used, in part or in full without
	the express permission of the original author. The original author retains
	the rights to use, modify, and/or relicense this code without notice.

*/

#ifndef icSafeL_h
#ifndef icMySQL_h
#define icSafeL_h

#include <string>
#include <vector>
#include <map>

#include <windows.h>
#include <winsock2.h>

#include "safel.h"

/**
 * A structure representing a single record (or row) from a database query.
 */

struct icDatabaseRecord
{
public:
  /// Constructor.
  icDatabaseRecord ()
  {
    valid = false;
  }

  ~icDatabaseRecord ()
  {
    Clear ();
  }

  void Clear ()
  {
	fieldMap.empty();
    columnArray.clear();
    fieldArray.clear();
  }

  void AddData (std::vector<std::string> fields, std::vector<std::string> columns)
  {
    valid = true;
    size_t fieldCount = fields.size ();
    std::string field;
    std::string column;

    for (size_t index = 0; index < fieldCount; index++)
    {
      field = fields[index];
      column = columns[index];
      columnArray.push_back (column);
      fieldArray.push_back (field);
	  fieldMap[column] = field;
    }
  }

  /**
   * Check whether this record is valid or not.
   *
   * @return true if the record is valid, false otherwise.
   */
  bool IsValid ()
  {
    return valid;
  }

  /**
   * Get the number of fields in this record set.
   */
  size_t GetFieldCount ()
  {
    return fieldArray.size ();
  }

  /**
   * Return the value of the given column.
   *
   * @return the value, or an empty string if the given column is not found.
   */
  const char* GetValue (const char* columnName)
  {
	  return fieldMap[columnName].c_str();
  }

  /**
   * Return the value of the nth column.
   *
   * @return the value, or an empty string if the given column is not found.
   */
  const char* GetValue (size_t index) const
  {
    if (index < fieldArray.size ())
    {
	  return fieldArray[index].c_str();
    }
    else
    {
      return "";
    }
  }

  /**
   * Return the value of the nth column.
   *
   * @return the value, or an empty string if the given column is not found.
   */
  const char* operator [] (size_t n) const
  {
    if (n < fieldArray.size ())
    {
	  return fieldArray[n].c_str();
    }
    else
    {
      return "";
    }
  }

  /**
   * Return the name of the nth column.
   *
   * @return the name, or an empty string if the given column is not found.
   */
  const char* GetColumnName (size_t n) const
  {
    if (n < columnArray.size ())
    {
      return columnArray[n].c_str();
    }
    else
    {
      return "";
    }
  }

private:
  bool valid;
  std::map<std::string, std::string> fieldMap;
  std::vector<std::string> columnArray;
  std::vector<std::string> fieldArray;
#if 0
  bool valid;
  csHash<csString, csString> fieldMap;
  csArray<csString> columnArray;
  csArray<csString> fieldArray;
#endif
};

class icSafeLResultSet
{
public:
  /// Default constructor.
  icSafeLResultSet ();

  /**
   * Constructor.
   *
   * @param resultSet The SafeL result set to clone
   */
  icSafeLResultSet (SAFEL_RES* resultSet);

  /// Destructor.
  ~icSafeLResultSet ();

  /**
   * Get the number of records in this result set.
   */
  virtual size_t GetRecordCount ();

  /**
   * Retrieve the next record in the result set.
   *
   * This advances the result set's cursor to the next record and returns it.
   *
   * @return the next database record
   */
  icDatabaseRecord NextRecord ();
  bool NextRecord (icDatabaseRecord* Record);
  void ClearRecord (icDatabaseRecord* Record);
  void NextRecord (char** rowBuffer, unsigned int* fieldcount, unsigned long* lengths);
  void ResetSet (void);

private:
  /// The result set structure.
  SAFEL_RES* recordset;
};

class icSafeL
{
public:
  icSafeL ();
  virtual ~icSafeL ();

  /// iComponent Implementation
  bool Initialize (void);

  /**
   * Connect to the database.
   *
   * @param host The hostname of the database server to connect to
   * @param username The username to use when connecting to the database server
   * @param password The password to use when connecting to the database server
   * @param database The name of the database to use
   * @param port The port on which to connect to the database server
   */
  bool Connect (const char* host, const char* username,
                       const char* password, const char* database,
                       unsigned short port);

  /**
   * Disconnect from the database.
   */
  void Disconnect ();

  /**
   * Perform the given SQL command.
   *
   * @param cmd The SQL command to execute
   *
   * @return FIXME: a long??
   */
  long acCommand (const char *cmd, int f_UI, bool f_End, bool f_Update = false);

  /**
   * Perform a SQL command and return a result set.
   *
   * @param cmd The SQL command to execute
   *
   * @return the result set
   */
  bool acSelectCommand (const char* cmd, int f_UI, bool f_End);

  /**
   * Perform the given SQL command and return a scalar result.
   *
   * @param cmd The SQL command to execute
   *
   * @return the result value
   */
  int GetScalar (const char* cmd);

  /**
   * TODO: Retrieve the ID of the last inserted record. (?)
   *
   * @return the ID of the last inserted record
   */
  int GetLastInsertID ();

  /**
   * Insert a record into a database table.
   *
   * @param table The table to modify
   * @param fields The fields to populate in the new record
   * @param values The values for each field in the new record
   * @param fieldcount The number of fields to add to the new record
   *
   * @return FIXME: an unsigned int??
   */
  unsigned int Insert (const char *table, const char **fields, const char **values, int fieldcount, int f_UI, bool f_End);
  unsigned int Insert(const char *table, std::vector<std::string> f_vec_Field, std::vector<std::string> f_vec_Value, int fieldcount);

  /**
   * Modify (UPDATE) a record in a database table.
   *
   * @param table The table to modify
   * @param fields The fields to populate in the record
   * @param values The values for each field in the record
   * @param fieldcount The number of fields to modify in the record
   * @param whereclause The WHERE clause defining which records to modify
   *
   * @return FIXME: a bool??
   */
  bool Update (const char *table, const char **fields, const char **values, int fieldcount, const char *whereclause, int f_UI, bool f_End);

  /**
   * This function is used to create a legal SQL string that you can use in an SQL statement.
   *
   * @param string     Escaped string
   * @param data       Un-escaped data buffer
   * @param datalength Length of data buffer
   */
  void EscapeString(std::string& string, const char * data, unsigned long datalength);

  bool ExecutePreparedStatement(const char* SQL, unsigned char inParamCount, char** inBuffer, unsigned long* inLengths, unsigned char* ourParamCount, char** outBuffer, unsigned long* outLengths);

private:
  SAFEL *connection;

  /// Internal version of acSelectCommand.
  bool IssueSelectCommand (const char* cmd, int f_UI, bool f_End);
};

#endif
#endif