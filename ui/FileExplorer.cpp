//
// Created by bogonets on 2017. 12. 4..
//

#include "FileExplorer.hpp"

FileExplorer::FileExplorer(wxWindow * parent) : wxListCtrl(parent,
                                                           wxID_ANY,
                                                           wxDefaultPosition,
                                                           wxSize(250, 100),
                                                           wxLC_REPORT)
{
    initialize();
}

FileExplorer::~FileExplorer()
{
    /* EMPTY */
}

void FileExplorer::initialize()
{
    SetBackgroundColour(wxColour(0xCCCCCC));
    InsertColumn(0, _("FileName"));
    SetColumnWidth(0, 200);
    InsertColumn(1, _("Check"));
    SetColumnWidth(1, 50);
}