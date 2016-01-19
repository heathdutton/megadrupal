/**
 * @file
 * Provides Intense loader.
 */

(function($, Drupal) {

  "use strict";

  Drupal.behaviors.intense = {
    attach: function (context) {

      $(".intense", context).once("intense", function() {
        Intense( this );
      });

    }
  };

})(jQuery, Drupal);
