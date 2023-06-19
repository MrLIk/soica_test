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

void ImageEnhancer::on_enhance_image(QImage image_qt) try
{
    // This may copy data
    // It should be usefull to add IImage wrapper class
    // Restrict to work only with RGB 3 channels
    image_qt.convertTo(QImage::Format::Format_RGB888);

    // Save backend from to large image
    auto orig_width = image_qt.width();
    auto orig_height = image_qt.height();
    auto orig_size = QSize(orig_width, orig_height);
    auto new_size = orig_width >= orig_height
            ? QSize(side_longest_length, side_shortest_length)
            : QSize(side_shortest_length, side_longest_length);

    QImage scaled_image = image_qt;
    if (orig_size != new_size) {
        qDebug() << "Rescale input to" << new_size;
        scaled_image = scaled_image.scaled(new_size);
    }

    // Copy the QImage to the RawRGBImage
    // It should be rewritten to use some other non copy wrapper class
    auto image_ptr = std::make_unique<uint8_t[]>(scaled_image.sizeInBytes());
    std::copy(scaled_image.bits(),
              scaled_image.bits() + scaled_image.sizeInBytes(),
              image_ptr.get());
    auto image = std::make_shared<backend::RawRGBImage>(std::move(image_ptr),
                                                        scaled_image.width(),
                                                        scaled_image.height(),
                                                        size_t(scaled_image.bytesPerLine()));
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

    // Restore the original size
    if (res_image_qt.size() != orig_size) {
        qDebug() << "Rescale result image to" << orig_size;
        res_image_qt = res_image_qt.scaled(orig_size);
    }

    image_enhanced(res_image_qt);
}
catch(const std::exception& e)
{
    exception_thrown(e.what());
}
catch(...)
{
    exception_thrown("Unknown exception!");
}

} // namespace soica_test
