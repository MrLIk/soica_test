#include "MainWindow.h"

#include <QDebug>
#include <QPushButton>
#include <QFileSystemModel>
#include <QTreeView>
#include <QLabel>
#include <QHBoxLayout>

namespace soica_test::gui {

// Will work only with those files
const QStringList MainWindow::image_file_extentions {"*.png", "*.jpg", ".jpeg", "*.bmp"};

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // May be should be moved out of the form
    auto model = new QFileSystemModel(this);
    model->setRootPath(QDir::currentPath());
    model->setNameFilterDisables(false);
    model->setNameFilters(image_file_extentions);

    auto tree = new QTreeView(this);
    tree->setModel(model);

    // Hack
    for (int i = 1; i < model->columnCount(); ++i) {
        tree->hideColumn(i);
    }

    // It is possible to split into separate files
    auto central_widget = new QWidget(this);
    auto central_layout = new QHBoxLayout(central_widget);

    auto processing_widget = new QWidget(this);
    auto processing_widget_layout = new QVBoxLayout(processing_widget);

    auto images_widget = new QWidget(this);
    auto images_widget_layout = new QHBoxLayout(images_widget);

    auto input_image_label = new QLabel(this);
    input_image_label->setStyleSheet("background-color: black;");
    input_image_label->setAlignment(Qt::AlignCenter);

    output_image_label = new QLabel(this);
    output_image_label->setStyleSheet("background-color: black;");
    output_image_label->setAlignment(Qt::AlignCenter);

    // Process the selection of the file from the TreeView
    connect(tree, &QTreeView::clicked, [this, model, input_image_label](const QModelIndex &index){
        // Take the current file path
        auto image_path = model->filePath(index);
        auto image = QImage();
        if (!image.load(image_path)) {
            throw std::runtime_error("Can not load image: " + image_path.toStdString());
        }

        // Remember the current image to use in future
        current_input_image = image;
        auto pixmap = QPixmap::fromImage(image).scaled(input_image_label->size(),
                                                       Qt::KeepAspectRatio);

        // Show the input image
        input_image_label->setPixmap(pixmap);
        output_image_label->clear();
    });

    images_widget_layout->addWidget(input_image_label);
    images_widget_layout->addWidget(output_image_label);

    auto process_button = new QPushButton("Process image");

    // Start the image processing
    connect(process_button, &QPushButton::pressed, [this](){
        // Disable whole window to not reselect other file
        this->setEnabled(false);
        // Seng image to processor
        enhance_image(current_input_image);
    });

    processing_widget_layout->addWidget(images_widget);
    processing_widget_layout->addWidget(process_button);

    // We want the TreeView be smaler then images area
    central_layout->addWidget(tree, 1);
    central_layout->addWidget(processing_widget, 5);

    setCentralWidget(central_widget);
}

void MainWindow::on_image_enhanced(QImage image)
{
    // Show the resulting ia=mage
    auto pixmap = QPixmap::fromImage(image).scaled(output_image_label->size(),
                                                   Qt::KeepAspectRatio);
    output_image_label->setPixmap(pixmap);

    // Enable form again
    setEnabled(true);
}

}
