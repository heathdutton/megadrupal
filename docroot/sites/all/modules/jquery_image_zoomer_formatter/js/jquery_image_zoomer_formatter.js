/**
 * @file
 * Javascript file to initialize the zoomer on images.
 */

(function($) {
    Drupal.behaviors.jquery_image_zoomer_formatter = {
        attach: function(context, settings) {
            var custom_class = Drupal.settings.jquery_image_zoomer_formatter['zoomer_custom_class'];
            custom_class = custom_class.trim();
            if (custom_class != '') {
                $(".jqueryimage_zoomer").zoomer({
                    customClass: custom_class,
                });
            } else {
                $(".jqueryimage_zoomer").zoomer();
            }

        }
    };
})(jQuery);
