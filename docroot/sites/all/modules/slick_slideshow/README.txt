# Slick Slideshow
[Library Website](http://kenwheeler.github.io/slick/)
Fully responsive. Scales with it's container.
Separate settings per breakpoint
Uses CSS3 when available. Fully functional when not.
Swipe enabled. Or disabled, if you prefer.
Desktop mouse dragging
Infinite looping.
Autoplay, dots, arrows, callbacks, etc...

Install:
Dependencies:
* [Libraries API 2.x](https://drupal.org/project/libraries)
* [jQuery Update](https://drupal.org/project/jquery_update) - Set to at least v1.7
* [Slick Library](https://github.com/kenwheeler/slick/archive/master.zip)

Steps:
* Download the Slick library https://github.com/kenwheeler/slick/archive/master.zip
* Unzip the file and rename the folder to "slick"
* Put the folder in a libraries directory (ex: sites/all/libraries)
* Enable the Slick Slideshow module
* Find the image field to turn into a slideshow and set the field's display format to "Slick Slideshow" and set the correct option values.

Core Module:
The Slick Slideshow module creates a drupal library creates an image field formatter with slick slideshow settings.

Check the API file to see how you can create or update a Slick slideshow from an external module, allowing to pass more settings to Slick than just what's in the admin UI at the moment.

Example API Call:

$settings = array(
  'slidesToShow' => 1,
  'slidesToScroll' => 1,
  'infinite' => TRUE,
  'onBeforeChange' => 'function(slider, index) {}',
  'onAfterChange' => 'function(slider, index) {}',
  'onInit' => 'function(slider) {}',
  'responsive' => '[
    {
      breakpoint: 800,
      settings: {
        slidesToShow: 3,
        slidesToScroll: 3,
        infinite: true,
        dots: true
      }
    }
  ]'
);

/**
 * Implements slick_slideshow_create().
 *
 * @param string $field_selector
 *  The selector of the field that you want to turn into a slideshow.
 *  Can be an id or class.
 *
 * @param array $settings
 *  Slick Slideshow settings passed to the javascript init.
 *
 * @param string $target
 *  (optional) If the slideshow slide elements aren't the direct children
 *  of the $field_selector, specify a target selector.
 */
slick_slideshow_create('.field-name-field-slide', $settings, '.field-items');

/*
 * Implements slick_slideshow_update().
 *
 * @param string $field_selector
 *  Field element selector that the Slick slideshow belongs to.
 * @param array $settings
 *  Settings array with options to update.
 * @param string $target (optional)
 *  If the $field_selector isn't the element that Slick should be created on,
 *  specify the $target as a child element of the $field_selector.
 */
slick_slideshow_update($field_selector, $settings, $target);
