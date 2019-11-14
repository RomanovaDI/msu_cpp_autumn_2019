#pragma once

std::string format(int n, std::string ss)
{
    std::string s1 = "{";
    std::string s2 = "}";
    if ((ss.find(s1) != std::string::npos) || (ss.find(s2) != std::string::npos))
        throw std::runtime_error("Error");
    return ss;
}

template <class T>
std::string format(int n, std::string ss, T var)
{
    std::ostringstream s1;
    s1 << "{" << n << "}";
    std::string sold = s1.str();
    std::ostringstream s2;
    s2 << var;
    std::string snew = s2.str();
    while (ss.find(sold) != std::string::npos)
        ss.replace(ss.find(sold), sold.length(), snew);
    return format(++n, ss);
}

template <class T, class... Args>
std::string format(int n, std::string ss, T var, Args... args)
{
    std::ostringstream s1;
    s1 << "{" << n++ << "}";
    std::string sold = s1.str();
    std::ostringstream s2;
    s2 << var;
    std::string snew = s2.str();
    while (ss.find(sold) != std::string::npos)
        ss.replace(ss.find(sold), sold.length(), snew);
    return format(n, ss, args...);
}

std::string format(const char* text)
{
    int n = 0;
    std::string ss(text);
    return format(n, ss);
}

template <class... Args>
std::string format(const char* text, Args... args)
{
    int n = 0;
    std::string ss(text);
    return format(n, ss, args...);
}