// DBSynchronization.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>

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

	//DB_CLASS dbClass = DB_CLASS::DB_MSSQL;
	DB_CLASS dbClass = DB_CLASS::DB_MYSQL;
	auto findDBNode = std::find_if(dbNodes.begin(), dbNodes.end(), [=](const CDBNode& dbNode) { return dbNode.m_dbClass == dbClass; });
	
	GetDBDSNString(tszDSN, findDBNode->m_dbClass, findDBNode->m_tszDBHost, findDBNode->m_nPort, findDBNode->m_tszDBUserId, findDBNode->m_tszDBPasswd, findDBNode->m_tszDBName);

	CBaseODBC BaseODBC(findDBNode->m_dbClass, tszDSN);
	BaseODBC.Connect();
	BaseODBC.DBMSInfo(tszServerName, tszDBMSName, tszDBMSVersion);

	CDBSynchronizer dbSync(BaseODBC);
	dbSync.Synchronize(_T("E:\\GitHub\\CPP\\DBSynchronization\\GameDB.xml"));
	dbSync.PrintDBSchema();

	dbSync.DBToCreateXml(_T("E:\\GitHub\\CPP\\DBSynchronization\\GameDB2.xml"));
}

void TestComment(CDBSynchronizer dbSync)
{
	if( dbSync.GetDBClass() == DB_CLASS::DB_MSSQL )
	{
		_tstring ret = dbSync.MSSQLGetTableColumnComment(_T("tbla_Account"), _T(""));
		ret = dbSync.MSSQLGetTableColumnComment(_T("tbla_Account"), _T("Idx"));

		dbSync.MSSQLProcessTableColumnComment(_T("TestB"), _T(""), _T("테스트B"));
		dbSync.MSSQLProcessTableColumnComment(_T("TestB"), _T("userSerialNo"), _T("테스트B_userSerialNo"));
		dbSync.MSSQLProcessTableColumnComment(_T("TestB"), _T(""), _T(""));
		dbSync.MSSQLProcessTableColumnComment(_T("TestB"), _T("userSerialNo"), _T(""));

		ret = dbSync.MSSQLGetProcedureParamComment(_T("spa_LoginProcess"), _T(""));
		ret = dbSync.MSSQLGetProcedureParamComment(_T("spa_LoginProcess"), _T("@v_UserId"));
		dbSync.MSSQLProcessProcedureParamComment(_T("spa_LoginProcess"), _T(""), _T("유저 로그인 프로세스__111"));
		dbSync.MSSQLProcessProcedureParamComment(_T("spa_LoginProcess"), _T("@v_UserId"), _T("로그인 ID__111"));
		dbSync.MSSQLProcessProcedureParamComment(_T("spa_LoginProcess"), _T(""), _T(""));
		dbSync.MSSQLProcessProcedureParamComment(_T("spa_LoginProcess"), _T("@v_UserId"), _T(""));
		dbSync.MSSQLProcessProcedureParamComment(_T("spa_LoginProcess"), _T(""), _T("유저 로그인 프로세스"));
		dbSync.MSSQLProcessProcedureParamComment(_T("spa_LoginProcess"), _T("@v_UserId"), _T("로그인 ID"));

		ret = dbSync.MSSQLGetFunctionParamComment(_T("udf_Char_SplitFnc"), _T(""));
		ret = dbSync.MSSQLGetFunctionParamComment(_T("udf_Char_SplitFnc"), _T("@v_Delimiter"));
		dbSync.MSSQLProcessFunctionParamComment(_T("udf_Char_SplitFnc"), _T(""), _T("Split 함수 구현(공백문자 제거)__111"));
		dbSync.MSSQLProcessFunctionParamComment(_T("udf_Char_SplitFnc"), _T("@v_Delimiter"), _T("구분자__111"));
		dbSync.MSSQLProcessFunctionParamComment(_T("udf_Char_SplitFnc"), _T(""), _T(""));
		dbSync.MSSQLProcessFunctionParamComment(_T("udf_Char_SplitFnc"), _T("@v_Delimiter"), _T(""));
		dbSync.MSSQLProcessFunctionParamComment(_T("udf_Char_SplitFnc"), _T(""), _T("Split 함수 구현(공백문자 제거)"));
		dbSync.MSSQLProcessFunctionParamComment(_T("udf_Char_SplitFnc"), _T("@v_Delimiter"), _T("구분자"));
	}
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
