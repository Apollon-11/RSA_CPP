<h1>RSA (educational implementation)</h1>

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

<h1>RSA (учебная реализация)</h1>

Этот репозиторий содержит учебную реализацию RSA на C++ с пользовательским типом `BigInt` и простым файловым вводом-выводом. Цель проекта — автономность и обучение — он не предназначен для использования в производственных целях.

Ключевые моменты:
- Пользовательская реализация `BigInt` (основание 1e9 цифр).

- Шифрование/дешифрование RSA с блочной обработкой (сообщение разбивается на блоки меньше модуля `n`).

- Оптимизация CRT для дешифрования.

Сроки разработки:
- Начальная разработка: февраль - май 2024 г.
- Переработан и завершен: февраль 2026 г.

Использование:
1. Введите параметры ключа в файл `key.txt` в виде трех чисел: `p`, `q`, `e` (десятичные) — по одному на строку.

2. Запустите программу и выберите режим `2` (шифрование) — укажите файл `first_packet.txt`, когда это потребуется, или оставьте значение по умолчанию. Программа запишет файл `second_packet.txt` с зашифрованными блоками (один блок на строку).

3. Выберите режим `1` (расшифровка) и укажите файл `second_packet.txt` — программа выведет расшифрованное сообщение.

Ограничения и предупреждения
- Эта реализация предназначена только для образовательных/демонстрационных целей. НЕ используйте её для криптографии в реальных условиях.

- Заполнение (OAEP / PKCS#1) не реализовано — небезопасно для реального использования.

- Пользовательский `BigInt` не оптимизирован для повышения производительности; большие ключи (например, 2048-битные) будут работать медленно.

Если вы хотите повысить производительность, рассмотрите возможность интеграции хорошо протестированной библиотеки для работы с большими целыми числами (например, GMP или Boost.Multiprecision). Проект намеренно является самодостаточным.
