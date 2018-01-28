#include <wx/listctrl.h>

#ifndef IMAGESEARCHER_UI_FILEEXPLORER_H
#define IMAGESEARCHER_UI_FILEEXPLORER_H


class FileExplorer : public wxListCtrl{

private:
    long _highlighted_item = -1;

public:
    FileExplorer(wxWindow * parent);
    ~FileExplorer();

private:
    void initialize();

public:
    void xmlCheck(wxString filename);
    long highlightItem(wxString filename);
};


#endif //IMAGESEARCHER_UI_FILEEXPLORER_H
