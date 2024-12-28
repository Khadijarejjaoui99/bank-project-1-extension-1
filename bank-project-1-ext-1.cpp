#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

const string CLIENTS_FILE_NAME = "clients-list.txt";

struct stClient
{
    string AccountNumber, PINCode, Name, Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

enum enMainMenuOptions
{
    eListClients = 1,
    eAddClients = 2,
    eDeleteClient = 3,
    eUpdateClient = 4,
    eFindClient = 5,
    eTransactions = 6,
    eExit = 7
};

enum enTransactionsMenuOptions
{
    eDeposit = 1,
    eWithdraw = 2,
    eTotalBalance = 3,
    eMainMenu = 4
};

void ShowMainMenuScreen();

void ShowTransactionsMenu();

vector<string> SplitString(string Str, string Delim)
{
    int pos = 0;
    string sWord = "";
    vector<string> vString;

    while ((pos = Str.find(Delim)) != string::npos)
    {
        sWord = Str.substr(0, pos);

        if (sWord != "")
            vString.push_back(sWord);

        Str.erase(0, pos + Delim.length());
    }

    if (Str != "")
        vString.push_back(Str);

    return vString;
}

stClient ConvertDataLineToRecord(string DataLine, string Separator = "#//#")
{
    vector<string> vString = SplitString(DataLine, Separator);
    stClient Client;

    Client.AccountNumber = vString[0];
    Client.PINCode = vString[1];
    Client.Name = vString[2];
    Client.Phone = vString[3];
    Client.AccountBalance = stod(vString[4]);

    return Client;
}

vector<stClient> LoadClientDataFromFile(string FileName)
{
    fstream MyFile;
    vector<stClient> vClients;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line = "";
        stClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertDataLineToRecord(Line);
            vClients.push_back(Client);
        }

        MyFile.close();
    }

    return vClients;
}

void PrintClientRecord(stClient Client)
{
    cout << "| " << left << setw(15) << Client.AccountNumber;
    cout << "| " << left << setw(10) << Client.PINCode;
    cout << "| " << left << setw(35) << Client.Name;
    cout << "| " << left << setw(15) << Client.Phone;
    cout << "| " << left << setw(12) << Client.AccountBalance;
}

void ShowAllClients(vector<stClient> vClients)
{
    cout << "\n\t\t\t\tClient List (" << vClients.size() << ") client(s)." << endl;
    cout << "________________________________________________________________________________________________\n\n";
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "PIN Code";
    cout << "| " << left << setw(35) << "Client Name";
    cout << "| " << left << setw(15) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n________________________________________________________________________________________________\n\n";

    if (vClients.size() == 0)
        cout << "\t\t\t\t No Client available in the system!";
    else
    {
        for (stClient &Client : vClients)
        {
            PrintClientRecord(Client);
            cout << endl;
        }
    }
    cout << "\n\n_________________________________________________________________________________________________\n";
}

void ShowAllClientsScreen()
{
    system("cls");
    vector<stClient> vClients = LoadClientDataFromFile(CLIENTS_FILE_NAME);

    ShowAllClients(vClients);
}

bool IsAccountNumberExists(string AccountNumber, string FileName)
{
    fstream MyFile;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        stClient Client;
        string Line;

        while (getline(MyFile, Line))
        {
            Client = ConvertDataLineToRecord(Line);

            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();

                return true;
            }
        }
        MyFile.close();
    }

    return false;
}

stClient ReadClientRecord()
{
    stClient Client;

    cout << "Please Enter Account Number: ";
    getline(cin >> ws, Client.AccountNumber);

    while (IsAccountNumberExists(Client.AccountNumber, CLIENTS_FILE_NAME))
    {
        cout << "Account Number (" << Client.AccountNumber << ") Exist. Please enter another account number: ";
        getline(cin, Client.AccountNumber);
    }

    cout << "Please Enter PIN Code: ";
    getline(cin, Client.PINCode);
    cout << "Please Enter Name: ";
    getline(cin, Client.Name);
    cout << "Please Enter Phone: ";
    getline(cin, Client.Phone);
    cout << "Please Enter Account Balance: ";
    cin >> Client.AccountBalance;

    return Client;
}

string ConvertRecordToDataLine(stClient Client, string Separator = "#//#")
{
    string DataLine = "";

    DataLine += Client.AccountNumber + Separator;
    DataLine += Client.PINCode + Separator;
    DataLine += Client.Name + Separator;
    DataLine += Client.Phone + Separator;
    DataLine += to_string(Client.AccountBalance);

    return DataLine;
}

void AddDataLineToFile(string FileName, string DataLine)
{
    fstream MyFile;

    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << DataLine << endl;
        MyFile.close();
    }
}

void AddNewClient()
{
    stClient Client = ReadClientRecord();

    AddDataLineToFile(CLIENTS_FILE_NAME, ConvertRecordToDataLine(Client));
}

void AddNewClients()
{
    char AddMore = 'y';

    do
    {

        cout << "\nAdding New Client:\n\n";

        AddNewClient();

        cout << "\nClient Added Successfully, do you want to add more clients: ";
        cin >> AddMore;

    } while (tolower(AddMore) == 'y');
}

void ShowAddClientsScreen()
{
    system("cls");
    cout << "==================================\n";
    cout << "\tAdd New Clients Screen\n";
    cout << "==================================\n";

    AddNewClients();
}

string ReadAccountNumber()
{
    string AccountNumber;

    cout << "\nPlease enter Account Number: ";
    getline(cin >> ws, AccountNumber);

    return AccountNumber;
}

void PrintClientCard(stClient Client)
{
    cout << "\nThe followings are Client Details:\n";
    cout << "----------------------------------------\n";
    cout << "Account Number: " << Client.AccountNumber << endl;
    cout << "PIN Code: " << Client.PINCode << endl;
    cout << "Name: " << Client.Name << endl;
    cout << "Phone: " << Client.Phone << endl;
    cout << "Account Balance: " << Client.AccountBalance << endl;
    cout << "----------------------------------------\n";
}

void MarkClientForDelete(vector<stClient> &vClients, string AccountNumber)
{
    for (stClient &C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            break;
        }
    }
}

void SavaClientsToFile(vector<stClient> vClients, string FileName)
{
    fstream MyFile;

    MyFile.open(FileName, ios::out);

    if (MyFile.is_open())
    {
        for (stClient &C : vClients)
        {
            if (!C.MarkForDelete)
                MyFile << ConvertRecordToDataLine(C) << endl;
        }
    }
}

bool FindClientByAccountNumber(string AccountNumber, stClient &Client, vector<stClient> vClients)
{
    for (stClient &C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }

    return false;
}

void DeleteClientByAccountNumber(vector<stClient> &vClients, string AccountNumber)
{
    stClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, Client, vClients))
    {
        PrintClientCard(Client);

        cout << "\nAre you sure you want to delete this client? y/n? ";
        cin >> Answer;

        if (tolower(Answer) == 'y')
        {
            MarkClientForDelete(vClients, AccountNumber);
            SavaClientsToFile(vClients, CLIENTS_FILE_NAME);
            vClients = LoadClientDataFromFile(CLIENTS_FILE_NAME);

            cout << "\nClient Deleted Successfully\n";
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") Not Found!\n";
    }
}

void ShowDeleteClientScreen()
{
    system("cls");
    cout << "==================================\n";
    cout << "\tDelete Client Screen\n";
    cout << "==================================\n";

    vector<stClient> vClients = LoadClientDataFromFile(CLIENTS_FILE_NAME);
    string AccountNumber = ReadAccountNumber();
    DeleteClientByAccountNumber(vClients, AccountNumber);
}

stClient UpdateClientRecord(string AccountNumber)
{
    stClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "\nPlease Enter PIN Code: ";
    getline(cin >> ws, Client.PINCode);

    cout << "Please Enter Name: ";
    getline(cin, Client.Name);

    cout << "Please Enter Phone: ";
    getline(cin, Client.Phone);

    cout << "Please Enter Account Balance: ";
    cin >> Client.AccountBalance;

    return Client;
}

void UpdateClientByAccountNumber(vector<stClient> &vClients, string AccountNumber)
{
    stClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, Client, vClients))
    {
        PrintClientCard(Client);

        cout << "\nAre you sure you want to update this client? y/n? ";
        cin >> Answer;

        if (tolower(Answer) == 'y')
        {
            for (stClient &C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = UpdateClientRecord(AccountNumber);
                    break;
                }
            }

            SavaClientsToFile(vClients, CLIENTS_FILE_NAME);

            cout << "\nClient Updated Successfully\n";
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") Not Found!\n";
    }
}

void ShowUpdateClientScreen()
{
    system("cls");
    cout << "==================================\n";
    cout << "\tUpdate Client Screen\n";
    cout << "==================================\n";

    vector<stClient> vClients = LoadClientDataFromFile(CLIENTS_FILE_NAME);
    string AccountNumber = ReadAccountNumber();
    UpdateClientByAccountNumber(vClients, AccountNumber);
}

void ShowFindClientScreen()
{
    system("cls");
    cout << "==================================\n";
    cout << "\tFind Client Screen\n";
    cout << "==================================\n";

    vector<stClient> vClients = LoadClientDataFromFile(CLIENTS_FILE_NAME);
    string AccountNumber = ReadAccountNumber();
    stClient Client;

    if (FindClientByAccountNumber(AccountNumber, Client, vClients))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number (" << AccountNumber << ") Not Found!\n";
}

void ShowExitScreen()
{
    system("cls");
    cout << "==================================\n";
    cout << "\tProgram Ends :-)\n";
    cout << "==================================\n";
}

void ApplyTransaction(vector<stClient> &vClients, string AccountNumber, double Amount)
{
    char Answer = 'n';

    cout << "\nAre you sure you want to perform this transactions: ";
    cin >> Answer;

    if (tolower(Answer) == 'y')
    {
        for (stClient &Client : vClients)
        {
            if (Client.AccountNumber == AccountNumber)
            {
                Client.AccountBalance += Amount;
                SavaClientsToFile(vClients, CLIENTS_FILE_NAME);
                cout << "\nDone Successfully. New Balance now is: " << Client.AccountBalance << endl;
                break;
            }
        }
    }
}

void DepositBalanceByAccountNumber()
{
    vector<stClient> vClients = LoadClientDataFromFile(CLIENTS_FILE_NAME);
    stClient Client;
    double Amount = 0;
    string AccountNumber = ReadAccountNumber();

    while (!FindClientByAccountNumber(AccountNumber, Client, vClients))
    {
        AccountNumber = ReadAccountNumber();
    }

    PrintClientCard(Client);

    cout << "\nPlease enter deposit amount: ";
    cin >> Amount;

    ApplyTransaction(vClients, AccountNumber, Amount);
}

void ShowDepositScreen()
{
    system("cls");
    cout << "==================================\n";
    cout << "\tDeposit Screen\n";
    cout << "==================================\n";
    DepositBalanceByAccountNumber();
}

void WithdrawBalanceByAccountNumber()
{
    vector<stClient> vClients = LoadClientDataFromFile(CLIENTS_FILE_NAME);
    stClient Client;
    double Amount = 0;
    string AccountNumber = ReadAccountNumber();

    while (!FindClientByAccountNumber(AccountNumber, Client, vClients))
    {
        AccountNumber = ReadAccountNumber();
    }

    PrintClientCard(Client);

    cout << "\nPlease enter withdraw amount: ";
    cin >> Amount;

    while (Amount > Client.AccountBalance)
    {
        cout << "\nAmount Exceed the balance, you can withdraw up to: " << Client.AccountBalance << endl;
        cout << "Please enter another amount: ";
        cin >> Amount;
    }

    ApplyTransaction(vClients, AccountNumber, Amount * -1);
}

void ShowWithdrawScreen()
{
    system("cls");
    cout << "==================================\n";
    cout << "\tWithdraw Screen\n";
    cout << "==================================\n";
    WithdrawBalanceByAccountNumber();
}

void PrintClientInfo(stClient Client)
{
    cout << "| " << left << setw(15) << Client.AccountNumber;
    cout << "| " << left << setw(40) << Client.Name;
    cout << "| " << left << setw(20) << Client.AccountBalance;
}

void ShowBalanceList(vector<stClient> vClients)
{

    cout << "\n\t\t\t\tBalance List (" << vClients.size() << ") client(s)." << endl;
    cout << "________________________________________________________________________________________________\n\n";
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(20) << "Balance";
    cout << "\n________________________________________________________________________________________________\n\n";

    double TotalBalance = 0;

    if (vClients.size() == 0)
        cout << "\t\t\t\t No Client available in the system!";
    else
    {
        for (stClient &Client : vClients)
        {
            PrintClientInfo(Client);
            TotalBalance += Client.AccountBalance;
            cout << endl;
        }
    }
    cout << "\n\n_________________________________________________________________________________________________\n";

    cout << "\n\t\t\t\t\t Total Balances = " << std::fixed << std::setprecision(2) << TotalBalance << endl;
}

void ShowTotalBalanceScreen()
{
    system("cls");
    vector<stClient> vClients = LoadClientDataFromFile(CLIENTS_FILE_NAME);

    ShowBalanceList(vClients);
}

short ReadTransactionsMenuOpetion()
{
    short Option;

    do
    {
        cout << "Choose what you want to do? [1 to 4]? ";
        cin >> Option;

    } while (Option < 1 || Option > 4);

    return Option;
}

void GoBackToTransactionsMenu()
{
    cout << "\n\nPlease press any key to go back to transactions menu...";
    system("pause>0");
    ShowTransactionsMenu();
}

void PerformTransactionsMenuOption(enTransactionsMenuOptions TransactionsMenuOption)
{
    switch (TransactionsMenuOption)
    {
    case enTransactionsMenuOptions::eDeposit:
        ShowDepositScreen();
        GoBackToTransactionsMenu();
        break;
    case enTransactionsMenuOptions::eWithdraw:
        ShowWithdrawScreen();
        GoBackToTransactionsMenu();
        break;
    case enTransactionsMenuOptions::eTotalBalance:
        ShowTotalBalanceScreen();
        GoBackToTransactionsMenu();
        break;
    case enTransactionsMenuOptions::eMainMenu:
        ShowMainMenuScreen();
        break;
    }
}

void ShowTransactionsMenu()
{
    system("cls");
    cout << "============================================\n";
    cout << "\tTransactions Menu Screen\n";
    cout << "============================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menu.\n";
    cout << "============================================\n";

    PerformTransactionsMenuOption((enTransactionsMenuOptions)ReadTransactionsMenuOpetion());
}

void GoBackToMainMenu()
{
    cout << "\n\nPlease press any key to go back to main menu...";
    system("pause>0");
    ShowMainMenuScreen();
}

short ReadMainMenuOpetion()
{
    short Option;

    do
    {
        cout << "Choose what you want to do? [1 to 7]? ";
        cin >> Option;

    } while (Option < 1 || Option > 7);

    return Option;
}

void PerformMainMenuOption(enMainMenuOptions MainMenuOption)
{
    switch (MainMenuOption)
    {
    case enMainMenuOptions::eListClients:
        ShowAllClientsScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eAddClients:
        ShowAddClientsScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eDeleteClient:
        ShowDeleteClientScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eUpdateClient:
        ShowUpdateClientScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eFindClient:
        ShowFindClientScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eTransactions:
        ShowTransactionsMenu();
        break;
    case enMainMenuOptions::eExit:
        ShowExitScreen();
        break;
    }
}

void ShowMainMenuScreen()
{
    system("cls");
    cout << "=================================================\n";
    cout << "\t\tMain Menu Sreen\n";
    cout << "=================================================\n";

    cout << "\t[1] Show Clients List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Exit.\n";
    cout << "=================================================\n";
    PerformMainMenuOption((enMainMenuOptions)ReadMainMenuOpetion());
}

int main()
{
    ShowMainMenuScreen();
    system("pause>0");
    return 0;
}