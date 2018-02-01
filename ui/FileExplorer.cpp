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
    SetColumnWidth(0, 190);
    InsertColumn(1, _("Check"));
    SetColumnWidth(1, 60);
}

void FileExplorer::xmlCheck(wxString filename)
{
    SetItem(FindItem(0, filename), 1, "O");
}

long FileExplorer::highlightItem(wxString filename)
{
    SetItemTextColour(_highlighted_item, *wxBLACK);

    _highlighted_item = FindItem(0, filename);
    SetItemTextColour(_highlighted_item, wxColour(0x99FF99));

    return _highlighted_item;
}
