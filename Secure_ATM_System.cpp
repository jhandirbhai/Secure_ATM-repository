#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;

// ================= GLOBAL DATA =================
// accounts[i]:
// [0] Account Number
// [1] Password
// [2] Balance
// [3] Status (Open / Frozen)
// [4...] Transaction History
vector<vector<string>> accounts;

// ================= MESSAGE DATA =================
// adminMessages:
// accNo | text | status
vector<vector<string>> adminMessages;

// ================= STRONG PASSWORD =================
string strongPassword() {
    string pass;
    int u, l, d, s;

    while (true) {
        u = l = d = s = 0;
        cout << "Set Strong Password: ";
        cin >> pass;

        if (pass.length() < 8) {
            cout << "❌ Minimum 8 characters required\n";
            continue;
        }

        for (char c : pass) {
            if (c >= 'A' && c <= 'Z') u = 1;
            else if (c >= 'a' && c <= 'z') l = 1;
            else if (c >= '0' && c <= '9') d = 1;
            else s = 1;
        }

        if (u && l && d && s) {
            cout << "✅ Strong Password Accepted\n";
            return pass;
        }
        cout << "❌ Must include Upper, Lower, Digit & Symbol\n";
    }
}

// ================= ADD TRANSACTION =================
void addTransaction(int index, const string& msg) {
    accounts[index].push_back(msg);
}

// ================= SIMPLE INTEGER INPUT =================
int getIntInputSimple(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        cin >> input;

        bool valid = true;
        for (char c : input) {
            if (c < '0' || c > '9') {
                valid = false;
                break;
            }
        }

        if (valid) return stoi(input);
        cout << "❌ Invalid input. Enter a valid number.\n";
    }
}

// ================= CREATE ACCOUNT =================
void createAccount() {
    string acc;
    cout << "Enter Account Number: ";
    cin >> acc;

    for (auto& a : accounts) {
        if (a[0] == acc) {
            cout << "❌ Account already exists\n";
            return;
        }
    }

    string pass = strongPassword();

    vector<string> newAcc;
    newAcc.push_back(acc);
    newAcc.push_back(pass);
    newAcc.push_back("0");
    newAcc.push_back("Open");
    newAcc.push_back("Account created with balance 0");

    accounts.push_back(newAcc);
    cout << "✅ Account Created Successfully\n";
}

// ================= ADD MONEY (ADMIN) =================
void addMoney() {
    string acc;
    cout << "Enter Account Number: ";
    cin >> acc;

    int amt = getIntInputSimple("Enter Amount: ");
    if (amt <= 0) {
        cout << "❌ Invalid amount\n";
        return;
    }

    for (int i = 0; i < accounts.size(); i++) {
        if (accounts[i][0] == acc) {
            int bal = stoi(accounts[i][2]);
            bal += amt;
            accounts[i][2] = to_string(bal);
            addTransaction(i, "Admin added money: " + to_string(amt));
            cout << "💰 Money Added Successfully\n";
            return;
        }
    }
    cout << "❌ Account Not Found\n";
}

// ================= SHOW ALL ACCOUNTS =================
void showAllAccounts() {
    if (accounts.empty()) {
        cout << "No accounts available\n";
        return;
    }

    cout << "\nAccNo\tStatus\tBalance\n";
    for (auto& a : accounts) {
        cout << a[0] << "\t" << a[3] << "\t" << a[2] << endl;
    }

    cout << "\n(Unfreeze not allowed here)\n";
}

// ================= OTP GENERATE =================
string generateOTP() {
    srand(time(0));
    int otp = rand() % 900000 + 100000; // 6-digit OTP
    return to_string(otp);
}

// ================= USER FROZEN FLOW =================
void frozenUserMenu(const string& accNo) {
    cout << "\n🚫 Account Frozen\n";
    cout << "1. Send Message to Admin\n";
    cout << "2. Exit\n";
    int ch = getIntInputSimple("Choice: ");

    if (ch != 1) return;

    for (auto& m : adminMessages) {
        if (m[0] == accNo && m[2] == "Pending") {
            cout << "❌ You already have an active message\n";
            return;
        }
    }

    string msg;
    cout << "Enter message for admin: ";
    cin.ignore();
    getline(cin, msg);

    adminMessages.push_back({
        accNo,
        msg,
        "Pending"
    });

    cout << "📨 Message Sent. Account will remain frozen until admin action\n";
}

// ================= TRANSFER MONEY =================
void transferMoney(int sender) {
    if (accounts[sender][3] == "Frozen") {
        cout << "❌ Your account is frozen\n";
        return;
    }

    string rec;
    cout << "Enter Receiver Account: ";
    cin >> rec;

    int amt = getIntInputSimple("Enter Amount: ");
    int senderBal = stoi(accounts[sender][2]);

    if (amt <= 0 || senderBal < amt) {
        cout << "❌ Invalid transfer\n";
        return;
    }

    for (int i = 0; i < accounts.size(); i++) {
        if (accounts[i][0] == rec) {
            if (accounts[i][3] == "Frozen") {
                cout << "❌ Receiver account is frozen\n";
                return;
            }
            string Otp = generateOTP();
            cout << "OTP Generated! " << Otp << endl;
            string input;
            cout << "Enter OTP: " ;
            cin >> input;
            if(Otp == input){
            accounts[sender][2] = to_string(senderBal - amt);
            accounts[i][2] = to_string(stoi(accounts[i][2]) + amt);

            addTransaction(sender, "Transferred " + to_string(amt) + " to " + rec);
            addTransaction(i, "Received " + to_string(amt) + " from " + accounts[sender][0]);

            cout << "🔁 Transfer Successful\n";
            return;
            }else{
                cout << "\nWrong OTP // Entered!\ntry again" << endl;
                return;

            }
        }
    }
    cout << "❌ Receiver Not Found\n";
}

// ================= SHOW HISTORY =================
void showHistory(int index) {
    if (accounts[index].size() <= 4) {
        cout << "No transactions yet\n";
        return;
    }

    cout << "\n--- TRANSACTION HISTORY ---\n";
    for (int i = 4; i < accounts[index].size(); i++) {
        cout << (i - 3) << ". " << accounts[index][i] << endl;
    }
}

// ================= USER LOGIN =================
int userLogin() {
    string acc, pass;
    cout << "Enter Account Number: ";
    cin >> acc;

    int idx = -1;
    for (int i = 0; i < accounts.size(); i++) {
        if (accounts[i][0] == acc) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        cout << "❌ Account not found\n";
        return -1;
    }

    if (accounts[idx][3] == "Frozen") {
        frozenUserMenu(acc);
        return -1;
    }

    int attempts = 0;
    while (attempts < 3) {
        cout << "Password: ";
        cin >> pass;

        if (accounts[idx][1] == pass){
             string oTp = generateOTP();
             cout << "OTP Generated: " << oTp <<  endl;
             string input;
             cout << "Enter OTP: ";
             cin >> input;
             if(oTp ==  input) {
                cout << "Login Successfully!" << endl;
                return idx;
             }else{
                cout << "Wrong OTP! entered//\nLogin Failed!\tTry again later" << endl;
                return -1;
             }
        }
        attempts++;
        cout << "❌ Wrong password (" << 3 - attempts << " left)\n";
    }

    accounts[idx][3] = "Frozen";
    frozenUserMenu(acc);
    return -1;
}

// ================= ADMIN MESSAGE BOX =================
void messageBox() {
    if (adminMessages.empty()) {
        cout << "📭 No messages\n";
        return;
    }

    for (int i = 0; i < adminMessages.size(); i++) {
        if (adminMessages[i][2] == "Pending") {
            cout << "\n[Account: " << adminMessages[i][0] << "]\n";
            cout << "Message: " << adminMessages[i][1] << endl;
            cout << "Status: Pending\n";

            cout << "1. Unfreeze\n2. Ignore\nChoice: ";
            int ch;
            cin >> ch;

            if (ch == 1) {
                for (auto& acc : accounts) {
                    if (acc[0] == adminMessages[i][0]) {
                        acc[3] = "Open";
                        break;
                    }
                }
                adminMessages[i][2] = "Resolved";
                cout << "✅ Account Unfrozen\n";
            }
        }
    }
}

// ================= MAIN =================
int main() {
    cout << "\n__________________________________________________________________________\n";
    int choice;

    do {
        cout << "\n===== ATM SYSTEM =====\n";
        cout << "1. Admin Login\n";
        cout << "2. User Login\n";
        cout << "3. Exit\n";
        choice = getIntInputSimple("Choice: ");

        if (choice == 1) {
            string adminPass;
            cout << "Enter ADMIN_PASS: ";
            cin >> adminPass;

            if (adminPass == "Jhandir_Bro") {
                int ch;
                do {
                    cout << "\n--- ADMIN PANEL ---\n";
                    cout << "1. Create Account\n";
                    cout << "2. Add Money\n";
                    cout << "3. Show All Accounts\n";
                    cout << "4. Message Box\n";
                    cout << "5. Logout\n";
                    ch = getIntInputSimple("Choice: ");

                    if (ch == 1) createAccount();
                    else if (ch == 2) addMoney();
                    else if (ch == 3) showAllAccounts();
                    else if (ch == 4) messageBox();

                } while (ch != 5);
            } else {
                cout << "❌ Wrong admin password\n";
            }
        }
        else if (choice == 2) {
            int index = userLogin();
            if (index >= 0) {
                int ch;
                do {
                    cout << "\n--- USER MENU ---\n";
                    cout << "1. Check Balance\n";
                    cout << "2. Transfer Money\n";
                    cout << "3. Transaction History\n";
                    cout << "4. Logout\n";
                    ch = getIntInputSimple("Choice: ");

                    if (ch == 1) {
                        cout << "💳 Balance: " << accounts[index][2] << endl;
                        addTransaction(index, "Checked balance");
                    }
                    else if (ch == 2) {    
                        transferMoney(index);
                    }
                    else if (ch == 3) showHistory(index);

                } while (ch != 4);
            }
        }

    } while (choice != 3);

    cout << "👋 Program Ended\n";
    return 0;
}