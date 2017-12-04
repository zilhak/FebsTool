//
// Created by bogonets on 2017. 12. 4..
//

#include "FileExplorer.hpp"

FileExplorer::FileExplorer() : wxListCtrl(NULL)
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
    AppendColumn(wxT("Name"));
    AppendColumn(wxT("check"));
    AppendColumn(wxT(""));
}