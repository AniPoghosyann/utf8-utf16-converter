#include<iostream>
#include<bitset>
using namespace std;

int main(int argc, char** argv){

        // Argument validation
        if(argc<3){
                string arg_help = argv[1]; // assumes argv[1] exists
                if(arg_help == "--help"){
                        cerr<<"usage: "<<endl;
                        cerr<<" "<<argv[0]<<" <from-encoding> <to-encoding> <source-file> <destination-file>"<<endl;
                        cerr<<"arguments:"<<endl;
                        cerr<<"<from-encoding> Source text encoding(e.g., utf8, utf16le, utf16be) "<<endl;
                        cerr<<"<to-encoding> Target text encoding (e.g., utf8, utf16le, utf16be)" <<endl;
                        cerr<<"<source-file> Input file name(e.g., input.txt)"<<endl;
                        cerr<<"<destination-file> Outputfile name (e.g., output.txt)"<<endl;
                        cerr<<"Notes:"<<endl;
                        cerr<<"-When using UTF-16, specify the endian type"<<endl;
                        cerr<<"         *'utf16le' for UTF-16 little endian"<<endl;
                        cerr<<"         *'utf16be' for UTF-16 Big endian"<<endl;
                        cerr<<" -- help  display this and exit"<<endl;
                }else{
                        cerr<<"<form-encoding> is wrong (e.g.,  --help)"<<endl;
                        return 1;
                }
                return 1;
        }else if(argc<5){
                cerr<<"not enough arguments"<<endl;
                return 1;
        }else if(argc>5){
                cerr<<" to much argument"<<endl;
                cerr<<"Note: use --help"<<endl;
                return 1;
        }

        // Read arguments
        string from_encoding = argv[1];
        string to_encoding = argv[2];
        const char* source_file = argv[3];
        const char* destination_file = argv[4];

        // Validate encodings
        if(from_encoding!="utf8"){
                if(from_encoding!="utf16le"){
                        if(from_encoding!="utf16be"){
                                cerr<<argv[1]<<":  <form-encoding> can be only 'utf8'  'utf16le'  'utf16be' "<<endl;
                                return 1;
                        }
                }
        }

        if(to_encoding!="utf8"){
                if(to_encoding!="utf16le"){
                        if(to_encoding!="utf16be"){
                                cerr<<argv[2]<<":  <to-encoding> can be only 'utf8'  'utf16le'  'utf16be' "<<endl;
                                return 1;
                        }
                }
        }

        // Prevent same encoding conversion
        if(from_encoding==to_encoding){
                cerr<<argv[1]<<" "<<argv[2]<<":  encoding type remains the  same"<<endl;
                return 1;
        }

        // Open input file
        FILE* input_file = fopen(source_file, "r");
        if(input_file==nullptr){
                cerr<<argv[3]<<":  <input-file> does not exist"<<endl;
                return 1;
        }

        // Get file size
        fseek(input_file, 0, SEEK_END);
        long byte_count = ftell(input_file);
        fseek(input_file, 0, SEEK_SET);

        // ================= UTF-8 -> UTF-16 =================
        if( from_encoding == "utf8"){

                // Read raw bytes
                uint8_t buffer[byte_count];
                fread(buffer, byte_count, 1, input_file);

                // Basic UTF-8 check
                if(buffer[0]<248){

                        fclose(input_file);

                        int16_t utf16_buffer[byte_count];
                        int utf16_index = 1;

                        // -------- UTF-16 BE --------
                        if(to_encoding=="utf16be"){

                                // BOM for BE
                                utf16_buffer[0] = 0b1111111111111110;

                                cout<<"successfully converted from  "<<argv[1]<<" to "<<argv[2]<<"(big endian)"<<endl;

                                for(int i=0; i<byte_count-1; i++){

                                        uint8_t byte = buffer[i];
                                        uint16_t code_point = byte;

                                        if(byte<=127){
                                                byte = byte & 0b01111111;

                                        }else if (byte<=2048){
                                                code_point = byte & 0b00011111;
                                                byte = buffer[i+1];
                                                code_point = code_point<<6 | (byte & 0b00111111);
                                                i++;

                                        }else if (byte<=15351167){
                                                code_point = byte & 0b00001111;
                                                byte = buffer[i+1];
                                                code_point = code_point<<6 | (byte & 0b00111111);
                                                byte = buffer[i+2];
                                                code_point = code_point<<6 | (byte & 0b00111111);
                                                i=i+2;
                                        }

                                        // swap bytes for big endian
                                        utf16_buffer[utf16_index] = (code_point<<8) | (code_point>>8);
                                        utf16_index++;
                                }

                        // -------- UTF-16 LE --------
                        }else if(to_encoding == "utf16le"){

                                cout<<"successfully converted from  "<<argv[1]<<" to "<<argv[2]<<"(little endian)"<<endl;

                                // BOM for LE
                                utf16_buffer[0] = 0b1111111011111111;

                                for(int i=0; i<byte_count-1; i++){

                                        uint8_t byte = buffer[i];
                                        uint16_t code_point = byte;

                                        if(byte>=128 and byte<=191){

                                        }else if(byte<=127){
                                                byte = byte & 0b01111111;
                                                code_point = byte;

                                        }else if (byte<=2048){
                                                code_point = byte & 0b00011111;
                                                byte = buffer[i+1];
                                                code_point = code_point<<6 | (byte & 0b00111111);
                                                i++;

                                        }else if (byte<=15351167){
                                                code_point = byte & 0b00001111;
                                                byte = buffer[i+1];
                                                code_point = code_point<<6 | (byte & 0b00111111);
                                                byte = buffer[i+2];
                                                code_point = code_point<<6 | (byte & 0b00111111);
                                                i=i+2;
                                        }

                                        utf16_buffer[utf16_index] = code_point;
                                        utf16_index++;
                                }

                        }else{
                                cerr<<"not valid endian type "<<endl;
                                return 1;
                        }

                        // Write output file
                        FILE* output_file = fopen(destination_file, "w");
                        fwrite(utf16_buffer, sizeof(uint16_t),utf16_index, output_file);
                        fclose(output_file);

                }else{
                        cerr<<"your source-file is not utf8"<<endl;
                        return 1;
                }

        // ================= UTF-16 LE -> UTF-8 =================
        }else if (from_encoding=="utf16le" and to_encoding=="utf8"){

                int unit_count = byte_count/2;

                uint16_t buffer[byte_count];
                fread(buffer, byte_count , 1, input_file);

                // Check BOM
                if(buffer[0]== 0b1111111011111111){

                        fclose(input_file);

                        uint8_t utf8_buffer[byte_count-2];
                        int utf8_index = 0;

                        FILE* output_file = fopen(destination_file, "w");

                        cout<<"in le"<<endl;

                        for(int i=1; i<unit_count; i++){

                                uint16_t code_unit = buffer[i];
                                uint8_t out_byte = code_unit;

                                if(code_unit<=127){

                                        out_byte = code_unit;
                                        fwrite(&out_byte, 1, 1, output_file);

                                }else if (code_unit<=2048){

                                        uint8_t second = 0b10111111 & code_unit;
                                        uint8_t temp = 0b10000000 | second;
                                        uint8_t second_byte = temp;

                                        code_unit = code_unit>>6;
                                        temp = 0b11000000 | code_unit;
                                        out_byte = temp;

                                        fwrite(&out_byte, 1, 1, output_file);
                                        fwrite(&second_byte, 1, 1, output_file);

                                }else if (code_unit<=15351167){

                                        cout<<"--------3 byte section enterance ----------"<<endl;

                                        bitset<32> debug_bits = code_unit;
                                        cout<<"yndhanury: "<<debug_bits<<endl;

                                        uint8_t third = 0b10111111 & code_unit;
                                        uint8_t third_byte = 0b10000000 | third;

                                        bitset<8> dbg3 = third_byte;
                                        cout<<"3rdy: "<<dbg3<<endl;

                                        code_unit = code_unit>>6;

                                        bitset<32> dbg_after = code_unit;
                                        cout<<"after shift : "<<dbg_after<<endl;

                                        uint8_t second = 0b10111111 & code_unit;
                                        uint8_t second_byte = 0b1000000 | code_unit;

                                        bitset<8> dbg2 = second_byte;
                                        cout<<"2rdy: "<<dbg2<<endl;

                                        code_unit = code_unit>>6;

                                        bitset<32> dbg_after2 = code_unit;
                                        cout<<"after scond shift: "<<dbg_after2<<endl;

                                        uint8_t first_byte = 0b11100000 | code_unit;

                                        bitset<8> dbg1 = first_byte;
                                        cout<<"1iny : "<<dbg1<<endl;

                                        fwrite(&first_byte, 1, 1, output_file);
                                        fwrite(&second_byte ,1 ,1 ,output_file);
                                        fwrite(&third_byte, 1, 1, output_file);
                                }
                        }

                }else{
                        cerr<<"your source-file.txt is not utf16le "<<endl;
                        return 1;
                }

        // ================= UTF-16 BE -> UTF-8 =================
        }else if (from_encoding=="utf16be" and to_encoding=="utf8"){

                int unit_count = byte_count/2;

                uint16_t buffer[byte_count];
                fread(buffer, byte_count , 1, input_file);

                // Check BOM
                if(buffer[0]==0b1111111111111110){

                        fclose(input_file);

                        uint8_t utf8_buffer[byte_count-2];
                        int utf8_index = 0;

                        FILE* output_file = fopen(destination_file, "w");

                        cout<<"in be"<<endl;

                        for(int i=1; i<unit_count; i++){

                                uint16_t code_unit = buffer[i];
                                uint8_t out_byte = code_unit;

                                // swap endian
                                code_unit = (code_unit>>8) | (code_unit<<8);

                                if(code_unit<=127){

                                        out_byte = code_unit;
                                        fwrite(&out_byte, 1, 1, output_file);

                                }else if (code_unit<=2048){

                                        uint8_t second = (code_unit<<2)>>2;
                                        uint8_t temp = 0b10000000 | second;
                                        uint8_t second_byte = temp;

                                        code_unit = code_unit>>6;
                                        temp = 0b11000000 | code_unit;
                                        out_byte = temp;

                                        fwrite(&out_byte, 1, 1, output_file);
                                        fwrite(&second_byte, 1, 1, output_file);

                                }else if( code_unit<=15351167){

                                        bitset<32> dbg = code_unit;
                                        cout<<"yndhanury: "<<dbg<<" b="<<code_unit<<endl;

                                        uint8_t third = 0b10111111 & code_unit;
                                        uint8_t third_byte = 0b10000000 | third;

                                        bitset<8> dbg3 = third_byte;
                                        cout<<"3rdy: "<<dbg3<<endl;

                                        code_unit = code_unit>>6;

                                        bitset<32> dbg_after = code_unit;
                                        cout<<"after shift : "<<dbg_after<<endl;

                                        uint8_t second = 0b10111111 & code_unit;
                                        uint8_t second_byte = 0b1000000 | code_unit;

                                        bitset<8> dbg2 = second_byte;
                                        cout<<"2rdy: "<<dbg2<<endl;

                                        code_unit = code_unit>>6;

                                        uint8_t first_byte = 0b11100000 | code_unit;

                                        bitset<8> dbg1 = first_byte;
                                        cout<<"1iny : "<<dbg1<<endl;

                                        fwrite(&third_byte, 1, 1, output_file);
                                        fwrite(&second_byte ,1 ,1 ,output_file);
                                        fwrite(&first_byte, 1, 1, output_file);
                                }
                        }

                        fclose(output_file);

                }else{
                        cerr<<"your source-file is not utf16be"<<endl;
                        return 1;
                }

        }else{
                cout<<"not valid format"<<endl;
        }

        return 0;
}
