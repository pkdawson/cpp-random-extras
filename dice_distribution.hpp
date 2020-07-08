#include <string_view>
#include <stdexcept>
#include <charconv>

template <class int_type = int, class dist_type = std::uniform_int_distribution<int_type>>
class dice_distribution
{
public:
    dice_distribution(std::string_view expression)
    {
        auto idxd = expression.find('d');
        if (idxd == std::string_view::npos)
        {
            throw std::invalid_argument("no dice");
        }

        std::string_view svDice = expression.substr(0, idxd);

        auto idxSides = idxd + 1;
        auto idxSidesEnd = expression.find_first_not_of("1234567890", idxSides + 1);
        std::string_view svSides = expression.substr(idxSides, idxSidesEnd - idxSides);

        std::from_chars(svDice.data(), svDice.data() + svDice.size(), dice);
        std::from_chars(svSides.data(), svSides.data() + svSides.size(), sides);

        auto idxop = expression.find_first_of("+-");
        if (idxop != std::string_view::npos)
        {
            auto idxMod = expression.find_first_not_of(" ", idxop + 1);
            std::from_chars(expression.data() + idxMod, expression.data() + expression.size(), mod);
            if (expression[idxop] == '-')
                mod *= -1;
        }
        else
        {
            mod = 0;
        }
    }

    dice_distribution(int_type dice, int_type sides, int_type mod = 0) : dice(dice), sides(sides), mod(mod)
    {
    }

    template <class TEngine>
    int_type operator()(TEngine &eng) const
    {
        dist_type dist(1, sides);
        int_type sum = 0;

        for (int i = 0; i < dice; ++i)
        {
            sum += dist(eng);
        }
        return sum + mod;
    }

    int_type min() const
    {
        return dice + mod;
    }

    int_type max() const
    {
        return dice * sides + mod;
    }

private:
    int_type dice;
    int_type sides;
    int_type mod;
};
