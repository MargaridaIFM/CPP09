/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfrancis <mfrancis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 11:07:27 by mfrancis          #+#    #+#             */
/*   Updated: 2026/02/09 14:28:04 by mfrancis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <iostream>


int main(int argc, char *argv[])
{
    if(argc == 2)
    {
        try
        {
            BitcoinExchange btc("data.csv");
            btc.processInputFile(argv[1]);
        }
        catch (...)
        {
            std:: cerr << "Error: could not open file." << std::endl;
            return 1;
        }
    }
    else 
    {   
        std::cerr << "Invalid number of arguments" << std::endl;
        return 1;
    }
    return 0;
}
// int main(int argc, char **argv) {
//     if (argc != 2) {
//         std::cerr << "Error: could not open file." << std::endl;
//         return 1;
//     }
//     try {
//         BitcoinExchange btc("data.csv");
//         btc.processInputFile(argv[1]);
//     } catch (...) {
//         std::cerr << "Error: could not open file." << std::endl;
//         return 1;
//     }
//     return 0;
// }