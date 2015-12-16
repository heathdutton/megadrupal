var subs_id = Drupal.settings.rankingalive.id;

(function() {
    Drupal.behaviors.rankingalive = {
        attach: function(context, settings) {
            var insert = document.createElement("script");
            insert.type = "text/javascript";
            insert.async = true;
            insert.src = "http://track.rankingalive.com/track.js";
            var s = document.getElementsByTagName("script")[0];
            s.parentNode.insertBefore(insert, s);
        }
    };
})(jQuery);
