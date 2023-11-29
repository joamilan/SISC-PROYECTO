#include <iostream>
#include <windows.h>

using namespace std;

void mostrarMenu() {
    cout << "---- Menu ----" << endl;
    cout << "1. Iniciar Chat" << endl;
    cout << "2. Terminar Comunicacion Serie" << endl;
    cout << "Ingrese su eleccion: ";
}

int main() {
    HANDLE hComPort;
    char output[255];
    char input[255];

    // Configurar el puerto serial
    hComPort = CreateFile("COM4", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, NULL, 0);

    if (hComPort == INVALID_HANDLE_VALUE) {
        cout << "No se pudo abrir el puerto COM4." << endl;
        return 1;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hComPort, &dcbSerialParams)) {
        cout << "Error al obtener los atributos del puerto serie." << endl;
        CloseHandle(hComPort);
        return 1;
    }

    // Ajustar la velocidad de baudios a 9600 para lectura y escritura
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hComPort, &dcbSerialParams)) {
        cout << "Error al configurar los atributos del puerto serie." << endl;
        CloseHandle(hComPort);
        return 1;
    }

    int opcion;

    do {
        mostrarMenu();
        cin >> opcion;

        switch (opcion) {
            case 1: {
                // Iniciar Chat
                cout << "Iniciando Chat..." << endl;

                while (true) {
                    // Leer línea completa desde el puerto serial
                    memset(input, 0, sizeof(input));  // Limpiar el buffer de entrada
                    ReadFile(hComPort, input, sizeof(input) - 1, NULL, NULL);

                    if (input[0] != '\0') {
                        cout << "Datos Leidos: " << input << endl;
                    }

                    // Escribir línea completa en el puerto serial
                    cout << "Escriba ('exit' para salir): ";
                    cin.getline(output, sizeof(output));

                    if (strcmp(output, "exit") == 0) {
                        // Salir del chat
                        break;
                    }

                    if (output[0] != '\0') {
                        // Escritura
                        WriteFile(hComPort, output, strlen(output), NULL, NULL);
                        cout << "Datos Enviados: " << output << endl;
                    }

                    // Delay de 2 segundos antes de la siguiente iteración
                    Sleep(5000);
                }

                break;
            }
            case 2:
                // Terminar Comunicación Serie
                cout << "Terminando Comunicacion Serie..." << endl;
                break;
            default:
                cout << "Opción no válida. Inténtelo de nuevo." << endl;
                break;
        }

    } while (opcion != 2);

    CloseHandle(hComPort);
    return 0;
}
