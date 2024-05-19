

#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <quan/fs/get_file_dir.hpp>
#include <quan/fs/get_wkg_dir.hpp>

// for sed do transforms
std::string transform_text(const char* text)
{

    std::string str ="";
// asssume not extended syntax
    for (const char * ptr = text; *ptr != '\0'; ++ptr){

         if ( *ptr == '/'){
             str += "\\/";
         }else {
             str += *ptr;
         }
    }
    return str;
}

int main(int argc, const char * argv[])
{
     if(argc < 3) {
				std::cout << "useage : replacetxt srctext tgttext [ comma-separated-extensions-list ] \n";
           return 0;
      }

     std::string extensions = "";

     if (argc >= 4){
		  std::size_t const len = strlen(argv[3]) + 1;
		  char* parse = new char [len];
		  strcpy (parse,argv[3]);
		  const char delims[] = ", \t";
		  
		  const char* ptr = strtok(parse,delims);
		  int count = 0;
		  while(ptr !=0) {
			 std::string str2 = ptr;
			 if(count == 0){
				 extensions = " \\( -name '" + str2 + "'";
			 }
			 else {
				 extensions += " -o -name '"  + str2 + "'";
			 }
			 ++count;
			 ptr =  strtok(NULL,delims);
		  }
		  extensions += " \\)";
        delete [] parse;
     }else {
     		extensions = " *"; // replace everywhere
         // could see if its a text file?
     } 

     std::string sed_string = "s/" +  transform_text(argv[1]) + "/" + transform_text(argv[2]) + "/g";

     // use extended regular expressions? Not currently
     // could add -r switch?
    
     std::string cmd = "find " +  quan::fs::get_wkg_dir() + extensions + " -exec sed -i -r '" + sed_string + "' {} \\;" ;

     std::cout << "replacetxt \""  << argv[1]  << "\" with \"" << argv[2] << "\" in  \"" << argv[3] << "\"" << std::endl;

     system(cmd.c_str());
     
}
