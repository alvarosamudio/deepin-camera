#ifndef IMAGESETTINGS_H
#define IMAGESETTINGS_H

#include <QAudioEncoderSettings>
#include <QVideoEncoderSettings>
#include <QEvent>

#include <DDialog>
#include <DComboBox>
#include <DSlider>

DWIDGET_USE_NAMESPACE

class QCameraImageCapture;

class ImageSettings : public DDialog
{
    Q_OBJECT

public:
    explicit ImageSettings(QCameraImageCapture *imageCapture, QWidget *parent = nullptr);
    ~ImageSettings();

    QImageEncoderSettings imageSettings() const;
    void setImageSettings(const QImageEncoderSettings &settings);

protected:
    void changeEvent(QEvent *e) override;

private:
    QVariant boxValue(const QComboBox *box) const;
    void selectComboBoxItem(QComboBox *box, const QVariant &value);

    DComboBox *imageCodecBox;
    DSlider *imageQualitySlider;
    DComboBox *imageResolutionBox;
    QCameraImageCapture *imagecapture;
};

#endif
