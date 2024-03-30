#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

struct Question {
    std::string questionText;
    std::vector<std::string> options;
    int correctOption;
};

class QuizGame {
private:
    std::vector<Question> questions;
    std::vector<int> userAnswers;
    int currentQuestionIndex;
    int score;

public:
    QuizGame(const std::string& filename) {
        loadQuestions(filename);
        currentQuestionIndex = 0;
        score = 3;
    }

    void run() {
        char choice;
        do {
            displayQuestion();
            getUserAnswer();
            std::cout << "\n-----------------\n";
            std::cout << "Press 'e' to go to the next question, 'q' to go back, or 'f' to finish: ";
            std::cin >> choice;

            if (choice == 'e') {
                ++currentQuestionIndex;
            }
            else if (choice == 'q') {
                --currentQuestionIndex;
            }
        } while (choice != 'f' && currentQuestionIndex < questions.size() && currentQuestionIndex >= 0);

        displayResult();
    }

private:
    void loadQuestions(const std::string& filename) {
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file '" << filename << "'.\n";
            exit(EXIT_FAILURE);
        }

        Question currentQuestion;
        std::string line;

        while (std::getline(file, line)) {
            if (line.empty()) {
                questions.push_back(currentQuestion);
                currentQuestion = Question();
            }
            else if (line[0] == 'Q') {
                currentQuestion.questionText = line.substr(2);
            }
            else if (line[0] == 'A') {
                currentQuestion.options.push_back(line.substr(2));
            }
            else if (line[0] == 'C') {
                currentQuestion.correctOption = std::stoi(line.substr(2));
            }
        }

        file.close();
    }

    void displayQuestion() const {
        const Question& currentQuestion = questions[currentQuestionIndex];

        std::cout << "Question " << currentQuestionIndex + 1 << ":\n";
        std::cout << currentQuestion.questionText << "\n";

        for (size_t i = 0; i < currentQuestion.options.size(); ++i) {
            std::cout << i + 1 << ". " << currentQuestion.options[i] << "\n";
        }
    }

    void getUserAnswer() {
        int userAnswer;
        do {
            std::cout << "Your answer (1-" << questions[currentQuestionIndex].options.size() << "): ";
            std::cin >> userAnswer;
        } while (userAnswer < 1 || userAnswer > questions[currentQuestionIndex].options.size());

        userAnswers.push_back(userAnswer);


    }

    void displayResult() const {
        std::cout << "Test finished!\n";
        std::cout << "Your score: " << score << "/" << questions.size() << "\n";

        std::cout << "Review Answers:\n";
        for (size_t i = 0; i < questions.size(); ++i) {
            std::cout << "Question " << i + 1 << ": ";
            std::cout << "Your answer: " << userAnswers[i] << ", Correct answer: " << questions[i].correctOption << "\n";

        }
    }
};

bool isUserAllowed(const std::string& userName, const std::string& allowedUsersFile) {
    std::ifstream file(allowedUsersFile);
    std::string line;

    while (std::getline(file, line)) {
        if (line == userName) {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

int main() {
    std::string userName;
    std::cout << "Enter your name: ";
    std::cin >> userName;
    std::cin >> userName;


    if (!isUserAllowed(userName, "login.txt")) {
        std::cout << "Wrong Username! Please Try Again.\n";
        return 1; // Terminate the program
    }

    QuizGame quiz("questions.txt");
    quiz.run();

    return 0;
}
