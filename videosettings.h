#ifndef VIDEOSETTINGS_H
#define VIDEOSETTINGS_H

#include <QAudioEncoderSettings>
#include <QVideoEncoderSettings>
#include <QEvent>

#include <DDialog>
#include <DComboBox>
#include <DSlider>

DWIDGET_USE_NAMESPACE

class QMediaRecorder;

class VideoSettings : public DDialog
{
    Q_OBJECT

public:
    explicit VideoSettings(QMediaRecorder *mediaRecorder, QWidget *parent = nullptr);
    ~VideoSettings();

    QAudioEncoderSettings audioSettings() const;
    void setAudioSettings(const QAudioEncoderSettings&);

    QVideoEncoderSettings videoSettings() const;
    void setVideoSettings(const QVideoEncoderSettings&);

    QString format() const;
    void setFormat(const QString &format);

protected:
    void changeEvent(QEvent *e) override;

private:
    QVariant boxValue(const QComboBox*) const;
    void selectComboBoxItem(QComboBox *box, const QVariant &value);

    DComboBox *audioCodecBox;
    DComboBox *audioSampleRateBox;
    DSlider *audioQualitySlider;
    DComboBox *videoCodecBox;
    DSlider *videoQualitySlider;
    DComboBox *videoResolutionBox;
    DComboBox *videoFramerateBox;
    DComboBox *containerFormatBox;
    QMediaRecorder *mediaRecorder;
};

#endif
