(function ($) {
     Drupal.behaviors.bd_colour = {
        attach: function (context, settings) {
            $("#bd-colour .content").text(window.screen.colorDepth + ' bit');
        }
    };
})(jQuery);
