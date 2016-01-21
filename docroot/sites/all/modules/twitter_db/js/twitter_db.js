(function ($) {
  Drupal.behaviors.twitter_db = {
    attach: function (context, settings) {
      // Check if the jQuery Plugin is installed
      if (Drupal.settings.twitter_db.jquery_plugin) {
        // Check if the Cycle plugin should be used on tweets
        if (Drupal.settings.twitter_db.cycle_enabled) {
          $('#twitter-db .tweet-list').cycle({
            fx: Drupal.settings.twitter_db.cycle_effect,
            pause: 1
          }); 
        }
      }
    }
  };
})(jQuery);

