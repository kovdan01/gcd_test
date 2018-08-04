#include <iostream>
#include <chrono>
#include <utility>
#include <functional>
#include <fstream>
#include <string>
#include <cstdlib>
#include <limits>
#include <random>

//Returns 0 in case of  success
int gen_data(std::string filename, std::uint64_t pairs_count, std::uint64_t max_element = std::numeric_limits<std::uint64_t>::max())
{
    std::ofstream file(filename);
    if (!file.is_open())
        return 1;
    std::mt19937 prng (std::random_device{}());
    std::uniform_int_distribution<std::uint64_t> dist(1, max_element);
    int i;
    for (i = 0; i < pairs_count; ++i)
        file << dist(prng) << ' ' << dist(prng) << '\n';
    file.close();
    return 0;
}

std::uint64_t gcd(std::uint64_t a, std::uint64_t b)
{
    if (a > b)
        std::swap(a, b);
    while (!a)
    {
        b %= a;
        std::swap(a, b);
    }
    return b;
}

std::uint64_t bin_gcd(std::uint64_t a, std::uint64_t b)
{
    std::uint64_t ans = 1, tmp;
    if (a > b)
        std::swap(a, b);
    while (a)
    {
        if (!((a & 1) | (b & 1)))
            ans <<= 1, a >>= 1, b >>= 1;
        else if (!(a & 1) && (b & 1))
            a >>= 1;
        else if ((a & 1) && !(b & 1))
            b >>= 1;
        else
        {
            tmp = a;
            a = (b - a) >> 1;
            b = tmp;
        }
        if (a > b)
            std::swap(a, b);
    }
    return (ans * b);
}

std::uint64_t gcd3(std::uint64_t a, std::uint64_t b)
{
    std::uint64_t ans = 0, tmp;
    if (a > b)
        std::swap(a, b);
    while (a)
    {
        if (!((a & 1) | (b & 1)))
            ++ans, a >>= 1, b >>= 1;
        else if (!(a & 1) && (b & 1))
            a >>= 1;
        else if ((a & 1) && !(b & 1))
            b >>= 1;
        else
        {
            tmp = a;
            a = (b - a) >> 1;
            b = tmp;
        }
        if (a > b)
            std::swap(a, b);
    }
    return (b << ans);
}

std::uint64_t gcd4(std::uint64_t a, std::uint64_t b) 
{
    if (a == b)
		return a;
	if (a == 0)
		return b;
	if (b == 0)
			return a;
	if ((~a & 1) != 0) {
		if ((b & 1) != 0)
			return gcd4(a >> 1, b);
		else
			return gcd4(a >> 1, b >> 1) << 1;
	}
	if ((~b & 1) != 0)
		return gcd4(a, b >> 1);
	if (a > b)
		return gcd4((a - b) >> 1, b);
	return gcd4((b - a) >> 1, a);
}

std::uint64_t gcd5(std::uint64_t a, std::uint64_t b) 
{
    std::uint64_t shift;
	if (a == 0)
		return b;
	if (b == 0)
		return a;
	for (shift = 0; ((a | b) & 1) == 0; ++shift) 
    {
		a >>= 1;
		b >>= 1;
	}
	while ((a & 1) == 0)
		a >>= 1;
    do 
    {
        while ((b & 1) == 0)
            b >>= 1;
        if (a > b) 
        {
            std::uint64_t t = b;
            b = a;
            a = t;
        }
        b = b - a;
    } while (b != 0);
	return a << shift;
}

std::uint64_t gcd6 (std::uint64_t a, std::uint64_t b) 
{
    std::uint64_t shift;
    if (a == 0 || b == 0)
        return a | b;
    for (shift = 0; ((a | b) & 1) == 0; ++shift) 
    {
        a >>= 1;
        b >>= 1;
    }
    while ((a & 1) == 0)
        a >>= 1;
    do 
    {
        while ((b & 1) == 0) 
            b >>= 1;
        if (a < b) 
            b -= a;
        else
        {
            std::uint64_t diff = a - b;
            a = b;
            b = diff;
        }
        b >>= 1;
    } while (b != 0);
    return a << shift;
}

std::uint64_t gcd7(std::uint64_t a, std::uint64_t b)
{
    while (b^=a^=b^=a%=b);
    return a;
}

//Returns number of milliseconds in case of success, -1 otherwise
std::int64_t test_algo(std::string input_filename, std::function<std::uint64_t (std::uint64_t, std::uint64_t)> func)
{
    std::ifstream file (input_filename);
    if (!file.is_open())
        return -1;
    using std::chrono::steady_clock;
    steady_clock::time_point begin, end;
    begin = steady_clock::now();
    std::uint64_t a, b;
    while (file >> a >> b)
    {
        func(a, b);
    }
    end = steady_clock::now();
    file.close();
    return std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count();
}

void help()
{
    std::cout << "\tParameter\tType\tDescription\n\n"
                 "\t1st\t\tString\t\t\tName of file where tests are placed or where they will be generated.\n\n"
                 "\t2nd (optional)\tNon-negative integer\tNumber of pairs in the test file you want to generate.\n"
                 "\t\t\twithin 2^64-1\n\n"
                 "\t3rd (optional)\tNon-negative integer\tMaximum value of generated numbers.\n"
                 "\t\t\twithin 2^64-1\n\n"
                 "If you specify only 1 argument, no data will be generated and the data that has been already generated will be used.\n"
                 "If you specify 2 or more arguments, the old data will be lost and new data will be generated.\n"
                 "If the 3rd argument is not specified, the default value 2^64-1 will be used\n\n";
}

void file_error(std::string filename)
{
    std::cout << "An error occured while opening " << filename << " file\n";
}

int main(int argc, char* argv[])
{
    std::string filename;
    if (argc <= 1)
    {
        std::cout << "Please specify correct parameters. For more information use --help option.\n";
        return 1;
    }
    filename = std::string(argv[1]);
    if (!filename.compare(std::string("--help")))
    {
        help();
        return 0;
    }
    if (argc == 3)
    {
        if (gen_data(filename, std::atoll(argv[2])))
        {
            file_error(filename);
            return 1;
        }
    }
    else if (argc > 3)
    {
        if (gen_data(filename, std::atoll(argv[2]), std::atoll(argv[3])))
        {
            file_error(filename);
            return 1;
        } 
    }
    std::int64_t gcd_time = test_algo(filename, gcd), bin_gcd_time = test_algo(filename, bin_gcd), 
            gcd3_time = test_algo(filename, gcd3), gcd4_time = test_algo(filename, gcd4),
            gcd5_time = test_algo(filename, gcd5), gcd6_time = test_algo(filename, gcd6), gcd7_time = test_algo(filename, gcd7);
    if (gcd_time == -1 || bin_gcd_time == -1 || gcd3_time == -1 || gcd4_time == -1 || gcd5_time == -1 || gcd6_time == -1 || gcd7_time == -1)
    {
        file_error(filename);
        return 1;
    }
    std::cout << "\tFunction\tTime in milliseconds\n"
                 "\tgcd\t\t"<< gcd_time << "\n"
                 "\tbin_gcd\t\t"<< bin_gcd_time << "\n"
                 "\tgcd3\t\t"<< gcd3_time << "\n"
                 "\tgcd4\t\t"<< gcd4_time << "\n"
                 "\tgcd5\t\t"<< gcd5_time << "\n"
                 "\tgcd6\t\t"<< gcd6_time << "\n"
                 "\tgcd7\t\t"<< gcd7_time << '\n';
}
