//
// Created by bogonets on 18. 3. 7.
//

#include <FebsToolApp.hpp>
#include <ui/SelectDialog.hpp>
#include <ui/detection/DetectionFrame.hpp>
#include <ui/segmentation/SegmentationFrame.hpp>
#include <ui/filemanager/FileManagerFrame.hpp>

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
    wxInitAllImageHandlers();
//    wxPNGHandler *png_handler = new wxPNGHandler();
//    wxImage::AddHandler(png_handler);

    SelectDialog * select_dialog = new SelectDialog();

    wxDialog * opened_dialog = nullptr;

    while (true) {
        select_dialog->ShowModal();
        int dialog_id = select_dialog->getSelectedID();

        switch (dialog_id) {
            case SelectDialog::ButtonID::CLASSIFY:
                break;
            case SelectDialog::ButtonID::DETECTION:
                opened_dialog = new DetectionFrame("Detection");
                break;
            case SelectDialog::ButtonID::SEGMENTATION:
                opened_dialog = new SegmentationFrame("Segmentation");
                break;
            case SelectDialog::ButtonID::FILE_MANAGER:
                opened_dialog = new FileManagerFrame("FileManager");
                break;
            default:
                opened_dialog = nullptr;
        }
        if (opened_dialog == nullptr || opened_dialog->ShowModal() == wxID_CANCEL) {
            break;
        }
        delete [] opened_dialog;
    }

    Exit(); // abnormal exit!
    return true;
}

//int FebsToolApp::OnExit()
//{
//    return 0;
//};