#include <QApplication>
#include <QDebug>
#include <QScreen>

#include "ImageEnhancer.h"

#include "HINProcessor.h"
#include "OpenCVInferencer.h"
#include "OpenCVImage2Tensor.h"
#include "OpenCVTensor2Image.h"

#include "MainWindow.h"

using namespace soica_test;

int main(int argc, char *argv[]) try
{
    QApplication a(argc, argv);

    // The python code normalize image to range [0, 1]
    // So will multiply on 1.0 / 255.0
    auto normalize_input_image_value = 1.0f / 255.0f;
    auto image_to_tensor = std::make_shared<backend::OpenCVImage2Tensor>(normalize_input_image_value);

    // The result tensor should be clamped to [0.0, 1.0] and scaled back to 255
    auto normalize_tensor_to_min_value = 0.0f;
    auto normalize_tensor_to_max_value = 1.0f;
    auto denormalize_result_image_value = 255.0f;
    auto tensor_to_image = std::make_shared<backend::OpenCVTensor2Image>(normalize_tensor_to_min_value,
                                                                         normalize_tensor_to_max_value,
                                                                         denormalize_result_image_value);

    // Hardcode the model path for simplicity
    auto onnx_model_path = "HINet-GoPro.onnx";
    // The CPU is only checked backend but others should work too
    auto inferencer = std::make_shared<backend::OpenCVInferencer>(onnx_model_path,
                                                                  backend::OpenCVInferencer::BackendType::CPU);

    // We need only second output from the model
    // It was called "output_2" during onnx conversation
    auto output_layer_name = "output_2";
    auto hin_processor = std::make_shared<backend::HINProcessor>(inferencer,
                                                                 image_to_tensor,
                                                                 tensor_to_image,
                                                                 output_layer_name);

    auto manager = std::make_shared<ImageEnhancer>(hin_processor);

    // Process the exception from the ImageEnhancer thread
    QObject::connect(manager.get(), &ImageEnhancer::exception_thrown, [](QString message){
        qDebug() << "The exception was thrown!" << message;
        exit(-1);
    });

    gui::MainWindow main_window;

    QObject::connect(&main_window, &gui::MainWindow::enhance_image,
                     manager.get(), &ImageEnhancer::on_enhance_image);
    QObject::connect(manager.get(), &ImageEnhancer::image_enhanced,
                     &main_window, &gui::MainWindow::on_image_enhanced);

    // Fix the size of the main window relative to the screen for simplicity
    auto screen_geometry = a.primaryScreen()->geometry();
    auto win_size = QSize(screen_geometry.width() - 40,
                          screen_geometry.height() * (2.0f / 3.0f));
    auto win_position = QPoint(20,
                               screen_geometry.height() * (1.0f / 6.0f));
    main_window.setFixedSize(win_size);
    main_window.move(win_position);
    main_window.show();

    return a.exec();
}
catch (...)
{
    qDebug() << "The unknown exception is occured!";
    exit(-1);
}
