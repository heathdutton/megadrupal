
-- SUMMARY --

This jVectorMap module is a simple libraries API wrapper for the super cool
jVectorMap jQuery JavaScript library.  It provides all the wiring to start
creating maps in your drupal website.

Please refer to http://jvectormap.com/ for a more full descripton of jVectorMap.

For a full description of the module, visit the project page:
  http://www.drupal.org/project/jvectormap

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/jvectormap


-- REQUIREMENTS --

* jVectorMap JavaScript library


-- INSTALLATION --


* Install library as usual, for futher information see
  https://drupal.org/node/1440066.
* The JavaScript & CSS files library files should have the version number 
  removed i.e. jquery-jvectormap-1.2.2.min.js => jquery.jvectormap.min.js.


-- API --

    file: myModule.module
    // Example #1.
    $form['default-world-map'] = array(
      '#theme' => 'jvectormap',
    );
    // #Example #2.
    $form['custom-map'] = array(
      '#theme' => 'jvectormap',
      '#attributes' => array(
        'id' => 'custom-world-map', // Optional as one will get auto-generated.
        'style' => 'width:300px;height:200px', // Optional - default size 600x400.
      ),
      // Optional as it defaults to jquery-jvectormap-world-mill-en.js
      // if it exists in jvectormap libraries folder.
      '#map_path' => drupal_get_path('module', 'example') . '/js/custom_world_en.js',
    );

    file: myModule.js
    (function ($) {
      Drupal.behaviors.example = {
        attach: function (context, settings) {
          // Example #1.
          $('div#jvectormap-auto-identifier-0').vectorMap();
          // Example #2.
          $('div#custom-world-map').vectorMap({
            map: 'custom_world_en',
          });
        }
      };
    })(jQuery);

-- FAQ --

Q: No map renders and the map JavaScript file gives error:
     'Uncaught TypeError: Cannot read property 'fn' of undefined'.

A: Modify the map js file so it essentially uses jQuery.noConflict()

  File: map.js
  (function ($) {
    $.fn.vectorMap('addMap', 'world_mill_en'..........
  })(jQuery);

-- CONTACT --

Current maintainer:
* Lucas Hedding (heddn) - http://drupal.org/user/1463982
