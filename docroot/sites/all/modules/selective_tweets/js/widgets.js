(function($) {

  Drupal.behaviors.selective_tweets = {
    attach: function(context, settings) {
      window.twttr = (function(d, s, id) {
        var js, fjs = d.getElementsByTagName(s)[0], t = window.twttr || {};
        if (d.getElementById(id)) return t;
        js = d.createElement(s);
        js.id = id;
        js.src = "https://platform.twitter.com/widgets.js";
        fjs.parentNode.insertBefore(js, fjs);

        t._e = [];
        t.ready = function(f) {
          t._e.push(f);
        };

        return t;
      }(document, "script", "twitter-wjs"));

      // Apply theme settings to Tweets.
      var selective_tweets = Drupal.settings.selective_tweets;
      $('.selective-tweets-block').each(function(index) {
        var id = $(this).attr('id');
        tweet_settings = selective_tweets[id];
        $('#' + id + ' blockquote').attr('data-theme', tweet_settings['theme']);
      });

      // Request Twitter’s widgets JavaScript scan for new buttons and widgets.
      if (typeof twttr.widgets != 'undefined') {
        twttr.widgets.load();
      }
    }
  };

})(jQuery);
