(function ($) {
  Drupal.behaviors.readmore = {
    attach : function(context, settings) {
      $('.readmore-summary .readmore-link').click(function(e) {
        e.preventDefault();
        var summary = $(this).closest('.readmore-summary');
        summary.hide();
        summary.next('.readmore-text').slideDown(300);
      });
      $('.readmore-text .readless-link').click(function(e) {
        e.preventDefault();
        var text = $(this).closest('.readmore-text');
        text.slideUp(300);
        text.prev('.readmore-summary').slideDown(300);
      });
    }
  };
})(jQuery);
