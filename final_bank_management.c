#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TRANSACTIONS 100
#define MAX_ACCOUNTS 1000

// Structure for transactions
struct Transaction {
    char description[150];
    float amount;
    time_t timestamp;
    char transaction_id[13]; // 12-digit unique ID + null terminator
};

// Structure for account details
struct Account {
    long long accountNumber;
    char name[100];
    float balance;
    char password_hash[65]; // Simplified password storage
    int accountType; // 0: Savings, 1: Checking, 2: Business
    struct Transaction transactions[MAX_TRANSACTIONS];
    int transactionCount;
    int isLocked;
    time_t lastLogin;
};

// Function prototypes
long long generateAccountNumber(struct Account *accounts, int count);
int isAccountNumberUnique(struct Account *accounts, int count, long long accNum);
void createAccount(struct Account *accounts, int *count);
void deposit(struct Account *accounts, int count);
void withdraw(struct Account *accounts, int count);
void checkBalance(struct Account *accounts, int count);
void displayAccount(struct Account *accounts, int count);
int authenticate(struct Account *accounts, int count, long long accNum);
void saveToFile(struct Account *accounts, int count);
void loadFromFile(struct Account *accounts, int *count);
void calculateInterest(struct Account *accounts, int count);
void createBackup(struct Account *accounts, int count);
void generateTransactionID(char *id);
void transferMoney(struct Account *accounts, int count);
void clearInputBuffer(void);

// Main function
int main() {
    struct Account accounts[MAX_ACCOUNTS];
    int count = 0;
    int choice;

    // Initialize accounts array
    memset(accounts, 0, sizeof(accounts));
    srand((unsigned int)time(NULL)); // Seed random number generator
    loadFromFile(accounts, &count);

    while (1) {
        printf("\n=== Advanced Bank Management System v2.0 ===\n");
        printf("1. Create New Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Check Balance\n");
        printf("5. Display Account Details\n");
        printf("6. Calculate Interest\n");
        printf("7. Create Backup\n");
        printf("8. Transfer Money\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: createAccount(accounts, &count); break;
            case 2: deposit(accounts, count); break;
            case 3: withdraw(accounts, count); break;
            case 4: checkBalance(accounts, count); break;
            case 5: displayAccount(accounts, count); break;
            case 6: calculateInterest(accounts, count); break;
            case 7: createBackup(accounts, count); break;
            case 8: transferMoney(accounts, count); break;
            case 9:
                saveToFile(accounts, count);
                printf("Thank you for using our banking system!\n");
                return 0;
            default: printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

// Clear input buffer
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Generate a unique random 10-digit account number
long long generateAccountNumber(struct Account *accounts, int count) {
    long long accNum;
    do {
        accNum = 1000000000LL + ((long long)rand() % 9000000000LL); // 10-digit number
    } while (!isAccountNumberUnique(accounts, count, accNum));
    return accNum;
}

// Check if account number is unique
int isAccountNumberUnique(struct Account *accounts, int count, long long accNum) {
    for (int i = 0; i < count; i++) {
        if (accounts[i].accountNumber == accNum) {
            return 0; // Not unique
        }
    }
    return 1; // Unique
}

// Generate a 12-digit transaction ID
void generateTransactionID(char *id) {
    snprintf(id, 13, "%012lld", (long long)(rand() % 1000000000000LL));
}

// Create a new account
void createAccount(struct Account *accounts, int *count) {
    if (*count >= MAX_ACCOUNTS) {
        printf("Maximum account limit reached!\n");
        return;
    }

    struct Account *newAcc = &accounts[*count];
    printf("\nEnter account holder's name: ");
    if (scanf(" %99[^\n]s", newAcc->name) != 1) {
        printf("Error reading name!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    printf("Set a password: ");
    if (scanf("%64s", newAcc->password_hash) != 1) {
        printf("Error reading password!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    printf("Select account type (0: Savings, 1: Checking, 2: Business): ");
    if (scanf("%d", &newAcc->accountType) != 1 || newAcc->accountType < 0 || newAcc->accountType > 2) {
        printf("Invalid account type!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    newAcc->accountNumber = generateAccountNumber(accounts, *count);
    newAcc->balance = 0.0;
    newAcc->transactionCount = 1;
    newAcc->isLocked = 0;
    time(&newAcc->lastLogin);

    strcpy(newAcc->transactions[0].description, "Account Created");
    newAcc->transactions[0].amount = 0.0;
    time(&newAcc->transactions[0].timestamp);
    generateTransactionID(newAcc->transactions[0].transaction_id);

    printf("Account created successfully!\n");
    printf("Your account number is: %lld\n", newAcc->accountNumber);
    (*count)++;
    saveToFile(accounts, *count);
}

// Authenticate user with retry limit
int authenticate(struct Account *accounts, int count, long long accNum) {
    char password[65];
    int attempts = 0;
    const int max_attempts = 3;

    for (int i = 0; i < count; i++) {
        if (accounts[i].accountNumber == accNum) {
            if (accounts[i].isLocked) {
                printf("Account is locked! Contact customer support.\n");
                return -1;
            }
            while (attempts < max_attempts) {
                printf("Enter password (attempt %d/%d): ", attempts + 1, max_attempts);
                if (scanf("%64s", password) != 1) {
                    printf("Error reading password!\n");
                    clearInputBuffer();
                    return -1;
                }
                clearInputBuffer();
                if (strcmp(accounts[i].password_hash, password) == 0) {
                    time(&accounts[i].lastLogin);
                    return i;
                }
                printf("Incorrect password!\n");
                attempts++;
            }
            accounts[i].isLocked = 1;
            printf("Too many failed attempts. Account locked.\n");
            saveToFile(accounts, count);
            return -1;
        }
    }
    printf("Account not found!\n");
    return -1;
}

// Deposit money
void deposit(struct Account *accounts, int count) {
    long long accNum;
    float amount;

    printf("\nEnter account number: ");
    if (scanf("%lld", &accNum) != 1) {
        printf("Invalid input! Please enter a valid account number.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int index = authenticate(accounts, count, accNum);
    if (index != -1) {
        printf("Enter amount to deposit: $");
        if (scanf("%f", &amount) != 1 || amount <= 0) {
            printf("Invalid amount! Must be positive.\n");
            clearInputBuffer();
            return;
        }
        clearInputBuffer();

        accounts[index].balance += amount;
        if (accounts[index].transactionCount < MAX_TRANSACTIONS) {
            snprintf(accounts[index].transactions[accounts[index].transactionCount].description,
                     150, "Deposited $%.2f", amount);
            accounts[index].transactions[accounts[index].transactionCount].amount = amount;
            time(&accounts[index].transactions[accounts[index].transactionCount].timestamp);
            generateTransactionID(accounts[index].transactions[accounts[index].transactionCount].transaction_id);
            accounts[index].transactionCount++;
        } else {
            printf("Warning: Transaction history full, not recorded.\n");
        }
        printf("Deposit successful! New balance: $%.2f\n", accounts[index].balance);
        saveToFile(accounts, count);
    }
}

// Withdraw money
void withdraw(struct Account *accounts, int count) {
    long long accNum;
    float amount;

    printf("\nEnter account number: ");
    if (scanf("%lld", &accNum) != 1) {
        printf("Invalid input! Please enter a valid account number.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int index = authenticate(accounts, count, accNum);
    if (index != -1) {
        printf("Enter amount to withdraw: $");
        if (scanf("%f", &amount) != 1 || amount <= 0) {
            printf("Invalid amount! Must be positive.\n");
            clearInputBuffer();
            return;
        }
        clearInputBuffer();

        if (amount <= accounts[index].balance) {
            accounts[index].balance -= amount;
            if (accounts[index].transactionCount < MAX_TRANSACTIONS) {
                snprintf(accounts[index].transactions[accounts[index].transactionCount].description,
                         150, "Withdrawn $%.2f", amount);
                accounts[index].transactions[accounts[index].transactionCount].amount = -amount;
                time(&accounts[index].transactions[accounts[index].transactionCount].timestamp);
                generateTransactionID(accounts[index].transactions[accounts[index].transactionCount].transaction_id);
                accounts[index].transactionCount++;
            } else {
                printf("Warning: Transaction history full, not recorded.\n");
            }
            printf("Withdrawal successful! New balance: $%.2f\n", accounts[index].balance);
            saveToFile(accounts, count);
        } else {
            printf("Insufficient balance!\n");
        }
    }
}

// Check balance
void checkBalance(struct Account *accounts, int count) {
    long long accNum;

    printf("\nEnter account number: ");
    if (scanf("%lld", &accNum) != 1) {
        printf("Invalid input! Please enter a valid account number.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int index = authenticate(accounts, count, accNum);
    if (index != -1) {
        printf("Account Holder: %s\n", accounts[index].name);
        printf("Account Type: %s\n",
               accounts[index].accountType == 0 ? "Savings" :
               accounts[index].accountType == 1 ? "Checking" : "Business");
        printf("Current Balance: $%.2f\n", accounts[index].balance);
        printf("Last Login: %s", ctime(&accounts[index].lastLogin));
    }
}

// Display account details
void displayAccount(struct Account *accounts, int count) {
    long long accNum;

    printf("\nEnter account number: ");
    if (scanf("%lld", &accNum) != 1) {
        printf("Invalid input! Please enter a valid account number.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int index = authenticate(accounts, count, accNum);
    if (index != -1) {
        printf("\nAccount Details:\n");
        printf("Account Number: %lld\n", accounts[index].accountNumber);
        printf("Account Holder: %s\n", accounts[index].name);
        printf("Account Type: %s\n",
               accounts[index].accountType == 0 ? "Savings" :
               accounts[index].accountType == 1 ? "Checking" : "Business");
        printf("Balance: $%.2f\n", accounts[index].balance);
        printf("Status: %s\n", accounts[index].isLocked ? "Locked" : "Active");
        printf("\nTransaction History:\n");
        for (int i = 0; i < accounts[index].transactionCount; i++) {
            printf("%d. [%s] %s - $%.2f - %s", i + 1,
                   accounts[index].transactions[i].transaction_id,
                   accounts[index].transactions[i].description,
                   accounts[index].transactions[i].amount,
                   ctime(&accounts[index].transactions[i].timestamp));
        }
    }
}

// Calculate interest
void calculateInterest(struct Account *accounts, int count) {
    long long accNum;

    printf("\nEnter account number: ");
    if (scanf("%lld", &accNum) != 1) {
        printf("Invalid input! Please enter a valid account number.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int index = authenticate(accounts, count, accNum);
    if (index != -1) {
        float interest_rate;
        switch (accounts[index].accountType) {
            case 0: interest_rate = 0.045; break; // Savings: 4.5%
            case 1: interest_rate = 0.01; break;  // Checking: 1%
            case 2: interest_rate = 0.025; break; // Business: 2.5%
            default: interest_rate = 0; break;
        }

        float interest = accounts[index].balance * interest_rate;
        accounts[index].balance += interest;
        if (accounts[index].transactionCount < MAX_TRANSACTIONS) {
            snprintf(accounts[index].transactions[accounts[index].transactionCount].description,
                     150, "Interest credited $%.2f at %.1f%%", interest, interest_rate * 100);
            accounts[index].transactions[accounts[index].transactionCount].amount = interest;
            time(&accounts[index].transactions[accounts[index].transactionCount].timestamp);
            generateTransactionID(accounts[index].transactions[accounts[index].transactionCount].transaction_id);
            accounts[index].transactionCount++;
        } else {
            printf("Warning: Transaction history full, interest not recorded.\n");
        }
        printf("Interest calculated and added: $%.2f\n", interest);
        printf("New balance: $%.2f\n", accounts[index].balance);
        saveToFile(accounts, count);
    }
}

// Transfer money between accounts
void transferMoney(struct Account *accounts, int count) {
    long long fromAcc, toAcc;
    float amount;

    printf("\nEnter your account number: ");
    if (scanf("%lld", &fromAcc) != 1) {
        printf("Invalid input! Please enter a valid account number.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int fromIndex = authenticate(accounts, count, fromAcc);
    if (fromIndex != -1) {
        printf("Enter recipient account number: ");
        if (scanf("%lld", &toAcc) != 1) {
            printf("Invalid input! Please enter a valid account number.\n");
            clearInputBuffer();
            return;
        }
        clearInputBuffer();

        int toIndex = -1;
        for (int i = 0; i < count; i++) {
            if (accounts[i].accountNumber == toAcc) {
                toIndex = i;
                break;
            }
        }

        if (toIndex == -1) {
            printf("Recipient account not found!\n");
            return;
        }

        printf("Enter amount to transfer: $");
        if (scanf("%f", &amount) != 1 || amount <= 0) {
            printf("Invalid amount! Must be positive.\n");
            clearInputBuffer();
            return;
        }
        clearInputBuffer();

        if (amount <= accounts[fromIndex].balance) {
            accounts[fromIndex].balance -= amount;
            accounts[toIndex].balance += amount;

            char trans_id[13];
            generateTransactionID(trans_id);

            if (accounts[fromIndex].transactionCount < MAX_TRANSACTIONS) {
                snprintf(accounts[fromIndex].transactions[accounts[fromIndex].transactionCount].description,
                         150, "Transferred $%.2f to %lld", amount, toAcc);
                accounts[fromIndex].transactions[accounts[fromIndex].transactionCount].amount = -amount;
                time(&accounts[fromIndex].transactions[accounts[fromIndex].transactionCount].timestamp);
                strcpy(accounts[fromIndex].transactions[accounts[fromIndex].transactionCount].transaction_id, trans_id);
                accounts[fromIndex].transactionCount++;
            } else {
                printf("Warning: Sender transaction history full, transfer not fully recorded.\n");
            }

            if (accounts[toIndex].transactionCount < MAX_TRANSACTIONS) {
                snprintf(accounts[toIndex].transactions[accounts[toIndex].transactionCount].description,
                         150, "Received $%.2f from %lld", amount, fromAcc);
                accounts[toIndex].transactions[accounts[toIndex].transactionCount].amount = amount;
                time(&accounts[toIndex].transactions[accounts[toIndex].transactionCount].timestamp);
                strcpy(accounts[toIndex].transactions[accounts[toIndex].transactionCount].transaction_id, trans_id);
                accounts[toIndex].transactionCount++;
            } else {
                printf("Warning: Recipient transaction history full, transfer not fully recorded.\n");
            }

            printf("Transfer successful!\n");
            printf("Your new balance: $%.2f\n", accounts[fromIndex].balance);
            saveToFile(accounts, count);
        } else {
            printf("Insufficient balance!\n");
        }
    }
}

// Save accounts to file
void saveToFile(struct Account *accounts, int count) {
    FILE *file = fopen("bank_data.dat", "wb");
    if (file == NULL) {
        perror("Error saving data");
        return;
    }
    if (fwrite(&count, sizeof(int), 1, file) != 1) {
        printf("Error writing account count!\n");
        fclose(file);
        return;
    }
    if (fwrite(accounts, sizeof(struct Account), count, file) != (size_t)count) {
        printf("Error writing accounts!\n");
        fclose(file);
        return;
    }
    fclose(file);
}

// Load accounts from file
void loadFromFile(struct Account *accounts, int *count) {
    FILE *file = fopen("bank_data.dat", "rb");
    if (file == NULL) {
        *count = 0;
        return;
    }
    if (fread(count, sizeof(int), 1, file) != 1) {
        printf("Error reading account count from file!\n");
        *count = 0;
        fclose(file);
        return;
    }
    if (*count > MAX_ACCOUNTS) {
        printf("File contains too many accounts! Loading limited to %d.\n", MAX_ACCOUNTS);
        *count = MAX_ACCOUNTS;
    }
    if (fread(accounts, sizeof(struct Account), *count, file) != (size_t)*count) {
        printf("Error reading accounts from file!\n");
        *count = 0;
    }
    fclose(file);
}

// Create a backup file
void createBackup(struct Account *accounts, int count) {
    char filename[40];
    time_t now;
    time(&now);
    strftime(filename, 40, "backup_%Y%m%d_%H%M%S.dat", localtime(&now));

    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error creating backup");
        return;
    }
    if (fwrite(&count, sizeof(int), 1, file) != 1) {
        printf("Error writing account count to backup!\n");
        fclose(file);
        return;
    }
    if (fwrite(accounts, sizeof(struct Account), count, file) != (size_t)count) {
        printf("Error writing accounts to backup!\n");
        fclose(file);
        return;
    }
    fclose(file);
    printf("Backup created successfully: %s\n", filename);
}