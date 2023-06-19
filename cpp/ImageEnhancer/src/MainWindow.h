#pragma once

#include <QMainWindow>
#include <QLabel>

namespace soica_test::gui {

/*
 * Main window of the application
 */


class MainWindow : public QMainWindow
{
    Q_OBJECT

    static const QStringList image_file_extentions;

public:
    explicit MainWindow(QWidget* parent = nullptr);

public slots:
    void on_image_enhanced(QImage image);

signals:
    void enhance_image(QImage image);

private:
    QImage current_input_image;
    QLabel* output_image_label;

};

} // namespace gui
