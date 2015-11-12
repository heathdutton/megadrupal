(function ($) {
    Drupal.behaviors.selectbox = {
        attach: function(context, settings) {
            $("SELECT").selectBox();
        }
    };

})(jQuery);