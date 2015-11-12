/**
 * @file
 * Javascript file for twitter/fb callback function.
 */

// Declaring settings variables here.
(function ($) {
    Drupal.behaviors.social_share_statistics = {
        attach: function (context, settings) {
            var fbapi;
            window.fbapi = Drupal.settings['api'];
            window.token = Drupal.settings['token'];
        }
    };
})(jQuery);

function share_callback(platform,token) {
    var loc = window.location;
    window.location.href = loc + '/' + platform + '/success/' + token;
}
