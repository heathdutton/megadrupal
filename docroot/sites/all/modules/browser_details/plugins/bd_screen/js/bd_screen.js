(function ($) {
     Drupal.behaviors.bd_screen = {
        attach: function (context, settings) {
            $("#bd-screen .content").text(window.screen.availWidth + 'x' + window.screen.availHeight);
        }
    };
})(jQuery);
