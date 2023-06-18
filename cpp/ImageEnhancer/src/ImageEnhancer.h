#pragma once

#include "HINProcessor.h"

#include <QImage>
#include <QObject>
#include <QThread>

namespace soica_test {

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

private:
    std::shared_ptr<backend::IImageProcessor> image_processor;

    QThread thread;
};

} // namespace soica_test
