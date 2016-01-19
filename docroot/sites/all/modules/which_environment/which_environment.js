/**
 * @file
 * Which Environment? - Overlay JS.
 */

(function($) {
    Drupal.behaviors.which_environment = {
        attach: function (context, settings) {
            var which_type = settings.which_environment.which_environment_type;
            var overlay = document.createElement("div");
            overlay.setAttribute("id","which_environment");
            overlay.setAttribute("class", "which_environment");
            document.body.appendChild(overlay);
            jQuery('#which_environment').prepend('<div class="environment-checker">' + which_type + '</div>');
        }
    };
})(jQuery);
