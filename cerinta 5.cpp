#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

bool isPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return false;
    return true;
}

int main(int argc, char* argv[]) {
    // LOGICA PENTRU PROCESUL COPIL
    if (argc > 2) {
        int start = std::stoi(argv[1]);
        int end = std::stoi(argv[2]);
        HANDLE hWritePipe = (HANDLE)(uintptr_t)std::stoull(argv[3]);

        for (int i = start; i <= end; i++) {
            if (isPrime(i)) {
                DWORD written;
                WriteFile(hWritePipe, &i, sizeof(int), &written, NULL);
            }
        }
        int sentinel = -1;
        DWORD written;
        WriteFile(hWritePipe, &sentinel, sizeof(int), &written, NULL);
        return 0;
    }

    // LOGICA PENTRU PROCESUL PĂRINTE
    HANDLE hPipesRead[10];
    PROCESS_INFORMATION pi[10];

    for (int i = 0; i < 10; i++) {
        HANDLE hRead, hWrite;
        SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE }; // Permit moștenirea handle-ului

        if (!CreatePipe(&hRead, &hWrite, &sa, 0)) return 1;
        SetHandleInformation(hRead, HANDLE_FLAG_INHERIT, 0); // Doar scrierea se moștenește

        hPipesRead[i] = hRead;

        int start = i * 1000 + 1;
        int end = (i + 1) * 1000;

        // Construim comanda pentru a relansa același .exe cu parametri
        std::string cmd = std::string(argv[0]) + " " + std::to_string(start) + " " + 
                          std::to_string(end) + " " + std::to_string((uintptr_t)hWrite);
        
        STARTUPINFOA si = { sizeof(STARTUPINFOA) };
        if (!CreateProcessA(NULL, (LPSTR)cmd.c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi[i])) {
            std::cerr << "Eroare la creare proces " << i << "\n";
        }
        CloseHandle(hWrite); // Părintele nu are nevoie de scriere
    }

    // Citirea rezultatelor
    for (int i = 0; i < 10; i++) {
        int prime;
        DWORD read;
        std::cout << "\nInterval " << (i * 1000 + 1) << "-" << (i + 1) * 1000 << ":\n";
        while (ReadFile(hPipesRead[i], &prime, sizeof(int), &read, NULL) && read > 0) {
            if (prime == -1) break;
            std::cout << prime << " ";
        }
        CloseHandle(hPipesRead[i]);
        WaitForSingleObject(pi[i].hProcess, INFINITE);
        CloseHandle(pi[i].hProcess);
        CloseHandle(pi[i].hThread);
    }

    return 0;
}