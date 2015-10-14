/**
 * @file
 * Attaches the behaviors for the Overlay child pages.
 */

(function($) {
    Drupal.behaviors.sso_multi_domain = {
        attach: function(context, settings) {
            $(window).load(function() {
                window.location.href = Drupal.settings.sso_multi_domain['destination'];
            });
        }
    };
})(jQuery);
