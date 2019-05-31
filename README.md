To stream from PC use ffmpeg

    ffmpeg -re -i YOURFILE.mp3 -ac 1 -f u8 -flush_packets 0 -ar 40000 "udp://YOURIP:44444?pkt_size=1400"