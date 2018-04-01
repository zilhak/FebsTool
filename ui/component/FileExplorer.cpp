//
// Created by bogonets on 2017. 12. 4..
//

#include <ui/component/FileExplorer.hpp>
#include <wx/dir.h>
#include <wx/filename.h>
#include <algorithm>

constexpr static const long HILIGHTED_COLOUR = 0x99FF99;

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

void FileExplorer::openDir(wxString directory, std::vector<wxString> ext_list)
{
    _file_list.clear();
    _directory = directory;

    wxDir dir(directory);
    wxString file;

    dir.GetFirst(&file);
    wxFileName file_name(directory + "/" + file);

    do {
        file_name.Assign(directory + "/" + file);
        for (auto const & ext : ext_list) {
            if (file_name.GetExt() == ext) {
                _file_list.push_back(file);
                break;
            }
        }
    } while (dir.GetNext(&file));

    std::sort(_file_list.begin(), _file_list.end());

    refreshList();
}

void FileExplorer::refreshList()
{
    DeleteAllItems();

    wxFileName file("");

    for (int index = 0; index < _file_list.size(); index++) {
        file.Assign(_directory + "/" + _file_list.at(index));
        InsertItem(index, _file_list.at(index));
        if (wxFileExists(_directory + "/" + file.GetName() + ".xml")) {
            SetItem(index, 1, ("O"));
        } else {
            SetItem(index, 1, ("X"));
        }
    }
}

void FileExplorer::deleteFile(wxString file)
{
    std::remove(_file_list.begin(), _file_list.end(), file);

    refreshList();

    if (_highlighted_item < _file_list.size()) {
        highlightItem(_highlighted_item);
    } else {
        _highlighted_item = _file_list.size() - 1;
        highlightItem(_highlighted_item);
    }
}

void FileExplorer::xmlCheck(wxString filename)
{
    SetItem(FindItem(0, filename), 1, "O");
}

long FileExplorer::highlightItem(wxString filename)
{
    if (_highlighted_item != -1) {
        SetItemTextColour(_highlighted_item, *wxBLACK);
    }

    _highlighted_item = FindItem(0, filename);
    SetItemTextColour(_highlighted_item, wxColour(HILIGHTED_COLOUR));

    Refresh();

    return _highlighted_item;
}

long FileExplorer::highlightItem(long index)
{
    if (_highlighted_item != -1) {
        SetItemTextColour(_highlighted_item, *wxBLACK);
    }

    _highlighted_item = index;
    SetItemTextColour(_highlighted_item, wxColour(HILIGHTED_COLOUR));

    Refresh();
}

long FileExplorer::getHighlightedItemIndex()
{
    return _highlighted_item;
}

wxString FileExplorer::getHighlightedItem()
{
    return _file_list.at(_highlighted_item);
}