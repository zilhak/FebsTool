//
// Created by bogonets on 18. 3. 7.
//

#include <FebsToolApp.hpp>
#include <ui/SelectDialog.hpp>
#include <ui/detection/DetectionFrame.hpp>
#include <ui/segmentation/SegmentationFrame.hpp>
#include <ui/filemanager/FileManagerFrame.hpp>
#include <memory>

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

    std::shared_ptr<SelectDialog> select_dialog(new SelectDialog());

    std::shared_ptr<wxDialog> opened_dialog = nullptr;

    while (true) {
        select_dialog->ShowModal();
        select_dialog->Hide();
        int dialog_id = select_dialog->getSelectedID();

        switch (dialog_id) {
            case SelectDialog::ButtonID::CLASSIFY:
                break;
            case SelectDialog::ButtonID::DETECTION:
                opened_dialog = std::shared_ptr<wxDialog>(new DetectionFrame("Detection"));
                break;
            case SelectDialog::ButtonID::SEGMENTATION:
                opened_dialog = std::shared_ptr<wxDialog>(new SegmentationFrame("Segmentation"));
                break;
            case SelectDialog::ButtonID::FILE_MANAGER:
                opened_dialog = std::shared_ptr<wxDialog>(new FileManagerFrame("FileManager"));
                break;
            default:
                opened_dialog = nullptr;
        }
        if (opened_dialog == nullptr || opened_dialog->ShowModal() == wxID_CANCEL) {
            break;
        }
    }

    return true;
}

int FebsToolApp::OnRun()
{
    return 0;
}