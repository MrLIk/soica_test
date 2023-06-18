#include "ImageEnhancer.h"

#include "RawRGBImage.h"

#include <QDebug>

namespace soica_test {

ImageEnhancer::ImageEnhancer(std::shared_ptr<backend::IImageProcessor> image_processor,
                             QObject* parent)
    : QObject(parent)
    , image_processor(image_processor)
{
    thread.setObjectName("ImageEnhancer");
    this->moveToThread(&thread);
    thread.start();
}

ImageEnhancer::~ImageEnhancer()
{
    thread.quit();
    thread.wait();
}

void ImageEnhancer::on_enhance_image(QImage image_qt)
{
    // This may copy data
    // It should be usefull to add IImage wrapper class
    image_qt.convertTo(QImage::Format::Format_RGB888);

    auto image_ptr = std::make_unique<uint8_t[]>(image_qt.sizeInBytes());
    std::copy(image_qt.bits(),
              image_qt.bits() + image_qt.sizeInBytes(),
              image_ptr.get());
    auto image = std::make_shared<backend::RawRGBImage>(std::move(image_ptr),
                                                        image_qt.width(),
                                                        image_qt.height(),
                                                        size_t(image_qt.bytesPerLine()));
    QElapsedTimer timer;
    qDebug() << "Start processing ...";
    timer.start();
    auto res_image = image_processor->process(image);
    qDebug() << "Processing is finished! Time in ms:" << timer.elapsed();

    auto res_image_qt = QImage(res_image->data(),
                               res_image->width(),
                               res_image->height(),
                               res_image->linesize(),
                               QImage::Format::Format_RGB888).copy();

    image_enhanced(res_image_qt);
}

} // namespace soica_test
