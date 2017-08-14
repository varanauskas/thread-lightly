#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

const char kInputFile[] = "messages.htm";

const char kHeader[] = "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" /><link rel=\"stylesheet\" href=\"../html/style.css\" type=\"text/css\" /></head><body>";
const char kFooter[] = "</body></html>";
const char kThreadStart[] = "<div class=\"thread\">";
const char kMessageStart[] = "<div class=\"message\">";
const char kMessageHeaderStart[] = "<div class=\"message_header\">";
const char kEnd[] = "</div>";

int main() {
  char buffer;
  int depth = 0, i = 0;
  string tag, thread;
  bool inTag = false,
    inThread = false,
    inParticipant = false;

  ifstream src(kInputFile);

  while (src.get(buffer)) {
    if (buffer == '<') inTag = true; // Start of tag

    if (inTag) tag += buffer;
    if (inThread) thread += buffer;

    // End of tag
    if (buffer == '>') {

      // <.thread>
      if (tag == kThreadStart) {
        inThread = true;
        thread += tag;
      }
      
      // <.thread>...</.thead>
      if (inThread) {

        // <.message> || <.message_header>
        if (tag == kMessageStart || tag == kMessageHeaderStart) depth++;

        // </>
        else if (tag == kEnd) {
          
          // </.message> || </.message_header>
          if (depth > 0) depth--;
          
          // </.thread>
          else {
            stringstream fileName;
            fileName << "thread_" << i << ".html"; i++;
            ofstream out(fileName.str().c_str());

            out << kHeader << thread << kFooter;

            out.close();
            
            thread.clear();
            inThread = false;
          }
        }
      }

      tag.clear();
      inTag = false;
    }
  }

  src.close();

  return 0;
}