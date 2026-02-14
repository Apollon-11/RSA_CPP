RSA (educational implementation)

This repository contains an educational implementation of RSA in C++ with a custom `BigInt` type and simple file-based IO. The goal of the project is autonomy and learning - it is not intended for production use.

Key points
- Custom `BigInt` implementation (base 1e9 digits).
- RSA encrypt/decrypt with block processing (message split into blocks smaller than modulus `n`).
- CRT optimization for decryption.

Timeline
- Initial development: Feb - May 2024
- Reworked and completed: Feb 2026

Usage
1. Put your key parameters into `key.txt` as three numbers: `p` `q` `e` (decimal) - one per line.
2. Run the program and choose mode `2` (encrypt) - provide the `first_packet.txt` when asked or leave default. The program will write `second_packet.txt` with encrypted blocks (one block per line).
3. Choose mode `1` (decrypt) and provide the `second_packet.txt` file - the program will output the decrypted message.

Limitations & Warnings
- This implementation is for educational/demonstration purposes only. Do NOT use it for production cryptography.
- No padding (OAEP / PKCS#1) is implemented - insecure for real use.
- The custom `BigInt` is not optimized for performance; large keys (e.g., 2048-bit) will be slow.

If you want improved performance, consider integrating a well-tested big integer library (e.g., GMP or Boost.Multiprecision). The project is intentionally self-contained.
