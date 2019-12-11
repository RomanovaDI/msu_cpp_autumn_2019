#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>

template <class T>
std::string to_string(T&& x)
{
	std::stringstream ss;
	ss << x;
	return ss.str();
}

template <class... argsT>
std::string format(const std::string& str, argsT&&... obj)
{
	std::vector<std::string> args{to_string(std::forward<argsT>(obj))...};
	std::ostringstream res;
	for (size_t i = 0; i < str.length(); ++i) {
		if (str[i] == '{') {
			i++;
			if (str[i] == '}') {
				throw std::runtime_error("No index");
			}
			size_t id;
			std::string tmp_num;
			while(str[i] != '}' && i < str.length()) {
				if (std::isdigit(str[i])) {
					tmp_num += str[i];
					++i;
				} else {
					throw std::runtime_error("Wrong index");
				}
			}
			if (i == str.length()) {
				throw std::runtime_error("Wrong format");
			}
			id = std::stoull(tmp_num);
			if (id < args.size()) {
				res << args[id];
			} else {
				throw std::runtime_error("Big index");
			}
		} else {
			if (str[i] == '}') {
				throw std::runtime_error("Wrong format");
			}
			res << str[i];
		}	
	}
	return res.str();
}