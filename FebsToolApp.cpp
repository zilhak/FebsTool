//
// Created by bogonets on 18. 3. 7.
//

#include "FebsToolApp.hpp"
#include "ui/SelectDialog.hpp"

FebsToolApp::FebsToolApp() : wxApp()
{
    /* EMPTY */
}

FebsToolApp::~FebsToolApp()
{
    /* EMPTY */
}

bool FebsToolApp::OnInit()
{
    bool running = true;

    wxPNGHandler *png_handler = new wxPNGHandler();
    wxImage::AddHandler(png_handler);

    SelectDialog * select_dialog = new SelectDialog();
    select_dialog->ShowModal();

    wxDialog * opened_dialog = nullptr;
    int dialog_id = select_dialog->getSelectedID();

    while (true) {
        switch (dialog_id) {
            case 1100:
                break;
        }
        if (opened_dialog == nullptr || opened_dialog->ShowModal() == wxID_CLOSE) {
            break;
        }
    }

    return true;
}