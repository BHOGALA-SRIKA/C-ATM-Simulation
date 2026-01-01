#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ACCOUNTS_FILE "accounts.txt"
#define MAX_ACCOUNTS 10
#define HISTORY_LEN 100

// --- Structure Definition ---
typedef struct {
    int accountNumber;
    int pin;
    float balance;
} Account;

// --- Global Variables ---
Account accounts[MAX_ACCOUNTS];
int numAccounts = 0;
int loggedIn = -1;

// --- Function Prototypes ---
// (Required so functions can call each other regardless of order)
void initialize_data();
void login();
void atm_menu();
void create_account();
int authenticate(int accNum, int pin);
void deposit();
void withdraw();
void change_pin();
void show_history();
void save_accounts();
void load_accounts();
void log_transaction(const char *entry);
void clear_input_buffer();

// --- Main Function ---
int main() {
    initialize_data();
    printf("Welcome to the Secure ATM System\n");
    while (1) {
        login();
    }
    return 0;
}

// --- Logic Implementation ---

void initialize_data() {
    load_accounts();
    if (numAccounts == 0) {
        accounts[0] = (Account){1001, 1234, 5000.0f};
        accounts[1] = (Account){1002, 5678, 2500.0f};
        accounts[2] = (Account){1003, 9090, 750.75f};
        numAccounts = 3;
        save_accounts();
    }
}

void login() {
    int acc, pin, tries = 3;
    while (tries > 0) {
        printf("\n--- Login ---\nEnter Account Number (0 to create new): ");
        if (scanf("%d", &acc) != 1) {
            printf("Invalid input. Please enter numbers only.\n");
            clear_input_buffer();
            continue;
        }

        if (acc == 0) {
            create_account();
            continue;
        }

        printf("Enter PIN: ");
        if (scanf("%d", &pin) != 1) {
            printf("Invalid input.\n");
            clear_input_buffer();
            continue;
        }

        loggedIn = authenticate(acc, pin);
        if (loggedIn != -1) {
            printf("\nLogin successful! Welcome, Account %d.\n", acc);
            atm_menu();
            return; // Returns to main loop after logout
        }
        
        tries--;
        printf("Login failed. Tries left: %d\n", tries);
    }
    printf("Too many failed attempts. System Locking.\n");
    exit(0);
}

void atm_menu() {
    int choice;
    do {
        printf("\n== ATM Menu (Acc: %d) ==\n", accounts[loggedIn].accountNumber);
        printf("1. Balance Inquiry\n2. Deposit\n3. Withdraw\n4. Change PIN\n5. Transaction History\n6. Logout\n7. Create Additional Account\nChoice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Use numbers 1-7.\n");
            clear_input_buffer();
            continue;
        }

        switch (choice) {
            case 1: printf("Current Balance: Rs. %.2f\n", accounts[loggedIn].balance); break;
            case 2: deposit(); break;
            case 3: withdraw(); break;
            case 4: change_pin(); break;
            case 5: show_history(); break;
            case 6: loggedIn = -1; printf("Logged out successfully.\n"); return;
            case 7: create_account(); break;
            default: printf("Invalid choice.\n");
        }
    } while (1);
}

// --- Operations ---

void deposit() {
    float amt;
    printf("Deposit amount (0 to cancel): Rs. ");
    if (scanf("%f", &amt) != 1 || amt < 0) {
        printf("Invalid amount.\n");
        clear_input_buffer();
        return;
    }
    if (amt == 0) return;

    accounts[loggedIn].balance += amt;
    save_accounts();
    
    char msg[HISTORY_LEN];
    sprintf(msg, "Deposited: Rs. %.2f | Balance: Rs. %.2f", amt, accounts[loggedIn].balance);
    log_transaction(msg);
    printf("Deposit successful.\n");
}

void withdraw() {
    float amt;
    printf("Withdraw amount (0 to cancel): Rs. ");
    if (scanf("%f", &amt) != 1 || amt < 0) {
        printf("Invalid amount.\n");
        clear_input_buffer();
        return;
    }
    if (amt == 0) return;

    if (amt > accounts[loggedIn].balance) {
        printf("Error: Insufficient funds.\n");
        return;
    }

    accounts[loggedIn].balance -= amt;
    save_accounts();
    
    char msg[HISTORY_LEN];
    sprintf(msg, "Withdrew: Rs. %.2f | Balance: Rs. %.2f", amt, accounts[loggedIn].balance);
    log_transaction(msg);
    printf("Withdrawal successful.\n");
}

void change_pin() {
    int new_pin;
    printf("Enter new 4-digit PIN (0 to cancel): ");
    if (scanf("%d", &new_pin) != 1) {
        clear_input_buffer();
        return;
    }
    if (new_pin == 0) return;
    if (new_pin < 1000 || new_pin > 9999) {
        printf("Error: PIN must be exactly 4 digits.\n");
        return;
    }
    accounts[loggedIn].pin = new_pin;
    save_accounts();
    log_transaction("Action: PIN changed.");
    printf("PIN updated successfully.\n");
}

// --- Account Handling ---

void create_account() {
    if (numAccounts >= MAX_ACCOUNTS) {
        printf("System full. Cannot create more accounts.\n");
        return;
    }
    int pin;
    float balance;
    int acc = (numAccounts == 0) ? 1001 : accounts[numAccounts - 1].accountNumber + 1;

    printf("\nCreating Account Number: %d\n", acc);
    printf("Set 4-digit PIN: ");
    if (scanf("%d", &pin) != 1 || pin < 1000 || pin > 9999) {
        printf("Invalid PIN. Creation aborted.\n");
        clear_input_buffer();
        return;
    }
    printf("Initial deposit amount: Rs. ");
    if (scanf("%f", &balance) != 1 || balance < 0) {
        printf("Invalid amount. Creation aborted.\n");
        clear_input_buffer();
        return;
    }

    accounts[numAccounts++] = (Account){acc, pin, balance};
    save_accounts();
    printf("Account %d created successfully!\n", acc);
}

int authenticate(int accNum, int pin) {
    for (int i = 0; i < numAccounts; i++) {
        if (accounts[i].accountNumber == accNum && accounts[i].pin == pin) {
            return i;
        }
    }
    return -1;
}

// --- File I/O ---

void save_accounts() {
    FILE *fp = fopen(ACCOUNTS_FILE, "w");
    if (!fp) return;
    for (int i = 0; i < numAccounts; i++) {
        fprintf(fp, "%d %d %.2f\n", accounts[i].accountNumber, accounts[i].pin, accounts[i].balance);
    }
    fclose(fp);
}

void load_accounts() {
    FILE *fp = fopen(ACCOUNTS_FILE, "r");
    if (!fp) return;
    numAccounts = 0;
    while (fscanf(fp, "%d %d %f", &accounts[numAccounts].accountNumber,
                  &accounts[numAccounts].pin, &accounts[numAccounts].balance) == 3) {
        numAccounts++;
        if (numAccounts >= MAX_ACCOUNTS) break;
    }
    fclose(fp);
}

void log_transaction(const char *entry) {
    char fname[64];
    sprintf(fname, "history_%d.txt", accounts[loggedIn].accountNumber);
    FILE *fp = fopen(fname, "a");
    if (fp) {
        fprintf(fp, "%s\n", entry);
        fclose(fp);
    }
}

void show_history() {
    char fname[64], line[HISTORY_LEN];
    sprintf(fname, "history_%d.txt", accounts[loggedIn].accountNumber);
    FILE *fp = fopen(fname, "r");
    if (!fp) {
        printf("No transaction history found.\n");
        return;
    }
    printf("\n--- Transaction History ---\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}