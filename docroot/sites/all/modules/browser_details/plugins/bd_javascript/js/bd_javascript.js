(function ($) {
     Drupal.behaviors.bd_javascript = {
        attach: function (context, settings) {
            $("#bd-javascript .content").text('Enabled');
        }
    };
})(jQuery);
