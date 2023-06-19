#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>

#include "ImageEnhancer.h"

#include "HINProcessor.h"
#include "OpenCVInferencer.h"
#include "OpenCVImage2Tensor.h"
#include "OpenCVTensor2Image.h"

#include <iostream>
#include <exception>

using namespace soica_test;

int main(int argc, char *argv[]) try
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("console_enhancer");

    QCommandLineParser parser;
    parser.setApplicationDescription("Command line image enhancer");
    parser.addHelpOption();
    parser.addOptions({
        { "onnx_model_path", "Path to the onnx model", "onnx_model_path" },
        { "input_image_path", "Input image path", "input_image_path" },
        { "output_image_path", "Output image path", "output_image_path" }
    });
    parser.process(a);

    if (!parser.isSet("onnx_model_path")) {
        qDebug() << "onnx_model_path is not set!";
        parser.showHelp();
    }
    auto onnx_model_path = parser.value("onnx_model_path");

    if (!parser.isSet("input_image_path")) {
        qDebug() << "input_image_path is not set!";
        parser.showHelp();
    }
    auto input_image_path = parser.value("input_image_path");

    if (!parser.isSet("output_image_path")) {
        qDebug() << "output_image_path is not set!";
        parser.showHelp();
    }
    auto output_image_path = parser.value("output_image_path");

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

    // The CPU is only checked backend but others should work too
    auto inferencer = std::make_shared<backend::OpenCVInferencer>(onnx_model_path.toStdString(),
                                                                  backend::OpenCVInferencer::BackendType::CPU);

    // We need only second output from the model
    // It was called "output_2" during onnx conversation
    auto output_layer_name = "output_2";
    auto hin_processor = std::make_shared<backend::HINProcessor>(inferencer,
                                                                 image_to_tensor,
                                                                 tensor_to_image,
                                                                 output_layer_name);

    auto manager = std::make_shared<ImageEnhancer>(hin_processor);

    QImage image;
    if (!image.load(input_image_path)) {
        throw std::runtime_error("Can not load image " + input_image_path.toStdString());
    }

    qDebug() << "Load image" << input_image_path;

    // Will exit when receive the result image
    auto on_image_enhanced = [&output_image_path](QImage image){
        qDebug() << "Receive the image_enhanced signal!";
        if (!image.save(output_image_path)) {
            throw std::runtime_error("Can not save result image! Path: " + output_image_path.toStdString());
        }
        qDebug() << "Result image saved:" << output_image_path;
        exit(0);
    };
    QObject::connect(manager.get(), &ImageEnhancer::image_enhanced, on_image_enhanced);

    manager->on_enhance_image(image);

    return a.exec();
}
catch (const std::exception& e)
{
    qDebug() << "The exception is occuired!";
    qDebug() << e.what();
    exit(-1);
}
catch (...)
{
    qDebug() << "The unknown exception is occured!";
    exit(-1);
}
