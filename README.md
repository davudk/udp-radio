# udp-radio
This C program reads a file and transfers its contents to the specified IP endpoint. The transfer is lossy, since the UDP Internet protocol is used. This meant to be used with WAV files (or other compatible file types).

I used this to stream an audio file to another computer, which received it with [VLC](https://www.videolan.org/vlc/) and was able to play it live. Try adjusting the packet/datagram size and sleep duration (between successive sends) in order to get a good bitrate.
