#include <wx/listctrl.h>
#include <vector>

#ifndef IMAGESEARCHER_UI_FILEEXPLORER_H
#define IMAGESEARCHER_UI_FILEEXPLORER_H


class FileExplorer : public wxListCtrl{

private:
    long _highlighted_item = -1;

private:
    wxString _directory;
    std::vector<wxString> _file_list;

public:
    FileExplorer(wxWindow * parent);
    ~FileExplorer();

private:
    void initialize();

public:
    void openDir(wxString dir, std::vector<wxString>);
    void deleteFile(wxString file_name);
    void refreshList();

public:
    void xmlCheck(wxString filename);
    long highlightItem(wxString filename);
    long highlightItem(long index);

public:
    wxString getHighlightedItem();
    long getHighlightedItemIndex();
};


#endif //IMAGESEARCHER_UI_FILEEXPLORER_H
