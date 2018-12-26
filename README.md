# macssb

Single-site browser for macOS.

## Features

### Native Tabs

[![native-tabs][native-tabs-gif]][native-tabs]

Uses macOS' native tabs. Easily drag tabs from one window to another, show all tabs, and more are all available.

### Picture-in-picture `⌃⌘P`

[![picture-in-picture][picture-in-picture-gif]][picture-in-picture]

Use macOS' picture-in-picture on the first video element found. Note this
does not work if the video is in an `iframe` element.

### Airplay `⌃⌘A`

[![airplay][airplay-gif]][airplay]

Presents macOS' Airplay menu for the first video element found. Note this
does not work if the video is in an `iframe` element.

### AdBlock

Includes a content-blocker.

## Usage

- Make a new folder in  `sites/` (see `sites/Example/`) to get started.
- edit `sites/YOUR_SITE/config.json`.
- run `./script/build` to build all sites (alternatively run `./script/build sites/YOUR_SITE`).

Your .app will now be available in `./sites-build/YOUR SITE.app`.

## Requirements

- Xcode
- Recent version of macOS

[picture-in-picture]: https://gfycat.com/SilverBackBullfrog
[picture-in-picture-gif]: https://giant.gfycat.com/SilverBackBullfrog.gif
[airplay]: https://gfycat.com/DeliciousSophisticatedFoxterrier
[airplay-gif]: https://giant.gfycat.com/DeliciousSophisticatedFoxterrier.gif
[native-tabs]: https://gfycat.com/TatteredAromaticKakarikis
[native-tabs-gif]: https://giant.gfycat.com/TatteredAromaticKakarikis.gif
