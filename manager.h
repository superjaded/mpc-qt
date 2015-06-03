#ifndef MANAGER_H
#define MANAGER_H
// A simple playback manager designed to control the MainWindow like it were a
// widget.  Due to needing to communicate with the playlists, it sort of takes
// control of the mpv widget away from its host.


#include <QObject>
#include <QList>
#include <QUrl>
#include <QUuid>
#include <QSize>
#include <QVariant>

class MpvWidget;

class PlaybackManager : public QObject
{
    Q_OBJECT
public:
    enum PlaybackState { StoppedState, PausedState, PlayingState,
                         BufferingState };
    enum PlaybackType { None, File, Disc, Stream, Device };

    explicit PlaybackManager(QObject *parent = 0);
    void setMpvWidget(MpvWidget *mpvWidget, bool makeConnections = false);

public slots:
    // load functions
    void openFiles(QList<QUrl> what);           // from quick-load dialog
    void playDisc(QUrl where);                  // from menu
    void playStream(QUrl stream);               // from menu
    void playItem(QUuid playlist, QUuid item);  // called by playlistwindow
    void playDevice(QUrl device);   // I don't have a device to test this

    // control functions
    void pausePlayer();
    void unpausePlayer();
    void stopPlayer();
    void stepBackward();
    void stepForward();
    void navigateToNextChapter();
    void navigateToPrevChapter();
    void navigateToChapter(int64_t chapter);
    void navigateToTime(double time);
    void speedUp();
    void speedDown();

    // output functions
    void setPlaybackSpeed(double speed);
    void setAudioTrack(int64_t id);
    void setSubtitleTrack(int64_t id);
    void setVideoTrack(int64_t id);
    void setVolume(int64_t volume);
    void setMute(bool muted);

private slots:
    void mpvw_playTimeChanged(double time);
    void mpvw_playLengthChanged(double length);
    void mpvw_playbackStarted();
    void mpvw_pausedChanged(bool yes);
    void mpvw_playbackFinished();
    void mpvw_mediaTitleChanged(QString title);
    void mpvw_chaptersChanged(QVariantList chapters);
    void mpvw_tracksChanged(QVariantList tracks);
    void mpvw_videoSizeChanged(QSize size);

signals:
    void timeChanged(double time, double length);
    void titleChanged(QString title);
    void videoSizeChanged(QSize size);
    void stateChanged(PlaybackState state);
    void typeChanged(PlaybackType type);
    // These signals transmit a list of (id, description) pairs
    void chaptersAvailable(QList<QPair<int64_t,QString>> chapters);
    void audioTracksAvailable(QList<QPair<int64_t,QString>> tracks);
    void videoTracksAvailable(QList<QPair<int64_t,QString>> tracks);
    void subtitleTracksAvailable(QList<QPair<int64_t,QString>> tracks);
    void timeChanged(double time);
    void lengthChanged(double time);
    void nowPlayingChanged(QUuid item);
    void finishedPlaying(QUuid item);

private:
    MpvWidget *mpvWidget_;
    QUuid nowPlayingList;
    QUuid nowPlayingItem;

    double mpvLength;
};

#endif // MANAGER_H