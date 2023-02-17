#include "imagesettings.h"

#include <QComboBox>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QDebug>
#include <QCameraImageCapture>
#include <QMediaService>
#include <QPushButton>

ImageSettings::ImageSettings(QCameraImageCapture *imageCapture, QWidget *parent) :
    DDialog(parent),
    imagecapture(imageCapture)
{
    setTitle(tr("Image Settings"));
    setIcon(QIcon::fromTheme("camera-photo"));

    QWidget *widget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(widget);

    QGroupBox *groupBox = new QGroupBox(tr("Image"), widget);
    QFormLayout *formLayout = new QFormLayout(groupBox);

    imageResolutionBox = new QComboBox(groupBox);
    imageResolutionBox->addItem(tr("Default Resolution"));
    const QList<QSize> supportedResolutions = imagecapture->supportedResolutions();
    for (const QSize &resolution : supportedResolutions) {
        imageResolutionBox->addItem(QString("%1x%2").arg(resolution.width()).arg(resolution.height()),
                                    QVariant(resolution));
    }
    formLayout->addRow(tr("Resolution:"), imageResolutionBox);

    imageCodecBox = new QComboBox(groupBox);
    imageCodecBox->addItem(tr("Default image format"), QVariant(QString()));
    const QStringList supportedImageCodecs = imagecapture->supportedImageCodecs();
    for (const QString &codecName : supportedImageCodecs) {
        QString description = imagecapture->imageCodecDescription(codecName);
        imageCodecBox->addItem(codecName + ": " + description, QVariant(codecName));
    }
    formLayout->addRow(tr("Image Format:"), imageCodecBox);

    imageQualitySlider = new QSlider(Qt::Horizontal, groupBox);
    imageQualitySlider->setRange(0, int(QMultimedia::VeryHighQuality));
    formLayout->addRow(tr("Quality:"), imageQualitySlider);

    mainLayout->addWidget(groupBox);
    addContent(widget);

    QPushButton *okButton = new QPushButton(tr("OK"));
    QPushButton *cancelButton = new QPushButton(tr("Cancel"));
    addButton(okButton, true);
    addButton(cancelButton, false);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

ImageSettings::~ImageSettings()
{
}

void ImageSettings::changeEvent(QEvent *e)
{
    DDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        break;
    default:
        break;
    }
}

QImageEncoderSettings ImageSettings::imageSettings() const
{
    QImageEncoderSettings settings = imagecapture->encodingSettings();
    settings.setCodec(boxValue(imageCodecBox).toString());
    settings.setQuality(QMultimedia::EncodingQuality(imageQualitySlider->value()));
    settings.setResolution(boxValue(imageResolutionBox).toSize());

    return settings;
}

void ImageSettings::setImageSettings(const QImageEncoderSettings &imageSettings)
{
    selectComboBoxItem(imageCodecBox, QVariant(imageSettings.codec()));
    selectComboBoxItem(imageResolutionBox, QVariant(imageSettings.resolution()));
    imageQualitySlider->setValue(imageSettings.quality());
}

QVariant ImageSettings::boxValue(const QComboBox *box) const
{
    int idx = box->currentIndex();
    if (idx == -1)
        return QVariant();

    return box->itemData(idx);
}

void ImageSettings::selectComboBoxItem(QComboBox *box, const QVariant &value)
{
    for (int i = 0; i < box->count(); ++i) {
        if (box->itemData(i) == value) {
            box->setCurrentIndex(i);
            break;
        }
    }
}
