#include "mathrsa.h"
#include "bigint.h"

math drsa;

// Standard decrypt (fallback)
BigInt decrypt(const BigInt& crypt_message, const BigInt& secret_key, const BigInt& prime_multiplication){
    return drsa.pow_mod(crypt_message, secret_key, prime_multiplication);
}

// CRT-based decrypt: needs p and q (prime factors of n)
BigInt decrypt_crt(const BigInt& c, const BigInt& d, const BigInt& p, const BigInt& q) {
    // compute m1 = c^{d mod (p-1)} mod p
    BigInt dp = d % (p - BigInt(1));
    BigInt dq = d % (q - BigInt(1));
    BigInt m1 = drsa.pow_mod(c % p, dp, p);
    BigInt m2 = drsa.pow_mod(c % q, dq, q);
    // compute qinv = q^{-1} mod p
    BigInt qinv = drsa.inverse(q, p);
    if (qinv == BigInt(0)) {
        // fallback
        return drsa.pow_mod(c, d, p * q);
    }
    // normalize qinv to [0, p-1]
    qinv = qinv % p;
    if (qinv < BigInt(0)) qinv = qinv + p;
    BigInt h1 = ((m1 - m2) * qinv) % p;
    if (h1 < BigInt(0)) h1 = h1 + p;
    BigInt m1cand = m2 + h1 * q;

    BigInt n = p * q;
    m1cand = m1cand % n;
    if (m1cand < BigInt(0)) m1cand = m1cand + n;

    // alternative recombination using p inverse mod q
    BigInt pinv = drsa.inverse(p, q);
    BigInt m2cand = m1; // default
    if (!(pinv == BigInt(0))) {
        pinv = pinv % q;
        if (pinv < BigInt(0)) pinv = pinv + q;
        BigInt h2 = ((m2 - m1) * pinv) % q;
        if (h2 < BigInt(0)) h2 = h2 + q;
        m2cand = m1 + h2 * p;
        m2cand = m2cand % n;
        if (m2cand < BigInt(0)) m2cand = m2cand + n;
    }

    // verify candidates by checking residues
    if ((m1cand % p) == m1 && (m1cand % q) == m2) return m1cand;
    if ((m2cand % p) == m1 && (m2cand % q) == m2) return m2cand;

    // fallback to direct pow_mod
    return drsa.pow_mod(c, d, n);
}
