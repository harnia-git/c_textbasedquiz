#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

typedef struct {
    char question[MAX_LINE_LENGTH];
    char choices[4][MAX_LINE_LENGTH];
    char correctAnswer;
} Quiz;

int loadQuizzes(const char *filename, Quiz quizzes[], int maxQuizzes) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("ファイルを開けませんでした");
        return 0;
    }

    int numQuizzes = 0;
    while (numQuizzes < maxQuizzes && !feof(file)) {
        Quiz *quiz = &quizzes[numQuizzes];
        if (fgets(quiz->question, MAX_LINE_LENGTH, file) == NULL) break;

        for (int i = 0; i < 4; i++) {
            if (fgets(quiz->choices[i], MAX_LINE_LENGTH, file) == NULL) break;
            // 改行文字を除去
            quiz->choices[i][strcspn(quiz->choices[i], "\n")] = 0;
        }

        char answerLine[MAX_LINE_LENGTH];
        if (fgets(answerLine, MAX_LINE_LENGTH, file) == NULL) break;
        quiz->correctAnswer = answerLine[7];  // "正解: "の後の文字を取得

        numQuizzes++;
        // 空行を読み飛ばす
        fgets(answerLine, MAX_LINE_LENGTH, file);
    }

    fclose(file);
    return numQuizzes;
}

void playQuiz(Quiz quizzes[], int numQuizzes) {
    int score = 0;

    for (int i = 0; i < numQuizzes; i++) {
        printf("%s\n", quizzes[i].question);
        for (int j = 0; j < 4; j++) {
            printf("%s\n", quizzes[i].choices[j]);
        }
        printf("あなたの答え: ");

        char answer;
        scanf(" %c", &answer);  // 入力バッファをクリアするためにスペースを前に置く

        if (answer == quizzes[i].correctAnswer) {
            printf("正解です！\n\n");
            score++;
        } else {
            printf("不正解です。正解は %c です。\n\n", quizzes[i].correctAnswer);
        }
    }

    printf("クイズ終了！あなたのスコアは %d/%d です。\n", score, numQuizzes);
}

int main() {
    const char *filename = "quizzes.txt";
    Quiz quizzes[10];  // 最大10問

    int numQuizzes = loadQuizzes(filename, quizzes, 10);
    if (numQuizzes > 0) {
        playQuiz(quizzes, numQuizzes);
    } else {
        printf("クイズをロードできませんでした。\n");
    }

    return 0;
}
