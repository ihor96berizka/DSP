#include <algorithm>
#include <iostream>
#include <vector>
#include <thread>

using namespace std;
std::vector<double> convolve(const std::vector<double>& x, std::vector<double>& y)
{
    std::vector<double>  convolve (x.size() + y.size() - 1);

    std::reverse(y.begin(), y.end());

    for (size_t i = 0; i < x.size(); ++i)
    {
        for (size_t j = 0; j < y.size(); ++j)
        {
            convolve.at(i + j) += x[i] * y[j];
        }
    }
    return convolve;
}
int main()
{
    std::vector<double>  m_signal_x = {1, 2, 3};
    std::vector<double>  m_signal_y = {1, 3, 5};

    std::vector<double> m_signal_convolve = convolve(m_signal_x, m_signal_y);
    cout << "signal conv...\n";
    for (auto const&  item: m_signal_convolve)
    {
        cout << item << ' ';
    }
    return 0;
}
