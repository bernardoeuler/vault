#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PASSWORD_LENGTH 3

int validate_input(char* input);
unsigned short* get_password(char* input);
int verify_password(const unsigned short* target_password, unsigned short* password);

int main() {
    const unsigned short VAULT_PASSWORD[PASSWORD_LENGTH] = {1, 0, 6};
    char input[PASSWORD_LENGTH * 2];
    unsigned short* password;

    printf("Digite a senha: ");
    fgets(input, PASSWORD_LENGTH * 2, stdin);

    if (input[strlen(input) - 1] == '\n') {
        input[strlen(input) - 1] = '\0';
    }

    if (validate_input(input)) {
        printf("Entrada inválida. Tente novamente.\n");
        exit(1);
    }

    password = get_password(input);

    printf("Senha %s\n", verify_password(VAULT_PASSWORD, password) ? "incorreta" : "correta");

    return 0;
}

int validate_input(char* input) {
    if (strlen(input) != PASSWORD_LENGTH * 2 - 1) {
        return 1;
    }

    for (unsigned short i = 0; i < strlen(input); i++) {
        if (i % 2 == 0 && (input[i] < 48 || input[i] > 57)) {
            return 1;
        } else if (i % 2 != 0 && input[i] != ' ') {
            return 1;
        }
    }

    return 0;
}

unsigned short* get_password(char* input) {
    unsigned short* password;

    password = calloc(PASSWORD_LENGTH, sizeof(password));

    for (unsigned short i = 0; i < strlen(input); i++) {
        if (i % 2 == 0) {
            password[i / 2] = input[i] - 48;
        }
    }

    return password;
}

int verify_password(const unsigned short* target_password, unsigned short* password) {
    unsigned short correct_digits = 0;
    unsigned short correct_positions[PASSWORD_LENGTH];
    int skip = 0;

    for (unsigned short i = 0; i < PASSWORD_LENGTH; i++) {
        correct_positions[i] = 0;
    } 

    for (unsigned short i = 0; i < PASSWORD_LENGTH; i++) {
        if (password[i] == target_password[i]) {
            printf("O dígito %hu está correto na posição %u.\n", password[i], i + 1);
            correct_digits++;
            correct_positions[i] = 1;
            continue;
        }

        for (unsigned short j = 0; j < PASSWORD_LENGTH; j++) {
            if (correct_positions[j] == 0 && i != j && password[i] == target_password[j]) {
                printf("O dígito %hu está correto, mas na posição errada.\n", password[i]);
                skip = 1;
                break;
            }
        }

        if (!skip) printf("O dígito %hu não faz parte da senha.\n", password[i]);
    }

    printf("\nCorrects: %hu\n", correct_digits);

    if (correct_digits == PASSWORD_LENGTH) {
        return 0;
    }

    return 1;
}
