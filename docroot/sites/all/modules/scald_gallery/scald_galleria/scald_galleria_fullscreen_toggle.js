/**
 * @file
 * scald_galleria_fullscreen_toggle.js
 *
 * Makes it possible to toggle fullscreen mode in galleria.
 */
(function ($) {

  /**
   * Adding a fullscreen link to the galleria.
   */
  Drupal.behaviors.addFullscreenToGalleria = {
    attach: function (context, settings) {
      if (typeof Galleria != 'undefined') {
        Galleria.ready(function () {
          var gallery = this;
          $(gallery._target).parent().find('.galleria-fullscreen-link').click(function () {
            gallery.toggleFullscreen();
          });
        });
      }
    }
  }

}(jQuery));
