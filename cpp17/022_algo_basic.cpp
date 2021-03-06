#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <tuple>
#include <iterator>
#include <algorithm>
#include <random>

namespace std {
ostream& operator << (ostream& os, const pair<int, string> p)
{
    return os << "(" << p.first << ", " << p.second << ")";
}
}

static void print(const std::vector<int>& v)
{
    std::copy(begin(v), end(v), std::ostream_iterator<int> { std::cout, ", " });
    std::cout << std::endl;
}

#if false
int main()
{
    {   // copy
        std::cout << "-- copy --" << std::endl;
        std::vector<std::pair<int, std::string>> v {
            { 1, "one"   },
            { 2, "two"   },
            { 3, "three" },
            { 4, "four"  },
            { 5, "five"  }
        };
        std::map<int, std::string> m;
        std::copy_n(begin(v), 3, std::inserter(m, begin(m)));
        // std::copy(begin(v), end(v), std::inserter(m, begin(m)));
        auto shell_it ( std::ostream_iterator<std::pair<int, std::string>>{ std::cout, ", " } );
        std::copy(begin(m), end(m), shell_it);
        std::cout << std::endl;
    }

    {   // move
        std::cout << "-- move -- " << std::endl;
        std::vector<std::pair<int, std::string>> v {
            { 1, "one"   },
            { 2, "two"   },
            { 3, "three" },
            { 4, "four"  },
            { 5, "five"  }
        };
        std::map<int, std::string> m;
        std::move(begin(v), end(v), std::inserter(m, begin(m)));
        auto shell_it ( std::ostream_iterator<std::pair<int, std::string>>{ std::cout, ", " } );
        std::copy(begin(v), end(v), shell_it); // should be empty
        std::cout << std::endl;
        std::copy(begin(m), end(m), shell_it);
        std::cout << std::endl;
    }

    std::random_device r_dev;
    std::mt19937 g { r_dev() };

    { // sort
        std::cout << "-- sort -- " << std::endl;
        std::vector<int> v = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        std::cout << "is sorted: " << std::is_sorted(begin(v), end(v)) << std::endl;
        std::shuffle(begin(v), end(v), g);
        std::cout << "is sorted: " << std::is_sorted(begin(v), end(v)) << std::endl;
        print(v);
        std::sort(begin(v), end(v));
        std::cout << "is sorted: " << std::is_sorted(begin(v), end(v)) << std::endl;
        print(v);
    }
    { // partition
        std::cout << "-- partition -- " << std::endl;
        std::vector<int> v = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        std::shuffle(begin(v), end(v), g);
        std::cout << "is sorted: " << std::is_sorted(begin(v), end(v)) << std::endl;
        print(v);
        std::partition(begin(v), end(v), [](int i) {
            return i < 4;
        });
        std::cout << "is sorted: " << std::is_sorted(begin(v), end(v)) << std::endl;
        print(v);
    }
    { // partial_sort
        std::cout << "-- partial_sort -- " << std::endl;
        std::vector<int> v = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        std::shuffle(begin(v), end(v), g);
        std::cout << "is sorted: " << std::is_sorted(begin(v), end(v)) << std::endl;
        print(v);
        auto middle = (std::next(begin(v), int(v.size() / 2)));
        std::partial_sort(begin(v), middle, end(v));
        std::cout << "is sorted: " << std::is_sorted(begin(v), end(v)) << std::endl;
        print(v);
    }
    { // sort custom types
        std::cout << "-- sort custom types -- " << std::endl;
        struct somestruct {
            int a;
            int b;
        };
        std::vector<somestruct> v {
            { 252, 113 },
            { 2, 3 },
            { 82, 33 },
            { 22, 33 },
            { 12, 13 },
            { 222, 333 },
        };
        std::sort(begin(v), end(v), [](const somestruct& l, const somestruct& r){
            return l.b < r.b;
        });
        for ( const auto& [a, b] : v ) {
            std::cout << a << ", " << b << std::endl;
        }
    }
    { // remove and remove_if
        std::cout << "-- remove and remove_if -- " << std::endl;
        std::vector<int> v = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        print(v);
        const auto new_end = std::remove(begin(v), end(v), 2);
        v.erase(new_end, end(v));
        print(v);

        auto odd_number = [](int i){ return i % 2 != 0; };
        const auto new_end2 = std::remove_if(begin(v), end(v), odd_number);
        v.erase(new_end2, end(v));
        print(v);
    }
    { // replace and replace_if
        std::cout << "-- replace and replace_if -- " << std::endl;
        std::vector<int> v = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        print(v);
        std::replace(begin(v), end(v), 2, 222);
        print(v);
        auto odd_number = [](int i){ return i % 2 != 0; };
        std::replace_if(begin(v), end(v), odd_number, 73);
        print(v);
    }
    { // copy_if and remove_copy_if
        std::cout << "-- copy_if and remove_copy_if -- " << std::endl;
        std::vector<int> v = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        std::vector<int> v1;
        std::vector<int> v2;
        print(v);
        auto odd_number = [](int i){ return i % 2 != 0; };
        auto even_number = [](int i){ return i % 2 == 0; };
        std::remove_copy_if(begin(v), end(v), std::back_inserter(v1), odd_number);
        std::copy_if(begin(v), end(v), std::back_inserter(v2), even_number);
        print(v1);
        print(v2);
    }
    { // transform
        std::cout << "-- transform -- " << std::endl;
        std::vector<int> v = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        print(v);
        std::transform(begin(v), end(v), v.begin(), [](int i){
            return i * 2;
        });
        print(v);
    }
    { // transform2
        std::cout << "-- transform2 -- " << std::endl;
        std::vector<int> v = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        std::vector<int> result;
        print(v);
        std::transform(begin(v), end(v), std::back_inserter(result), [](int i){
            return i * 2;
        });
        print(result);
    }
}
#endif
