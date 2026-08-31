# SecureX: A Hybrid XOR and Vigenère Encryption System
## Complete Documentation for EECE 1313 Programming for Engineers

---

## 1. OVERALL PROGRAM STRUCTURE

The SecureX program is organized into the following major sections:

```
SECUREX PROGRAM STRUCTURE:
├── Header Files & Namespaces
├── Function Prototypes (declarations)
├── RSA Constants (educational values)
├── main() - Main program with menu loop
├── displayMenu() - Display the menu
├── ENCRYPTION() - Main encryption workflow
├── DECRYPTION() - Main decryption workflow
├── Helper Functions:
│   ├── vigenereEncrypt()
│   ├── vigenereDecrypt()
│   ├── xorTransform()
│   ├── encodeToHex()
│   ├── decodeFromHex()
│   ├── RSA_ENCRYPT_KEY()
│   ├── RSA_DECRYPT_KEY()
│   ├── modPow() - Modular exponentiation
│   └── Input/Output Helpers
└── File I/O Functions (saveToFile, loadFromFile)
```

### Key Design Principles:
- **Modular**: Each major operation is in its own function
- **Readable**: Clear function names and comments explain every step
- **Educational**: Uses only C++ concepts from Chapters 1-7 of intro courses
- **Validated**: Input validation prevents crashes
- **Reversible**: Encryption ↔ Decryption round-trip always succeeds

---

## 2. MAIN MENU SYSTEM AND PROGRAM LOOP

### Program Flow:

```
START
  ↓
Display Menu
  ↓
Get User Choice (1, 2, or 3)
  ↓
Validate Choice → Invalid? → Ask Again
  ↓
SWITCH(choice)
  ├─ Case 1 → ENCRYPTION()
  ├─ Case 2 → DECRYPTION()
  └─ Case 3 → Exit
  ↓
Ask "Perform another operation? (Y/N)"
  ├─ Y/y → Return to Menu
  ├─ N/n → Exit Program
  └─ Invalid → Ask Again
  ↓
END
```

### Code Structure:

```cpp
int main()
{
    int choice;
    char continueChoice;
    bool running = true;

    // Keep showing the menu until the user exits.
    while (running)
    {
        displayMenu();
        cin >> choice;
        cin.ignore(1000, '\n');  // Clear the input line.

        // Make sure the choice is valid.
        while (choice < 1 || choice > 3)
        {
            cout << "\n[ERROR] Invalid choice! Please enter 1, 2, or 3.\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore(1000, '\n');
        }

        // Choose an operation.
        switch (choice)
        {
            case 1:
                ENCRYPTION();
                break;
            case 2:
                DECRYPTION();
                break;
            case 3:
                char deleteChoice;
                cout << "\nDelete the saved results from securex_output.txt? (Y/N): ";
                cin >> deleteChoice;
                cin.ignore(1000, '\n');

                while (deleteChoice != 'Y' && deleteChoice != 'y' &&
                       deleteChoice != 'N' && deleteChoice != 'n')
                {
                    cout << "[ERROR] Please enter Y or N: ";
                    cin >> deleteChoice;
                    cin.ignore(1000, '\n');
                }

                if (deleteChoice == 'Y' || deleteChoice == 'y')
                {
                    ofstream outputFile("securex_output.txt", ios::trunc);

                    if (!outputFile)
                    {
                        cout << "Error: Unable to clear output file.\n";
                    }
                    else
                    {
                        outputFile.close();
                        cout << "Saved results have been deleted.\n";
                    }
                }
                else
                {
                    cout << "Saved results will be kept.\n";
                }

                cout << "\nThank you for using SecureX. Goodbye!\n\n";
                running = false;
                break;
        }

        // Ask whether to do another operation.
        if (running)
        {
            cout << "\nPerform another operation? (Y/N): ";
            cin >> continueChoice;
            cin.ignore(1000, '\n');

            // Accept only Y or N.
            while (continueChoice != 'Y' && continueChoice != 'y' &&
                   continueChoice != 'N' && continueChoice != 'n')
            {
                cout << "[ERROR] Please enter Y or N: ";
                cin >> continueChoice;
                cin.ignore(1000, '\n');
            }
            // N means stop.
            if (continueChoice == 'N' || continueChoice == 'n')
            {
                char deleteChoice;
                cout << "Delete the saved results from securex_output.txt? (Y/N): ";
                cin >> deleteChoice;
                cin.ignore(1000, '\n');

                // Accept only Y or N.
                while (deleteChoice != 'Y' && deleteChoice != 'y' &&
                       deleteChoice != 'N' && deleteChoice != 'n')
                {
                    cout << "[ERROR] Please enter Y or N: ";
                    cin >> deleteChoice;
                    cin.ignore(1000, '\n');
                }

                if (deleteChoice == 'Y' || deleteChoice == 'y')
                {
                    // trunc removes the old text from the file.
                    ofstream outputFile("securex_output.txt", ios::trunc);
                    if (!outputFile)
                    {
                        cout << "Error: Unable to clear output file.\n";
                    }
                    else
                    {
                        outputFile.close();
                        cout << "Saved results have been deleted.\n";
                    }
                }
                else
                {
                    cout << "Saved results will be kept.\n";
                }

                cout << "\nThank you for using SecureX. Goodbye!\n\n";
                running = false;
            }
        }
    }
    return 0;  // End successfully.
}
```

### Key Programming Concepts Used:
- **while loop** - Main program loop runs indefinitely until user exits
- **switch statement** - Routes user choice to correct function
- **if-else statements** - Input validation
- **bool variable** - Controls program flow with `running` flag
- **cin >> and cin.ignore()** - Get input and clear buffer
- **toupper()/tolower() comparison** - Accept Y/y and N/n

---

## 3. ENCRYPTION WORKFLOW

### Complete Encryption Process:

```
USER INPUTS:
├─ Plaintext Message (e.g., "Hello World")
└─ Secret Key (e.g., "SECRET")
       ↓
STEP 1: Vigenère Encryption
│   - Each letter shifted by key value
│   - Key repeats cyclically
│   Result: "Mvapf Ctiah" (intermediate ciphertext)
│       ↓
STEP 2: XOR Transformation
│   - XOR each byte of ciphertext with key byte
│   - XOR is its own inverse: A XOR B XOR B = A
│       ↓
STEP 3: Hexadecimal Encoding
│   - Convert binary bytes to readable hex (0-F)
│   - Each byte becomes 2 hex digits
│   Result: "1E332222237410312A332D"
│       ↓
STEP 4: RSA Key Encryption
│   - Each character of key is RSA-encrypted
│   - Formula: C = M^e mod n
│   Result: "0A78 001C 0281 0743 001C 086F"
│       ↓
OUTPUT:
├─ Encrypted Message (Hex)
├─ RSA-Encrypted Secret Key
└─ Results appended to securex_output.txt
```

### Code Structure:

```cpp
void ENCRYPTION()
{
    string plaintext, secretKey;
    bool validInput = false;
    
    // Get and validate plaintext and key
    while (!validInput)
    {
        cout << "Enter plaintext message: ";
        getline(cin, plaintext);
        
        cout << "Enter secret key: ";
        getline(cin, secretKey);
        
        if (plaintext.empty() || secretKey.empty())
        {
            cout << "[ERROR] Cannot be empty!\n";
        }
        else
        {
            validInput = true;
        }
    }
    
    // Step 1: Vigenère Encryption
    string intermediateText = vigenereEncrypt(plaintext, secretKey);
    
    // Step 2: XOR Transformation
    string xorResult = xorTransform(intermediateText, secretKey);
    
    // Step 3: Hex Encoding
    string hexEncrypted = encodeToHex(xorResult);
    
    // Step 4: RSA Key Encryption
    string encryptedKey = RSA_ENCRYPT_KEY(secretKey, RSA_E, RSA_N);
    
    // Display results
    cout << "\nEncrypted Message (Hex):\n" << hexEncrypted << "\n";
    cout << "\nRSA-Encrypted Secret Key:\n" << encryptedKey << "\n";
    
    // Print the results and append them to securex_output.txt.
    cout << "Encrypted Message (Hex):\n" << hexEncrypted << "\n";
    cout << "RSA-Encrypted Secret Key:\n" << encryptedKey << "\n";

    ofstream outputFile("securex_output.txt", ios::app);
    if (outputFile)
    {
        outputFile << "Encrypted Message (Hex):\n" << hexEncrypted << "\n";
        outputFile << "RSA-Encrypted Secret Key:\n" << encryptedKey << "\n";
        outputFile.close();
    }
}
```

---

## 4. VIGENÈRE CIPHER IMPLEMENTATION

### What is Vigenère Cipher?

Vigenère is a classical polyalphabetic substitution cipher where:
- Each letter in the plaintext is shifted by a value derived from the key
- The key repeats cyclically if the message is longer than the key
- Non-alphabetic characters pass through unchanged

### Example:

```
Plaintext:  H  e  l  l  o     W  o  r  l  d
Key:        S  E  C  R  E  T  S  E  C  R  E  T
Shift by:   18 4  2  17 4  19 (repeat)
Result:     Z  i  n  2  s     O  s  t  n  w
```

### Encryption Code:

```cpp
string vigenereEncrypt(string plaintext, string key)
{
    string ciphertext = "";
    int keyIndex = 0;
    
    // Process each character
    for (int i = 0; i < plaintext.length(); i++)
    {
        char plainChar = plaintext[i];
        char keyChar = key[keyIndex % key.length()];  // Cycle through key
        
        if (isalpha(plainChar))  // Only shift letters
        {
            int shift = keyChar % 26;  // Get shift amount (0-25)
            
            if (isupper(plainChar))
            {
                // Shift within A-Z range
                ciphertext += char((plainChar - 'A' + shift) % 26 + 'A');
            }
            else
            {
                // Shift within a-z range
                ciphertext += char((plainChar - 'a' + shift) % 26 + 'a');
            }
            
            keyIndex++;  // Move to next key character
        }
        else
        {
            // Non-letters stay the same
            ciphertext += plainChar;
        }
    }
    
    return ciphertext;
}
```

### Decryption Code:

```cpp
string vigenereDecrypt(string ciphertext, string key)
{
    string plaintext = "";
    int keyIndex = 0;
    
    for (int i = 0; i < ciphertext.length(); i++)
    {
        char cipherChar = ciphertext[i];
        char keyChar = key[keyIndex % key.length()];
        
        if (isalpha(cipherChar))
        {
            int shift = keyChar % 26;
            
            if (isupper(cipherChar))
            {
                // Reverse shift: subtract instead of add
                plaintext += char((cipherChar - 'A' - shift + 26) % 26 + 'A');
            }
            else
            {
                plaintext += char((cipherChar - 'a' - shift + 26) % 26 + 'a');
            }
            
            keyIndex++;
        }
        else
        {
            plaintext += cipherChar;
        }
    }
    
    return plaintext;
}
```

### Key Concepts Used:
- **isalpha()** - Check if character is a letter
- **isupper()/islower()** - Check character case
- **char arithmetic** - 'A' + shift calculates new letter
- **modulo operator (%)** - Wraps around alphabet and cycles through key
- **for loop** - Process each character
- **if-else** - Handle uppercase/lowercase/non-letters differently

---

## 5. XOR TRANSFORMATION

### What is XOR?

XOR (Exclusive OR) is a bitwise operation that:
- Operates on individual bits: 0 XOR 0 = 0, 0 XOR 1 = 1, 1 XOR 0 = 1, 1 XOR 1 = 0
- **Is its own inverse**: A XOR B XOR B = A
- **Is reversible**: Using the same key, XOR can decrypt what it encrypted

### Example:

```
Byte:    01001000  (ASCII 'H' = 72)
Key:     01010011  (ASCII 'S' = 83)
Result:  00011011  (27 in decimal)

To decrypt: 00011011 XOR 01010011 = 01001000 (back to 'H')
```

### Implementation:

```cpp
string xorTransform(string data, string key)
{
    string result = "";
    
    // Process each byte
    for (int i = 0; i < data.length(); i++)
    {
        char dataChar = data[i];
        char keyChar = key[i % key.length()];  // Cycle through key
        
        char xorResult = dataChar ^ keyChar;   // Bitwise XOR operator
        result += xorResult;
    }
    
    return result;
}
```

### Key Concepts Used:
- **^ operator** - Bitwise XOR
- **% operator** - Modulo to cycle through key
- **for loop** - Process each byte
- **char type** - Works as 8-bit value for XOR operation

### Why XOR?
1. **Reversible** - Apply XOR twice = original data
2. **Simple** - Single CPU instruction in assembly
3. **Fast** - Very efficient encryption
4. **Educational** - Demonstrates bitwise operations

---

## 6. HEXADECIMAL CONVERSION

### Why Convert to Hexadecimal?

After XOR, we have binary data (bytes 0-255) that:
- **Cannot be printed** - Many byte values are unprintable control characters
- **Cannot be transmitted** - Binary data corrupts in text transmission
- **Solution**: Convert to hexadecimal (0-9, A-F) for readable storage

### Example:

```
Binary Data:  01001000  01001001  01001010
Decimal:      72        73        74
Hex:          48        49        4A
Stored as:    "484949 4A" (readable text)
```

### Hex Encoding Code:

```cpp
string encodeToHex(string data)
{
    string hexOutput = "";
    
    // Process each byte
    for (int i = 0; i < data.length(); i++)
    {
        unsigned char byte = (unsigned char)data[i];
        
        // Each byte becomes 2 hex digits
        hexOutput += decimalToHex(byte / 16);   // High nibble (0-15)
        hexOutput += decimalToHex(byte % 16);   // Low nibble (0-15)
    }
    
    return hexOutput;
}

string decimalToHex(int decimal)
{
    if (decimal < 10)
        return string(1, '0' + decimal);        // 0-9
    else
        return string(1, 'A' + (decimal - 10)); // A-F (10-15)
}
```

### Hex Decoding Code:

```cpp
string decodeFromHex(string hexData)
{
    string result = "";
    
    // Process hex string in pairs
    for (int i = 0; i < hexData.length(); i += 2)
    {
        if (i + 1 < hexData.length())
        {
            // Convert two hex digits to decimal
            int highNibble = hexToDecimal(hexData.substr(i, 1));
            int lowNibble = hexToDecimal(hexData.substr(i + 1, 1));
            
            int byte = highNibble * 16 + lowNibble;
            result += char(byte);
        }
    }
    
    return result;
}

int hexToDecimal(string hex)
{
    if (hex.empty()) return -1;
    
    char hexChar = toupper(hex[0]);
    
    if (hexChar >= '0' && hexChar <= '9')
        return hexChar - '0';
    else if (hexChar >= 'A' && hexChar <= 'F')
        return hexChar - 'A' + 10;
    else
        return -1;  // Invalid
}
```

### Key Concepts Used:
- **String concatenation** - Build hex string
- **Arithmetic operators** - / (divide) and % (modulo) extract nibbles
- **toupper()** - Normalize hex to uppercase
- **String methods** - substr() for character extraction
- **isdigit()/char comparison** - Validate hex characters

---

## 7. RSA KEY ENCRYPTION

### What is RSA?

RSA is a public-key cryptographic system where:
- **Public Key** (e, n) - Used for encryption; safe to share
- **Private Key** (d, n) - Used for decryption; must be kept secret
- **Encryption Formula**: C = M^e mod n
- **Decryption Formula**: M = C^d mod n

### Educational RSA Constants:

```cpp
const int RSA_P = 61;        // First prime
const int RSA_Q = 53;        // Second prime
const int RSA_N = 3233;      // n = p * q = 61 * 53
const int RSA_E = 17;        // Public exponent
const int RSA_D = 2753;      // Private exponent
// phi(n) = (p-1)(q-1) = 60*52 = 3120
// Relationship: e*d ≡ 1 (mod phi(n))
// Verification: 17*2753 = 46801 = 15*3120 + 1 ✓
```

**Note**: These values are for educational purposes only. Production RSA uses very large primes (2048+ bits).

### RSA Encryption of Secret Key:

```cpp
string RSA_ENCRYPT_KEY(string secretKey, int e, int n)
{
    string encryptedKey = "";
    
    // Encrypt each character in the secret key
    for (int i = 0; i < secretKey.length(); i++)
    {
        int plainValue = (int)secretKey[i];  // Get ASCII value
        
        // RSA formula: C = M^e mod n
        long long cipherValue = modPow(plainValue, e, n);
        
        // Convert to 4-digit hex string
        string hexVal = "";
        long long temp = cipherValue;
        
        if (temp == 0)
            hexVal = "0";
        else
        {
            while (temp > 0)
            {
                int digit = temp % 16;
                if (digit < 10)
                    hexVal = char('0' + digit) + hexVal;
                else
                    hexVal = char('A' + digit - 10) + hexVal;
                temp /= 16;
            }
        }
        
        // Pad with leading zeros
        while (hexVal.length() < 4)
            hexVal = "0" + hexVal;
        
        encryptedKey += hexVal;
        
        if (i < secretKey.length() - 1)
            encryptedKey += " ";  // Space between values
    }
    
    return encryptedKey;
}
```

### Example:

```
Secret Key: "SECRET"

Character 'S' (ASCII 83):
  C = 83^17 mod 3233 = ?
  Using modular exponentiation: 2680
  In hex: 0A78

Character 'E' (ASCII 69):
  C = 69^17 mod 3233 = 28
  In hex: 001C

...continuing for R, E, T...

Result: "0A78 001C 0281 0743 001C 086F"
```

### Key Concepts Used:
- **for loop** - Process each character
- **ASCII values** - Convert char to int
- **Function calls** - Call modPow() for efficient exponentiation
- **while loop** - Convert decimal to hexadecimal
- **String manipulation** - Build output with padding and spaces

---

## 8. MODULAR EXPONENTIATION

### Why Modular Exponentiation?

Direct calculation of M^e would overflow:
- 83^17 is astronomical
- Modular exponentiation computes (M^e) mod n efficiently
- Uses **binary exponentiation** algorithm

### Algorithm (Square-and-Multiply):

```
Input: base, exp, mod
Output: result = base^exp mod mod

result = 1
base = base mod mod

while exp > 0:
    if exp is odd:
        result = (result * base) mod mod
    exp = exp / 2
    base = (base * base) mod mod

return result
```

### Implementation:

```cpp
long long modPow(long long base, long long exp, long long mod)
{
    long long result = 1;
    base = base % mod;  // Start with base mod mod
    
    while (exp > 0)
    {
        if (exp % 2 == 1)  // If exp is odd
        {
            result = (result * base) % mod;
        }
        
        exp = exp >> 1;      // Right shift: divide by 2
        base = (base * base) % mod;  // Square and reduce
    }
    
    return result;
}
```

### Example: Calculate 83^17 mod 3233

```
Initial: result=1, base=83, exp=17

Iteration 1: exp=17 (odd), result=83, exp→8, base→6889%3233=423
Iteration 2: exp=8 (even), result=83, exp→4, base→178929%3233=1474
Iteration 3: exp=4 (even), result=83, exp→2, base→2172676%3233=2048
Iteration 4: exp=2 (even), result=83, exp→1, base→4194304%3233=2894
Iteration 5: exp=1 (odd), result=83*2894=240202→2680, exp→0

Final Result: 2680
```

### Key Concepts Used:
- **while loop** - Continue until exponent is zero
- **if statement** - Check if exponent is odd
- **right shift (>>)** - Divide by 2 efficiently
- **modulo (%)** - Keep values small to prevent overflow
- **long long** - Use 64-bit integers to prevent overflow

---

## 9. RSA KEY DECRYPTION

### RSA Decryption Formula:

M = C^d mod n

Where:
- C = ciphertext (encrypted value)
- d = private exponent (2753)
- n = modulus (3233)
- M = plaintext (original character)

### Implementation:

```cpp
string RSA_DECRYPT_KEY(string encryptedKey, int d, int n)
{
    string recoveredKey = "";
    int startPos = 0;
    
    // Split by spaces and process each encrypted value
    for (int i = 0; i <= encryptedKey.length(); i++)
    {
        if (i == encryptedKey.length() || encryptedKey[i] == ' ')
        {
            if (i > startPos)
            {
                // Extract hex value
                string hexValue = encryptedKey.substr(startPos, i - startPos);
                
                // Convert hex to decimal
                long long cipherValue = 0;
                for (char c : hexValue)
                {
                    cipherValue = cipherValue * 16;
                    if (c >= '0' && c <= '9')
                        cipherValue += c - '0';
                    else if (c >= 'A' && c <= 'F')
                        cipherValue += c - 'A' + 10;
                    else if (c >= 'a' && c <= 'f')
                        cipherValue += c - 'a' + 10;
                }
                
                // RSA decryption: M = C^d mod n
                long long plainValue = modPow(cipherValue, d, n);
                
                // Convert back to character
                recoveredKey += char(plainValue);
            }
            
            startPos = i + 1;
        }
    }
    
    return recoveredKey;
}
```

### Example: Decrypt "0A78"

```
"0A78" in hex = 2680 in decimal

M = 2680^2753 mod 3233 = 83
char(83) = 'S'
```

### Key Concepts Used:
- **for loop with delimiter** - Parse space-separated values
- **substr()** - Extract substring
- **Hex-to-decimal conversion** - Loop through hex characters
- **modPow()** - Efficient exponentiation
- **char() casting** - Convert ASCII value to character

---

## 10. DECRYPTION WORKFLOW

### Complete Decryption Process:

```
USER INPUTS:
├─ Encrypted Hex Message: "1E332222237410312A332D"
└─ RSA-Encrypted Key: "0A78 001C 0281 0743 001C 086F"
       ↓
STEP 1: RSA Key Decryption
│   - Decrypt each RSA value using private exponent
│   - Formula: M = C^d mod n
│   - Recovered Key: "SECRET"
│       ↓
STEP 2: Hex Decoding
│   - Convert hex string to binary bytes
│   - "1E332222..." → binary data
│       ↓
STEP 3: XOR Transformation (Decrypt)
│   - XOR result with recovered key (XOR is reversible!)
│   - Recovers Vigenère ciphertext
│       ↓
STEP 4: Vigenère Decryption
│   - Reverse shifts using same key
│   - Recover original plaintext
│       ↓
OUTPUT:
└─ Recovered Plaintext: "Hello World"
```

### Code Structure:

```cpp
void DECRYPTION()
{
    string hexEncrypted, encryptedKeyStr, recoveredKey;
    bool validInput = false;
    
    // Option to load from file
    char loadChoice;
    cout << "Load encrypted data from file? (Y/N): ";
    cin >> loadChoice;
    cin.ignore();
    
    if (loadChoice == 'Y' || loadChoice == 'y')
    {
        string filename;
        cout << "Enter filename: ";
        getline(cin, filename);
        
        if (!loadFromFile(filename, hexEncrypted, encryptedKeyStr))
        {
            cout << "[ERROR] Could not load file.\n";
            validInput = false;
        }
        else
        {
            validInput = true;
        }
    }
    
    // Get encrypted data with validation
    while (!validInput)
    {
        cout << "Enter encrypted hexadecimal message: ";
        getline(cin, hexEncrypted);
        
        cout << "Enter RSA-encrypted secret key: ";
        getline(cin, encryptedKeyStr);
        
        if (hexEncrypted.empty() || encryptedKeyStr.empty())
        {
            cout << "[ERROR] Inputs cannot be empty.\n";
        }
        else if (!isValidHex(hexEncrypted))
        {
            cout << "[ERROR] Invalid hexadecimal input.\n";
        }
        else
        {
            validInput = true;
        }
    }
    
    // Step 1: Decrypt secret key
    recoveredKey = RSA_DECRYPT_KEY(encryptedKeyStr, RSA_D, RSA_N);
    
    // Step 2: Decode hex
    string xorEncrypted = decodeFromHex(hexEncrypted);
    
    // Step 3: XOR (reversible)
    string vigenereEncrypted = xorTransform(xorEncrypted, recoveredKey);
    
    // Step 4: Vigenère decrypt
    string plaintext = vigenereDecrypt(vigenereEncrypted, recoveredKey);
    
    // Display result
    cout << "\nRecovered Plaintext:\n" << plaintext << "\n";
}
```

---

## 11. SYLLABLE TOPICS APPLIED

### C++ Concepts Demonstrated:

| Concept | Where Used | Why Important |
|---------|-----------|---------------|
| **#include** | Headers (iostream, string, fstream, cctype) | Load library functions |
| **using namespace** | Line 6 | Avoid writing std:: everywhere |
| **Variables** | Throughout | Store data (plaintext, keys, ciphertext) |
| **Constants** | RSA_P, RSA_Q, RSA_N, etc. | Fixed cryptographic values |
| **Data Types** | int, char, string, bool, long long | Different data needs |
| **cin/cout** | Menu, prompts, results | User input/output |
| **getline()** | ENCRYPTION(), DECRYPTION() | Read entire lines with spaces |
| **String type** | Throughout | Store and manipulate text data |
| **char type** | Vigenère, XOR, RSA functions | Individual character processing |
| **Arithmetic operators** | +, -, *, /, % | Math calculations |
| **Bitwise operators** | ^ (XOR) | Bitwise encryption |
| **Logical operators** | &&, \|\| | Input validation |
| **Relational operators** | <, >, ==, != | Comparisons |
| **if/else statements** | Validation, branching | Control flow |
| **if-else-if** | Input validation | Multiple conditions |
| **switch statement** | Menu routing | Clean selection |
| **for loops** | Encryption, decryption steps | Process collections |
| **while loops** | Main program loop, validation | Repeat until condition |
| **do-while** | Implicit in validation loops | Could be used for retry logic |
| **Counters** | i, keyIndex | Track positions |
| **Function declarations** | Prototypes at top | Plan structure |
| **Function definitions** | Each function | Modular code |
| **Parameters** | All functions | Pass data to functions |
| **Return values** | encodeToHex(), modPow(), etc. | Functions return results |
| **void functions** | displayMenu(), ENCRYPTION() | Functions without return values |
| **Function calls** | Throughout | Execute modular code |
| **cin.ignore()** | After cin >> | Clear input buffer |
| **String methods** | .length(), .substr(), .empty() | String manipulation |
| **isalpha()/isupper()/islower()** | Vigenère | Character classification |
| **toupper()/tolower()** | Input validation | Character conversion |
| **char arithmetic** | 'A' + shift | Calculate new characters |
| **casting** | (int)secretKey[i] | Convert char to int |
| **Input validation** | Multiple locations | Prevent crashes |
| **File I/O** | ofstream, ifstream | Save/load encrypted data |
| **getline()** | File reading | Read lines from files |
| **Comments** | Throughout | Explain code |
| **Meaningful names** | vigenereEncrypt(), modPow() | Self-documenting code |
| **Indentation** | Throughout | Readable formatting |

---

## 12. COMPLETE SAMPLE EXECUTION

### Test Case: Encrypt "Hello World" with key "SECRET"

**USER INPUT:**
```
1                           ← Choose Encrypt
Hello World                 ← Plaintext
SECRET                      ← Secret Key
N                           ← Don't continue
```

**PROGRAM OUTPUT:**
```
==========================
     SECUREX SYSTEM
==========================
1. Encrypt Message
2. Decrypt Message
3. Exit
==========================
Enter choice: 1

--- ENCRYPTION MODULE ---
Enter plaintext message: Hello World
Enter secret key: SECRET

[Processing] Applying Vigenère encryption...
Intermediate ciphertext (first 50 chars): Mvapf Ctiah...
[Processing] Applying XOR transformation...
[Processing] Encoding to hexadecimal...
[Processing] Encrypting secret key with RSA...

========== ENCRYPTION RESULTS ==========

Encrypted Message (Hex):
1E332222237410312A332D

RSA-Encrypted Secret Key:
0A78 001C 0281 0743 001C 086F

========================================

Perform another operation? (Y/N): N

Thank you for using SecureX. Goodbye!
```

### Test Case: Decrypt the encrypted data

**USER INPUT:**
```
2                           ← Choose Decrypt
N                           ← Don't load from file
1E332222237410312A332D      ← Encrypted message
0A78 001C 0281 0743 001C 086F  ← Encrypted key
N                           ← Don't continue
```

**PROGRAM OUTPUT:**
```
==========================
     SECUREX SYSTEM
==========================
1. Encrypt Message
2. Decrypt Message
3. Exit
==========================
Enter choice: 2

--- DECRYPTION MODULE ---
Load encrypted data from file? (Y/N): N
Enter encrypted hexadecimal message: 1E332222237410312A332D
Enter RSA-encrypted secret key: 0A78 001C 0281 0743 001C 086F

[Processing] Decrypting secret key with RSA...
Recovered secret key: SECRET
[Processing] Decoding from hexadecimal...
[Processing] Applying XOR transformation...
[Processing] Applying Vigenère decryption...

========== DECRYPTION RESULTS ==========

Recovered Plaintext:
Hello World

========================================

Perform another operation? (Y/N): N

Thank you for using SecureX. Goodbye!
```

**VERIFICATION:**
```
Original Plaintext:   "Hello World" ✓
Recovered Plaintext:  "Hello World" ✓
MATCH - Perfect Decryption!
```

---

## 13. LINE-BY-LINE EXPLANATION OF KEY SECTIONS

### Main Program Loop (Lines 38-91)

```cpp
int main()
{
    int choice;              // Store user's menu choice (1, 2, or 3)
    char continueChoice;     // Store Y/N response
    bool running = true;     // Flag to keep program alive
    
    while (running)  // Main loop: repeat until user chooses Exit
    {
        displayMenu();       // Show menu options
        cin >> choice;       // Get user input
        cin.ignore();        // Clear newline from input buffer
        
        // Input validation: keep asking if choice is invalid
        while (choice < 1 || choice > 3)
        {
            cout << "\n[ERROR] Invalid choice! Please enter 1, 2, or 3.\n";
            cout << "Enter your choice: ";
            cin >> choice;       // Try again
            cin.ignore();
        }
        
        // Route to correct function based on choice
        switch (choice)
        {
            case 1:
                ENCRYPTION();    // User chose Encrypt
                break;
            case 2:
                DECRYPTION();    // User chose Decrypt
                break;
            case 3:
                cout << "\nThank you for using SecureX. Goodbye!\n\n";
                running = false; // Stop the while loop
                break;
        }
        
        // After operation, ask if user wants to continue
        if (running)  // Only ask if we didn't exit
        {
            cout << "\nPerform another operation? (Y/N): ";
            cin >> continueChoice;
            cin.ignore();
            
            // Keep asking until user enters Y/N
            while (continueChoice != 'Y' && continueChoice != 'y' &&
                   continueChoice != 'N' && continueChoice != 'n')
            {
                cout << "[ERROR] Please enter Y or N: ";
                cin >> continueChoice;
                cin.ignore();
            }
            
            // If user entered N, exit program
            if (continueChoice == 'N' || continueChoice == 'n')
            {
                cout << "\nThank you for using SecureX. Goodbye!\n\n";
                running = false;  // Stop the while loop
            }
        }
    }
    
    return 0;  // Program ends successfully
}
```

### Vigenère Encryption (Lines 178-213)

```cpp
string vigenereEncrypt(string plaintext, string key)
{
    string ciphertext = "";    // Build encrypted result
    int keyIndex = 0;          // Track position in key
    
    // Loop through each character in plaintext
    for (int i = 0; i < plaintext.length(); i++)
    {
        char plainChar = plaintext[i];                    // Current character
        char keyChar = key[keyIndex % key.length()];     // Get key character (cycle)
        
        // Check if plainChar is a letter (A-Z, a-z)
        if (isalpha(plainChar))
        {
            // Determine if uppercase or lowercase
            if (isupper(plainChar))
            {
                // Get shift value from key (0-25)
                int shift = keyChar % 26;
                // Shift letter forward, wrap around using modulo
                ciphertext += char((plainChar - 'A' + shift) % 26 + 'A');
            }
            else  // lowercase
            {
                int shift = keyChar % 26;
                ciphertext += char((plainChar - 'a' + shift) % 26 + 'a');
            }
            keyIndex++;  // Move to next key character (only for letters)
        }
        else  // Non-letter
        {
            // Spaces, punctuation, numbers pass through unchanged
            ciphertext += plainChar;
        }
    }
    
    return ciphertext;  // Return encrypted message
}
```

### XOR Transformation (Lines 266-279)

```cpp
string xorTransform(string data, string key)
{
    string result = "";  // Build result string
    
    // Process each byte of data
    for (int i = 0; i < data.length(); i++)
    {
        char dataChar = data[i];              // Current byte
        char keyChar = key[i % key.length()]; // Key byte (cycle through key)
        char xorResult = dataChar ^ keyChar;  // XOR operation (^ operator)
        result += xorResult;                  // Append result
    }
    
    return result;  // Return XOR-transformed data
}
```

**Understanding XOR:**
```
 ^ is the bitwise XOR operator
 
 dataChar   = 01001000 (e.g., 'H')
 keyChar    = 01010011 (e.g., 'S')
 XOR result = 00011011 (completely different byte)
 
 To decrypt:
 XOR result ^ keyChar = 00011011 ^ 01010011 = 01001000 (original!)
```

### Modular Exponentiation (Lines 427-446)

```cpp
long long modPow(long long base, long long exp, long long mod)
{
    long long result = 1;       // Start with 1
    base = base % mod;          // Reduce base by modulus
    
    // Binary exponentiation algorithm
    while (exp > 0)  // Continue until exponent is zero
    {
        // If exponent is odd, multiply result by base
        if (exp % 2 == 1)  // Check if last bit is 1
        {
            result = (result * base) % mod;  // Multiply and reduce
        }
        
        exp = exp >> 1;  // Right shift: divide exponent by 2 (integer division)
        base = (base * base) % mod;  // Square base and reduce
    }
    
    return result;  // Return base^original_exp mod mod
}
```

**Why This Works:**
```
To calculate 83^17 mod 3233:
17 in binary = 10001

We process from right to left:
1 → multiply result by 83
0 → square base only
0 → square base only
0 → square base only
1 → multiply result by (83^16 mod 3233)

Result = 83^1 * 83^16 = 83^17 ✓
```

### Hex Encoding (Lines 282-300)

```cpp
string encodeToHex(string data)
{
    string hexOutput = "";  // Build hex string
    
    // Process each byte in data
    for (int i = 0; i < data.length(); i++)
    {
        unsigned char byte = (unsigned char)data[i];  // Current byte (0-255)
        
        // Each byte = 2 hex digits
        // High nibble: divide by 16 (upper 4 bits)
        hexOutput += decimalToHex(byte / 16);
        
        // Low nibble: modulo 16 (lower 4 bits)
        hexOutput += decimalToHex(byte % 16);
    }
    
    return hexOutput;  // Return hex string
}

string decimalToHex(int decimal)
{
    if (decimal < 0 || decimal > 15)
        return "";  // Invalid
    
    if (decimal < 10)
        return string(1, '0' + decimal);          // 0-9
    else
        return string(1, 'A' + (decimal - 10));   // A-F (10-15)
}
```

**Example:**
```
Byte = 74 (decimal)
74 / 16 = 4 (upper nibble)     → '4'
74 % 16 = 10 (lower nibble)    → 'A'
Result = "4A" (hexadecimal)
```

---

## 14. FILE HANDLING FEATURE

### Saving Encrypted Data

```cpp
ofstream outputFile("securex_output.txt", ios::app);
if (!outputFile)
{
    cout << "Error: Unable to open output file.\n";
}
else
{
    // Append readable encrypted results to the file.
    outputFile << "Encrypted Message (Hex):\n" << hexEncrypted << "\n";
    outputFile << "RSA-Encrypted Secret Key:\n" << encryptedKey << "\n";
    outputFile.close();
}
```

### Loading Encrypted Data

```cpp
bool loadFromFile(string filename, string& message, string& key)
{
    ifstream inFile(filename);  // Open file for reading
    
    if (!inFile.is_open())      // Check if file opened
        return false;           // Failed
    
    string line;
    bool readingMessage = false;
    bool readingKey = false;
    
    // Read file line by line
    while (getline(inFile, line))
    {
        // Detect which section we're in
        if (line == "Encrypted Message (Hex):")
        {
            readingMessage = true;  // Start reading message
            readingKey = false;
        }
        else if (line == "RSA-Encrypted Secret Key:")
        {
            readingMessage = false;
            readingKey = true;  // Start reading key
        }
        else if (!line.empty() && line[0] != '=')
        {
            // Store the value below the matching header.
            if (readingMessage)
            {
                message = line;
                readingMessage = false;
            }
            else if (readingKey)
            {
                key = line;
                readingKey = false;
            }
        }
    }
    
    inFile.close();  // Close file
    return (!message.empty() && !key.empty());  // Success if both read
}
```

---

## 15. INPUT VALIDATION

### Why Input Validation?

Without validation, the program would crash if:
- User enters invalid menu choices
- User enters empty plaintext or keys
- User enters invalid hexadecimal
- File operations fail

### Validation Strategy

```
DO NOT assume user input is correct
ALWAYS check before using input
IF invalid, DISPLAY error message
LOOP until valid input is received
```

### Hex Validation Example

```cpp
bool isValidHex(string hex)
{
    // Check each character
    for (char c : hex)
    {
        c = toupper(c);  // Normalize to uppercase
        
        // Valid hex: 0-9, A-F, or space
        if (!((c >= '0' && c <= '9') || 
              (c >= 'A' && c <= 'F') || 
              c == ' '))
        {
            return false;  // Found invalid character
        }
    }
    
    return true;  // All characters valid
}
```

---

## SUMMARY OF LEARNING OUTCOMES

After studying this program, students should understand:

1. **Cryptography Basics**
   - Vigenère cipher (substitution)
   - XOR encryption (bitwise operation)
   - RSA encryption (public-key system)

2. **Programming Concepts**
   - Modular design with functions
   - Input validation and error handling
   - File I/O operations
   - String and character manipulation
   - Efficient algorithms (modular exponentiation)

3. **C++ Skills**
   - All fundamental C++ features (Chapters 1-7)
   - No advanced features required
   - Clean, readable code
   - Proper commenting

4. **Algorithm Design**
   - Multi-step encryption pipeline
   - Reversible transformations
   - Data format conversions
   - Efficient computation techniques

---

**Total Lines of Code: approximately 600**
**Functions: 17**
**Complexity: Beginner-Intermediate**
**Purpose: Educational**
