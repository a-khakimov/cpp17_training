#include <iostream>
#include <iomanip>
#include <thread>
#include <string>
#include <sstream>
#include <future>

using namespace std::chrono_literals;

struct pcout : public std::stringstream {
    static inline std::mutex cout_mutex;
    ~pcout() {
        std::lock_guard<std::mutex> l { cout_mutex };
        std::cout << rdbuf();
        std::cout.flush();
    }
};

static std::string create(const char* s)
{
    pcout{} << "3s create " << std::quoted(s) << std::endl;
    std::this_thread::sleep_for(3s);
    return {s};
}


static std::string concat(const std::string& a, const std::string& b)
{
    pcout{} << "5s concat " << std::quoted(a) << " + " << std::quoted(b) << std::endl;
    std::this_thread::sleep_for(5s);
    return a + b;
}

static std::string twice(const std::string& s)
{
    pcout{} << "3s twice " << std::quoted(s) << std::endl;
    std::this_thread::sleep_for(3s);
    return s + s;
}

template <typename F>
static auto asynchronize(F f)
{
    return [f](auto ... xs) {
        return [=] () {
            return std::async(std::launch::async, f, xs ...);
        };
    };
}

template <typename F>
static auto fut_unwrap(F f)
{
    return [f](auto ... xs) {
        return f(xs.get()...);
    };
}

template <typename F>
static auto async_adapter(F f)
{
    return [f](auto ... xs) {
        return [=] () {
            return std::async(std::launch::async, fut_unwrap(f), xs()...);
        };
    };
}

#if false
int main()
{
    auto pcreate = asynchronize(create);
    auto pconcat = async_adapter(concat);
    auto ptwice = async_adapter(twice);

    auto result = pconcat(
                    ptwice(
                        pconcat(
                            pcreate("foo "),
                            pcreate("bar "))),
                    pconcat(
                        pcreate("this "),
                        pcreate("that ")))().get();

    std::cout << result << std::endl;
}
#endif
