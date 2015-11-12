(function($) {

  Drupal.behaviors.selective_tweets_async_load = {
    attach: function(context, settings) {

      // Loop over all Selective Tweets blocks and ajax load the content.
      $('.selective-tweets-async-load').once('async-load', function() {
        var container = $(this).parent();
        var throbber = $(this);
        var id = $(this).attr('id');
        // Get block id.
        var bid = id.replace('selective-tweets-async-load-', '');

        // Create container to load block content in.
        $(this).after('<div class="selective-tweets-content"></div>');

        // Load block content.
        $(container).find('.selective-tweets-content').load('/selective-tweets/feed/load/' + bid + '/0', function () {
          throbber.remove();
          Drupal.attachBehaviors(container);
        });
      });

    }
  };

  // An ajax command to trigger attachBehaviors.
  Drupal.ajax.prototype.commands.selective_tweets_attach_behaviors = function(ajax, response, status) {
    Drupal.attachBehaviors($(response.container));
  }

})(jQuery);
