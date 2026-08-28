# SecureX - Quick Start Guide

## Compilation

Using Microsoft Visual C++ (Windows):
```powershell
cl.exe /Zi /EHsc /nologo SecureXProject.cpp
```

This creates `SecureXProject.exe`

## Execution

Run the program:
```powershell
.\SecureXProject.exe
```

## How to Use

### Encrypt a Message

1. Start the program
2. Enter `1` at the menu
3. Enter your plaintext message (e.g., "Hello World")
4. Enter your secret key (e.g., "SECRET")
5. View the encrypted message (hex) and RSA-encrypted key
6. The results are automatically appended to `securex_output.txt`
7. Choose whether to perform another operation

### Decrypt a Message

1. At the menu, enter `2`
2. Optionally load from file, or enter manually
  - Enter `Y` and then `securex_output.txt` to load saved results
  - Enter `N` to type the encrypted values manually
3. Enter the encrypted hexadecimal message
4. Enter the RSA-encrypted secret key
5. View the recovered plaintext

### Exit

Enter `3` at the menu to exit

After choosing `N` at "Perform another operation?", choose whether to delete
or keep the saved results in `securex_output.txt`.

## Example Run

```
Plaintext:  "Hello World"
Key:        "SECRET"

Encrypted:
  Hex Message: 1E332222237410312A332D
  RSA Key:     0A78 001C 0281 0743 001C 086F

Decrypted:
  Plaintext:   "Hello World"  ✓ (matches original)
```

## File Handling

- Encryption and decryption results are automatically appended to `securex_output.txt`
- The file contains readable text, including hexadecimal encrypted data
- During decryption, enter `Y` to load data from this file
- When exiting, choose whether to delete or keep the saved results
- The file may contain the original plaintext and secret key, so do not share it

## Important Notes

- The RSA constants (p=61, q=53, n=3233) are for educational purposes
- This program demonstrates cryptography concepts, not production security
- All operations are fully reversible and understandable at the introductory C++ level
- `cin.ignore(1000, '\n')` clears leftover input before `getline()` is used
- See DOCUMENTATION.md for complete technical details

## Program Features

✓ Complete encryption/decryption workflow
✓ Input validation at every step
✓ Automatic file I/O for saving and loading encrypted data
✓ Modular, well-documented code
✓ Suitable for university course presentations
✓ Only uses Chapters 1-7 C++ concepts
✓ No external libraries required
✓ Compiles with standard C++ compilers
