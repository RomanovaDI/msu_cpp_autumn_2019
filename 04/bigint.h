#pragma once
#include <iostream>
#include <cstring>

constexpr int base = 10;
constexpr int default_size = 10;

class BigInt {
	char *storage;
	size_t available_size;
	size_t current_size;
	bool sign;

public:
	void resize(size_t sz)
    {
		char *new_storage = new char[sz]();
		for (size_t i = 0; i < current_size; ++i) {
			new_storage[i] = storage[i];
		}
		delete[] storage;
		storage = new_storage;
	}

	BigInt(long long int num = 0)
        : available_size(default_size), current_size(0), sign(0)
    {
		if (num < 0) {
			sign = 1;
			num = -num;
		}
		storage = new char[available_size]();
		for (; num > 0; num /= base) {
			if (current_size == available_size) {
				available_size <<= 1;
				this->resize(available_size);
			}
			storage[current_size++] = num % base;
		}
	}

	BigInt(const BigInt &other)
        : available_size(other.available_size),
		current_size(other.current_size), sign(other.sign)
    {
		storage = new char[other.available_size]();
		memcpy(storage, other.storage, other.current_size);
	}

	BigInt(BigInt &&other)
        : available_size(other.available_size),
		current_size(other.current_size), sign(other.sign)
    {
		storage = nullptr;
		std::swap(storage, other.storage);
	}

	BigInt& operator= (BigInt &&other)
    {
		if (this == &other) {
			return *this;
		}
		this->sign = other.sign;
		this->current_size = other.current_size;
		this->available_size = other.available_size;
		std::swap(storage, other.storage);
		return *this;
	}

	BigInt& operator= (const BigInt &other)
    {
		if (*this == other) {
			return *this;
		}
		this->sign = other.sign;
		delete[] storage;
		this->available_size = other.available_size;
		this->current_size = other.current_size;
		storage = new char[other.available_size]();
		memcpy(storage, other.storage, other.current_size);
		return *this;
	}

	~BigInt()
    {
		delete[] storage;
	}

	bool get_sign() const
    {
		return sign;
	}

	BigInt operator-() const
    {
		BigInt res(*this);
		res.sign = !res.sign;
		return res;
	}

	void set_sign(bool sgn)
    {
		sign = sgn;
	}

	BigInt abs() const
    {
		BigInt res(*this);
		res.set_sign(0);
		return res;
	}

	BigInt operator+ (const BigInt &other) const
    {
		if (sign == other.sign) {
			BigInt res(other);
			for (size_t i = 0, carry = 0; i < std::max(current_size, other.current_size) || carry; ++i) {
				if (i == res.current_size) {
					if (!res.current_size || res.current_size == res.available_size) {
 						res.available_size <<= 1;
						res.resize(res.available_size);
					}
					++res.current_size;
				}
				char term = (i < current_size) ? storage[i] : 0;
				res.storage[i] += (term + carry);
				carry = res.storage[i] >= base;
				if (carry) {
					res.storage[i] -= base;
				}
			}
		    return res;
		}
		return *this - (-other);
	}

	BigInt operator- (const BigInt &other) const
    {
		if (sign == other.sign) {
			if ((*this).abs() >= other.abs()) {
				BigInt res = *this;
				for (size_t i = 0,  carry = 0; i < other.current_size || carry; ++i) {
					char sub = (i < other.current_size) ? other.storage[i] : 0;
					res.storage[i] -= (sub + carry);
					carry = res.storage[i] < 0;
					if (carry) {
						res.storage[i] += base;
			    	}
				}
				while (res.current_size > 0 && (res.storage[res.current_size - 1] == 0)) {
					--res.current_size;
				}
				if (res.current_size == 0) {
					res.sign = 0;
				}
				return res;
		    }
		    return -(other - *this);
		}
		return -other + *this;
	}

    bool operator> (const BigInt &other) const
    {
		if (sign != other.sign) {
		    return sign < other.sign;
		}
		if (current_size != other.current_size) {
		    return current_size * (!sign ? 1 : -1) > other.current_size * (!other.sign ? 1: -1);
		}
		for (int i = current_size - 1; i >= 0; --i) {
			if (storage[i] != other.storage[i]) {
		    	return storage[i] * (!sign ? 1: -1) > other.storage[i] * (!sign ? 1 : -1);
		    }
		}
		return false;
    }

    bool operator< (const BigInt &other) const
    {
		return other > *this;
    }

    bool operator== (const BigInt &other) const
    {
    	if (current_size == 0 && other.current_size == 0) {
    		return true;
    	}
		return !(*this < other || *this > other);
    }

    bool operator!= (const BigInt &other) const
    {
		return (*this < other) || (other < *this);
    }

    bool operator<= (const BigInt &other) const
    {
		return !(other < *this);
    }
    bool operator>= (const BigInt &other) const
    {
		return !(*this < other);
    }

    friend std::ostream& operator<< (std::ostream& os, const BigInt &num)
    {
		if (num.get_sign()) {
			os << '-';
		}
		if (num.current_size == 0) {
			os << '0';
		}
		for (int i = num.current_size - 1; i >= 0; --i) {
			os << (int)num.storage[i];
		}
		return os;
	}
};