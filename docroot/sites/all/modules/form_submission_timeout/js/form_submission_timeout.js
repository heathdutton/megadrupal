(function ($) {
  Drupal.behaviors.submission_timeout = {
    attach: function(context, settings) {
      $('.sub-out-remove-form-id').mouseover(function() {
        $(this).parent().addClass('fst-overlay');
        $(this).parent().parent().css("position", "relative");
      });
      $('.sub-out-remove-form-id').mouseout(function() {
        $(this).parent().removeClass('fst-overlay');
      });
    }
  }
})(jQuery);