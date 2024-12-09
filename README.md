# GradrigoWavExporter

[![Windows (MSVC)](https://github.com/casqade/GradrigoWavExporter/actions/workflows/windows-build-msvc.yml/badge.svg)](https://github.com/casqade/GradrigoWavExporter/actions/workflows/windows-build-msvc.yml)
[![Windows (MSYS2)](https://github.com/casqade/GradrigoWavExporter/actions/workflows/windows-build-msys2.yml/badge.svg)](https://github.com/casqade/GradrigoWavExporter/actions/workflows/windows-build-msys2.yml)
[![GitHub Releases](https://img.shields.io/github/release/casqade/GradrigoWavExporter.svg)](https://github.com/casqade/GradrigoWavExporter/releases/latest)

---

This cmdlet allows exporting Gradrigo boxes as .wav files. 

A while ago I discovered a chiptune sequencer [Gradrigo](https://adam.sporka.eu/gradrigo.html) developed by [Adam Sporka](https://adam.sporka.eu). 
This turned out to be and ideal tool for writing dynamic game soundtracks, 
however several limitations prevented me from using it right away. 

Sadly, Gradrigo is publicly available only in the form of Windows dll, 
and since the game I'm writing soundtrack for is multiplatform, 
I can't use the library directly. 
Therefore, the only way to use Gradrigo's music is to export it as an audio file. 
However, according to [official manual](https://adam.sporka.eu/gradrigo-language-manual.html),
this feature is not supported:
> This feature is currently not implemented in this version of Gradrigo but will be very soon.

Since Gradrigo is already several years old, 
I assumed "very soon" will never happen and wrote this tool.

P.S.: I started this project about 2-3 weeks ago, after verifying there were no alternatives. 
As I'm uploading the finished sources, a quick search shows that... coincidentally, Adam
actually started [his own alternative](https://github.com/adam-sporka/gradriguito)! 
I'm not sure if Gradriguito is going to be a commandline version of Gradrigo or a slightly different tool, 
but it's in very early stages of development, 
so my cmdlet probably stays relevant until at least 2025. 


## Help Entry

```
NOTE: Boxes with infinite loops are NOT available for export!

Usage: GradrigoWavExporter [options] [<boxes>]

Examples:
  GradrigoWavExporter --script=myScript.gr -sr=44100 box1 box2


Options:
  -h --help             Show this help

  -l                    List available boxes
  --list

  -d=FILENAME           Override path to Gradrigo DLL
  --dll=FILENAME          Default path is 'gradrigo.dll'

  -s=FILENAME           Specify path to Gradrigo script
  --script=FILENAME

  -sr=NUMBER            Override sample rate.
  --sample-rate=NUMBER    Default sample rate is 44100

  -mem=BYTES            Specify memory limit for samples buffer.
  --memory-limit=BYTES    Default memory limit is 10 MiB
```


## Usage

1. Download [latest release](https://github.com/Casqade/GradrigoWavExporter/releases/download/v1.0/GradrigoWavExporter-win64.zip)
2. Download [gradrigo.dll](https://github.com/adam-sporka/gradrigo-for-unity/blob/main/Assets/Gradrigo/Plugins/x86_64/gradrigo.dll)
3. Put it in the directory alongside `GradrigoWavExporter.exe`
4. Open Command Line there and refer to [Help Entry](https://github.com/Casqade/GradrigoWavExporter/edit/main/README.md#help-entry) for examples
