/**
 * @file
 * Javascript for Social Profile Field.
 */

/**
 * Show Social Network icon depending on URL.
 */
(function ($) {
    Drupal.behaviors.socialProfileField = {
        attach: function (context) {
            $(".edit-field-social-profile-url", context).live("change", function (event) {
                if ($(this).val()) {
                    var url = $.url($(this).val());
                    var domain = url.attr('host').replace('.', '-').replace('.', '-');
                }
                var $wrapper = $(this).parent('.form-type-textfield');
                if (domain) {
                    $wrapper.attr('rel', domain).addClass(domain);
                } else {
                    var domain = $wrapper.attr('rel');
                    if (domain) {
                        $wrapper.removeAttr('rel').removeClass(domain);
                    }
                }
            });
            $(".edit-field-social-profile-url", context).change();
        }
    };
})(jQuery);
