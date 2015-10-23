/**
 * @file
 * JavaScript functions for the Client-side adaptive image module.
 */

(function ($) {
  Drupal.behaviors.csAdaptiveBackgroundImage = {
    attach: function(context, settings) {
      /**
       * Retrieves an adapted image based element's data attributes
       * and the current client width.
       */
      var getAdaptedBackgroundImage = function(element, excluded_breakpoint) {
        var selected_breakpoint = 'max';
        var breakpoints = $(element).attr('data-adaptive-background-image-breakpoints');
        if (breakpoints) {
          // Find applicable target resolution.
          $.each(breakpoints.split(' '), function(key, breakpoint) {
            if (document.documentElement.clientWidth <= Number(breakpoint) && (selected_breakpoint == 'max' || Number(breakpoint) < Number(selected_breakpoint))) {
              selected_breakpoint = breakpoint;
            }
          });
        }
        if (selected_breakpoint != excluded_breakpoint) {
          return $(element).attr('data-adaptive-background-image-' + selected_breakpoint + '-img');
        }
        else {
          return false;
        }
      };

      // Insert adapted images.
      $('noscript.adaptive-background-image', context).once('adaptive-background-image', function() {
        var img = getAdaptedBackgroundImage(this);
        $(this).after(img);
        Drupal.attachBehaviors(img);
      });

      // Replace adapted images on window resize.
      $(window).resize(function() {
        $('noscript.adaptive-background-image-processed').each(function() {
          // Replace image if it does not match the same breakpoint.
          var excluded_breakpoint = $(this).next('div.adaptive-background-image').attr('data-adaptive-background-image-breakpoint');
          var img = getAdaptedBackgroundImage(this, excluded_breakpoint);
          if (img) {
            $(this).next('div.adaptive-background-image').replaceWith(img);
            Drupal.attachBehaviors(img);
          }
        });
      });
    }
  };
})(jQuery);