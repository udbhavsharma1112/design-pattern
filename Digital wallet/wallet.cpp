#include <bits/stdc++.h>
using namespace std;

class User;
class Transaction;

// -------------------- ICurrency --------------------
class ICurrency {
protected:
    double balance;
    double perDollar;
public:
    ICurrency(): balance(0), perDollar(0) {}
    ICurrency(double balance, double perDollar) : balance(balance), perDollar(perDollar) {}
    
    virtual ~ICurrency() {}

    ICurrency& operator+=(const ICurrency &currency) {
        this->balance += currency.getBalance();
        return *this;
    }

    ICurrency& operator-=(const ICurrency &currency) {
        this->balance -= currency.getBalance();
        return *this;
    }

    double getBalance() const {
        return this->balance;
    }

    void setBalance(double balance) {
        this->balance = balance;
    }
    
    double getPerDollar() const {
        return this->perDollar;
    }
};

// -------------------- Currencies --------------------
class FkRupee : public ICurrency {
public:
    FkRupee(double balance) : ICurrency(balance, 80) {}
};

class IndianRupee : public ICurrency {
public:
    IndianRupee(double balance) : ICurrency(balance, 80) {}
};

// -------------------- History --------------------
class History {
public:
    double amount;
    int userId;
    string userName;
    bool isCredit;

    History() : amount(0), userId(-1), userName(""), isCredit(false) {}

    History(double amount, int userId, string userName, bool isCredit) 
        : amount(amount), userId(userId), userName(userName), isCredit(isCredit) {}

    void printHistory() const {
        string type = isCredit ? "CREDIT" : "DEBIT";
        cout << userId << "\t" << userName << "\t" << amount << "\t" << type << endl;
    }
};

// -------------------- AccountStatement --------------------
class AccountStatement {
    vector<History> statement;
public:
    void addStatement(const History& history) {
        statement.push_back(history);
    }

    void printAccountStatement() const {
        cout << "ID\tSentTo\tAmount\tType" << endl;
        cout << "--------------------------------" << endl;
        for (const auto& history : statement) {
            history.printHistory();
        }
    }
};

// -------------------- Transaction --------------------
class Transaction {
public:
    bool transferMoney(User& sentFrom, User& sentTo, double amount);
};

// -------------------- Wallet --------------------
class Wallet {
private:
    unique_ptr<ICurrency> balance;
    AccountStatement statement;
    Transaction transaction; 

public:
    Wallet() = default;

    Wallet(ICurrency* initialBalance) : balance(initialBalance) {}

    double getCurrentBalance() const {
        return balance ? balance->getBalance() : 0.0;
    }

    ICurrency* getBalanceObject() const {
        return balance.get();
    }

    bool addBalance(double value) {
        if (balance) {
            balance->setBalance(balance->getBalance() + value);
            return true;
        }
        return false;
    }

    bool withdrawBalance(double value) {
        if (!balance || balance->getBalance() < value) {
            cout << "Sorry, Insufficient balance" << endl;
            return false;
        }
        balance->setBalance(balance->getBalance() - value);
        return true;
    }

    bool withdrawBalance(double value, int userId, string userName) {
        if (!balance || balance->getBalance() < value) {
            cout << "Sorry, Insufficient balance" << endl;
            return false;
        }
        
        History history(value, userId, userName, false); // false for debit
        statement.addStatement(history);
        balance->setBalance(balance->getBalance() - value);
        return true;
    }

    bool addBalance(double value, int userId, string userName) {
        if (!balance) return false;
        
        History history(value, userId, userName, true); // true for credit
        statement.addStatement(history);
        balance->setBalance(balance->getBalance() + value);
        return true;
    }

    bool transferMoney(User& sentFrom, User& sentTo, double amount) {
        return transaction.transferMoney(sentFrom, sentTo, amount);
    }

    void getAccountStatement() const {
        statement.printAccountStatement();
    }
};

// -------------------- User --------------------
class User {
    int userId;
    string userName;
    Wallet wallet;

public:
    User() : userId(-1), userName("") {}

    User(int userId, string userName, ICurrency* balance) 
        : userId(userId), userName(userName), wallet(balance) {}

    int getUserId() const {
        return userId;
    }

    string getUserName() const {
        return userName;
    }
    
    double getBalance() const {
        return wallet.getCurrentBalance();
    }

    ICurrency* getBalanceObject() const {
        return wallet.getBalanceObject();
    }

    bool withdrawBalance(double amount, int userId, string userName) {
        return wallet.withdrawBalance(amount, userId, userName);
    }

    bool addBalance(double amount, int userId, string userName) {
        return wallet.addBalance(amount, userId, userName);
    }

    bool transferMoney(User& sentTo, double amount) {
        return wallet.transferMoney(*this, sentTo, amount);
    }

    void getAccountStatement() const {
        cout << "Account Statement for " << userName << " (ID: " << userId << ")" << endl;
        wallet.getAccountStatement();
    }
};

// -------------------- Transaction Implementation --------------------
bool Transaction::transferMoney(User& sentFrom, User& sentTo, double amount) {
    bool success = sentFrom.withdrawBalance(amount, sentTo.getUserId(), sentTo.getUserName());
    if (success) {
        sentTo.addBalance(amount, sentFrom.getUserId(), sentFrom.getUserName());
        cout << "Money Transfer of " << amount << " successful to " << sentTo.getUserName() << endl;
    }
    return success;
}

// -------------------- Main Function --------------------
int main() {
    ICurrency* balanceA = new FkRupee(1000.0);
    ICurrency* balanceB = new FkRupee(500.0);

    User userA(1, "Udbhav", balanceA);
    User userB(2, "Dora", balanceB);

    userA.transferMoney(userB, 200.0);
    
    cout << "\nUser A's Statement:" << endl;
    userA.getAccountStatement();
    
    cout << "\nUser B's Statement:" << endl;
    userB.getAccountStatement();
    
    cout << "\nFinal Balances:" << endl;
    cout << "User A: " << userA.getBalance() << endl;
    cout << "User B: " << userB.getBalance() << endl;

    ICurrency* balanceC = new FkRupee(10000.0);
    User userC(3, "Murarka", balanceC);
    userC.transferMoney(userA, 800.01);
    userB.transferMoney(userA,500);
    userA.getAccountStatement();
    cout<<"balance:"<<userA.getBalance()<<endl;
    userB.transferMoney(userA,500);
    userA.getAccountStatement();

    
    return 0;
}