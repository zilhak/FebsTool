#include <wx/listctrl.h>

#ifndef IMAGESEARCHER_UI_FILEEXPLORER_H
#define IMAGESEARCHER_UI_FILEEXPLORER_H


class FileExplorer : public wxListCtrl{

public:
    FileExplorer(wxWindow * parent);
    ~FileExplorer();

private:
    void initialize();

};


#endif //IMAGESEARCHER_UI_FILEEXPLORER_H
