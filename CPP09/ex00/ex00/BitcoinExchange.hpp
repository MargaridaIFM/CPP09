/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfrancis <mfrancis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 11:07:36 by mfrancis          #+#    #+#             */
/*   Updated: 2026/02/09 14:20:06 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>
#include <iostream>


class BitcoinExchange {
    private:
        std::map<std::string, double> _rates;
    
        static std::string trim(const std::string &s);
        static bool isValidDate(const std::string &date);
        static bool parseValue(const std::string &s, double &out);
        double rateForDate(const std::string &date) const;
        
    public:
        BitcoinExchange();
        explicit BitcoinExchange(const std::string &csvPath);
        ~BitcoinExchange();

        void loadRates(const std::string &csvPath);
        void processInputFile(const std::string &inputPath) const;

};

#endif

// class BitcoinExchange
// {
//     private:
//         typedef std::map
//     public:
//         BitcoinExchange(const char *file);
//         BitcoinExchange (const BitcoinExchange *copy);
//         BitcoinExchange &operator=(const BitcoinExchange &copy);
//         ~BitcoinExchange();

//         void run();
//         int checkFormat();
//         int checkDate();
//         int checkValue();
        
// }

// #endif