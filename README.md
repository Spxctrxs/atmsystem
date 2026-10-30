# ATM Management System

A small, menu-driven ATM program written in C. It lets you create accounts, sign in with an account number and PIN, check balances, deposit and withdraw money, transfer funds between accounts, and view transaction history.

> **For learning and demonstration only.** This program is not suitable for real banking or for storing real account information. PINs are obfuscated with a simple XOR operation, not securely encrypted. Account data is stored locally without protection.

## Requirements

- A C compiler, such as GCC (MinGW-w64 on Windows) or Microsoft Visual C.
- A terminal or command prompt.

## Build and run on Windows

Open PowerShell in the project folder:

```powershell
cd C:\path\to\atmsystem
```

### With GCC / MinGW-w64

```powershell
gcc .\atmsystem.c -o .\atmsystem.exe
.\atmsystem.exe
```

### With Microsoft Visual C

Run these commands from a **Developer Command Prompt for Visual Studio**:

```cmd
cl /W4 /TC atmsystem.c /Fe:atmsystem.exe
atmsystem.exe
```

## How to use it

1. Start the program. At the main menu, enter `1` to create an account.
2. Note the account number printed by the program, enter your name, choose a 4-digit PIN, and enter an initial deposit.
3. At the main menu, enter `2` to log in with that account number and PIN.
4. Choose an option from the ATM menu:
   - `1` — Check your balance.
   - `2` — Deposit a positive amount.
   - `3` — Withdraw a positive amount, up to your available balance.
   - `4` — Transfer a positive amount to another account in this program. You need the recipient's account number.
   - `5` — View transaction records for your account.
   - `6` — Log out and return to the main menu.
5. From the main menu, enter `3` to exit.

The program assigns account numbers starting at `1001`. The demo supports up to 100 accounts.

## Local data files

The program reads and writes `accounts.dat` (account records) and appends transaction entries to `logs.txt`. These files are created in the program's **current working directory** when you use it; run the program from the project folder to keep them there. The repository's `.gitignore` excludes both files so local account data and logs are not committed.

Deleting these files removes the corresponding locally saved accounts or transaction history. Keep backups only if you understand that account data is stored in an unprotected format.
