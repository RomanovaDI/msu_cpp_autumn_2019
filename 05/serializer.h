// serializer.h
#pragma once
#include <typeinfo>

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
    Error operator()(ArgsT... args)
    {
        return process(args...);
    }
    
private:
    // process использует variadic templates
    template <class T>
    Error process(T&& val)
    {
        if (typeid(val) == typeid(uint64_t))
            out_ << val << Separator;
        else if (typeid(val) == typeid(bool))
            if (val)
                out_ << "true" << Separator;
            else if (!val)
                out_ << "false" << Separator;
            else
                return Error::CorruptedArchive;
        else
            return Error::CorruptedArchive;
        return Error::NoError;
    }

    template <class T, class... Args>
    Error process(T&& val, Args&&... args)
    {
        if (typeid(val) == typeid(uint64_t))
            out_ << val << Separator;
        else if (typeid(val) == typeid(bool))
            if (val)
                out_ << "true" << Separator;
            else if (!val)
                out_ << "false" << Separator;
            else
                return Error::CorruptedArchive;
        else
            return Error::CorruptedArchive;
        return process(std::forward<Args>(args)...);
    }
};

uint64_t convert(std::string text)
{
    uint64_t value;
    std::istringstream iss(text);
    iss >> value;
    return value;
}

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
    Error operator()(ArgsT&... args)
    {
        return process(args...);
    }

private:
    // process использует variadic templates
    template <class T>
    Error process(T& val)
    {
        std::string text;
        in_ >> text;
        if (text.compare("true") == 0) {
            if (typeid(val) != typeid(bool))
                return Error::CorruptedArchive;
            val = true;
        } else if (text.compare("false") == 0) {
            if (typeid(val) != typeid(bool))
                return Error::CorruptedArchive;
            val = false;
        } else if (text.compare(std::to_string(convert(text))) == 0) {
            if (typeid(val) != typeid(uint64_t))
                return Error::CorruptedArchive;
            val = convert(text);
        } else
            return Error::CorruptedArchive;
        return Error::NoError;
    }

    template <class T, class... Args>
    Error process(T& val, Args&... args)
    {
        std::string text;
        in_ >> text;
        if (text.compare("true") == 0) {
            if (typeid(val) != typeid(bool))
                return Error::CorruptedArchive;
            val = true;
        } else if (text.compare("false") == 0) {
            if (typeid(val) != typeid(bool))
                return Error::CorruptedArchive;
            val = false;
        } else if (text.compare(std::to_string(convert(text))) == 0) {
            if (typeid(val) != typeid(uint64_t))
                return Error::CorruptedArchive;
            val = convert(text);
        } else
            return Error::CorruptedArchive;
        return process(args...);
    }
};