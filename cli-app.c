#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_FLASHCARDS 100

int player1_score = 0;
int player2_score = 0;

struct Flashcard {
    char question[100];
    char answer[100];
};

struct Flashcard flashcards[MAX_FLASHCARDS];
int num_flashcards = 0;

int daily_streak = 0;
time_t last_quiz_time = 0;


void loadFlashcards() {
    FILE *file = fopen("flashcards.txt", "r");
    if (file == NULL) {
        printf("No flashcards found.\n");
        return;
    }

    while (fscanf(file, "%99[^\n]\n%99[^\n]\n", flashcards[num_flashcards].question, flashcards[num_flashcards].answer) == 2) {
        num_flashcards++;
    }

    fclose(file);
}


void saveFlashcards() {
    FILE *file = fopen("flashcards.txt", "w");
    if (file == NULL) {
        printf("Error saving flashcards.\n");
        return;
    }

    for (int i = 0; i < num_flashcards; i++) {
        fprintf(file, "%s\n%s\n", flashcards[i].question, flashcards[i].answer);
    }

    fclose(file);
}

void loadStreak() {
    FILE *file = fopen("streak.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d %ld", &daily_streak, &last_quiz_time);
        fclose(file);
    }
}

void saveStreak() {
    FILE *file = fopen("streak.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d %ld", daily_streak, last_quiz_time);
        fclose(file);
    }
}

void addFlashcard() {
    if (num_flashcards >= MAX_FLASHCARDS) {
        printf("Maximum number of flashcards reached!\n");
        return;
    }

    printf("Enter question: ");
    fflush(stdin);
    fgets(flashcards[num_flashcards].question, sizeof(flashcards[num_flashcards].question), stdin);
    flashcards[num_flashcards].question[strcspn(flashcards[num_flashcards].question, "\n")] = 0;

    printf("Enter answer: ");
    fgets(flashcards[num_flashcards].answer, sizeof(flashcards[num_flashcards].answer), stdin);
    flashcards[num_flashcards].answer[strcspn(flashcards[num_flashcards].answer, "\n")] = 0;

    num_flashcards++;
    saveFlashcards();
}

void viewFlashcards() {
    if (num_flashcards == 0) {
        printf("No flashcards available.\n");
        return;
    }

    for (int i = 0; i < num_flashcards; i++) {
        printf("Question: %s\n", flashcards[i].question);
        printf("Answer: %s\n\n", flashcards[i].answer);
    }
}

void quiz() {
    if (num_flashcards == 0) {
        printf("No flashcards available.\n");
        return;
    }

    int choice;
    do {
        for (int i = 0; i < num_flashcards; i++) {
            printf("Question %d: %s\n", i + 1, flashcards[i].question);

            char user_answer[100];
            printf("Your answer (or 0 to exit): ");
            fgets(user_answer, sizeof(user_answer), stdin);

            if (user_answer[0] == '0') {
                return; // Exit the quiz
            }

            user_answer[strcspn(user_answer, "\n")] = 0;

            if (strcmp(user_answer, flashcards[i].answer) == 0) {
                printf("Correct!\n\n");
            } else {
                printf("Incorrect. The correct answer is: %s\n\n", flashcards[i].answer);
            }
        }

        printf("Quiz completed. Enter 0 to exit");
        scanf("%d", &choice);
        getchar(); // Consume trailing newline
        printf("%d",choice);
        if (choice != 0) {
            // Update streak
            time_t current_time = time(NULL);
            struct tm *tm_info = localtime(&current_time);

            if (current_time - last_quiz_time <= 24 * 3600 &&
                tm_info->tm_mday != last_quiz_time) {
                daily_streak++;
            } else {
                daily_streak = 1;
            }

            last_quiz_time = current_time;
        }

    } while (choice != 0);
}

void viewStreak() {
    printf("Your current streak is: %d\n", daily_streak);
}

void versusMode() {
    if (num_flashcards == 0) {
        printf("No flashcards available.\n");
        return;
    }

    int choice;
    do {
        printf("Versus Mode Quiz\n");

        for (int i = 0; i < num_flashcards; i++) {
            printf("Question %d: %s\n", i + 1, flashcards[i].question);

            char user_answer_player1[100];
            printf("Player 1, your answer (or 0 to exit): ");
            fgets(user_answer_player1, sizeof(user_answer_player1), stdin);

            if (user_answer_player1[0] == '0') {
                return; // Exit the quiz
            }

            user_answer_player1[strcspn(user_answer_player1, "\n")] = 0;

            if (strcmp(user_answer_player1, flashcards[i].answer) == 0) {
                printf("Player 1 is Correct!\n\n");
                player1_score++;
            } else {
                printf("Player 1 is Incorrect\n\n");
            }

            char user_answer_player2[100];
            printf("Player 2, your answer (or 0 to exit): ");
            fgets(user_answer_player2, sizeof(user_answer_player2), stdin);

            if (user_answer_player2[0] == '0') {
                return; // Exit the quiz
            }

            user_answer_player2[strcspn(user_answer_player2, "\n")] = 0;

            if (strcmp(user_answer_player2, flashcards[i].answer) == 0) {
                printf("Player 2 is Correct!\n\n");
                player2_score++;
            } else {
                printf("Player 2 is Incorrect");
            }
        }

            printf("Player 1 Score: %d\n", player1_score);
            printf("Player 2 Score: %d\n", player2_score);
        printf("Quiz completed. Enter 0 to exit\n");
        scanf("%d", &choice);
        getchar(); // Consume trailing newline
        printf("%d", choice);
        if (choice != 0) {
            // Update streak
            time_t current_time = time(NULL);
            struct tm *tm_info = localtime(&current_time);

            if (current_time - last_quiz_time <= 24 * 3600 &&
                tm_info->tm_mday != last_quiz_time) {
                daily_streak++;
            } else {
                daily_streak = 1;
            }

            last_quiz_time = current_time;

        }

    } while (choice != 0);
}



int main() {
    loadFlashcards();
    loadStreak();

    int choice;

    do {
        printf("Flashcard App\n");
        printf("1. Add Flashcard\n");
        printf("2. View Flashcards\n");
        printf("3. Quiz\n");
        printf("4. View Streak\n");
        printf("5. Versus Mode\n");
        printf("6. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        getchar(); 

        switch (choice) {
            case 1:
                addFlashcard();
                break;
            case 2:
                viewFlashcards();
                break;
            case 3:
                quiz();
                break;
            case 4:
                viewStreak();
                break;
            case 5: // Assuming 6 is the choice for Versus Mode
                versusMode();
                break;
            case 6:
                saveStreak();
                saveFlashcards();
                printf("Exiting...\n");
                break;
            
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 5);

    return 0;
}
