#include <cstdint>
#include <array>
#include <bit>

namespace xoshiro
{
    enum class RngType
    {
        XoshiroPlusPlus,
        XoshiroStarStar,
        XoshiroPlus,
        XoroshiroPlusPlus,
        XoroshiroStarStar,
        XoroshiroPlus,
        XoroshiroStar,
    };

    template <class out_type, int state_bits, RngType rng_type>
    class random_engine
    {
    public:
        using result_type = out_type;
        using state_type = std::array<result_type, state_bits / 8 / sizeof(result_type)>;

        static constexpr result_type min() noexcept
        {
            return std::numeric_limits<result_type>::lowest();
        }

        static constexpr result_type max() noexcept
        {
            return std::numeric_limits<result_type>::max();
        }

        constexpr random_engine(uint64_t seed)
        {
            for (int i = 0; i < s.size(); ++i)
            {
                seed = splitmix64(seed);
                s[i] = seed;
            }
            next();
            next();
        }

        constexpr random_engine(uint64_t seed1, uint64_t seed2)
        {
            // is there a better way to seed with two 64-bit ints?
            for (int i = 0; i < s.size(); i += 2)
            {
                seed1 = splitmix64(seed1);
                seed2 = splitmix64(seed2);
                s[i] = seed1;
                s[i + 1] = seed2;
            }
            next();
            next();
        }

        constexpr random_engine(state_type newState)
        {
            s = newState;
        }

        // default constructor provided only for deserialization
        constexpr random_engine()
        {
        }

        // serialize
        constexpr state_type state() const
        {
            return s;
        }

        // deserialize
        constexpr void state(state_type newState)
        {
            s = newState;
        }

        constexpr result_type operator()() noexcept
        {
            return next();
        }

        constexpr result_type next() noexcept;

    private:
        state_type s;

        static constexpr uint64_t splitmix64(uint64_t x) noexcept
        {
            uint64_t z = (x += 0x9e3779b97f4a7c15);
            z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
            z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
            return z ^ (z >> 31);
        }
    };

    // xoshiro256++ | xoshiro256plusplus.c
    template <>
    constexpr uint64_t random_engine<uint64_t, 256, RngType::XoshiroPlusPlus>::next() noexcept
    {
        const uint64_t result = std::rotl(s[0] + s[3], 23) + s[0];

        const uint64_t t = s[1] << 17;

        s[2] ^= s[0];
        s[3] ^= s[1];
        s[1] ^= s[2];
        s[0] ^= s[3];

        s[2] ^= t;

        s[3] = std::rotl(s[3], 45);

        return result;
    }

    // xoshiro256** | xoshiro256starstar.c
    template <>
    constexpr uint64_t random_engine<uint64_t, 256, RngType::XoshiroStarStar>::next() noexcept
    {
        const uint64_t result = std::rotl(s[1] * 5, 7) * 9;

        const uint64_t t = s[1] << 17;

        s[2] ^= s[0];
        s[3] ^= s[1];
        s[1] ^= s[2];
        s[0] ^= s[3];

        s[2] ^= t;

        s[3] = std::rotl(s[3], 45);

        return result;
    }

    // xoshiro256+ | xoshiro256plus.c
    template <>
    constexpr uint64_t random_engine<uint64_t, 256, RngType::XoshiroPlus>::next() noexcept
    {
        const uint64_t result = s[0] + s[3];

        const uint64_t t = s[1] << 17;

        s[2] ^= s[0];
        s[3] ^= s[1];
        s[1] ^= s[2];
        s[0] ^= s[3];

        s[2] ^= t;

        s[3] = std::rotl(s[3], 45);

        return result;
    }

    // xoroshiro128++ | xoroshiro128plusplus.c
    template <>
    constexpr uint64_t random_engine<uint64_t, 128, RngType::XoroshiroPlusPlus>::next() noexcept
    {
        const uint64_t s0 = s[0];
        uint64_t s1 = s[1];
        const uint64_t result = std::rotl(s0 + s1, 17) + s0;

        s1 ^= s0;
        s[0] = std::rotl(s0, 49) ^ s1 ^ (s1 << 21);
        s[1] = std::rotl(s1, 28);

        return result;
    }

    // xoroshiro128** | xoroshiro128starstar.c
    template <>
    constexpr uint64_t random_engine<uint64_t, 128, RngType::XoroshiroStarStar>::next() noexcept
    {
        const uint64_t s0 = s[0];
        uint64_t s1 = s[1];
        const uint64_t result = std::rotl(s0 * 5, 7) * 9;

        s1 ^= s0;
        s[0] = std::rotl(s0, 24) ^ s1 ^ (s1 << 16);
        s[1] = std::rotl(s1, 37);

        return result;
    }

    // xoroshiro128+ | xoroshiro128plus.c
    template <>
    constexpr uint64_t random_engine<uint64_t, 128, RngType::XoroshiroPlus>::next() noexcept
    {
        const uint64_t s0 = s[0];
        uint64_t s1 = s[1];
        const uint64_t result = s0 + s1;

        s1 ^= s0;
        s[0] = std::rotl(s0, 24) ^ s1 ^ (s1 << 16);
        s[1] = std::rotl(s1, 37);

        return result;
    }

    // xoshiro128++ | xoshiro128plusplus.c
    template <>
    constexpr uint32_t random_engine<uint32_t, 128, RngType::XoshiroPlusPlus>::next() noexcept
    {
        const uint32_t result = std::rotl(s[0] + s[3], 7) + s[0];

        const uint32_t t = s[1] << 9;

        s[2] ^= s[0];
        s[3] ^= s[1];
        s[1] ^= s[2];
        s[0] ^= s[3];

        s[2] ^= t;

        s[3] = std::rotl(s[3], 11);

        return result;
    }

    // xoshiro128** | xoshiro128starstar.c
    template <>
    constexpr uint32_t random_engine<uint32_t, 128, RngType::XoshiroStarStar>::next() noexcept
    {
        const uint32_t result = std::rotl(s[1] * 5, 7) * 9;

        const uint32_t t = s[1] << 9;

        s[2] ^= s[0];
        s[3] ^= s[1];
        s[1] ^= s[2];
        s[0] ^= s[3];

        s[2] ^= t;

        s[3] = std::rotl(s[3], 11);

        return result;
    }

    // xoshiro128+ | xoshiro128plus.c
    template <>
    constexpr uint32_t random_engine<uint32_t, 128, RngType::XoshiroPlus>::next() noexcept
    {
        const uint32_t result = s[0] + s[3];

        const uint32_t t = s[1] << 9;

        s[2] ^= s[0];
        s[3] ^= s[1];
        s[1] ^= s[2];
        s[0] ^= s[3];

        s[2] ^= t;

        s[3] = std::rotl(s[3], 11);

        return result;
    }

    // xoroshiro64** | xoroshiro64starstar.c
    template <>
    constexpr uint32_t random_engine<uint32_t, 64, RngType::XoroshiroStarStar>::next() noexcept
    {
        const uint32_t s0 = s[0];
        uint32_t s1 = s[1];
        const uint32_t result = std::rotl(s0 * 0x9E3779BB, 5) * 5;

        s1 ^= s0;
        s[0] = std::rotl(s0, 26) ^ s1 ^ (s1 << 9);
        s[1] = std::rotl(s1, 13);

        return result;
    }

    // xoroshiro64* | xoroshiro64star.c
    template <>
    constexpr uint32_t random_engine<uint32_t, 64, RngType::XoroshiroStar>::next() noexcept
    {
        const uint32_t s0 = s[0];
        uint32_t s1 = s[1];
        const uint32_t result = s0 * 0x9E3779BB;

        s1 ^= s0;
        s[0] = std::rotl(s0, 26) ^ s1 ^ (s1 << 9);
        s[1] = std::rotl(s1, 13);

        return result;
    }

    // 64-bit
    using xoshiro256pp = random_engine<uint64_t, 256, RngType::XoshiroPlusPlus>;     // xoshiro256++
    using xoshiro256ss = random_engine<uint64_t, 256, RngType::XoshiroStarStar>;     // xoshiro256**
    using xoshiro256p = random_engine<uint64_t, 256, RngType::XoshiroPlus>;          // xoshiro256+
    using xoroshiro128pp = random_engine<uint64_t, 128, RngType::XoroshiroPlusPlus>; // xoroshiro128++
    using xoroshiro128ss = random_engine<uint64_t, 128, RngType::XoroshiroStarStar>; // xoroshiro128**
    using xoroshiro128p = random_engine<uint64_t, 128, RngType::XoroshiroPlus>;      // xoroshiro128+

    // 32-bit
    using xoshiro128pp = random_engine<uint32_t, 128, RngType::XoshiroPlusPlus>;   // xoshiro128++
    using xoshiro128ss = random_engine<uint32_t, 128, RngType::XoshiroStarStar>;   // xoshiro128**
    using xoshiro128p = random_engine<uint32_t, 128, RngType::XoshiroPlus>;        // xoshiro128+
    using xoroshiro64ss = random_engine<uint32_t, 64, RngType::XoroshiroStarStar>; // xoroshiro64**
    using xoroshiro64s = random_engine<uint32_t, 64, RngType::XoroshiroStar>;      // xoroshiro64*

}; // namespace xoshiro
