//*************************************************************
//*                                                           *
//*  SafeL Database Layer                                     *
//*  Written for TTO and donated to the CrystalSpace project  *
//*  by Stuart Rickard <stu@nickyandstu.co.uk>                *
//*  modified by David H. Bronke <whitelynx@gmail.com>        *
//*  23/04/2006                                               *
//*                                                           *
//*************************************************************

#pragma once

#include "csutil/scf.h"
#include "csutil/hashr.h"
#include "csutil/array.h"
#include "csutil/csstring.h"
#include "csutil/stringarray.h"

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
    fieldMap.Empty();
    columnArray.DeleteAll();
    fieldArray.DeleteAll();  
  }

  void AddData (csStringArray fields, csStringArray columns)
  {
    valid = true;
    size_t fieldCount = fields.GetSize ();
    csString field;
    csString column;

    for (size_t index = 0; index < fieldCount; index++)
    {
      field = fields[index];
      column = columns[index];
      columnArray.Push (column);
      fieldArray.Push (field);
      fieldMap.Put (column, field);
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
    return fieldArray.GetSize ();
  }

  /**
   * Return the value of the given column.
   *
   * @return the value, or an empty string if the given column is not found.
   */
  const char* GetValue (const char* columnName) const
  {
    return fieldMap.Get (columnName, "");
  }

  /**
   * Return the value of the nth column.
   *
   * @return the value, or an empty string if the given column is not found.
   */
  const char* GetValue (size_t index) const
  {
    if (index < fieldArray.GetSize ())
    {
      return fieldArray[index];
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
    if (n < fieldArray.GetSize ())
    {
      return fieldArray[n];
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
    if (n < columnArray.GetSize ())
    {
      return columnArray[n];
    }
    else
    {
      return "";
    }
  }

private:
  bool valid;
  csHash<csString, csString> fieldMap;
  csArray<csString> columnArray;
  csArray<csString> fieldArray;
};


/**
 * An interface representing a result set.
 */
struct icDatabaseResultSet : public virtual iBase
{
public:
  SCF_INTERFACE (icDatabaseResultSet, 0, 0, 1);

  /**
   * Get the number of records in this result set.
   */
  virtual size_t GetRecordCount () = 0;

  /**
   * Retrieve the next record in the result set.
   *
   * This advances the result set's cursor to the next record and returns it.
   *
   * @return the next database record
   */
  virtual icDatabaseRecord NextRecord () = 0;
  virtual void ClearRecord (icDatabaseRecord* Record) = 0;
  virtual bool NextRecord (icDatabaseRecord* Record) = 0;
  virtual void NextRecord (char** rowBuffer, unsigned int* fieldcount, unsigned long* lengths) = 0;
  virtual void ResetSet (void) = 0;

};

#if 0
/**
 * An interface representing a database.
 */
struct iDatabase : public virtual iBase
{
public:
  SCF_INTERFACE (iDatabase, 0, 0, 1);

  /**
   * Connect to the database.
   *
   * @param host The hostname of the database server to connect to
   * @param username The username to use when connecting to the database server
   * @param password The password to use when connecting to the database server
   * @param database The name of the database to use
   * @param port The port on which to connect to the database server
   */
  virtual bool Connect (const char* host, const char* username,
                       const char* password, const char* database,
                       unsigned short port) = 0;

  /**
   * Disconnect from the database.
   */
  virtual void Disconnect () = 0;

  /**
   * Perform the given SQL command.
   *
   * @param cmd The SQL command to execute
   *
   * @return FIXME: a long??
   */
  virtual long acCommand (const char *cmd) = 0;

  /**
   * Perform a SQL command and return a result set.
   *
   * @param cmd The SQL command to execute
   *
   * @return the result set
   */
  virtual csPtr<icDatabaseResultSet> acSelectCommand (const char* cmd) = 0;

  /**
   * Perform the given SQL command and return a scalar result.
   *
   * @param cmd The SQL command to execute
   *
   * @return the result value
   */
  virtual int GetScalar (const char* cmd) = 0;

  /**
   * TODO: Retrieve the ID of the last inserted record. (?)
   *
   * @return the ID of the last inserted record
   */
  virtual int GetLastInsertID () = 0;

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
  virtual unsigned int Insert (const char *table, const char **fields, const char **values, int fieldcount) = 0;

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
  virtual bool Update (const char *table, const char **fields, const char **values, int fieldcount, const char *whereclause) = 0;

  /**
   * This function is used to create a legal SQL string that you can use in an SQL statement.
   *
   * @param string     Escaped string
   * @param data       Un-escaped data buffer
   * @param datalength Length of data buffer
   */
  virtual void EscapeString(csString& string, const char * data, unsigned long datalength) = 0;

  virtual bool ExecutePreparedStatement(const char* SQL, unsigned char inParamCount, char** inBuffer, unsigned long* inLengths, unsigned char* ourParamCount, char** outBuffer, unsigned long* outLengths) = 0;

};
#endif

#endif