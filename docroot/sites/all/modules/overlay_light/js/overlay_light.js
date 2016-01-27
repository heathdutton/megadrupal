/**
 * @file
 * Creates a new close tab or adds the close button if no tabs exist.
 */

(function ($) {
  Drupal.behaviors.overlay_light = {
    attach:function(context) {
      var path = Drupal.settings.overlay_light.path;
      var basepath = Drupal.settings.overlay_light.basepath;
      $('#content:not(.processed)')
        .addClass('processed')
        .prepend('<div class="close-wrapper"><a href="' + basepath + path + '" id="overlay-close">Close</a></div>');
      $('div.breadcrumb').insertBefore('div#page > div.tabs-secondary');
    }
  };
})(jQuery);
