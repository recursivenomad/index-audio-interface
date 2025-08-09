# Index Audio Interface project attributions

&nbsp;






## Peter S. Hollander

Firmware adapted by Peter S. Hollander in 2025 under the [*BSD 3-Clause*][URL-BSD-3-Clause] license, an [*HPND*][URL-HPND] variant, and the [*MIT*][URL-MIT] license, with newly added files made freely available under your choice of the [*MIT-0*][URL-MIT-0] license or [*CC0-1.0*][URL-CC0] public domain dedication.

*Contact: <recursivenomad@protonmail.com>*  
*Repository: <https://gitlab.com/recursivenomad/index-audio-interface>*



## BambooMaster

The [UAC2](./uac2/) subdirectory relies almost entirely on BambooMaster's [`pico_usb_i2s_speaker`][URL-pico-usb-i2s-speaker] firmware.



## Raspberry Pi

The [UAC1](./uac1/) subdirectory relies almost entirely on Raspberry Pi's [`usb_sound_card`][URL-usb-sound-card] firmware from their pico-playground repository and [`pico_audio_i2s`][URL-pico-audio-i2s] library from their pico-extras repository as a base.



## Jerzy Kasenberg, Ha Thach, *et al.*

The firmware from BambooMaster includes adapted USB protocol definitions from the [`uac2_headset`][URL-uac2-headset] example originally written by Jerzy Kasenberg in Ha Thach's [TinyUSB][URL-TinyUSB] library.



## Dean Camera

The firmware from Raspberry Pi includes USB protocol headers from Dean Camera's [LUFA Library][URL-LUFA].

&nbsp;






# Tools utilized

- [VSCodium][URL-VSCodium]
- [Raspberry Pi Pico SDK][URL-Pico-SDK]
- [Raspberry Pi Pico Extras][URL-Pico-Extras]






[URL-BSD-3-Clause]: <https://opensource.org/license/bsd-3-clause>
[URL-HPND]: <https://opensource.org/license/historical-php>
[URL-MIT]: <https://opensource.org/license/mit/>
[URL-MIT-0]: <https://opensource.org/license/mit-0/>
[URL-CC0]: <https://creativecommons.org/publicdomain/zero/1.0/>

[URL-pico-usb-i2s-speaker]: <https://github.com/BambooMaster/pico_usb_i2s_speaker>
[URL-usb-sound-card]: <https://github.com/raspberrypi/pico-playground/tree/master/apps/usb_sound_card/>
[URL-pico-audio-i2s]: <https://github.com/raspberrypi/pico-extras/tree/master/src/rp2_common/pico_audio_i2s/>
[URL-uac2-headset]: <https://github.com/hathach/tinyusb/tree/master/examples/device/uac2_headset>
[URL-TinyUSB]: <https://github.com/hathach/tinyusb>
[URL-LUFA]: <https://github.com/abcminiuser/lufa>

[URL-VSCodium]: <https://vscodium.com/>
[URL-Pico-SDK]: <https://github.com/raspberrypi/pico-sdk>
[URL-Pico-Extras]: <https://github.com/raspberrypi/pico-extras>
