#ifndef __MUGUI_GUI_ABOUTDIALOG_H__
#define __MUGUI_GUI_ABOUTDIALOG_H__

#include <QDialog>
#include <ui_AboutDialog.h>

namespace MailUnit {
namespace Gui {

class AboutDialog : public QDialog, private Ui::AboutDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget * _parent = nullptr) :
        QDialog(_parent)
    {
        setupUi(this);
    }
}; // class AboutDialog

} // namespace Gui
} // namespace MailUnit

#endif // __MUGUI_GUI_ABOUTDIALOG_H__
