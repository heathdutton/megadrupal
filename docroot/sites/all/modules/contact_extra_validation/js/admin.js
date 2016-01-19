/**
 * @file
 * Js file for validation.
 */

(function($) {
    Drupal.behaviors.contact_extra_validation = {
        attach: function(context, settings) {
            $.fn.extend({
                limiter: function(limit, elem) {
                    $(this).on("keyup focus", function() {
                        setCount(this, elem);
                    });
                    function setCount(src, elem) {
                        var chars = src.value.length;
                        if (chars > limit) {
                            src.value = src.value.substr(0, limit);
                            chars = limit;
                        }
                        elem.html(limit - chars);
                    }
                    setCount($(this)[0], elem);
                }
            });
            var elem = jQuery("#chars");
            if (jQuery("#edit-message").is(":visible")) {
                jQuery("#edit-message").limiter(Drupal.settings.contact_validation.limit, elem);
            }
        }
    };
})(jQuery);
