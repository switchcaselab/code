//
// Created by Switch Case on 5/13/2026.
//

#if defined(__SSE2__) \
|| defined(_M_X64) || defined(_M_AMD64) \
|| (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
#define HAS_SSE2 1
#endif

#include <benchmark/benchmark.h>

#include <cctype>
#include <chrono>
#include <iostream>
#include <string>
#include <locale>
#ifdef HAS_SSE2
#include <immintrin.h>
#endif


using namespace std;

const string input(
    "mEHogujvUDQWkHfobSdEfZkmWbjFHsKFdBeafwKoydYSDicqhBGkOfPsyXnNjhTYaboTtQhmPFiEtvVHCGtjfNMBZgevjGVHwewekcFMmPmFamRcvvWrFmmHuRROCRGpFZsnCsVvNZbaQjAlYjbSfOIGplnczRUoyHNYcztRStyqSdvdGkjNEEXOReDATiQtSVIiTRVkddPksnNjebIlDaorSBjzJBnSYCiMdxMeIIPTKpSBsQNFouPDfUnRMupzeTudnOQPMqwWDYYqywDwtYnNCAccZhtwCzcxBraXdYlIggQYllBrJpnVjUfjphRpkTwUzIGvEsYYpBKodXsFBIcGXLKVxPVdDSbHJwfNrvxIXarjDvFPBXiqLQXavJGvNYAouhffkwpoCWFOgOtHzRLPZxoZIYbVbBiiKYjOQIrXtFfWLWovijPgVyJSqVkHYLtntNwoIzwFrngcyYOntoRTwaAFJkjgmchIGDVgYWJKmjhJvRSKLIHfiIpEZaELKLlNzoowuMgqYLLrRdzwCsblRzUgpONTZxqxUTTSQEbLFqJhetCaPdCVIrsntqaPvXknABSFGrMNGnKGKdlEmVtPyTxzzTfetBMGDxasFOZqzagrNQeWUhcvBEbGESqZlRFhexIlHLFdDwkTWenZXUEeZmyGxsylsZbPMPuzfEDpTwomlEyMZPHljQygdaDtPrhyxJhXdhzrlrfNVDsNjEyUhJCtcPYGKBXUIMhsDgRptNbLNNmeUJfZCAsytewffQEMPyAzLdYTiUgVaOPQVSkWqfzCmHEwvHUVojENxFpcAjrfnwujnoeEAIKUZNyNffXHIqNzwoEZgsXwHMbTuHwhYdrzkWHaHNpYVBCqIgCdjGRRmMDXpkEHUNImNIxktufKkGEfFuNHWTEbnWtHCEyBHFTJfyoRzYlJJrsuFDGMBDgFKzXMudQxUnNeeMZQOPZkNNxjQbOeFKFFQvUNPuGJcKBGjbHVuTkMtFPfaabAYelIYxZDZbCMjlqxLjGcGZyqoRtJkTQxakMqpKbHlxVBPMqQZsBUEfDoDxhzrkGAvGHXtmHOEbNHedFSUylGXbyJkYAYTlDUKohgKcukrNYdgDIdAxzDqBWOjxxdfiOmmOdSEZEtLfweKQMfDIHLqsFcLSWzSkYFrdVBDHFLqAONaAqynrWPfisHLCGzRkvrukYjsDggLmudRaditLqWNkYkavfeSoFQTYelxBEEnipoAjeBetWJPJzuhqJaCClQuNNtFHVPYqpnNJaBHJhoUMbluOzkICxImuImLtMNllylXUzZFNWjrSjMVvjtmIZpbYhZldsrXRfXVSjHZMhSRnWwqyqKTnIOBnDQdEmjSOZmkRowpOzGCwlZoHbTJGytcMQuZtaLjRbbiKLxjpSqKFeWlNeEmLDUKbFDnxPjaFZKVvFtbhcdrusitVBgiyYUcwIVaMRUKOAzlotKfeCqKkCPFdvXRrdJUxfXAVSalyovhVboNGEIUDntqwrflnzBUxlmzjxCfFMfZBbRpjpziCxvDEHTblyFkXukWrwBKcILesDuQXmpvJbVyuxsuBUflrkqbfdImzpZDQrJiTKmhxrqVLwpDPoeGHhtJhEmmDiWNfTpBJodQTyuwStLEADxxxRDQPbfiaAVcggItirKGGAycpVATpvttlNZTaCGTlbKDaEgQKKjwBJUJBPYlJcrkdpLwLhZskGGJRuUxIJIDWPTnIClfsXNCGtgGIIuwEiBeWcvpvhMLdOIRajtDSwTLKBDKgEbSqYVEpMqLKBnHTpvVYBMaKsmARnGUMEGoUZDlQRcEEGUiVqwFSwqFOzqTQGTgzkBxJUQuIbWpSXRjUfBTBlGwdxopTFEsnsMuSxDWnwxyjrqeRFGbtMzlIEqSBTffhPgcVvrWGqVvLjxEodmQwZLfHkOQaLrOXciVmWIiDYhSVUZbBQZMqVKSvbIvHoSzlrJSSPIrTmujXopGVefIcBitANJTdYQtNyinQQLRxJdyIZsCGANeJcUgdtpOZUQAsaHQPpBJhCvlYfQItCFgHYEOMmkQqMkwZiDxDBTrKqoMLGBSfkpkAZDMjwwbkephdtpPQbhUumbUlLxYrUfhcbVBFbcvBdsRgMvNeUMsnuZDRvbBWTNUuIUSBjtNCwITvicvrVuzKHLNtrEYzhPjgCRumvLfatTCXPhDiyMbzlRWFCqkmfmSgPetFXfnNPfZFlfMrqVPmanxJAyUZZUxvkUknTmDJJaqdcpwMVHttXRxCYiROMKcWmSMjIuHWbMuTJREiDjENknbUGapWhbcNRyhOhSycHOHaKTixGpkDXwbRNzHedVjxZKCifwzThfONXozCrNzyyLUtMSumWOqvMVGQrtzSwskYINnZVMJNeehbvGyaRhKHNfoViNJKfwtBCsdEjbQpdaXdMBlcHpPuBfGEOLzPYZuswhtwmEcjnbvCCGGJJAFVqBePNrWiJJfMzOdajdBXtNRGpCeIIzwUcSxbHgmbIoPeERcyOxZQQSZFWniEOdiKrJAFXsuwhRVGWNAlhMawGvSycsPucHOAvNIWXXYwPpsHoSETTwHquhxjoFraiebWPtxHnasRcuLTKKmLJxKjNWpzuIMSPcATjAxrMENCHkOpPXOovxGFaegUGEiWXZgTCaWexjTYxDEKOyzleiPfTHJkwkQcUQjzQtGrrrcsoeqJxpWgbsdQSjEapfGXHHYNzNJLsZmuODTdyoEMdMOANuxRxNAFbcpTHZvRUsRfTGCCBAEfhzWHBipPjtdhJBdgoMnqwmtOOWTkpuKBpsoQCRhbsFLYzqBoYzLsQpPLtxQtJyqDsXCjUNLTFmdQwTskAvekGXbaAUXIzTKgqBiixYSCBqZmhInHhxpsnJHqCDiubIphjEmpobwVtmwPGTqkzjnVeyTwAPbOgSUhZuiksprjizGoZSNLdHTwKqqFzplShpuSqZdmveQoQBgOmUzplTilrqaTpIlLQchHjRctWzjGeSnvoZnzpDaZWSKWzqLrjDakFtYcKdhuGGXcSLBrivDCpLFtvJbjknoBwUOAUNTJBBMZGspFjnBOjxQOGJtahcvKwWOVuzrFUkOXyVgwcVFiTPsXjrhzopdghQXtYNolYftuVvLgBQrgkCUKDrlANJgNDbLqAbxNFJapcXnUVVAcDTaMeKtMbnDonhfinOEegxrQUCsCsHALxTTSWxrRKuxOgpoSNcNduFNvsOgKsNWGLNIpKSbOtNuaGXGdpTOpsNIpqTUOaNvwmlioPNUTBLRYKyFQfprewKGHYOHhdFnNqgLRHeBgwctuGlAwDbUooHvESxrvDCmWbByNTpBNBJmQjwrsopNhYCHdXFWDTTTHMnmzYwLuGgbtmSMmAzMtLPokjzmNQlLezoIhDQHFyCwCeudWlhCPlIztlGRPXoLUweUeRKIxvwVPlXioZqvigsnFWesFUmqASYBiYiYLEjaWFoFvPRUEGnRsuSVMtzJCXcfjsniSoxUiiGjiXnjaGkUinCdYVylZaYKsUqjNofbhCwUQuhgDUOYXPkwTCZOfBlfUNLIVfCOMhtHAidpMPJdHrfawzHRTiiVEluZNZaZyPSyYEePSIaTAWKdnuoXoLFwtkVJrDPOpNftXTMoCfqUTbSVtPrNNQwhCIAPUNAIXAYsHyTDozKtqlTvUwBhAgeKoxcDbRYSGqrnIzFuiPNJuSQiXTVkDBvFTXaymJNLUWzVHwbKiiXOmJpGsaJzBvhnHWswIpmRjlxarqwRvdFLOXIBmSCQutEKLEfWGnHLVhgEWVRLybMypZCWhDVZTKwKzMDJYnifoxmWTMLTGMmbrHsZIBylWYgPMIFzgrYKXnVDetfrkhxADvTmhLfcJdsNwELACIAnorloPqcOCWhDwddRIfUwctoeBJeDRfRrxKAcREuiQDnkVMgtbOjvVIGvmpzxYfFAPTFRwqnMmRYAUOWwEURDgGowuUbhnipacqzYSyqfWAjnkXVDpNaDBoMHANXYnggTskXNIrhFNkFkQpaZgoGcqGWJdSOjQaUNndjgcHSKwOPnrSVoOhtkhFWKKpwDZlcWOMzoQVqWpld");

// const string input(4096, 'Z');

inline void ToLower_std(string& s)
{
    for (char& c : s)
    {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
}

static void BM_ToLower_std(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::string s = input;
        ToLower_std(s);
        benchmark::DoNotOptimize(s);
    }
    state.SetBytesProcessed(state.iterations() * static_cast<int64_t>(input.size()));
}

BENCHMARK(BM_ToLower_std);

inline void ToLower_Dumbest(string& s)
{
    for (char& c : s)
    {
        if (c == 'A') c = 'a';
        else if (c == 'B') c = 'b';
        else if (c == 'C') c = 'c';
        else if (c == 'D') c = 'd';
        else if (c == 'E') c = 'e';
        else if (c == 'F') c = 'f';
        else if (c == 'G') c = 'g';
        else if (c == 'H') c = 'h';
        else if (c == 'I') c = 'i';
        else if (c == 'J') c = 'j';
        else if (c == 'K') c = 'k';
        else if (c == 'L') c = 'l';
        else if (c == 'M') c = 'm';
        else if (c == 'N') c = 'n';
        else if (c == 'O') c = 'o';
        else if (c == 'P') c = 'p';
        else if (c == 'Q') c = 'q';
        else if (c == 'R') c = 'r';
        else if (c == 'S') c = 's';
        else if (c == 'T') c = 't';
        else if (c == 'U') c = 'u';
        else if (c == 'V') c = 'v';
        else if (c == 'W') c = 'w';
        else if (c == 'X') c = 'x';
        else if (c == 'Y') c = 'y';
        else if (c == 'Z') c = 'z';
    }
}

static void BM_ToLower_Dumbest(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::string s = input;
        ToLower_Dumbest(s);
        benchmark::DoNotOptimize(s);
    }
    state.SetBytesProcessed(state.iterations() * static_cast<int64_t>(input.size()));
}

BENCHMARK(BM_ToLower_Dumbest);

inline void ToLower_Dumb(string& s)
{
    for (char& c : s)
    {
        switch (c)
        {
        case 'A': c = 'a';
            break;
        case 'B': c = 'b';
            break;
        case 'C': c = 'c';
            break;
        case 'D': c = 'd';
            break;
        case 'E': c = 'e';
            break;
        case 'F': c = 'f';
            break;
        case 'G': c = 'g';
            break;
        case 'H': c = 'h';
            break;
        case 'I': c = 'i';
            break;
        case 'J': c = 'j';
            break;
        case 'K': c = 'k';
            break;
        case 'L': c = 'l';
            break;
        case 'M': c = 'm';
            break;
        case 'N': c = 'n';
            break;
        case 'O': c = 'o';
            break;
        case 'P': c = 'p';
            break;
        case 'Q': c = 'q';
            break;
        case 'R': c = 'r';
            break;
        case 'S': c = 's';
            break;
        case 'T': c = 't';
            break;
        case 'U': c = 'u';
            break;
        case 'V': c = 'v';
            break;
        case 'W': c = 'w';
            break;
        case 'X': c = 'x';
            break;
        case 'Y': c = 'y';
            break;
        case 'Z': c = 'z';
            break;
        }
    }
}

static void BM_ToLower_Dumb(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::string s = input;
        ToLower_Dumb(s);
        benchmark::DoNotOptimize(s);
    }
    state.SetBytesProcessed(state.iterations() * static_cast<int64_t>(input.size()));
}

BENCHMARK(BM_ToLower_Dumb);

inline void ToLower_Normal(string& s)
{
    for (char& c : s)
    {
        if (c >= 'A' && c <= 'Z')
            c = static_cast<char>(c - 'A' + 'a');
    }
}

static void BM_ToLower_Normal(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::string s = input;
        ToLower_Normal(s);
        benchmark::DoNotOptimize(s);
    }
    state.SetBytesProcessed(state.iterations() * static_cast<int64_t>(input.size()));
}

BENCHMARK(BM_ToLower_Normal);

inline void ToLower_Clever_partial(string& s)
{
    constexpr auto mask = 0x20;
    for (auto& c : s)
    {
        c |= mask;
    }
}

static void BM_ToLower_Clever_partial(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::string s = input;
        ToLower_Clever_partial(s);
        benchmark::DoNotOptimize(s);
    }
    state.SetBytesProcessed(state.iterations() * static_cast<int64_t>(input.size()));
}

BENCHMARK(BM_ToLower_Clever_partial);

inline void ToLower_Clever(string& s)
{
    constexpr auto GREATER_EQUAL_A = 0x80 - 'A';
    constexpr auto GREATER_THAN_Z = 0x80 - ('Z' + 1);
    constexpr auto HIGHEST_BIT_MASK = 0x80;
    for (auto& c : s)
    {
        const auto lowSevenBits = c & ~HIGHEST_BIT_MASK;
        const auto highestBit = c & HIGHEST_BIT_MASK;
        const auto ge_A = lowSevenBits + GREATER_EQUAL_A;
        const auto gt_Z = lowSevenBits + GREATER_THAN_Z;
        const auto isUpper = ge_A & ~gt_Z & HIGHEST_BIT_MASK & ~highestBit;
        c |= static_cast<char>(isUpper >> 2);
    }
}

static void BM_ToLower_Clever(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::string s = input;
        ToLower_Clever(s);
        benchmark::DoNotOptimize(s);
    }
    state.SetBytesProcessed(state.iterations() * static_cast<int64_t>(input.size()));
}

BENCHMARK(BM_ToLower_Clever);

inline void ToLower_SWAR_UB(string& s)
{
    constexpr uint64_t GREATER_EQUAL_A = 0x0101010101010101ULL * (0x80 - 'A');
    constexpr uint64_t GREATER_THAN_Z = 0x0101010101010101ULL * (0x80 - ('Z' + 1));
    constexpr uint64_t HIGHEST_BIT_MASK = 0x8080808080808080ULL;

    uint64_t index = 0;
    char* data = s.data();
    while (index < s.size())
    {
        auto chunk = *reinterpret_cast<uint64_t*>(data + index);
        const uint64_t lowSevenBits = chunk & ~HIGHEST_BIT_MASK;
        const uint64_t highestBit = chunk & HIGHEST_BIT_MASK;

        const uint64_t ge_A = lowSevenBits + GREATER_EQUAL_A;
        const uint64_t gt_Z = lowSevenBits + GREATER_THAN_Z;
        const uint64_t isUpper = ge_A & ~gt_Z & HIGHEST_BIT_MASK & ~highestBit;

        chunk |= (isUpper >> 2);
        *reinterpret_cast<uint64_t*>(data + index) = chunk;
        index += 8;
    }
}

static void BM_ToLower_SWAR_UB(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::string s = input;
        ToLower_SWAR_UB(s);
        benchmark::DoNotOptimize(s);
    }
    state.SetBytesProcessed(state.iterations() * static_cast<int64_t>(input.size()));
}

BENCHMARK(BM_ToLower_SWAR_UB);

inline void ToLower_SWAR(string& s)
{
    constexpr uint64_t GREATER_EQUAL_A = 0x0101010101010101ULL * (0x80 - 'A');
    constexpr uint64_t GREATER_THAN_Z = 0x0101010101010101ULL * (0x80 - ('Z' + 1));
    constexpr uint64_t HIGHEST_BIT_MASK = 0x8080808080808080ULL;

    uint64_t index = 0;
    char* data = s.data();
    while (index < s.size())
    {
        uint64_t chunk;
        memcpy(&chunk, data + index, 8);

        const uint64_t lowSevenBits = chunk & ~HIGHEST_BIT_MASK;
        const uint64_t highestBit = chunk & HIGHEST_BIT_MASK;

        const uint64_t ge_A = lowSevenBits + GREATER_EQUAL_A;
        const uint64_t gt_Z = lowSevenBits + GREATER_THAN_Z;
        const uint64_t isUpper = ge_A & ~gt_Z & HIGHEST_BIT_MASK & ~highestBit;

        chunk |= (isUpper >> 2);
        memcpy(data + index, &chunk, 8);
        index += 8;
    }
}

static void BM_ToLower_SWAR(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::string s = input;
        ToLower_SWAR(s);
        benchmark::DoNotOptimize(s);
    }
    state.SetBytesProcessed(state.iterations() * static_cast<int64_t>(input.size()));
}

BENCHMARK(BM_ToLower_SWAR);

#ifdef HAS_SSE2
inline void ToLower_SSE2(string& s)
{
    const auto SSE_GREATER_EQUAL_A = _mm_set1_epi8(0x80 - 'A');
    const auto SSE_GREATER_THAN_Z = _mm_set1_epi8(0x80 - ('Z' + 1));
    const auto SSE_HIGHEST_BIT_MASK = _mm_set1_epi8(static_cast<char>(0x80));

    uint64_t index = 0;
    char* data = s.data();
    while (index < s.size())
    {
        auto chunk = _mm_loadu_si128(reinterpret_cast<__m128i*>(data + index));
        const auto lowSevenBits = _mm_andnot_si128(SSE_HIGHEST_BIT_MASK, chunk);
        const auto highestBit = _mm_and_si128(chunk, SSE_HIGHEST_BIT_MASK);

        const auto ge_A = _mm_add_epi8(lowSevenBits, SSE_GREATER_EQUAL_A);
        const auto gt_Z = _mm_add_epi8(lowSevenBits, SSE_GREATER_THAN_Z);
        const auto isUpper = _mm_and_si128(_mm_andnot_si128(gt_Z, ge_A),
                                           _mm_andnot_si128(highestBit, SSE_HIGHEST_BIT_MASK));
        chunk = _mm_or_si128(chunk, _mm_srli_epi16(isUpper, 2));
        _mm_storeu_si128(reinterpret_cast<__m128i*>(data + index), chunk);
        index += 16;
    }
}

static void BM_ToLower_SSE2(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::string s = input;
        ToLower_SSE2(s);
        benchmark::DoNotOptimize(s);
    }
    state.SetBytesProcessed(state.iterations() * static_cast<int64_t>(input.size()));
}

BENCHMARK(BM_ToLower_SSE2);
#endif

inline void ToUpper_SWAR(string& s)
{
    constexpr uint64_t GREATER_EQUAL_a = 0x0101010101010101ULL * (0x80 - 'a');
    constexpr uint64_t GREATER_THAN_z = 0x0101010101010101ULL * (0x80 - ('z' + 1));
    constexpr uint64_t HIGHEST_BIT_MASK = 0x8080808080808080ULL;

    uint64_t index = 0;
    char* data = s.data();
    while (index < s.size())
    {
        uint64_t chunk;
        memcpy(&chunk, data + index, 8);

        const uint64_t lowSevenBits = chunk & ~HIGHEST_BIT_MASK;
        const uint64_t highestBit = chunk & HIGHEST_BIT_MASK;

        const uint64_t ge_A = lowSevenBits + GREATER_EQUAL_a;
        const uint64_t gt_Z = lowSevenBits + GREATER_THAN_z;
        const uint64_t isLower = ge_A & ~gt_Z & HIGHEST_BIT_MASK & ~highestBit;

        chunk ^= (isLower >> 2);
        memcpy(data + index, &chunk, 8);
        index += 8;
    }
}

static void BM_ToUpper_SWAR(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::string s = input;
        ToUpper_SWAR(s);
        benchmark::DoNotOptimize(s);
    }
    state.SetBytesProcessed(state.iterations() * static_cast<int64_t>(input.size()));
}

BENCHMARK(BM_ToUpper_SWAR);

#ifdef HAS_SSE2
inline void ToUpper_SSE2(string& s)
{
    const auto SSE_GREATER_EQUAL_a = _mm_set1_epi8(0x80 - 'a');
    const auto SSE_GREATER_THAN_z = _mm_set1_epi8(0x80 - ('z' + 1));
    const auto SSE_HIGHEST_BIT_MASK = _mm_set1_epi8(static_cast<char>(0x80));

    uint64_t index = 0;
    char* data = s.data();
    while (index < s.size())
    {
        auto chunk = _mm_loadu_si128(reinterpret_cast<__m128i*>(data + index));
        const auto lowSevenBits = _mm_andnot_si128(SSE_HIGHEST_BIT_MASK, chunk);
        const auto highestBit = _mm_and_si128(chunk, SSE_HIGHEST_BIT_MASK);

        const auto ge_A = _mm_add_epi8(lowSevenBits, SSE_GREATER_EQUAL_a);
        const auto gt_Z = _mm_add_epi8(lowSevenBits, SSE_GREATER_THAN_z);
        const auto isLower = _mm_and_si128(_mm_andnot_si128(gt_Z, ge_A),
                                           _mm_andnot_si128(highestBit, SSE_HIGHEST_BIT_MASK));
        chunk = _mm_xor_si128(chunk, _mm_srli_epi16(isLower, 2));
        _mm_storeu_si128(reinterpret_cast<__m128i*>(data + index), chunk);
        index += 16;
    }
}

static void BM_ToUpper_SSE2(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::string s = input;
        ToUpper_SSE2(s);
        benchmark::DoNotOptimize(s);
    }
    state.SetBytesProcessed(state.iterations() * static_cast<int64_t>(input.size()));
}

BENCHMARK(BM_ToUpper_SSE2);
#endif

inline void ToLower_ASCII_Compiler_Specific(string& s)
{
    for (char& c : s)
    {
#ifdef _MSC_VER
        c = static_cast<char>(__ascii_tolower(static_cast<unsigned char>(c)));
#else
        auto u = static_cast<unsigned char>(c);
        c = (char)(u | (((unsigned)(u - 'A') < 26u) << 5)); //_tolower
#endif
    }
}

static void BM_ToLower_ASCII_Compiler_Specific(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::string s = input;
        ToLower_ASCII_Compiler_Specific(s);
        benchmark::DoNotOptimize(s);
    }
    state.SetBytesProcessed(state.iterations() * static_cast<int64_t>(input.size()));
}

BENCHMARK(BM_ToLower_ASCII_Compiler_Specific);

BENCHMARK_MAIN();