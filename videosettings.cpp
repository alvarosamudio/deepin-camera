#include "videosettings.h"

#include <QComboBox>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QScrollArea>
#include <QDebug>
#include <QMediaRecorder>
#include <QMediaService>

#include <DComboBox>
#include <DSlider>
#include <DFrame>

DWIDGET_USE_NAMESPACE

VideoSettings::VideoSettings(QMediaRecorder *mediaRecorder, QWidget *parent) :
    DDialog(parent),
    mediaRecorder(mediaRecorder)
{
    setTitle(tr("Video Settings"));
    setIcon(QIcon::fromTheme("camera-video"));

    QWidget *widget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(widget);

    QScrollArea *scrollArea = new QScrollArea(widget);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidgetResizable(true);

    QWidget *scrollContent = new QWidget(scrollArea);
    QHBoxLayout *scrollLayout = new QHBoxLayout(scrollContent);

    DFrame *audioFrame = new DFrame(scrollContent);
    QFormLayout *audioForm = new QFormLayout(audioFrame);

    audioCodecBox = new DComboBox(audioFrame);
    audioCodecBox->addItem(tr("Default audio codec"), QVariant(QString()));
    const QStringList supportedAudioCodecs = mediaRecorder->supportedAudioCodecs();
    for (const QString &codecName : supportedAudioCodecs) {
        QString description = mediaRecorder->audioCodecDescription(codecName);
        audioCodecBox->addItem(codecName + ": " + description, QVariant(codecName));
    }
    audioForm->addRow(tr("Audio Codec:"), audioCodecBox);

    audioSampleRateBox = new DComboBox(audioFrame);
    const QList<int> supportedAudioSampleRates = mediaRecorder->supportedAudioSampleRates();
    for (int sampleRate : supportedAudioSampleRates)
        audioSampleRateBox->addItem(QString::number(sampleRate), QVariant(sampleRate));
    audioForm->addRow(tr("Sample Rate:"), audioSampleRateBox);

    audioQualitySlider = new DSlider(Qt::Horizontal, audioFrame);
    audioQualitySlider->setMinimum(0);
    audioQualitySlider->setMaximum(int(QMultimedia::VeryHighQuality));
    audioForm->addRow(tr("Quality:"), audioQualitySlider);

    scrollLayout->addWidget(audioFrame);

    DFrame *videoFrame = new DFrame(scrollContent);
    QFormLayout *videoForm = new QFormLayout(videoFrame);

    videoResolutionBox = new DComboBox(videoFrame);
    videoResolutionBox->addItem(tr("Default"));
    const QList<QSize> supportedResolutions = mediaRecorder->supportedResolutions();
    for (const QSize &resolution : supportedResolutions) {
        videoResolutionBox->addItem(QString("%1x%2").arg(resolution.width()).arg(resolution.height()),
                                    QVariant(resolution));
    }
    videoForm->addRow(tr("Resolution:"), videoResolutionBox);

    videoFramerateBox = new DComboBox(videoFrame);
    videoFramerateBox->addItem(tr("Default"));
    const QList<qreal> supportedFrameRates = mediaRecorder->supportedFrameRates();
    for (qreal rate : supportedFrameRates) {
        QString rateString = QString("%1").arg(rate, 0, 'f', 2);
        videoFramerateBox->addItem(rateString, QVariant(rate));
    }
    videoForm->addRow(tr("Framerate:"), videoFramerateBox);

    videoCodecBox = new DComboBox(videoFrame);
    videoCodecBox->addItem(tr("Default video codec"), QVariant(QString()));
    const QStringList supportedVideoCodecs = mediaRecorder->supportedVideoCodecs();
    for (const QString &codecName : supportedVideoCodecs) {
        QString description = mediaRecorder->videoCodecDescription(codecName);
        videoCodecBox->addItem(codecName + ": " + description, QVariant(codecName));
    }
    videoForm->addRow(tr("Video Codec:"), videoCodecBox);

    videoQualitySlider = new DSlider(Qt::Horizontal, videoFrame);
    videoQualitySlider->setMinimum(0);
    videoQualitySlider->setMaximum(int(QMultimedia::VeryHighQuality));
    videoForm->addRow(tr("Quality:"), videoQualitySlider);

    scrollLayout->addWidget(videoFrame);

    containerFormatBox = new DComboBox(scrollContent);
    containerFormatBox->addItem(tr("Default container"), QVariant(QString()));
    const QStringList formats = mediaRecorder->supportedContainers();
    for (const QString &format : formats) {
        containerFormatBox->addItem(format + ": " + mediaRecorder->containerDescription(format),
                                    QVariant(format));
    }

    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);

    QHBoxLayout *containerLayout = new QHBoxLayout;
    containerLayout->addWidget(new QLabel(tr("Container Format:"), widget));
    containerLayout->addWidget(containerFormatBox);
    mainLayout->addLayout(containerLayout);

    addContent(widget);

    addButton(tr("OK"), true, DDialog::ButtonRecommend);
    addButton(tr("Cancel"), false, DDialog::ButtonNormal);
    connect(this, &DDialog::accepted, this, &QDialog::accept);
    connect(this, &DDialog::rejected, this, &QDialog::reject);
}

VideoSettings::~VideoSettings()
{
}

void VideoSettings::changeEvent(QEvent *e)
{
    DDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        break;
    default:
        break;
    }
}

QAudioEncoderSettings VideoSettings::audioSettings() const
{
    QAudioEncoderSettings settings = mediaRecorder->audioSettings();
    settings.setCodec(boxValue(audioCodecBox).toString());
    settings.setQuality(QMultimedia::EncodingQuality(audioQualitySlider->value()));
    settings.setSampleRate(boxValue(audioSampleRateBox).toInt());
    return settings;
}

void VideoSettings::setAudioSettings(const QAudioEncoderSettings &audioSettings)
{
    selectComboBoxItem(audioCodecBox, QVariant(audioSettings.codec()));
    selectComboBoxItem(audioSampleRateBox, QVariant(audioSettings.sampleRate()));
    audioQualitySlider->setValue(audioSettings.quality());
}

QVideoEncoderSettings VideoSettings::videoSettings() const
{
    QVideoEncoderSettings settings = mediaRecorder->videoSettings();
    settings.setCodec(boxValue(videoCodecBox).toString());
    settings.setQuality(QMultimedia::EncodingQuality(videoQualitySlider->value()));
    settings.setResolution(boxValue(videoResolutionBox).toSize());
    settings.setFrameRate(boxValue(videoFramerateBox).value<qreal>());

    return settings;
}

void VideoSettings::setVideoSettings(const QVideoEncoderSettings &videoSettings)
{
    selectComboBoxItem(videoCodecBox, QVariant(videoSettings.codec()));
    selectComboBoxItem(videoResolutionBox, QVariant(videoSettings.resolution()));
    videoQualitySlider->setValue(videoSettings.quality());

    for (int i = 0; i < videoFramerateBox->count(); ++i) {
        qreal itemRate = videoFramerateBox->itemData(i).value<qreal>();
        if (qFuzzyCompare(itemRate, videoSettings.frameRate())) {
            videoFramerateBox->setCurrentIndex(i);
            break;
        }
    }
}

QString VideoSettings::format() const
{
    return boxValue(containerFormatBox).toString();
}

void VideoSettings::setFormat(const QString &format)
{
    selectComboBoxItem(containerFormatBox, QVariant(format));
}

QVariant VideoSettings::boxValue(const QComboBox *box) const
{
    int idx = box->currentIndex();
    if (idx == -1)
        return QVariant();

    return box->itemData(idx);
}

void VideoSettings::selectComboBoxItem(QComboBox *box, const QVariant &value)
{
    for (int i = 0; i < box->count(); ++i) {
        if (box->itemData(i) == value) {
            box->setCurrentIndex(i);
            break;
        }
    }
}
