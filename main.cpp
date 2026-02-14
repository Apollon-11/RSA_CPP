#include <iostream>
#include <fstream>
#include "decrypt.h"
#include "mathrsa.h"
#include "encrypt.h"
#include "key_gen.h"
#include <string> 
#include "hex.h"
#include "bigint.h"
#include <filesystem>
using namespace std;


int main()
{
    std::cout << "Select: decrypt (1) or encrypt (2): ";
    int t;
    std::cin >> t;
    switch (t) {
    case 1: {
        // read key file (default file name in working directory)
        std::string path = "key.txt";
        std::ifstream fin(path);
        BigInt a[3];

        if (!fin.is_open()) {
            std::cerr << "Error: key.txt not found in working directory" << std::endl;
            break;
        }
        int i = 0;
        while (i < 3 && fin >> a[i]) i++;
        fin.close();
        if (i < 3) {
            std::cerr << "Error: key.txt must contain three numbers (p q e)" << std::endl;
            break;
        }
        BigInt secret_key = Key_Gen(a[0], a[1], a[2]);
        BigInt prime_multiplication = a[0] * a[1];

        std::cin.ignore();
        std::cout << "Enter encrypted filename: ";
        std::string path3;
        std::cin >> path3;
        std::ifstream fon(path3);
        if (!fon.is_open()) {
            std::cerr << "Error: cannot open " << path3 << std::endl;
            break;
        }
        std::vector<BigInt> crypts;
        BigInt tmp;
        while (fon >> tmp) crypts.push_back(tmp);
        fon.close();
        if (crypts.empty()) {
            std::cerr << "Error: no data in " << path3 << std::endl;
            break;
        }
        std::string result_text;
        for (const BigInt &c : crypts) {
            BigInt m = decrypt_crt(c, secret_key, a[0], a[1]);
            // verify by re-encrypting, fallback if mismatch
            BigInt re = crypt(m, a[2], prime_multiplication);
            if (re != c) m = decrypt(c, secret_key, prime_multiplication);
            result_text += number_to_text(m);
        }
        std::cout << result_text << std::endl;
    }
    break;
	case 2:
	{
        std::cout << "Enter first packet filename: ";
        std::string path4;
        std::cin >> path4;
        std::ifstream fok(path4);
        BigInt b[2];
        if (!fok.is_open()) {
            std::cerr << "Error: cannot open " << path4 << std::endl;
            break;
        }
        int idx = 0;
        while (idx < 2 && fok >> b[idx]) idx++;
        fok.close();
        if (idx < 2) {
            std::cerr << "Error: " << path4 << " must contain 2 numbers (e n)" << std::endl;
            break;
        }
        std::cin.ignore();
        std::cout << "Enter message: ";
        std::string mes;
        std::getline(std::cin, mes);
        // determine block size (max bytes per block so that 256^k < n)
        BigInt n = b[1];
        BigInt limit = BigInt(1);
        int max_bytes = 0;
        BigInt base256 = BigInt(256);
        while ((limit * base256) < n) {
            limit = limit * base256;
            max_bytes++;
        }
        if (max_bytes == 0) {
            std::cerr << "Error: modulus too small for any byte of message" << std::endl;
            break;
        }
        // split message into chunks of max_bytes and ensure block < n
        std::vector<BigInt> encrypted_blocks;
        for (size_t pos = 0; pos < mes.size(); pos += max_bytes) {
            size_t len = std::min((size_t)max_bytes, mes.size() - pos);
            string chunk = mes.substr(pos, len);
            BigInt block = text_to_number(chunk);
            // if block >= n, reduce chunk until block < n
            while (!(block < n)) {
                len--;
                chunk = mes.substr(pos, len);
                block = text_to_number(chunk);
            }
            BigInt enc = crypt(block, b[0], b[1]);
            encrypted_blocks.push_back(enc);
        }
        // write blocks to file, one per line
        std::string pathOut = "second_packet.txt";
        std::ofstream fock(pathOut);
        if (!fock.is_open()) {
            std::cerr << "Error: cannot create " << pathOut << std::endl;
        } else {
            for (const BigInt &eb : encrypted_blocks) fock << eb << std::endl;
        }
        fock.close();
	}
	break;
	}
}
