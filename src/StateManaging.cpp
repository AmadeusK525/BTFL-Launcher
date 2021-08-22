#include "StateManaging.h"
#include "MainFrame.h"
#include "IsoHashes.h"

btfl::SQLDatabase::SQLDatabase(wxFileName& path)
{
	EnableForeignKeySupport(true);
	if ( path.IsOk() && !path.IsDir() )
	{
		Open(path.GetFullPath());
	}
}

bool btfl::SQLDatabase::Init()
{
	if ( !TableExists("user_data") )
	{
		CreateAllTables();
		return false;
	}

	return true;
}

void btfl::SQLDatabase::CreateAllTables()
{
	wxArrayString tUserData;
	tUserData.Add("user_state INTEGER");
	tUserData.Add("has_agreed_to_disclaimer INTEGER");
	tUserData.Add("iso_file_path TEXT");

	wxArrayString tLauncherData;
	tLauncherData.Add("install_path TEXT");
	tLauncherData.Add("installed_version TEXT");
	tLauncherData.Add("settings_xml TEXT");

	try
	{
		Begin();

		CreateTable("user_data", tUserData);
		CreateTable("launcher_data", tLauncherData);

		Commit();
	}
	catch ( wxSQLite3Exception& e )
	{
		wxMessageBox(e.GetMessage());
	}
}

bool btfl::SQLDatabase::CreateTable(const wxString& tableName, const wxArrayString& arguments,
	bool ifNotExists)
{
	if ( arguments.IsEmpty() )
	{
		wxMessageBox("You can't create a table with no arguments!");
		return false;
	}

	wxString update("CREATE TABLE ");
	if ( ifNotExists )
		update << "IF NOT EXISTS ";

	update << tableName;
	update << " (";

	bool first = true;

	for ( auto& str : arguments )
	{
		if ( !first )
			update << ", ";
		else
			first = false;

		update << str;
	}

	update << ");";
	ExecuteUpdate(update);

	return true;
}

bool btfl::SQLDatabase::InsertSQLEntry(const btfl::SQLEntry& sqlEntry)
{
	wxSQLite3Statement statement = ConstructInsertStatement(sqlEntry);
	statement.ExecuteUpdate();
	return true;
}

bool btfl::SQLDatabase::UpdateSQLEntry(const btfl::SQLEntry& sqlEntry)
{
	try
	{
		wxSQLite3Statement statement = ConstructUpdateStatement(sqlEntry, 1);
		statement.ExecuteUpdate();
	}
	catch ( wxSQLite3Exception& e )
	{
		wxMessageBox("Exception thrown - " + e.GetMessage());
	}

	return true;
}

wxSQLite3Statement btfl::SQLDatabase::ConstructInsertStatement(const btfl::SQLEntry& sqlEntry)
{
	wxString insert("INSERT INTO ");
	insert << sqlEntry.tableName << " (";

	wxString columnNames;
	wxString valueNames;

	bool first = true;

	wxSQLite3StatementBuffer buffer;

	for ( const std::pair<const wxString, int>& it : sqlEntry.integers )
	{
		if ( !first )
		{
			columnNames << ", ";
			valueNames << ", ";
		}
		else
			first = false;

		columnNames << it.first;
		valueNames << it.second;
	}

	for ( const std::pair<const wxString, wxString>& it : sqlEntry.strings )
	{
		if ( !first )
		{
			columnNames << ", ";
			valueNames << ", ";
		}
		else
			first = false;

		columnNames << it.first;
		valueNames << "'" << buffer.Format("%q", (const char*)it.second) << "'";
	}

	insert << columnNames << ") VALUES (" << valueNames << ");";

	try
	{
		wxSQLite3Statement statement = PrepareStatement(insert);
		return statement;
	}
	catch ( wxSQLite3Exception& e )
	{
		wxMessageBox(e.GetMessage());
	}

	return wxSQLite3Statement();
}

wxSQLite3Statement btfl::SQLDatabase::ConstructUpdateStatement(const btfl::SQLEntry& sqlEntry, int id)
{
	wxString update("UPDATE ");
	update << sqlEntry.tableName << " SET ";

	bool first = true;
	wxSQLite3StatementBuffer buffer;

	for ( const std::pair<const wxString, int>& it : sqlEntry.integers )
	{
		if ( !first )
			update << ", ";
		else
			first = false;

		update << it.first << " = " << it.second;
	}

	for ( const std::pair<const wxString, wxString>& it : sqlEntry.strings )
	{
		if ( !first )
			update << ", ";

		update << it.first << " = '" << buffer.Format("%q", (const char*)it.second) << "'";
		first = false;
	}

	update << " WHERE rowid = " << id << ";";

	try
	{
		wxSQLite3Statement statement = PrepareStatement(update);

		int i = 1;

		for ( std::pair<const wxString, wxMemoryBuffer>& it : sqlEntry.memBuffers )
			statement.Bind(i++, it.second);

		return statement;
	}
	catch ( wxSQLite3Exception& e )
	{
		wxMessageBox(e.GetMessage());
	}

	return wxSQLite3Statement();
}


////////////////////////////////////////////////////////////////
/////////////////////// Separate Functions /////////////////////
////////////////////////////////////////////////////////////////


btfl::SQLDatabase* pDatabase;

MainFrame* pMainFrame;

void btfl::Init()
{
	wxBitmap shape("Assets/FrameShape.png", wxBITMAP_TYPE_PNG);

	pMainFrame = new MainFrame(nullptr, -1, "Beyond The Forbidden Lands - Launcher",
		wxDefaultPosition, shape.GetSize(), wxFRAME_SHAPED | wxBORDER_NONE);
	pMainFrame->Hide();
	pMainFrame->SetShape(wxRegion(shape, wxColour(255, 255, 255)));
	pMainFrame->CenterOnScreen();
	pMainFrame->Layout();
	pMainFrame->Update();

	pMainFrame->LoadPatchNotes();

	wxSQLite3Database::InitializeSQLite();
	pDatabase = new btfl::SQLDatabase();
	pDatabase->Open("./LauncherData.db", db_encryption_key);

	if ( !pDatabase->Init() )
	{
		btfl::SQLEntry userEntry("user_data");
		userEntry.integers["user_state"] = btfl::LauncherState::STATE_ToSelectIso;
		userEntry.integers["has_agreed_to_disclaimer"] = false;
		userEntry.strings["iso_file_path"] = "";

		btfl::SQLEntry launcherEntry("launcher_data");
		launcherEntry.strings["install_path"] = "./Game";
		launcherEntry.strings["installed_version"] = "";
		launcherEntry.strings["settings_xml"] = "";

		pDatabase->InsertSQLEntry(userEntry);
		pDatabase->InsertSQLEntry(launcherEntry);
	}

	pMainFrame->Show();
}

void btfl::ShutDown()
{
	pDatabase->Close();
	wxSQLite3Database::ShutdownSQLite();
	delete pDatabase;
	pDatabase = nullptr;
}