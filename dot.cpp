#include <iostream>
#include <fstream>
 
#include <curl/curl.h>
 
static size_t Stringify(void*, size_t, size_t, void*);

int Dot()
{
  //prototypes
  void Header(std::ofstream*);
  void Footer(std::ofstream*);
  void Node(CURL*, std::ofstream*, std::string, unsigned int);

  //constants
  const char* DOT_NAME = "map.dot";
  const std::string SEED = "Bowstring";

  //objects and variables
  std::string html;
  CURL *curl_handle;
  std::ofstream dot;
  unsigned int depth;
 
  curl_global_init(CURL_GLOBAL_ALL);
  /* init the curl session */ 
  curl_handle = curl_easy_init();
  //follow redirection
  curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
  /* Switch on full protocol/debug output while testing */ 
  curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 0L);
  /* disable progress meter, set to 0L to enable and disable debug output */ 
  curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 0L);
  /* send all data to this function  */ 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, Stringify);

  //open file
  dot.open(DOT_NAME);
  if (dot.fail())
  {
    std::cerr << "unable to open :\"" << DOT_NAME << "\"\n";
    return 1;
  }

  //write the header
  Header(&dot);
  //write the nodes with recursion
  std::cout << "Select the depth: ";
  std::cin >> depth;
  Node(curl_handle, &dot, SEED, depth);
  //write the footer
  Footer(&dot);

  //close file
  dot.close();
  /* cleanup curl stuff */ 
  curl_easy_cleanup(curl_handle);

 
  return 0;
}
//-------------------------------------------------------------------
void Ref_Array(std::string* ref, std::string array[], int n)
{
  size_t mark[2] = {0,0};

  for(int a=0; a<n; a++)
  {
    mark[1] = (ref->find("\n",mark[0]+1));
    if(mark[1] == std::string::npos)
      mark[1] = ref->length();
    array[a] = ref->substr(mark[0],mark[1]-mark[0]);
    mark[0] = mark[1]+1;
  }
  return;
}
int Ref(std::string* html, std::string* references, size_t pos=0)
{
  //prototypes
  bool CleanRef(std::string*);
  //constants
  const std::string refFlag = "<a href=";
  //variables+obj
  size_t ref[2];
  bool valid;
  std::string temp;

  //find the beginning
  ref[0] = html->find(refFlag,pos)/* + refFlag.length()+1*/;
  if(ref[0] == std::string::npos)
    return 0;
  ref[0] += refFlag.length()+1;
  //find the end
  ref[1] = html->find("\"", ref[0]);
  if(ref[1] == std::string::npos)
    return 0;

  //store the append
  temp = html->substr(ref[0], ref[1]-ref[0]);
  valid = CleanRef(&temp);
  if (valid)
  {
    if (references->length() !=0) references->append("\n");
    references->append(temp);
  }
  return Ref(html,references,ref[1]) + valid;
}
bool CleanRef(std::string* ref)
{
  //constants
  const std::string internal = "/wiki/";
  const std::string invalid[] = {"Special:", "Portal:", "Category:", "Template:", "File:", "Wikipedia:", "Help:", "Talk:", "User:", "User_talk:", "Main_Page"};
  const int invalid_size = sizeof(invalid)/sizeof(std::string);

  if (ref->find(internal) == 0) {
    *ref = ref->substr(internal.length(), ref->length());
  } else return 0;

  for(int a=0; a<invalid_size; a++)
  {
    if(ref->find(invalid[a]) != std::string::npos)
      return 0;
  }
  return 1;
}
//-------------------------------------------------------------------
static size_t Stringify(void *contents, size_t size, size_t nmemb, void *stream)
{
  //string::append(char*, size_t);
  ((std::string*)stream)->append((char*)contents, size*nmemb);
  return size*nmemb; //return the amount written
}
//-------------------------------------------------------------------
void Header(std::ofstream* dot)
{
  *dot << "digraph start {\n";
  //*dot << "\tgraph [overlap=false];\n";
  *dot << "graph [ \n";
  *dot << "# fontname = \"Helvetica\",\n";
  *dot << "  #    fontsize = 1,\n";
  *dot << "      splines=true,\n";
  *dot << "  #    maxiter=100,\n";
  *dot << "  #    overlap=scale,\n";
  *dot << "  #    ratio=.5\n";
  *dot << "    root = \"Bowstring\",\n";
  *dot << "    ranksep = 20,\n";
  *dot << "    nodesep = 10,\n";
  *dot << "    nslimit1 = 1\n";
  *dot << "  ];\n";
  *dot << "  node [\n";
  *dot << "    shape = point\n";
  *dot << "  #    fontname = \"Helvetica\" \n";
  *dot << "  ];\n";
  *dot << "  edge [\n";
  *dot << "   arrowsize=.5\n";
  *dot << "  ];\n";
  return;
}
void Footer(std::ofstream* dot)
{
  *dot << "}";
  return;
}
//XXX may make this better by pointing to the url instead...
void Node(CURL* curl_handle, std::ofstream* dot, std::string url, unsigned int depth)
{
  //prototypes
  int Ref(std::string*, std::string*, size_t pos=0);
  void Ref_Array(std::string*, std::string[], int);
  void Line(std::ofstream*, std::string, std::string);
  void SubGraph(std::ofstream*, std::string, std::string[], const int);
  const char* URLize(std::string ref);

  //variables
  std::string ref = "";
  std::string html;

    //write page body to the html string
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &html);
  curl_easy_setopt(curl_handle, CURLOPT_URL, URLize(url));
  curl_easy_perform(curl_handle);

  //build reference array
  const int NUM = Ref(&html,&ref);
  std::string reference[NUM];
  Ref_Array(&ref, reference, NUM);

  //free up some memory
  // delete ref;
  // delete html;

  //enter file writing + recursion

  SubGraph(dot, url, reference, NUM);
  for(int a=0; a<NUM; a++)
  {
    //Line(dot, url, reference[a]); //write to file
    if (depth>0)
      Node(curl_handle, dot, reference[a], depth-1); //recurse
  }
  return;

}
void Line(std::ofstream* dot, std::string parent, std::string child)
{
  *dot << "\t\"" << parent << "\" -> \"" << child << "\";\n";
  return;
}
void SubGraph(std::ofstream* dot, std::string parent, std::string child[], const int NUM)
{
  *dot << "\t\"" << parent << "\" -> {\n";
  for(int a=0; a<NUM; a++)
    *dot << "\t\t\"" << child[a] << "\"\n";
  *dot << "\t};\n";
}
const char* URLize(std::string ref)
{
  const std::string PREFIX = "https://en.wikipedia.org/wiki/";
  std::string url = PREFIX + ref;
  return url.c_str();
}