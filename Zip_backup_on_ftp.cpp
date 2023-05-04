#include <iostream>
#include <fstream>
#include <string>
#include <zlib.h>
#include <curl/curl.h>

using namespace std;

// Funzione per la compressione del file
void compressFile(const char* input_filename, const char* output_filename) {
    gzFile output_file = gzopen(output_filename, "wb");
    ifstream input_file(input_filename, ios::binary);
    char buffer[1024];
    int num_read = 0;
    while ((num_read = input_file.read(buffer, sizeof(buffer)).gcount()) > 0) {
        gzwrite(output_file, buffer, num_read);
    }
    gzclose(output_file);
}

// Funzione per il trasferimento del file tramite FTP
int transferFile(const char* ftp_url, const char* ftp_userpwd, const char* local_file_path, const char* remote_file_name) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        cout << "Errore nella connessione FTP" << endl;
        return 1;
    }
    curl_easy_setopt(curl, CURLOPT_URL, ftp_url);
    curl_easy_setopt(curl, CURLOPT_USERPWD, ftp_userpwd);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_READDATA, fopen(local_file_path, "rb"));
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)filesize);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
    curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1L);
    curl_easy_setopt(curl, CURLOPT_FTP_FILEMETHOD, CURLFTPMETHOD_SINGLECWD);
    curl_easy_setopt(curl, CURLOPT_FTP_USE_EPSV, false);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "STOR remote_file_name");
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        cout << "Errore nel trasferimento FTP: " << curl_easy_strerror(res) << endl;
        return 1;
    }
    curl_easy_cleanup(curl);
    return 0;
}

int main() {
    // File da comprimere e nome del file ZIP
    const char* input_filename = "C:\\path\\to\\input\\file";
    const char* output_filename = "C:\\path\\to\\output\\file.zip";

    // Comprimi il file
    compressFile(input_filename, output_filename);

    // URL e credenziali per il server FTP
    const char* ftp_url = "ftp://example.com/path/to/remote/directory/"; 
    const char* ftp_userpwd = "username:password";

    // Nome del file ZIP
    const char* remote_file_name = "file.zip";

    // Trasferisci il file ZIP
    int result = transferFile(ftp_url, ftp_userpwd, output_filename, remote_file_name);
    if (result != 0) {
        cout << "Errore durante il trasferimento FTP" << endl;
        return 1;
    }

    cout << "File ZIP trasferito con successo" << endl;
    return 0;
}
