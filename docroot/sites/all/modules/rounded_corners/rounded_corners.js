
/**
 *  @file
 *  Initiate the jQuery Corners plugin.
 */

(function ($) {
  Drupal.behaviors.roundedCorners = {
    attach: function (context, settings) {

      // Set the useNative property.
      if (Drupal.settings.roundedCorners.settings) {
        $.fn.corner.defaults.useNative = Drupal.settings.roundedCorners.settings.useNative;
      }

      // Set the height and width on the div wrapping an element.
      if (Drupal.settings.roundedCorners.wrappingDivs) {
        var wrappingDivs = Drupal.settings.roundedCorners.wrappingDivs;

        // Add the rounded corners to the page.
        for (var key in wrappingDivs) {
          // Iterate over selectors and set the width and height of the wrapping 
          // div, according to the dimensions of the image.
          $(wrappingDivs[key]['selector']).each(function() {
            var $this = $(this).children('img');

            var imgWidth = $($this).width();
            var imgHeight = $($this).height();

            var $parent = $($this).parent('div');

            $parent.width(imgWidth);
            $parent.height(imgHeight);
          });
        }
      }

      // Set the rounded corners.
      if (Drupal.settings.roundedCorners.commands) {
        var roundedCorners = Drupal.settings.roundedCorners.commands;

        // Add the rounded corners to the page.
        for (var key in roundedCorners) {
          $(roundedCorners[key]['selector']).corner(roundedCorners[key]['effect'] + ' ' + roundedCorners[key]['corners'] + ' ' + roundedCorners[key]['width'] + 'px');
        }
      }
    }
  };
}(jQuery));
