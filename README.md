# stream-reader

A real-time broadcast event classifier written in C.

Classifies a sliding window of TV broadcast signals as `CONTENT`, `COMMERCIAL`, or `UNKNOWN` using a heuristic state machine and ring buffer. Modelled on the commercial detection problem — identifying ad breaks in a live stream from low-level signal events like scene changes, black frames, and logo absence.