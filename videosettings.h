#ifndef VIDEOSETTINGS_H
#define VIDEOSETTINGS_H

#include <QDialog>
#include <QAudioEncoderSettings>
#include <QVideoEncoderSettings>

QT_BEGIN_NAMESPACE
class QComboBox;
class QMediaRecorder;
namespace Ui { class VideoSettingsUi; }
QT_END_NAMESPACE

class VideoSettings : public QDialog
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

    Ui::VideoSettingsUi *ui;
    QMediaRecorder *mediaRecorder;
};

#endif // VIDEOSETTINGS_H
