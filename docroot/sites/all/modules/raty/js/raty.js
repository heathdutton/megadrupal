/**
 * @file
 *
 * Drupal integration with the jQuery Raty library.
 *
 */

(function ($) {

  Drupal.behaviors.raty = {
    attach: function (context, settings) {
      // The core images path.
      $image_path = settings.raty.image_path;
      $.each(settings.raty, function(fieldName, iteration) {
        if(!$.isPlainObject(iteration)) {
          return true;
        }
        // Now hit all the fields on the page.
        $('.raty-star.' + fieldName, context).once('raty-star', function() {
          $extra_settings = {};
          // Generate base settings for output version.
          $raty_settings = {
            readOnly  : true,
            width     : 'auto',
            number    : function() {
              return $(this).attr('data-number');
            },
            score     : function() {
              return $(this).attr('data-score');
            }
          }
          // User supplied custom images.
          if (iteration.custom_images) {
            // Custom images for output - 'on/off/half' state.
            $extra_settings.starOn = iteration.custom_image_on || $image_path + 'star-on.png';
            $extra_settings.starOff = iteration.custom_image_off || $image_path + 'star-off.png';
            $extra_settings.starHalf = iteration.custom_image_half || $image_path + 'star-half.png';
          }
          else {
            // Use raty's built in images.
            $extra_settings.path = $image_path;
          }

          // Merge settings.
          $.extend($raty_settings, $extra_settings);
          // Apply settings.
          $(this).raty($raty_settings);
          // Apply custom image widths.
          $img_width = iteration.image_width || 'auto';
          $(this).find('img').css('width', $img_width);

        });
      });
    }
  };
})(jQuery);
