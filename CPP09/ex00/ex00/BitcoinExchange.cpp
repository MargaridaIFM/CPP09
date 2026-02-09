/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfrancis <mfrancis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 14:20:18 by mfrancis          #+#    #+#             */
/*   Updated: 2026/02/09 14:20:20 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <cctype>

BitcoinExchange::BitcoinExchange() {}
BitcoinExchange::BitcoinExchange(const std::string &csvPath) { loadRates(csvPath); }
BitcoinExchange::~BitcoinExchange() {}

static bool isDigits(const std::string &s) {
    if (s.empty()) return false;
    for (size_t i = 0; i < s.size(); ++i)
        if (!std::isdigit(static_cast<unsigned char>(s[i])))
            return false;
    return true;
}

std::string BitcoinExchange::trim(const std::string &s) {
    size_t b = 0;
    while (b < s.size() && std::isspace(static_cast<unsigned char>(s[b]))) b++;
    size_t e = s.size();
    while (e > b && std::isspace(static_cast<unsigned char>(s[e-1]))) e--;
    return s.substr(b, e-b);
}

static bool isLeap(int y) {
    if (y % 400 == 0) return true;
    if (y % 100 == 0) return false;
    return (y % 4 == 0);
}

bool BitcoinExchange::isValidDate(const std::string &date) {
    if (date.size() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;

    std::string ys = date.substr(0,4);
    std::string ms = date.substr(5,2);
    std::string ds = date.substr(8,2);
    if (!isDigits(ys) || !isDigits(ms) || !isDigits(ds)) return false;

    int y = std::atoi(ys.c_str());
    int m = std::atoi(ms.c_str());
    int d = std::atoi(ds.c_str());
    if (m < 1 || m > 12) return false;
    if (d < 1) return false;

    int mdays[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int maxd = mdays[m-1];
    if (m == 2 && isLeap(y)) maxd = 29;
    if (d > maxd) return false;

    return true;
}

bool BitcoinExchange::parseValue(const std::string &s, double &out) {
    std::istringstream iss(s);
    iss >> out;
    if (iss.fail()) return false;
    char c;
    if (iss >> c) return false; // trailing junk
    return true;
}

void BitcoinExchange::loadRates(const std::string &csvPath) {
    std::ifstream file(csvPath.c_str());
    if (!file.is_open()) throw std::runtime_error("could not open data.csv");

    std::string line;
    if (!std::getline(file, line)) throw std::runtime_error("empty data.csv"); // header

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        size_t comma = line.find(',');
        if (comma == std::string::npos) continue;

        std::string date = trim(line.substr(0, comma));
        std::string rateStr = trim(line.substr(comma + 1));

        double rate;
        if (!isValidDate(date)) continue;
        if (!parseValue(rateStr, rate)) continue;
        _rates[date] = rate;
    }
}

double BitcoinExchange::rateForDate(const std::string &date) const {
    std::map<std::string, double>::const_iterator it = _rates.find(date);
    if (it != _rates.end()) return it->second;

    it = _rates.upper_bound(date);   // first key > date
    if (it == _rates.begin()) throw std::runtime_error("no earlier rate");
    --it;                            // nearest lower date
    return it->second;
}

void BitcoinExchange::processInputFile(const std::string &inputPath) const {
    std::ifstream file(inputPath.c_str());
    if (!file.is_open()) {
        std::cerr << "Error: could not open file." << std::endl;
        return;
    }

    std::string line;
    if (!std::getline(file, line)) return; // header (optional)

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        size_t pipe = line.find('|');
        if (pipe == std::string::npos) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        std::string date = trim(line.substr(0, pipe));
        std::string valueStr = trim(line.substr(pipe + 1));

        if (!isValidDate(date)) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        double value;
        if (!parseValue(valueStr, value)) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        if (value < 0.0) {
            std::cerr << "Error: not a positive number." << std::endl;
            continue;
        }
        if (value > 1000.0) {
            std::cerr << "Error: too large a number." << std::endl;
            continue;
        }

        try {
            double rate = rateForDate(date);
            std::cout << date << " => " << value << " = " << (value * rate) << std::endl;
        } catch (...) {
            std::cerr << "Error: bad input => " << line << std::endl;
        }
    }
}
