# Secure ATM Simulation System in C

A robust, file-based ATM simulation written in C. This project demonstrates core programming concepts including **File I/O**, **Data Structures**, **Authentication Logic**, and **Input Validation**.

 ## Features

* **Multi-User Support:** Handles multiple accounts with unique account numbers.
* **Persistent Storage:** Account data (PINs and balances) are saved to `accounts.txt`.
* **Transaction History:** Every deposit or withdrawal is logged to a user-specific text file (e.g., `history_1001.txt`).
* **Security:** Implements a 3-try login limit and 4-digit PIN verification.
* **Input Protection:** Includes buffer clearing to prevent infinite loops from invalid character inputs.

## Functionality

The system allows users to:
1.  **Create an Account:** Automated account number generation.
2.  **Login:** Secure access to existing accounts.
3.  **Check Balance:** Real-time balance inquiries.
4.  **Deposit/Withdraw:** Update funds with instant file synchronization.
5.  **Change PIN:** Securely update credentials.
6.  **View History:** Read previous transactions from local logs.

## Project Structure

| File | Description |
| :--- | :--- |
| `main.c` | The core logic and source code. |
| `accounts.txt` | Flat-file database storing account credentials and balances. |
| `history_XXXX.txt` | Transaction logs generated per account number. |

## How to Run

### Prerequisites
* A C compiler (GCC, Clang, or MinGW).

### Compilation and Execution
Open your terminal and run:
```bash
gcc main.c -o atm_system
./atm_system
```

## Sample Output
<img width="404" height="701" alt="Screenshot (282)" src="https://github.com/user-attachments/assets/825a0662-8a13-4614-a559-7f52173249de" />


### Future Enhancements
* [ ] Encryption: Hash PINs before saving them to the file.

* [ ] Admin Mode: A master login to view all system accounts.

* [ ] Timestamps: Add real-time dates to the transaction history using <time.h>.

* [ ] GUI: Transition from a CLI to a graphical interface using libraries like Raylib or GTK.
