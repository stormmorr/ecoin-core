/*

	icSafeL - SafeL Database Layer
	Copyright OSIREM (C) 2007
	www.bitolyl.com/osirem  svn://62.44.82.8/unification

	This source is proprietary, and cannot be used, in part or in full without
	the express permission of the original author. The original author retains
	the rights to use, modify, and/or relicense this code without notice.

*/
#include "pch.h"
#include "icSafeL.h"

/**************************************
 * Implementation of icSafeLResultSet *
 **************************************/
 
/// Constructor.
icSafeLResultSet::icSafeLResultSet ()
{
}

icSafeLResultSet::icSafeLResultSet (SAFEL_RES* resultSet)
{
  recordset = resultSet;
}

/// Destructor.
icSafeLResultSet::~icSafeLResultSet ()
{
  //safel_free_result (recordset);
}

size_t icSafeLResultSet::GetRecordCount ()
{
  return recordset->row_count;
}
void icSafeLResultSet::ClearRecord (icDatabaseRecord* Record)
{
  Record->Clear ();
}
icDatabaseRecord icSafeLResultSet::NextRecord ()
{
  icDatabaseRecord rec;

	if(recordset)
		{
		char** row = 0;// = safel_fetch_row(recordset);

		if(!row)
		  {
		  return rec;
		  }

		unsigned int fieldcount = 0;// safel_num_fields(recordset);
		SAFEL_FIELD* fields = 0;// safel_fetch_fields(recordset);

		std::vector<std::string> fieldnames;
		std::vector<std::string> fieldvalues;

		  // (fieldcount)

		for(unsigned int index = 0; index < fieldcount; index++)
		  {
		  fieldnames.push_back (fields[index].name);
		  fieldvalues.push_back (row[index]);
		  }

		rec.AddData(fieldvalues, fieldnames);
		}
	else
		{
		printf("ERROR row missmatch\n");
		}

  return rec;
}

bool icSafeLResultSet::NextRecord (icDatabaseRecord* Record)
{
  if(!Record)
  {
    return false;
  }

  char** row = 0;// safel_fetch_row(recordset);
  if (!row)
  {
    return false;
  }

  unsigned int fieldcount = 0;// safel_num_fields(recordset);
  SAFEL_FIELD* fields = 0;// safel_fetch_fields(recordset);

  std::vector<std::string> fieldnames (fieldcount);
  std::vector<std::string> fieldvalues (fieldcount);

  Record->Clear ();

  for (unsigned int index = 0; index < fieldcount; index++)
  {
    fieldnames.push_back (fields[index].name);
    fieldvalues.push_back (row[index]);
  }

  Record->AddData(fieldvalues, fieldnames);

  return true;
}

void icSafeLResultSet::NextRecord (char** rowBuffer, unsigned int* fieldcount, unsigned long* lengths)
{
  //SAFEL_ROW row = safel_fetch_row (recordset);
 // *fieldcount = safel_num_fields (recordset);

  //rowBuffer = (char**)realloc(rowBuffer, *fieldcount * sizeof(char*));
  if(!rowBuffer)
  {
    printf("Error allocating rowBuffer\n");
    return;
  }
  //memcpy(lengths, safel_fetch_lengths(recordset), *fieldcount * sizeof(unsigned long));
  for(size_t i = 0; i < 1; i++)
  {
    //rowBuffer[i] = (char*)malloc((lengths[i] + 1) * sizeof(char));
    if(!rowBuffer[i])
    {
      printf("Error allocating rowBuffer[%d]\n", i);
      return;
    }
    //memcpy(rowBuffer[i], row[i], lengths[i]);
    //rowBuffer[i][lengths[i]] = 0;
  }
}

void icSafeLResultSet::ResetSet (void)
{
	//cursor.Reset();
}

/*******************************
 * Implementation of icSafeL *
 *******************************/

/// Constructor.
icSafeL::icSafeL (void)
{
  //connection = 0;
  /*if (safel_library_init (0, NULL, NULL))
  {
    printf("Error initialising SafeL Library!\n");
    exit (1);
  }*/
}

/// Destructor.
icSafeL::~icSafeL ()
{
  //Disconnect ();
  //safel_library_end ();
  //connection = 0;
}

void icSafeL::Disconnect ()
{
#if 0
  //safel_close (connection);
#endif
}

bool icSafeL::Initialize (void)
{
#if 0
  //connection = safel_init (NULL);
#endif

  return true;
}

bool icSafeL::Connect (const char* host, const char* username, const char* password, const char* database, unsigned short port)
{
	g_VisualServerMode = 1;

	g_NetMode = ECOIN_NET_MINER;
	
	g_Schedular->acNet_Client_Data();
	
	printf("Connected to data-layer\n");

	return true;
}

long icSafeL::acCommand (const char *cmd, int f_UI, bool f_End, bool f_Update)
{
	//printf ("\nReceived DB Command: %s\n", cmd);
	fflush (stdout);
	if(f_Update)
		{
		if(!g_Schedular->acSend_Query_CMD((char *)cmd, f_UI, f_End), true)// (connection, cmd, strlen (cmd)))
			{
			printf("Query Failed. Retrying...\n");
			fflush (stdout);

			//if it fails once try again as it should re-connect
			if(!g_Schedular->acSend_Query_CMD((char *)cmd, f_UI, f_End), true)// (connection, cmd, strlen (cmd)))
				{
				printf("Failed database query\n");
				return false;
				}
			}
		}
	else
		{
		if(!g_Schedular->acSend_Query_CMD((char *)cmd, f_UI, f_End))// (connection, cmd, strlen (cmd)))
			{
			printf("Query Failed. Retrying...\n");
			fflush (stdout);

			//if it fails once try again as it should re-connect
			if(!g_Schedular->acSend_Query_CMD((char *)cmd, f_UI, f_End))// (connection, cmd, strlen (cmd)))
				{
				printf("Failed database query\n");
				return false;
				}
			}
		}

	return true;
}

bool icSafeL::acSelectCommand (const char* cmd, int f_UI, bool f_End)
{
  IssueSelectCommand (cmd, f_UI, f_End);

  return true;
}

bool icSafeL::IssueSelectCommand (const char* cmd, int f_UI, bool f_End)
{
	if(!g_Schedular->acSend_Query((char *)cmd, f_UI, f_End))// (connection, cmd, strlen (cmd)))
		{
		//if command fails first time, try again - it should re-connect
		if(!g_Schedular->acSend_Query((char *)cmd, f_UI, f_End))// (connection, cmd, strlen (cmd)))
			{
			printf("Failed database query\n");
			return false;
			}
		}

	return true;
}

int icSafeL::GetScalar (const char* cmd)
{
  //IssueSelectCommand (cmd);

  return 1;
}

int icSafeL::GetLastInsertID ()
{
	return (int)0;// safel_insert_id(connection);
}

unsigned int icSafeL::Insert (const char *table, const char **fields, const char **values, int fieldcount, int f_UI, bool f_End)
{
	char cmd[65535];
    int i;

	sprintf (cmd, "INSERT INTO %s (", table);
    for (i = 0; i < fieldcount; i++)
    {
        if (i>0) strcat (cmd, ", ");
        strcat (cmd, fields[i]);
    }

    strcat (cmd, ") VALUES (");
    for (i = 0; i < fieldcount; i++)
    {
        if (i > 0) strcat (cmd, ",");
        if (values[i] != NULL)
        {
			strcat (cmd, "'");
            strcat (cmd, values[i]);
            strcat (cmd, "'");
        }
        else
        {
            strcat (cmd, "NULL");
        }
    }
    strcat (cmd, ")");

	//Do the SQL
    if (acCommand (cmd, f_UI, f_End) == -1)
        return 0;

    return atoi(g_Entries[ag_GetEntry(g_Entries.size() - 1)][0][0].c_str());
}

unsigned int icSafeL::Insert(const char *table, std::vector<std::string> f_vec_Field, std::vector<std::string> f_vec_Value, int fieldcount)
{
	char cmd[1000];
	int i;

	sprintf(cmd, "INSERT INTO %s (", table);
	for (i = 0; i < f_vec_Field.size(); i++)
	{
		if (i>0) strcat(cmd, ", ");
		strcat(cmd, f_vec_Field[i].c_str());
	}

	strcat(cmd, ") VALUES (");
	for (i = 0; i < f_vec_Field.size(); i++)
	{
		if (i > 0) strcat(cmd, ",");
		if (f_vec_Field[i].length() != 0)
		{
			strcat(cmd, "'");
			strcat(cmd, f_vec_Field[i].c_str());
			strcat(cmd, "'");
		}
		else
		{
			strcat(cmd, "NULL");
		}
	}
	strcat(cmd, ")");

	//Do the SQL
	if (acCommand(cmd, false, false) == -1)
		return 0;

	return GetLastInsertID();
}

bool icSafeL::Update (const char *table, const char **fields, const char **values, int fieldcount, const char *whereclause, int f_UI, bool f_End)
{
  char cmd[1000];
  int i;

  sprintf (cmd, "UPDATE %s SET ", table);

  for (i = 0; i < fieldcount; i++)
    {
        if (i > 0) strcat (cmd, ",");
        strcat (cmd, fields[i]);
        if (values[i] != "")
        {
            strcat (cmd, "='");
            strcat (cmd, values[i]); 
            strcat (cmd, "'");
        }
        else
        {
            strcat (cmd, "=NULL");
        }
    }

    strcat (cmd, " WHERE ");
	strcat (cmd, whereclause);

    if(acCommand(cmd, true, f_UI, f_End) == -1)
		{
        return false;
		}
    return true;
}

void icSafeL::EscapeString(std::string& string, const char * data, unsigned long datalength)
{
  char* temp = (char*)malloc((datalength * 2) + 1);
  long templen = 0;// safel_real_escape_string(connection, temp, data, datalength);
  string.empty();
  string.append(temp, templen);
  free(temp);
}

bool icSafeL::ExecutePreparedStatement(const char* SQL, unsigned char inParamCount, char** inBuffer, unsigned long* inLengths, unsigned char* ourParamCount, char** outBuffer, unsigned long* outLengths)
{
  //SAFEL_STMT* statement = 0;// safel_stmt_init(connection);
  //if (!statement)
  //{
  //  printf("safel_stmt_init(), out of memory\n");
 //   return false;
 // }

  //if(safel_stmt_prepare(statement, SQL, strlen(SQL)))
 // {
 //   printf("safel_stmt_prepare() failed\n");
    //printf("%s\n", safel_stmt_error(statement));
 //   return false;
 // }

  /*unsigned long param_count = 0;// safel_stmt_param_count(statement);

  if (param_count != inParamCount) /* validate parameter count 
  {
    printf("Invalid parameter count returned by SafeL\n");
    return false;
  }*/

  //SAFEL_BIND* bind = (SAFEL_BIND*)calloc(inParamCount, sizeof(SAFEL_BIND));
  //memset(bind, 0, sizeof(bind));  //Not required because of calloc
  //if (!bind)
 // {
 //   printf("Out of memory while creating bind array\n");
 //   return false;
 // }

  /* BLOB PARAMS */
  /*for(int i = 0; i < inParamCount; i++)
  {
    bind[i].buffer_type = SAFEL_TYPE_BLOB;
    bind[i].buffer= (char *)inBuffer[i];
    //bind[i].buffer_length = STRING_SIZE;
    bind[i].is_null= 0;
    bind[i].length = &inLengths[i];
  }*/

  /* Bind the buffers */
  //if (safel_stmt_bind_param(statement, bind))
  //{
  //  printf("safel_stmt_bind_param() failed\n");
    //printf("%s\n", safel_stmt_error(statement));
  //  free(bind);
  //  return false;
  //}

  /* Execute the statement */
  //if (safel_stmt_execute(statement))
 // if(0)
  //  {
   // printf("safel_stmt_execute() failed\n");
    //printf("%s\n", safel_stmt_error(statement));
  //  free(bind);
   // return false;
   // }

  //my_ulonglong affected_rows = safel_stmt_affected_rows(statement);
#ifdef _DEBUG
  //printf("Total affected rows: %lu\n", (unsigned long) affected_rows);
#endif

  //SAFEL_RES* meta = safel_stmt_result_metadata(statement);

  /* Close the statement */
  //if (safel_stmt_close(statement))
 // if(0)
 // {
   // printf("Failed while closing the statement\n");
    //printf("%s\n", safel_stmt_error(statement));
  //  free(bind);
  //  return false;
 // }

  //free(bind);
  return true;
}