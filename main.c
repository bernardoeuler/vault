#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PASSWORD_LENGTH 3
#define INPUT_LENGTH PASSWORD_LENGTH * 2 + 1

int validate_input(char* input);
unsigned short* get_password(char* input);
int verify_password(const unsigned short* target_password, unsigned short* password);
void flush_stdin();

int main() {
    const unsigned short VAULT_PASSWORD[PASSWORD_LENGTH] = {1, 0, 6};
    char* input = malloc(INPUT_LENGTH);
    unsigned short* password;
    unsigned short validation_result;

    while (1) {
        printf("Digite a combinação de %hu dígitos para abrir o cofre: ", PASSWORD_LENGTH);
        fgets(input, INPUT_LENGTH, stdin);
        printf("\n");

        validation_result = validate_input(input);
        
        if (validation_result) {
            printf("Entrada inválida. Tente novamente.\n");
            printf("A entrada deve ser do formato \"x y z\".\n\n");
            if (validation_result == 1) flush_stdin();
            continue;
        }

        if (input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0';
        }

        password = get_password(input);

        if (!verify_password(VAULT_PASSWORD, password)) {
            break;
        }
    }
        
    printf("Parabéns! Você abriu o cofre!\n");

    return 0;
}

// Function to validate the user input
int validate_input(char* input) {
    // Check if the last non-null character is a new line
    if (!strchr(input, '\n')) {
        return 1;
    }

    // Check if the input length is incorrect
    if (strlen(input) < PASSWORD_LENGTH * 2) {
        return 2;
    }

    // Check if the characters are correct
    for (unsigned short i = 0; i < strlen(input); i++) {
        // Check if characters in even indexes are numeric or not
        if (i % 2 == 0 && (input[i] < 48 || input[i] > 57)) {
            return 3;
        // Check if characters in odd indexes are whitespace or not
        } else if (i % 2 != 0 && input[i] != ' ' && input[i] != '\n') {
            return 4;
        }
    }

    return 0;
}

// Function to convert the user input to an array that represents the password
unsigned short* get_password(char* input) {
    // Declare password variable
    unsigned short* password;

    // Initialize password variable
    password = calloc(PASSWORD_LENGTH, sizeof(password));

    // Convert numeric characters to numbers
    for (unsigned short i = 0; i < strlen(input); i++) {
        if (i % 2 == 0) {
            password[i / 2] = input[i] - 48;
        }
    }

    return password;
}

// Function to check if the password is correct
int verify_password(const unsigned short* target_password, unsigned short* password) {
    // Declare arrays
    int correct_digits[PASSWORD_LENGTH];
    int uncovered_digits[PASSWORD_LENGTH];
    int wrong_position_digits[PASSWORD_LENGTH];

    // Initialize values
    for (unsigned short i = 0; i < PASSWORD_LENGTH; i++) {
        correct_digits[i] = 0;
        uncovered_digits[i] = 0;
        wrong_position_digits[i] = 0;
    }

    // Check if there are correct digits
    for (unsigned short i = 0; i < PASSWORD_LENGTH; i++) {
        if (password[i] == target_password[i]) {
            correct_digits[i] = 1;
        }
    }

    // Check if there are correct digits but in the wrong position
    for (unsigned short i = 0; i < PASSWORD_LENGTH; i++) {
        for (unsigned j = 0; j < PASSWORD_LENGTH; j++) {
            if (i == j || correct_digits[i] || correct_digits[j] || uncovered_digits[j]) {
                continue;
            }

            if (password[i] == target_password[j]) {
                wrong_position_digits[i] = 1;
                uncovered_digits[j] = 1;
            }
        }
    }

    // Give feedback to the user
    for (unsigned short i = 0; i < PASSWORD_LENGTH; i++) {
        if (correct_digits[i]) {
            printf("O dígito %hu está correto na posição %u.\n", password[i], i + 1);
        } else if (wrong_position_digits[i]) {
            printf("O dígito %hu está correto, mas na posição errada.\n", password[i]);
        } else {
            printf("O dígito %hu não faz parte da senha.\n", password[i]);
        }
    }

    printf("\n");

    // Check if there is any incorrect digit to return an error (non-zero value)
    for (unsigned short i = 0; i < PASSWORD_LENGTH; i++) {
        if (!correct_digits[i]) {
            return 1;
        }
    }

    // Return zero when the password is correct
    return 0;
}

void flush_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}