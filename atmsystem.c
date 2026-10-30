#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ACCOUNTS_FILE "accounts.dat"
#define LOG_FILE "logs.txt"
#define MAX_NAME 50
#define MAX_ACCOUNTS 100
#define XOR_KEY 0x5A  // Simple XOR key for PIN encryption/decryption (Basically majaba)

// Structure to hold individual account details
typedef struct {
    int accountNumber;
    char name[MAX_NAME];
    int pin; // Stored in encrypted form. This is common knowledge sabu ni pin.
    double balance;
} Account;

// Function Prototypes
void createAccount();
int authenticateUser(int accNum, int pin);
void mainMenu(int accNum);
void checkBalance(int accNum);
void deposit(int accNum);
void withdraw(int accNum);
void transfer(int accNum);
void viewTransactionHistory(int accNum);
void changePIN(int accNum);
void clearInputLine(void);
void logTransaction(int accNum, const char *type, double amount);
int encryptDecryptPIN(int pin);
int getAccountIndex(int accNum, Account *accounts, int count);
int loadAccounts(Account *accounts);
int saveAccounts(Account *accounts, int count);

int main() {
    int choice, accNum, pin;
    // Load existing accounts from file. Plus this is the main function that runs the ATM system.
    while (1) {
        printf("\n====================================\n");
        printf("       ATM MANAGEMENT SYSTEM        \n");
        printf("====================================\n");
        printf("1. Create New Account\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            break;
        }

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                printf("\nEnter Account Number: ");
                scanf("%d", &accNum);
                printf("Enter 4-digit PIN: ");
                scanf("%d", &pin);

                if (authenticateUser(accNum, pin)) {
                    printf("\nLogin successful! Welcome.\n");
                    mainMenu(accNum);
                } else {
                    printf("\nError: Invalid Account Number or PIN.\n");
                }
                break;
            case 3:
                printf("\nThank you for using the ATM Management System. Goodbye!\n");
                exit(0);
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }
    return 0;
}

// Simple XOR encryption/decryption function for PIN security
int encryptDecryptPIN(int pin) {
    return pin ^ XOR_KEY;
}

void clearInputLine(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

// Helper to find index of account in array
int getAccountIndex(int accNum, Account *accounts, int count) {
    for (int i = 0; i < count; i++) {
        if (accounts[i].accountNumber == accNum) {
            return i;
        }
    }
    return -1;
}

// Loads accounts from binary file into array
int loadAccounts(Account *accounts) {
    FILE *file = fopen(ACCOUNTS_FILE, "rb");
    if (!file) return 0; // File doesn't exist, yet.

    int count = 0;
    while (fread(&accounts[count], sizeof(Account), 1, file)) {
        count++;
    }
    fclose(file);
    return count;
}

// Saves accounts array back to binary file. Basically this function ni ya kusave the account data to a binary file.
int saveAccounts(Account *accounts, int count) {
    FILE *file = fopen(ACCOUNTS_FILE, "wb");
    if (!file) {
        printf("Error: Unable to save account data!\n");
        return 0;
    }
    if (fwrite(accounts, sizeof(Account), count, file) != (size_t)count) {
        printf("Error: Unable to save account data!\n");
        fclose(file);
        return 0;
    }
    if (fclose(file) != 0) {
        printf("Error: Unable to save account data!\n");
        return 0;
    }
    return 1;
}

// Appends transaction logs to a text file. Sikudai kutengeneza binary log ingine.
void logTransaction(int accNum, const char *type, double amount) {
    FILE *file = fopen(LOG_FILE, "a");
    if (!file) return;

    time_t now = time(NULL);
    char *timestamp = ctime(&now);
    timestamp[strcspn(timestamp, "\n")] = 0; // Remove trailing newline

    fprintf(file, "[%s] Acc: %d | Type: %s | Amount: Ksh %.2f\n", timestamp, accNum, type, amount);
    fclose(file);
}

// Creates a new account record
void createAccount() {
    Account accounts[MAX_ACCOUNTS];
    int count = loadAccounts(accounts);

    if (count >= MAX_ACCOUNTS) {
        printf("\nSystem full. Cannot create more accounts.\n");
        return;
    }

    Account newAcc;
    // Generate simple account number starting at 1001
    newAcc.accountNumber = 1001 + count;

    printf("\n--- Create Account ---\n");
    printf("Account Number Assigned: %d\n", newAcc.accountNumber);
    
    printf("Enter Full Name: ");
    getchar(); // Clear buffer
    fgets(newAcc.name, MAX_NAME, stdin);
    newAcc.name[strcspn(newAcc.name, "\n")] = 0; // Remove trailing newline

    printf("Set a 4-digit PIN: ");
    int pin;
    scanf("%d", &pin);
    newAcc.pin = encryptDecryptPIN(pin); // Store encrypted PIN

    printf("Initial Deposit Amount (Ksh): ");
    scanf("%lf", &newAcc.balance);

    accounts[count] = newAcc;
    if (!saveAccounts(accounts, count + 1)) {
        return;
    }

    logTransaction(newAcc.accountNumber, "ACCOUNT_CREATED", newAcc.balance);
    printf("\nAccount created successfully!\n");
}

// For Validating login credentials
int authenticateUser(int accNum, int pin) {
    Account accounts[MAX_ACCOUNTS];
    int count = loadAccounts(accounts);
    int index = getAccountIndex(accNum, accounts, count);

    if (index != -1) {
        int decryptedPin = encryptDecryptPIN(accounts[index].pin);
        if (decryptedPin == pin) {
            return 1;
        }
    }
    return 0;
}

// ATM User Dashboard
void mainMenu(int accNum) {
    int choice;
    while (1) {
        printf("\n------------------------------------\n");
        printf("             ATM MENU               \n");
        printf("------------------------------------\n");
        printf("1. Check Balance\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Transfer Money\n");
        printf("5. View Transaction History\n");
        printf("6. Change PIN\n");
        printf("7. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: checkBalance(accNum); break;
            case 2: deposit(accNum); break;
            case 3: withdraw(accNum); break;
            case 4: transfer(accNum); break;
            case 5: viewTransactionHistory(accNum); break;
            case 6: changePIN(accNum); break;
            case 7:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice!\n");
        }
    }
}

// Displays current balance. It retrieves the account details from the binary file and shows the balance to the user.
void checkBalance(int accNum) {
    Account accounts[MAX_ACCOUNTS];
    int count = loadAccounts(accounts);
    int idx = getAccountIndex(accNum, accounts, count);

    if (idx != -1) {
        printf("\nAccount Holder: %s\n", accounts[idx].name);
        printf("Current Balance: Ksh %.2f\n", accounts[idx].balance);
    }
}

// Handles cash deposits. The function is connected to the main menu and allows users to deposit money into their account.

void deposit(int accNum) {
    Account accounts[MAX_ACCOUNTS];
    int count = loadAccounts(accounts);
    int idx = getAccountIndex(accNum, accounts, count);

    if (idx != -1) {
        double amount;
        printf("Enter amount to deposit (Ksh): ");
        scanf("%lf", &amount);

        if (amount <= 0) {
            printf("Invalid amount!\n");
            return;
        }

        accounts[idx].balance += amount;
        if (!saveAccounts(accounts, count)) {
            return;
        }
        logTransaction(accNum, "DEPOSIT", amount);
        printf("Successfully deposited Ksh %.2f. New Balance: Ksh %.2f\n", amount, accounts[idx].balance);
    }
}

// Handles cash withdrawals. Same shi as deposit function.
void withdraw(int accNum) {
    Account accounts[MAX_ACCOUNTS];
    int count = loadAccounts(accounts);
    int idx = getAccountIndex(accNum, accounts, count);

    if (idx != -1) {
        double amount;
        printf("Enter amount to withdraw (Ksh): ");
        scanf("%lf", &amount);

        if (amount <= 0) {
            printf("Invalid amount!\n");
            return;
        }
        if (amount > accounts[idx].balance) {
            printf("Insufficient funds! Current balance: Ksh %.2f\n", accounts[idx].balance);
            return;
        }

        accounts[idx].balance -= amount;
        if (!saveAccounts(accounts, count)) {
            return;
        }
        logTransaction(accNum, "WITHDRAWAL", amount);
        printf("Successfully withdrew Ksh %.2f. Remaining Balance: Ksh %.2f\n", amount, accounts[idx].balance);
    }
}

// Transfers funds between two internal accounts. BTW siko sure kama inafanya soo mchunge saa ya presentation
void transfer(int accNum) {
    Account accounts[MAX_ACCOUNTS];
    int count = loadAccounts(accounts);
    int senderIdx = getAccountIndex(accNum, accounts, count);

    if (senderIdx == -1) return;

    int recipientAcc;
    double amount;

    printf("Enter recipient Account Number: ");
    scanf("%d", &recipientAcc);

    if (recipientAcc == accNum) {
        printf("You cannot transfer money to your own account!\n");
        return;
    }

    int recipientIdx = getAccountIndex(recipientAcc, accounts, count);
    if (recipientIdx == -1) {
        printf("Recipient account not found!\n");
        return;
    }

    printf("Enter amount to transfer (Ksh): ");
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf("Invalid amount!\n");
        return;
    }
    if (amount > accounts[senderIdx].balance) {
        printf("Insufficient balance!\n");
        return;
    }

    // Executing money transfers
    accounts[senderIdx].balance -= amount;
    accounts[recipientIdx].balance += amount;

    if (!saveAccounts(accounts, count)) {
        return;
    }

    logTransaction(accNum, "TRANSFER_OUT", amount);
    logTransaction(recipientAcc, "TRANSFER_IN", amount);

    printf("Successfully transferred Ksh %.2f to Account %d.\n", amount, recipientAcc);
}

// Lets an authenticated account holder replace their PIN after verifying the current PIN.
void changePIN(int accNum) {
    Account accounts[MAX_ACCOUNTS];
    int count = loadAccounts(accounts);
    int idx = getAccountIndex(accNum, accounts, count);

    if (idx == -1) {
        printf("Error: Account not found.\n");
        return;
    }

    int currentPIN, newPIN, confirmedPIN;
    printf("Enter current 4-digit PIN: ");
    int inputResult = scanf("%d", &currentPIN);
    clearInputLine();
    if (inputResult != 1) {
        printf("Invalid PIN input.\n");
        return;
    }
    if (encryptDecryptPIN(accounts[idx].pin) != currentPIN) {
        printf("Incorrect current PIN.\n");
        return;
    }

    printf("Enter new 4-digit PIN: ");
    inputResult = scanf("%d", &newPIN);
    clearInputLine();
    if (inputResult != 1 || newPIN < 1000 || newPIN > 9999) {
        printf("PIN must be exactly 4 digits.\n");
        return;
    }

    printf("Confirm new 4-digit PIN: ");
    inputResult = scanf("%d", &confirmedPIN);
    clearInputLine();
    if (inputResult != 1 || confirmedPIN != newPIN) {
        printf("PIN confirmation does not match.\n");
        return;
    }
    if (newPIN == currentPIN) {
        printf("The new PIN must be different from the current PIN.\n");
        return;
    }

    accounts[idx].pin = encryptDecryptPIN(newPIN);
    if (!saveAccounts(accounts, count)) {
        return;
    }

    logTransaction(accNum, "PIN_CHANGE", 0.0);
    printf("PIN changed successfully.\n");
}

// Displays past transaction logs for the logged-in user
void viewTransactionHistory(int accNum) {
    FILE *file = fopen(LOG_FILE, "r");
    if (!file) {
        printf("\nNo transaction history available.\n");
        return;
    }

    char line[256];
    char targetAcc[20];
    snprintf(targetAcc, sizeof(targetAcc), "Acc: %d", accNum);

    printf("\n--- Transaction History for Account %d ---\n", accNum);
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, targetAcc)) {
            printf("%s", line);
            found = 1;
        }
    }
    fclose(file);

    if (!found) {
        printf("No records found for this account.\n");
    }
}