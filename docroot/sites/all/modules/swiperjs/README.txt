Installation

To install this module you must download it and install in /sites/all/modules

If you don't have the directory libraries create it.

Download the swiper library from https://github.com/nolimits4web/Swiper into sites/all/libraries directory.

Enable the module.


The directory estructure should be like this.

|
|----/sites/all/modules/contrib/swiperjs
|----/sites/all/libraries/Swiper/
|----/sites/all/libraries/Swiper/dev/
|----/sites/all/libraries/Swiper/dist/
|----/sites/all/libraries/Swiper/component.json



Remember, Please readalso:https://drupal.org/node/1342238

If you have enabled the module you must add a libraries_load('Swiper') in another module, and also you must load swiper css.
