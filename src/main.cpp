#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<optional>

#include "./tokenization.hpp"
#include "./parser.hpp"
#include "./generation.hpp"



int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr<<"Incorrect usage. Correct usage is:"<<std::endl;
        std::cerr<<"hydro <input.hy>"<< std::endl;
        return EXIT_FAILURE;
    }

    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }
    
    // std::cout<< contents << std::endl;
    
    Tokenizer tokenizer(std::move(contents));
    std::vector<Token> tokens = tokenizer.tokenize();


    
    Parser parser(std::move(tokens));

    std::optional<NodeProg> prog = parser.parse_prog();

    if (!prog.has_value()) {
         std::cerr<<"Error parsing"<<std::endl;
        exit(EXIT_FAILURE);

    }
    
     Generator generator(prog.value());
    //  std::cout<<generator.gen_prog()<<std::endl;
   
    {
        std::fstream file("out.asm", std::ios::out);
         file << generator.gen_prog();
 }
    system("nasm -f elf64 out.asm");
    system("ld -o out out.o");

    return EXIT_SUCCESS;
}
