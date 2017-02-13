#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <algorithm>

using namespace std;

void ProcessDirectory(string directory,string word);
void ProcessFile(string file,string word);
void ProcessEntity(struct dirent* entity,string word);
bool hasEnding (string const &fullString, string const &ending);
int stringMatchCount(string file, string word);

string path = "/home/skon/books/";
int fileCount = 0;
int matchCount = 0;
int fileMatchCount = 0;
int wordCount = 0;
int main()
{
  string word;
  string directory = "";
  cout << "Word to search for: ";
  cin >> word;
  // Convert to lower case
  transform(word.begin(), word.end(), word.begin(), ::tolower);
  ProcessDirectory(directory,word);
  cout << "The word \"" << word << "\" found " << matchCount << " times in " << fileMatchCount << " books and " << wordCount << " words" << endl; 
  cout << "Total Books:" << fileCount << endl;
  return 0;
}

bool hasEnding (string const &fullString, string const &ending) {
  if (fullString.length() >= ending.length()) {
    return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
  } else {
    return false;
  }
}

void ProcessDirectory(string directory, string word)
{
  string dirToOpen = path + directory;
  auto dir = opendir(dirToOpen.c_str());

  //set the new path for the content of the directory
  path = dirToOpen + "/";

  //  cout << "Process directory: " << dirToOpen.c_str() << endl;

  if(NULL == dir)
    {
      cout << "could not open directory: " << dirToOpen.c_str() << endl;
      return;
    }

  auto entity = readdir(dir);

  while(entity != NULL)
    {
      ProcessEntity(entity,word);
      entity = readdir(dir);
    }

  //we finished with the directory so remove it from the path
  path.resize(path.length() - 1 - directory.length());
  closedir(dir);
}

void ProcessEntity(struct dirent* entity, string word)
{
  //find entity type
  if(entity->d_type == DT_DIR)
    {//it's an direcotry
      //don't process the  '..' and the '.' directories
      if(entity->d_name[0] == '.')
	{
	  return;
	}

      //it's an directory so process it
      ProcessDirectory(string(entity->d_name),word);
      return;
    }

  if(entity->d_type == DT_REG)
    {//regular file
      ProcessFile(string(entity->d_name), word);
      return;
    }

  //there are some other types
  //read here http://linux.die.net/man/3/readdir
  cout << "Not a file or directory: " << entity->d_name << endl;
}

void ProcessFile(string file, string word)
{
  string fileType = ".txt";
  if (hasEnding(file,fileType)) {
      fileCount++;
      if (word.length()>0) {
	int matches = stringMatchCount(file,word);
	if (matches > 0) {
	  fileMatchCount++;
	  matchCount += matches;
	  cout << "   " << path << file;
	  cout << " " << matches << endl;;
	}
      }
  }
  //if you want to do something with the file add your code here
}

int stringMatchCount(string file, string word) {
  ifstream infile;
  int count = 0;
  string w;

  try{
    infile.open(path+file);

    while(infile>>w){
      transform(w.begin(), w.end(), w.begin(), ::tolower);
      wordCount++;
      if(word.compare(w) == 0) ++count;
    }

    infile.close();
  }catch(ifstream::failure e){
    //cout<<e<<endl;
  }
  return count;
}
