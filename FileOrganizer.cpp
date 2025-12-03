#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <algorithm>
using namespace std;
namespace fs = std::filesystem;

//Categorizing
unordered_map<string, vector<string>> categoryGroups = {
    {"Images", {".jpg", ".jpeg", ".png", ".bmp", ".gif", ".webp"}},
    {"Documents", {".pdf", ".docx", ".xlsx", ".txt", ".pptx"}},
    {"Music", {".mp3", ".wav", ".flac", ".aac"}},
    {"Videos", {".mp4", ".avi", ".mkv", ".mov"}},
    {"Archives", {".zip", ".rar", ".7z"}},
    {"SourceCode", {".cpp", ".h", ".hpp", ".py", ".js", ".java"}},
    {"Apps", {".exe", ".msi"}},
};

// Get category based on extension
string getCategory(const string& ext) {
    for (const auto& [category, extList] : categoryGroups) {
        if (find(extList.begin(), extList.end(), ext) != extList.end())
            return category;
    }
    return "Others";
}

//Name repetition func
fs::path getUniquePath(const fs::path& targetPath) {
    fs::path newPath = targetPath;
    int counter = 1;

    while (fs::exists(newPath)) {
        newPath = targetPath.parent_path() /
            (targetPath.stem().string() +
                " (" + to_string(counter) + ")" +
                targetPath.extension().string());
        counter++;
    }

    return newPath;
}

int main()
{
	
	// Get directory path from user
    cout << "Enter the path of the directory to organize: ";
    string ipath; getline(cin, ipath);
    fs::path sourceDir = ipath;
	
    // Validate directory
    if (!fs::exists(sourceDir) || !fs::is_directory(sourceDir)) {
        cout << "Invalid directory path." << endl;
        return EXIT_FAILURE;
    }


	// Iterate through files
    for (const auto& entry : fs::directory_iterator(sourceDir)) {
        if (fs::is_regular_file(entry.path())) {
            string ext = entry.path().extension().string();
            transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            
            string category = getCategory(ext);
            fs::path targetDir = sourceDir / category;

                if (!fs::exists(targetDir)) {
                    fs::create_directory(targetDir);
                }
                fs::path targetPath = targetDir / entry.path().filename();
                fs::path finalPath = getUniquePath(targetPath);
				
                //Name repetition handling
                try {
                        fs::rename(entry.path(), finalPath);
                        cout << "Moved: " << entry.path().filename().string()
                            << " to " << finalPath.string() << endl;                    
                }
                catch (const fs::filesystem_error& e) {
                    cout << "Error moving file: " << e.what() << endl;
                }
            }
        }
    }
    
cout << "Complete." << endl;

    return 0;
}
