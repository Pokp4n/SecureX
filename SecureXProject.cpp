#include <iostream>
#include <string>
#include <fstream>
#include <cctype>

using namespace std;

// Function prototypes

void displayMenu();
void ENCRYPTION();
void DECRYPTION();
string RSA_ENCRYPT_KEY(string secretKey, int e, int n);
string RSA_DECRYPT_KEY(string encryptedKey, int d, int n);
string vigenereEncrypt(string plaintext, string key);
string vigenereDecrypt(string ciphertext, string key);
string xorTransform(string data, string key);
string decimalToHex(int decimal);
int hexToDecimal(string hex);
string encodeToHex(string data);
string decodeFromHex(string hexData);
long long modPow(long long base, long long exp, long long mod);
bool saveToFile(string filename, string message, string key);
bool loadFromFile(string filename, string& message, string& key);
bool isValidHex(string hex);

// Small RSA values for learning only
const int RSA_P = 61;        // First prime
const int RSA_Q = 53;        // Second prime
const int RSA_N = 3233;      // n = p * q
const int RSA_E = 17;        // Public exponent (coprime with phi(n))
const int RSA_D = 2753;      // Private exponent (e*d ≡ 1 mod phi(n))
// phi(n) = (p-1)(q-1) = 60*52 = 3120

// Main program

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
                cout << "\nThank you for using SecureX. Goodbye!\n\n";
                running = false;  // Stop the loop.
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

// Display the menu

void displayMenu()
{
    cout << "\n==========================\n";
    cout << "     SECUREX SYSTEM\n";
    cout << "==========================\n";
    cout << "1. Encrypt Message\n";
    cout << "2. Decrypt Message\n";
    cout << "3. Exit\n";
    cout << "==========================\n";
    cout << "Enter choice: ";
}

// Encryption workflow:
// Vigenere -> XOR -> hexadecimal

void ENCRYPTION()
{
    string plaintext, secretKey, intermediateText, xorResult, hexEncrypted, encryptedKey;
    bool validInput = false;

    cout << "\n--- ENCRYPTION MODULE ---\n";

    // Get both inputs and check that they are not empty.
    while (!validInput)
    {
        cout << "Enter plaintext message: ";
        getline(cin, plaintext);

        cout << "Enter secret key: ";
        getline(cin, secretKey);

        // Both inputs are needed.
        if (plaintext.empty() || secretKey.empty())
        {
            cout << "[ERROR] Plaintext and secret key cannot be empty.\n";
            cout << "Please try again.\n\n";
        }
        else
        {
            validInput = true;
        }
    }

    // First layer: Vigenere encryption.
    cout << "\n[Processing] Applying Vigenère encryption...\n";
    intermediateText = vigenereEncrypt(plaintext, secretKey);
    cout << "Intermediate ciphertext (first 50 chars): ";
    cout << intermediateText.substr(0, min(50, (int)intermediateText.length())) << "...\n";

    // Second layer: XOR with the key.
    cout << "[Processing] Applying XOR transformation...\n";
    xorResult = xorTransform(intermediateText, secretKey);

    // Hex makes the binary result readable.
    cout << "[Processing] Encoding to hexadecimal...\n";
    hexEncrypted = encodeToHex(xorResult);

    // Protect the key with RSA.
    cout << "[Processing] Encrypting secret key with RSA...\n";
    encryptedKey = RSA_ENCRYPT_KEY(secretKey, RSA_E, RSA_N);

    // Show the results in the terminal.
    cout << "\n==========================\n";
    cout << "   SECUREX ENCRYPTION\n";
    cout << "==========================\n";
    cout << "Original Plaintext:\n" << plaintext << "\n\n";
    cout << "Original Secret Key:\n" << secretKey << "\n\n";
    cout << "Encrypted Message (Hex):\n" << hexEncrypted << "\n\n";
    cout << "RSA-Encrypted Secret Key:\n" << encryptedKey << "\n";
    cout << "==========================\n";

    // Append the same results to the output file.
    ofstream outputFile("securex_output.txt", ios::app);
    if (!outputFile)
    {
        cout << "Error: Unable to open output file.\n";
    }
    else
    {
        outputFile << "\n==========================\n";
        outputFile << "   SECUREX ENCRYPTION\n";
        outputFile << "==========================\n";
        outputFile << "Original Plaintext:\n" << plaintext << "\n\n";
        outputFile << "Original Secret Key:\n" << secretKey << "\n\n";
        outputFile << "Encrypted Message (Hex):\n" << hexEncrypted << "\n\n";
        outputFile << "RSA-Encrypted Secret Key:\n" << encryptedKey << "\n";
        outputFile << "==========================\n";
        outputFile.close();
    }
}

// Vigenere encryption
// Each letter is shifted using the repeating key.

string vigenereEncrypt(string plaintext, string key)
{
    string ciphertext = "";
    int keyIndex = 0;

    // Check each plaintext character.
    for (int i = 0; i < plaintext.length(); i++)
    {
        char plainChar = plaintext[i];
        char keyChar = key[keyIndex % key.length()];  // Repeat the key.

        // Shift letters only.
        if (isalpha(plainChar))
        {
            // Keep the original letter case.
            if (isupper(plainChar))
            {
                // Keep the result between A and Z.
                int shift = keyChar % 26;  // Shift is 0 to 25.
                ciphertext += char((plainChar - 'A' + shift) % 26 + 'A');
            }
            else
            {
                // Keep the result between a and z.
                int shift = keyChar % 26;
                ciphertext += char((plainChar - 'a' + shift) % 26 + 'a');
            }
            keyIndex++;  // Move to the next key letter.
        }
        else
        {
            // Spaces and symbols stay the same.
            ciphertext += plainChar;
        }
    }

    return ciphertext;
}

// Vigenere decryption
// Subtract the same shift used during encryption.

string vigenereDecrypt(string ciphertext, string key)
{
    string plaintext = "";
    int keyIndex = 0;

    // Process each character in ciphertext
    for (int i = 0; i < ciphertext.length(); i++)
    {
        char cipherChar = ciphertext[i];
        char keyChar = key[keyIndex % key.length()];

        if (isalpha(cipherChar))
        {
            if (isupper(cipherChar))
            {
                // Undo the shift.
                int shift = keyChar % 26;
                plaintext += char((cipherChar - 'A' - shift + 26) % 26 + 'A');
            }
            else
            {
                int shift = keyChar % 26;
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

// XOR transformation
// Using the same key again gets the original data back.

string xorTransform(string data, string key)
{
    string result = "";

    for (int i = 0; i < data.length(); i++)
    {
        char dataChar = data[i];
        char keyChar = key[i % key.length()];  // Repeat the key.
        char xorResult = dataChar ^ keyChar;   // ^ means bitwise XOR.
        result += xorResult;
    }

    return result;
}

// Convert bytes into readable hexadecimal text.

string encodeToHex(string data)
{
    string hexOutput = "";

    for (int i = 0; i < data.length(); i++)
    {
        unsigned char byte = (unsigned char)data[i];

        // One byte becomes two hex digits.
        hexOutput += decimalToHex(byte / 16);  // First digit.
        hexOutput += decimalToHex(byte % 16);  // Second digit.
    }

    return hexOutput;
}

// Convert hexadecimal text back into bytes.

string decodeFromHex(string hexData)
{
    string result = "";

    // Read two hex digits at a time.
    for (int i = 0; i < hexData.length(); i += 2)
    {
        if (i + 1 < hexData.length())
        {
            // Turn the two digits back into one byte.
            int highNibble = hexToDecimal(hexData.substr(i, 1));
            int lowNibble = hexToDecimal(hexData.substr(i + 1, 1));

            if (highNibble >= 0 && lowNibble >= 0)
            {
                int byte = highNibble * 16 + lowNibble;
                result += char(byte);
            }
        }
    }

    return result;
}

// Convert one decimal digit to hex.

string decimalToHex(int decimal)
{
    if (decimal < 0 || decimal > 15)
        return "";

    if (decimal < 10)
        return string(1, '0' + decimal);
    else
        return string(1, 'A' + (decimal - 10));
}

// Convert one hex digit to decimal.

int hexToDecimal(string hex)
{
    if (hex.empty())
        return -1;

    char hexChar = toupper(hex[0]);

    if (hexChar >= '0' && hexChar <= '9')
        return hexChar - '0';
    else if (hexChar >= 'A' && hexChar <= 'F')
        return hexChar - 'A' + 10;
    else
        return -1;  // Invalid digit.
}

// Encrypt the secret key with RSA.
// Formula: C = M^e mod n

string RSA_ENCRYPT_KEY(string secretKey, int e, int n)
{
    string encryptedKey = "";

    // Encrypt one character at a time.
    for (int i = 0; i < secretKey.length(); i++)
    {
        // Use the character's ASCII value.
        int plainValue = (int)secretKey[i];

        // Apply the RSA formula.
        long long cipherValue = modPow(plainValue, e, n);

        // Convert the result to hex.
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

        // Use four digits so every value has the same length.
        while (hexVal.length() < 4)
            hexVal = "0" + hexVal;

        encryptedKey += hexVal;

        // Separate values with spaces.
        if (i < secretKey.length() - 1)
            encryptedKey += " ";
    }

    return encryptedKey;
}

// Decrypt the RSA-protected key.
// Formula: M = C^d mod n

string RSA_DECRYPT_KEY(string encryptedKey, int d, int n)
{
    string recoveredKey = "";
    int startPos = 0;

    // Read each space-separated value.
    for (int i = 0; i <= encryptedKey.length(); i++)
    {
        // A space marks the end of one value.
        if (i == encryptedKey.length() || encryptedKey[i] == ' ')
        {
            if (i > startPos)
            {
                string hexValue = encryptedKey.substr(startPos, i - startPos);

                // Change the hex value into a number.
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

                // Apply the RSA decryption formula.
                long long plainValue = modPow(cipherValue, d, n);

                // Change the ASCII value back to a character.
                recoveredKey += char(plainValue);
            }

            startPos = i + 1;
        }
    }

    return recoveredKey;
}

// Calculate (base^exp) mod mod without making huge numbers.

long long modPow(long long base, long long exp, long long mod)
{
    long long result = 1;
    base = base % mod;

    // Repeatedly square the base.
    while (exp > 0)
    {
        // For an odd exponent, include the current base.
        if (exp % 2 == 1)
            result = (result * base) % mod;

        // Halve the exponent and square the base.
        exp = exp >> 1;  // Right shift divides by 2.
        base = (base * base) % mod;
    }

    return result;
}

// Decryption reverses the encryption steps.

void DECRYPTION()
{
    string hexEncrypted, encryptedKeyStr, recoveredKey, xorResult, plaintext;
    bool validInput = false;

    cout << "\n--- DECRYPTION MODULE ---\n";

    // The user can load the data from a file.
    char loadChoice;
    cout << "Load encrypted data from file? (Y/N): ";
    cin >> loadChoice;
    cin.ignore(1000, '\n');

    // Keep asking until the answer is Y or N.
    while (loadChoice != 'Y' && loadChoice != 'y' &&
           loadChoice != 'N' && loadChoice != 'n')
    {
        cout << "[ERROR] Please enter Y or N: ";
        cin >> loadChoice;
        cin.ignore(1000, '\n');
    }

    if (loadChoice == 'Y' || loadChoice == 'y')
    {
        string filename;
        cout << "Enter filename: ";
        getline(cin, filename);

        if (loadFromFile(filename, hexEncrypted, encryptedKeyStr))
        {
            cout << "[SUCCESS] Data loaded from file.\n";
            validInput = true;
        }
        else
        {
            cout << "[ERROR] Could not load file. Entering manual input mode.\n";
            validInput = false;
        }
    }

    // Otherwise, enter both values manually.
    while (!validInput)
    {
        cout << "Enter encrypted hexadecimal message: ";
        getline(cin, hexEncrypted);

        cout << "Enter RSA-encrypted secret key: ";
        getline(cin, encryptedKeyStr);

        // Both values are required.
        if (hexEncrypted.empty() || encryptedKeyStr.empty())
        {
            cout << "[ERROR] Encrypted message and key cannot be empty.\n";
            cout << "Please try again.\n\n";
        }
        else if (!isValidHex(hexEncrypted))
        {
            cout << "[ERROR] Invalid hexadecimal input.\n";
            cout << "Please enter valid hex characters (0-9, A-F).\n\n";
        }
        else
        {
            validInput = true;
        }
    }

    // Recover the secret key first.
    cout << "\n[Processing] Decrypting secret key with RSA...\n";
    recoveredKey = RSA_DECRYPT_KEY(encryptedKeyStr, RSA_D, RSA_N);
    cout << "Recovered secret key: " << recoveredKey << "\n";

    // Change the hex message back to bytes.
    cout << "[Processing] Decoding from hexadecimal...\n";
    string xorEncrypted = decodeFromHex(hexEncrypted);

    // XOR again to undo the first XOR.
    cout << "[Processing] Applying XOR transformation...\n";
    string vigenereEncrypted = xorTransform(xorEncrypted, recoveredKey);

    // Undo the Vigenere encryption.
    cout << "[Processing] Applying Vigenère decryption...\n";
    plaintext = vigenereDecrypt(vigenereEncrypted, recoveredKey);

    // Show the results in the terminal.
    cout << "\n==========================\n";
    cout << "   SECUREX DECRYPTION\n";
    cout << "==========================\n";
    cout << "Encrypted Message (Hex):\n" << hexEncrypted << "\n\n";
    cout << "RSA-Encrypted Secret Key:\n" << encryptedKeyStr << "\n\n";
    cout << "Recovered Secret Key:\n" << recoveredKey << "\n\n";
    cout << "Recovered Plaintext:\n" << plaintext << "\n";
    cout << "==========================\n";

    // Append the same results to the output file.
    ofstream outputFile("securex_output.txt", ios::app);
    if (!outputFile)
    {
        cout << "Error: Unable to open output file.\n";
    }
    else
    {
        outputFile << "\n==========================\n";
        outputFile << "   SECUREX DECRYPTION\n";
        outputFile << "==========================\n";
        outputFile << "Encrypted Message (Hex):\n" << hexEncrypted << "\n\n";
        outputFile << "RSA-Encrypted Secret Key:\n" << encryptedKeyStr << "\n\n";
        outputFile << "Recovered Secret Key:\n" << recoveredKey << "\n\n";
        outputFile << "Recovered Plaintext:\n" << plaintext << "\n";
        outputFile << "==========================\n";
        outputFile.close();
    }
}

// File input and output

bool saveToFile(string filename, string message, string key)
{
    ofstream outFile(filename);

    if (!outFile.is_open())
        return false;

    // Save the message and key in a simple text format.
    outFile << "--- SECUREX ENCRYPTED DATA ---\n";
    outFile << "\nENCRYPTED MESSAGE (HEX):\n";
    outFile << message << "\n\n";
    outFile << "RSA-ENCRYPTED SECRET KEY:\n";
    outFile << key << "\n";

    outFile.close();
    return true;
}

bool loadFromFile(string filename, string& message, string& key)
{
    ifstream inFile(filename);

    if (!inFile.is_open())
        return false;

    string line;
    bool readingMessage = false;
    bool readingKey = false;

    // Read the latest message and key from the saved results.
    while (getline(inFile, line))
    {
        if (line == "Encrypted Message (Hex):")
        {
            readingMessage = true;
            readingKey = false;
        }
        else if (line == "RSA-Encrypted Secret Key:")
        {
            readingMessage = false;
            readingKey = true;
        }
        else if (!line.empty() && line[0] != '=')
        {
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

    inFile.close();
    return (!message.empty() && !key.empty());
}

// Check whether a string contains valid hex characters.

bool isValidHex(string hex)
{
    for (char c : hex)
    {
        c = toupper(c);
        if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || c == ' '))
            return false;
    }
    return true;
}
