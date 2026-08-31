# SecureX

## Project Description
This project is a simple encryption and decryption system built using C++. It allows the user to encrypt a message, decrypt a message, and save the output into a file.

## Main File
- main.cpp (entry point)

Note: In this project, the main program is written in:
- `SecureXProject.cpp`

## Supporting Files
This project uses a single main source file and does not require extra class files.

Supporting file(s):
- `SecureXProject.cpp`
- `securex_output.txt` (output file generated during program execution)

## How to Compile
This project can be compiled using either:

- Microsoft Visual C++ (Windows Command Prompt / PowerShell)
- DEV C++ (as commonly used in class)

### Option 1: Using Command Prompt / PowerShell
```powershell
cl.exe /Zi /EHsc /nologo SecureXProject.cpp
```

This will generate:

```powershell
SecureXProject.exe
```

### Option 2: Using DEV C++
1. Open DEV C++.
2. Open `SecureXProject.cpp`.
3. Click on `Compile` or `Run`.
4. The program will compile and run in the console.

## How to Run
After the program is compiled, run it using:

```powershell
.\SecureXProject.exe
```

When the program starts, the user will see this menu:

```text
==========================
     SECUREX SYSTEM
==========================
1. Encrypt Message
2. Decrypt Message
3. Exit
==========================
Enter choice:
```

## What the User Should Enter

### If the user chooses Encrypt Message (option 1)
The program will ask for:
1. `plaintext message`  
   Example: `Hello World`
2. `secret key`  
   Example: `SECRET`

After that, the program will:
- apply encryption,
- display the encrypted message in hexadecimal format,
- and save the result in `securex_output.txt`.

### If the user chooses Decrypt Message (option 2)
The program will ask for:
1. whether to load data from file (`Y/N`)
2. if not loading from file, enter:
   - encrypted hexadecimal message
   - RSA-encrypted secret key

Then the program will show the recovered plaintext.

### If the user chooses Exit (option 3)
The program ends and closes.

## Save or Delete Output File
After the user chooses to stop the program, the system will ask:

```text
Delete the saved results from securex_output.txt? (Y/N):
```

What the user should enter:
- Enter `Y` if they want to delete the saved output file.
- Enter `N` if they want to keep the saved file for later use.

This is useful if the user wants to keep the encrypted message and key for decryption later.

## Example Usage
1. Run the program.
2. Enter `1` to encrypt.
3. Type a message such as `Hello World`.
4. Type a key such as `SECRET`.
5. The output will be shown on the screen.
6. If the user wants to decrypt later, enter `2` and provide the required values.
7. If finish, enter `3` to exit.
8. When asked whether to delete the saved file, enter `Y` or `N`.

## Notes
- The program is made for learning purposes.
- It uses basic C++ programming concepts.
- Output data is stored in a text file for later use.

## Submission Requirement
This project includes:
- main entry file (`SecureXProject.cpp`)
- simple user guide (`README.md`)
- program output file (`securex_output.txt`)
