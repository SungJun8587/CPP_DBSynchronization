// DBSynchronization.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>

void TestDBInfo(CDBQueryProcess dbProcess)
{
	int32 iDBCount = 0;
	int32 iSystemCount = 0;
	int32 iDatatypeCount = 0;

	std::unique_ptr<DB_INFO[]> pDBList;
	std::unique_ptr<DB_SYSTEM_INFO[]> pDBSystemInfo;
	std::unique_ptr<DB_SYSTEM_DATATYPE[]> pDBSystemDataType;

	dbProcess.GetDatabaseList(iDBCount, pDBList);
	for( int i = 0; i < iDBCount; i++ )
	{
		_tprintf(_T("%s\r\n"), pDBList[i].tszDBName);
	}

	dbProcess.GetDBSystemInfo(iSystemCount, pDBSystemInfo);
	for( int i = 0; i < iSystemCount; i++ )
	{
		_tprintf(_T("%s, %s, %s\r\n"), pDBSystemInfo[i].tszVersion, pDBSystemInfo[i].tszCharacterSet, pDBSystemInfo[i].tszCollation);
	}

	dbProcess.GetDBSystemDataTypeInfo(iDatatypeCount, pDBSystemDataType);
}

void TestMSSQLTableIndexFragmentationCheck(CDBQueryProcess dbProcess)
{
	int32 iIndexCount = 0;
	std::unique_ptr<MSSQL_INDEX_FRAGMENTATION[]> pRowStoreIndexFragmentation;

	dbProcess.MSSQLGetRowStoreIndexFragmentationCheck(_T(""), iIndexCount, pRowStoreIndexFragmentation);
	for( int i = 0; i < iIndexCount; i++ )
	{
		_tprintf(_T("%d, %s, %s, %d, %s, %s, %d, %f, %f, %d, %s\r\n")
			, pRowStoreIndexFragmentation[i].ObjectId, pRowStoreIndexFragmentation[i].tszSchemaName, pRowStoreIndexFragmentation[i].tszTableName, pRowStoreIndexFragmentation[i].IndexId
			, pRowStoreIndexFragmentation[i].tszIndexName, pRowStoreIndexFragmentation[i].tszIndexType, pRowStoreIndexFragmentation[i].PartitionNum, pRowStoreIndexFragmentation[i].AvgFragmentationInPercent
			, pRowStoreIndexFragmentation[i].AvgPageSpaceUsedInPercent, pRowStoreIndexFragmentation[i].PageCount, pRowStoreIndexFragmentation[i].tszAllocUnitTypeDesc);
	}

	iIndexCount = 0;
	std::unique_ptr<MSSQL_INDEX_FRAGMENTATION[]> pColumnStoreIndexFragmentation;

	dbProcess.MSSQLGetColumnStoreIndexFragmentationCheck(_T(""), iIndexCount, pColumnStoreIndexFragmentation);
	for( int i = 0; i < iIndexCount; i++ )
	{
		_tprintf(_T("%d, %s, %s, %d, %s, %s, %f\r\n")
			, pColumnStoreIndexFragmentation[i].ObjectId, pColumnStoreIndexFragmentation[i].tszSchemaName, pColumnStoreIndexFragmentation[i].tszTableName, pColumnStoreIndexFragmentation[i].IndexId
			, pColumnStoreIndexFragmentation[i].tszIndexName, pColumnStoreIndexFragmentation[i].tszIndexType, pColumnStoreIndexFragmentation[i].AvgFragmentationInPercent);
	}
}

void TestMSSQLIndexOptionProcess(CDBQueryProcess dbProcess)
{
	unordered_map<_tstring, _tstring> setIndexOptions;
	setIndexOptions.insert(make_pair(_T("STATISTICS_NORECOMPUTE"), _T("ON")));
	setIndexOptions.insert(make_pair(_T("IGNORE_DUP_KEY"), _T("ON")));
	setIndexOptions.insert(make_pair(_T("ALLOW_PAGE_LOCKS"), _T("ON")));

	dbProcess.MSSQLIndexOptionSet(_T("dbo"), _T("Account"), _T("ix_Account_Name"), setIndexOptions);

	dbProcess.MSSQLAlterIndexFragmentationNonOption(_T("dbo"), _T("Account"), _T("ix_Account_Name"), EMSSQLIndexFragmentation::REBUILD);
	dbProcess.MSSQLAlterIndexFragmentationNonOption(_T("dbo"), _T("Account"), _T("ix_Account_Name"), EMSSQLIndexFragmentation::REORGANIZE);
	dbProcess.MSSQLAlterIndexFragmentationNonOption(_T("dbo"), _T("Account"), _T("ix_Account_Name"), EMSSQLIndexFragmentation::DISABLE);

	unordered_map<_tstring, _tstring> rebuildIndexOptions;
	rebuildIndexOptions.insert(make_pair(_T("FILLFACTOR"), _T("80")));
	rebuildIndexOptions.insert(make_pair(_T("SORT_IN_TEMPDB"), _T("ON")));
	rebuildIndexOptions.insert(make_pair(_T("STATISTICS_NORECOMPUTE"), _T("ON")));

	dbProcess.MSSQLAlterIndexFragmentationOption(_T("dbo"), _T("Account"), _T(""), EMSSQLIndexFragmentation::REBUILD, rebuildIndexOptions);
}

void TestMYSQLCharacterSetCollationEngine(CDBQueryProcess dbProcess)
{
	int32 iCharsetCount = 0;
	int32 iStorageEngineCount = 0;

	std::unique_ptr<MYSQL_CHARACTER_SET[]> pCharacterSet;
	std::unique_ptr<MYSQL_COLLATION[]> pCollation;
	std::unique_ptr<MYSQL_CHARACTER_SET_COLLATION[]> pCharacterSetCollation;
	std::unique_ptr<MYSQL_STORAGE_ENGINE[]> pStorageEngine;

	dbProcess.MYSQLGetCharacterSets(_T(""), iCharsetCount, pCharacterSet);
	for( int i = 0; i < iCharsetCount; i++ )
	{
		_tprintf(_T("%s, %s\r\n"), pCharacterSet[i].tszCharacterSet, pCharacterSet[i].tszDefaultCollation);
	}

	dbProcess.MYSQLGetCollations(_T(""), iCharsetCount, pCollation);
	for( int i = 0; i < iCharsetCount; i++ )
	{
		_tprintf(_T("%s, %s\r\n"), pCollation[i].tszCharacterSet, pCollation[i].tszCollation);
	}

	dbProcess.MYSQLGetCharacterSetCollations(_T(""), iCharsetCount, pCharacterSetCollation);
	for( int i = 0; i < iCharsetCount; i++ )
	{
		_tprintf(_T("%s, %s\r\n"), pCharacterSetCollation[i].tszCharacterSet, pCharacterSetCollation[i].tszCollation);
	}

	dbProcess.MYSQLGetStorageEngines(iStorageEngineCount, pStorageEngine);
	for( int i = 0; i < iStorageEngineCount; i++ )
	{
		_tprintf(_T("%s\r\n"), pStorageEngine[i].tszEngine);
	}
}

void TestMYSQLTableFragmentationCheck(CDBQueryProcess dbProcess)
{
	int32 iTableCount = 0;
	std::unique_ptr<MYSQL_TABLE_FRAGMENTATION[]> pTableFragmentation;

	dbProcess.MYSQLGetTableFragmentationCheck(_T(""), iTableCount, pTableFragmentation);
}

void TestRenameObject(CDBQueryProcess dbProcess)
{
	if( dbProcess.GetDBClass() == EDBClass::MSSQL )
	{
		//_tstring sql = GetTableColumnOption(dbSync.GetDBClass(), _T("INT"), false, _T("0"), true, 10, 1, _T(""), _T(""), _T("안녕하세요"));
		//bool isFlag = dbSync.MSSQLRenameObject(_T("Table_2"), _T("Table_3"));
		//isFlag = dbSync.MSSQLRenameObject(_T("Table_1.Text"), _T("Text3"), EMSSQLRenameObjectType::COLUMN);
		//dbSync.MSSQLDBHelpText(EDBObjectType::PROCEDURE, _T("dbo"), _T("spa_LoginProcess"));
	}
	else
	{
		//dbProcess.MYSQLDBShowTable(_T("tblb_bbs"));
		//dbProcess.MYSQLDBShowObject(EDBObjectType::PROCEDURE, _T("BbsConfigProcess"));
	}
}

void TestComment(CDBQueryProcess dbProcess)
{
	if( dbProcess.GetDBClass() == EDBClass::MSSQL )
	{
		_tstring ret = dbProcess.MSSQLGetTableColumnComment(_T("dbo"), _T("tbla_Account"), _T(""));
		ret = dbProcess.MSSQLGetTableColumnComment(_T("dbo"), _T("tbla_Account"), _T("Idx"));

		dbProcess.MSSQLProcessTableColumnComment(_T("dbo"), _T("TestB"), _T(""), _T("테스트B"));
		dbProcess.MSSQLProcessTableColumnComment(_T("dbo"), _T("TestB"), _T("userSerialNo"), _T("테스트B_userSerialNo"));
		dbProcess.MSSQLProcessTableColumnComment(_T("dbo"), _T("TestB"), _T(""), _T(""));
		dbProcess.MSSQLProcessTableColumnComment(_T("dbo"), _T("TestB"), _T("userSerialNo"), _T(""));

		ret = dbProcess.MSSQLGetProcedureParamComment(_T("dbo"), _T("spa_LoginProcess"), _T(""));
		ret = dbProcess.MSSQLGetProcedureParamComment(_T("dbo"), _T("spa_LoginProcess"), _T("@v_UserId"));
		dbProcess.MSSQLProcessProcedureParamComment(_T("dbo"), _T("spa_LoginProcess"), _T(""), _T("유저 로그인 프로세스__111"));
		dbProcess.MSSQLProcessProcedureParamComment(_T("dbo"), _T("spa_LoginProcess"), _T("@v_UserId"), _T("로그인 ID__111"));
		dbProcess.MSSQLProcessProcedureParamComment(_T("dbo"), _T("spa_LoginProcess"), _T(""), _T(""));
		dbProcess.MSSQLProcessProcedureParamComment(_T("dbo"), _T("spa_LoginProcess"), _T("@v_UserId"), _T(""));
		dbProcess.MSSQLProcessProcedureParamComment(_T("dbo"), _T("spa_LoginProcess"), _T(""), _T("유저 로그인 프로세스"));
		dbProcess.MSSQLProcessProcedureParamComment(_T("dbo"), _T("spa_LoginProcess"), _T("@v_UserId"), _T("로그인 ID"));

		ret = dbProcess.MSSQLGetFunctionParamComment(_T("dbo"), _T("udf_Char_SplitFnc"), _T(""));
		ret = dbProcess.MSSQLGetFunctionParamComment(_T("dbo"), _T("udf_Char_SplitFnc"), _T("@v_Delimiter"));
		dbProcess.MSSQLProcessFunctionParamComment(_T("dbo"), _T("udf_Char_SplitFnc"), _T(""), _T("Split 함수 구현(공백문자 제거)__111"));
		dbProcess.MSSQLProcessFunctionParamComment(_T("dbo"), _T("udf_Char_SplitFnc"), _T("@v_Delimiter"), _T("구분자__111"));
		dbProcess.MSSQLProcessFunctionParamComment(_T("dbo"), _T("udf_Char_SplitFnc"), _T(""), _T(""));
		dbProcess.MSSQLProcessFunctionParamComment(_T("dbo"), _T("udf_Char_SplitFnc"), _T("@v_Delimiter"), _T(""));
		dbProcess.MSSQLProcessFunctionParamComment(_T("dbo"), _T("udf_Char_SplitFnc"), _T(""), _T("Split 함수 구현(공백문자 제거)"));
		dbProcess.MSSQLProcessFunctionParamComment(_T("dbo"), _T("udf_Char_SplitFnc"), _T("@v_Delimiter"), _T("구분자"));
	}
}

int main()
{
#ifdef	_MSC_VER
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
#endif
	 
#ifdef _UNICODE
	setlocale(LC_ALL, "korean");
#endif

	CServerConfig	ServerConfig;

	ServerConfig.Init(_T("server_config.json"));

	TCHAR tszDSN[DATABASE_DSN_STRLEN];
	TCHAR tszServerName[DATABASE_BUFFER_SIZE];
	TCHAR tszDBMSName[DATABASE_BUFFER_SIZE];
	TCHAR tszDBMSVersion[DATABASE_BUFFER_SIZE];

	CVector<CDBNode> dbNodes = ServerConfig.GetDBNodeVec();

	EDBClass dbClass = EDBClass::MSSQL;
	//EDBClass dbClass = EDBClass::MYSQL;
	auto findDBNode = std::find_if(dbNodes.begin(), dbNodes.end(), [=](const CDBNode& dbNode) { return dbNode.m_dbClass == dbClass; });
	
	GetDBDSNString(tszDSN, findDBNode->m_dbClass, findDBNode->m_tszDBHost, findDBNode->m_nPort, findDBNode->m_tszDBUserId, findDBNode->m_tszDBPasswd, findDBNode->m_tszDBName);

	CBaseODBC BaseODBC(findDBNode->m_dbClass, tszDSN);
	BaseODBC.Connect();
	BaseODBC.DBMSInfo(tszServerName, tszDBMSName, tszDBMSVersion);

	CDBQueryProcess dbProcess(BaseODBC);
	//TestDBInfo(dbProcess);
	//TestMSSQLTableIndexFragmentationCheck(dbProcess);
	TestMSSQLIndexOptionProcess(dbProcess);

	//TestMYSQLCharacterSetCollationEngine(dbProcess);
	//TestMYSQLTableFragmentationCheck(dbProcess);

	//TestRenameObject(dbProcess);
	
	//dbSync.Synchronize(_T("E:\\GitHub\\CPP\\DBSynchronization\\GameDB.xml"));
	//dbSync.PrintDBSchema();

	//dbSync.DBToCreateXml(_T("E:\\GitHub\\CPP\\DBSynchronization\\GameDB2.xml"));
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
