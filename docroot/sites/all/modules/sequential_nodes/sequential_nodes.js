Drupal.behaviors.top_jumper = {
    attach: function (context, settings) {
        (function ($) {
            $("a[href*='#jumplink']", context).click(function() {
                $("html, body").animate({scrollTop: 0},settings.sequential_nodes.mid_speed);
                return false;
            });
        })(jQuery);
    }
};
