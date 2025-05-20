#include <iostream>
#include <string>
#include <chrono>
#include <limits>
#include <filesystem>
#include <memory>
#include <iomanip>
#include <cctype>
#include <fstream>
#include "board/board.hpp"
#include "algorithms/solver.hpp"
#include "algorithms/ucs.hpp"
#include "algorithms/astar.hpp"
#include "algorithms/greedy.hpp"
#include "algorithms/iter_deepening.hpp"
#include "heuristics/manhattan.hpp"
#include "heuristics/blocking_cars.hpp"
#include "utils/file_reader.hpp"
#include "utils/output_writer.hpp"

#ifdef _WIN32
    #include <windows.h>
    void enableConsoleFeatures() {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    }
#endif

namespace ansi {
    constexpr const char* reset  = "\033[0m";
    constexpr const char* bold   = "\033[1m";
    constexpr const char* cyan   = "\033[96m";
    constexpr const char* yellow = "\033[93m";
    constexpr const char* green  = "\033[92m";
    constexpr const char* red    = "\033[91m";
}

void printDivider(char fill = '=') {
    std::cout << std::string(60, fill) << "\n";
}

void printHeader() {
    using namespace ansi;
    printDivider();
    std::cout << cyan << bold
              << "                              █████         █████                                   \n"
              << "                             ░░███         ░░███                                    \n"
              << " ████████  █████ ████  █████  ░███████      ░███████    ██████  █████ ████ ████████ \n"
              << "░░███░░███░░███ ░███  ███░░   ░███░░███     ░███░░███  ███░░███░░███ ░███ ░░███░░███\n"
              << " ░███ ░░░  ░███ ░███ ░░█████  ░███ ░███     ░███ ░███ ░███ ░███ ░███ ░███  ░███ ░░░ \n"
              << " ░███      ░███ ░███  ░░░░███ ░███ ░███     ░███ ░███ ░███ ░███ ░███ ░███  ░███    \n"
              << " █████     ░░████████ ██████  ████ █████    ████ █████░░██████  ░░████████ █████   \n"
              << "░░░░░       ░░░░░░░░ ░░░░░░  ░░░░ ░░░░░    ░░░░ ░░░░░  ░░░░░░    ░░░░░░░░ ░░░░░     \n"
              << "                               S O L V E R   O O P                                   "
              << reset << "\n";
    printDivider();
}

int promptChoice(const std::string& label, int min, int max) {
    std::cout << ansi::yellow << label << ansi::reset;
    int choice;
    while (!(std::cin >> choice) || choice < min || choice > max) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << ansi::red << "Input tidak valid. Pilih "
                  << min << "-" << max << ": " << ansi::reset;
    }
    return choice;
}

void solveOnce() {
    std::string filename;
    do {
        std::cout << "Masukkan path file konfigurasi puzzle (.txt): ";
        std::cin  >> filename;
        if (!std::filesystem::exists(filename))
            std::cout << ansi::red << "File tidak ditemukan. Coba lagi.\n" << ansi::reset;
    } while (!std::filesystem::exists(filename));

    std::cout << ansi::green << u8"\u2714  File terbaca dengan sukses!\n" << ansi::reset;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string reportBase;
    std::cout << "Simpan laporan ke test/<nama>.txt (kosong = tidak menyimpan): ";
    std::getline(std::cin, reportBase);
    std::unique_ptr<std::ofstream> report;
    if (!reportBase.empty()) {
        std::filesystem::create_directory("../test");
        std::string path = "../test/" + reportBase + ".txt";
        report = std::make_unique<std::ofstream>(path);
        if (!*report) {
            std::cerr << ansi::red << "Gagal membuat file laporan: " << path << ansi::reset << "\n";
            report.reset();
        }
    }

    Board board = FileReader::readBoard(filename);

    std::cout << "\nPilih algoritma:\n";
    std::cout << " 1. UCS (Uniform Cost Search)\n";
    std::cout << " 2. Greedy Best-First Search\n";
    std::cout << " 3. A*\n";
    std::cout << " 4. Iterative Deepening\n";
    int algoChoice = promptChoice("Pilihan: ", 1, 4);

    std::string heuristic;
    if (algoChoice == 2 || algoChoice == 3) {
        std::cout << "\nPilih heuristic:\n";
        std::cout << " 1. Manhattan Distance\n";
        std::cout << " 2. Blocking Cars\n";
        int hChoice = promptChoice("Pilihan: ", 1, 2);
        heuristic = (hChoice == 1) ? "manhattan" : "blocking";
        std::cout << ansi::green << u8"\u2714  Menggunakan "
                  << (heuristic == "manhattan" ? "Manhattan Distance" : "Blocking Cars")
                  << " heuristic\n" << ansi::reset;
    }

    std::unique_ptr<Solver> solver;
    switch (algoChoice) {
        case 1: solver = std::make_unique<UCS>(); break;
        case 2: solver = std::make_unique<GreedyBestFirst>(heuristic); break;
        case 3: solver = std::make_unique<AStar>(heuristic); break;
        case 4: solver = std::make_unique<IterativeDeepening>(); break;
    }

    std::cout << "\n===  Mencari solusi...\n";
    if (report) *report << "=== Rush Hour Solver Report ===\n\n";

    auto t0 = std::chrono::high_resolution_clock::now();
    auto result = solver->solve(board);
    auto t1 = std::chrono::high_resolution_clock::now();
    long long ms  = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

    std::cout << "\n";
    if (result.moves.empty()) {
        std::cout << ansi::red << u8"\u2718  Tidak ditemukan solusi!\n" << ansi::reset;
        if (report) *report << "Tidak ditemukan solusi!\n";
    } else {
        OutputWriter::printSolution(board, result);           
        if (report) {
            OutputWriter::printSolution(board, result, *report, false); 
            *report << "Waktu eksekusi: " << ms << " ms\n";
        }
        std::cout << ansi::cyan << "Waktu eksekusi: " << ms << " ms\n" << ansi::reset;
    }

    if (report) std::cout << ansi::green << u8"\u2714  Laporan tersimpan di " << report->tellp() << " byte.\n" << ansi::reset;
}

int main() {
#ifdef _WIN32
    enableConsoleFeatures();
#endif
    try {
        printHeader();
        char again;
        do {
            solveOnce();
            std::cout << "\nIngin mencoba puzzle lain? (y/n): ";
            std::cin >> again;
            again = static_cast<char>(std::tolower(again));
            std::cout << "\n";
        } while (again == 'y');

        std::cout << ansi::yellow << "Terima kasih telah menggunakan\n" << ansi::reset;
        printDivider();

    } catch (const std::exception& e) {
        std::cerr << ansi::red << "Error: " << e.what() << ansi::reset << std::endl;
        return 1;
    }
    return 0;
}
