(function ($) {
  Drupal.behaviors.tweetable_text = {
    attach: function (context, settings) {
      $(".tweetable-text", context).hover(
        function() {
          $(this).find('.tweetable-text-sharebuttons').fadeIn(200);
        },
        function() {
          $(this).find('.tweetable-text-sharebuttons').fadeOut(200);
        });
    }
  };
})(jQuery);
