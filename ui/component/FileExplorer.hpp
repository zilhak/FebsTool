#include <wx/listctrl.h>
#include <vector>

#ifndef IMAGESEARCHER_UI_FILEEXPLORER_H
#define IMAGESEARCHER_UI_FILEEXPLORER_H


class FileExplorer : public wxListCtrl{
public:
    DECLARE_EVENT_TABLE();

private:
    long _highlighted_item = -1;

private:
    wxString _directory;
    std::vector<wxString> _ext_list;
    std::vector<wxString> _file_list;

public:
    FileExplorer(wxWindow * parent);
    ~FileExplorer();

private:
    void initialize();

public:
    void openDir(wxString dir, std::vector<wxString> ext);
    void reopenDir();
    void deleteFile(wxString file_name);
    void refreshList();

public:
    void xmlCheck(wxString filename, bool check_mark = true);
    void xmlCheck(bool check_mark = true);
    long highlightItem(wxString filename);
    long highlightItem(long index);

public:
    wxString getHighlightedItem();
    long getHighlightedItemIndex();

public:
    bool prev();
    bool next();

public:
    void onSelect(wxListEvent & event);
    void onSize(wxSizeEvent & event);
};


#endif //IMAGESEARCHER_UI_FILEEXPLORER_H
