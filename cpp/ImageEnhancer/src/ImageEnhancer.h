#pragma once

#include "HINProcessor.h"

#include <QImage>
#include <QObject>
#include <QThread>

namespace soica_test {

/*
 * This class is a "bridge" between UI and backend
 */

class ImageEnhancer : public QObject
{
    Q_OBJECT   
public:
    explicit ImageEnhancer(std::shared_ptr<backend::IImageProcessor> image_processor,
                           QObject *parent = nullptr);
    ~ImageEnhancer() override;

public slots:
    // Pass by value because QImage uses implicit data sharing
    void on_enhance_image(QImage image_qt);

signals:
    // Pass by value because QImage uses implicit data sharing
    void image_enhanced(QImage image);
    // To pass exception message from inner thread
    void exception_thrown(QString message);

private:
    // This is hack: resize the input image to 1280x720 or 720x1280 if it is larger than that
    static constexpr std::size_t side_longest_length = 1280;
    static constexpr std::size_t side_shortest_length = 720;
    std::shared_ptr<backend::IImageProcessor> image_processor;

    // Use thread to be able to process separatly from UI
    QThread thread;
};

} // namespace soica_test
