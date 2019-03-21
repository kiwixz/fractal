# fractal

[![](https://img.shields.io/circleci/project/github/kiwixz/fractal/master.svg)](https://circleci.com/gh/kiwixz/fractal/)
[![](https://img.shields.io/github/repo-size/kiwixz/fractal.svg)](https://github.com/kiwixz/fractal/archive/master.zip)
[![](https://img.shields.io/badge/link-doxygen-blueviolet.svg)](https://kiwixz.github.io/fractal/doc/master/)


Tool to visualize [Mandelbrot set](https://en.wikipedia.org/wiki/Mandelbrot_set).

It's able to zoom progressively at the coordinates of your choice.  You can either display it in a window, or export a video.

`fractal -h` for more information.


## Demo

[Watch on Youtube!](https://www.youtube.com/watch?v=Uj1qerIS_fc)

Inspired by [a GIF from Wikipedia](https://commons.wikimedia.org/wiki/File:Mandelbrot_sequence_new.gif).  After 2h10 of computation, the output was 22 GiB.  The music is the famous Lacrimosa of Mozart's Requiem, from [here](https://www.youtube.com/watch?v=k1-TrAvp_xs).

Generated with:
```sh
fractal -x -0.743643887037158704752191506114774 -y 0.131825904205311970493132056385139 -z -10 -s 0.8 -d 67 -W 3840 -H 2160 --fps 60 -o "out.hevc"
ffmpeg -i "out.hevc" -c copy "out.mp4"
ffmpeg -i "out.mp4" -c copy "out.mkv"
ffmpeg -ss 121 -t 67 -i "lacrimosa.webm" -i "out.mkv" -c copy -c:a libopus -b:a 128k -af "afade=t=in:d=2" -y "out_audio.mkv"
```


## License

```
MIT License

Copyright (c) 2019 kiwixz

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
