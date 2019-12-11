// serializer.h
#pragma once
//#include <typeinfo>
#include <iostream>
#include <sstream>
#include <string>

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    static constexpr char Separator = ' ';
    std::ostream& out_;

public:

    explicit Serializer(std::ostream& out)
        : out_(out)
    {
    }

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(std::forward<ArgsT>(args)...);
    }
    
private:

    template <class T>
    Error process(T&& val)
    {
        return print_stream(val);
    }

    template <class T, class... Args>
    Error process(T&& val, Args&&... args)
    {
        if (process(val) == Error::CorruptedArchive)
            return Error::CorruptedArchive;
        return process(std::forward<Args>(args)...);
    }

    Error print_stream(uint64_t val)
    {
        out_ << val << Separator;
        return (out_) ? Error::NoError : Error::CorruptedArchive;
    }

    Error print_stream(bool val)
    {
        out_ << std::boolalpha << val << Separator;
        return (out_) ? Error::NoError : Error::CorruptedArchive;
    }
};

class Deserializer
{
    std::istream& in_;

public:

    explicit Deserializer(std::istream& in)
        : in_(in)
    {
    }

    template <class T>
    Error load(T& object)
    {
        return object.deserialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(std::forward<ArgsT>(args)...);
    }

private:

    template <class T>
    Error process(T&& val)
    {
        return load_val(val);
    }

    template <class T, class... Args>
    Error process(T&& val, Args&&... args)
    {
        if (process(val) == Error::CorruptedArchive)
            return Error::CorruptedArchive;
        return process(std::forward<Args>(args)...);
    }

    Error load_val(uint64_t& val)
    {
        in_ >> val;
        return (in_) ? Error::NoError : Error::CorruptedArchive;
    }

    Error load_val(bool& val)
    {
        in_ >> std::boolalpha >> val;
        return (in_) ? Error::NoError : Error::CorruptedArchive;
    }
};