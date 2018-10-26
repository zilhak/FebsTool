

#include <ui/filemanager/ManagerPanel.hpp>

BEGIN_EVENT_TABLE(ManagerPanel, wxPanel)
    EVT_PAINT(ManagerPanel::onPaint)
END_EVENT_TABLE()

ManagerPanel::ManagerPanel(wxWindow * parent, wxWindowID id) : wxPanel(parent, id)
{
    initialize();
}

ManagerPanel::~ManagerPanel()
{
    /* EMPTY */
}

void ManagerPanel::initialize()
{
    SetBackgroundColour(*wxWHITE);
}

void ManagerPanel::onMouse(wxMouseEvent & event)
{


    event.ResumePropagation(wxEVENT_PROPAGATE_MAX);
    event.Skip();
}

void ManagerPanel::onPaint(wxPaintEvent & event)
{
    if (!_load) {
        return;
    }
    wxPaintDC dc(this);
    wxBufferedDC bg(&dc);
    bg.Clear();

    drawImages(bg);
}

void ManagerPanel::drawImages(wxDC & dc)
{
    for (int i = 0; i < _resized_bitmap_vector.size(); ++i) {
        dc.DrawBitmap(_resized_bitmap_vector[i], _image_pos_vector[i].GetPosition());
    }
}

void ManagerPanel::setPath(wxString const & path)
{
    _path = path;
}

void ManagerPanel::setImages(std::vector<wxString> const & file_list)
{
    if (file_list.empty()) {
        return;
    }

    _image_vector.clear();

    int max_size = _row * _col;
    int num = static_cast<int>(file_list.size());

    if (num > max_size) {
        num = max_size;
    }

    for (int i = 0; i < num; ++i) {
        wxImage(_path + "/" + file_list[i]);
        _image_vector.emplace_back(wxImage(_path + "/" + file_list[i]));
    }

    _resized_bitmap_vector.clear();

    for (auto const & image : _image_vector) {
        _resized_bitmap_vector.emplace_back(wxBitmap(image.Scale(_width, _height, wxIMAGE_QUALITY_BOX_AVERAGE)));
    }
}

void ManagerPanel::setGrid(int row, int col)
{
    _row = row;
    _col = col;
    _image_pos_vector.clear();
    std::cout << GetSize().GetWidth() << std::endl;
    std::cout << GetSize().GetHeight() << std::endl;
    _width = GetSize().GetWidth() / col - _space_x * (col + 1) / col;
    _height = GetSize().GetHeight() / row - _space_y * (row + 1) / row;

    for (int i = 0; i < _row; i++) {
        for (int j = 0; j < _col; j++) {
            _image_pos_vector.emplace_back(wxRect((_width + _space_x) * j + _space_x,
                                                  (_height + _space_y) * i + _space_y, _width, _height));
        }
    }
}

bool ManagerPanel::teardown()
{
    _iter = 0;
    _load = true;

    Refresh();
}