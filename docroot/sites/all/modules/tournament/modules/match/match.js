(function($) {
  Drupal.behaviors.match_date = {
    attach: function (context) {
      $('.match-date', context).once('match-date', function () {
        $(this).focus(function() {
          $(this).datepicker({
            autoSize: true,
            dateFormat: Drupal.settings.match.dateFormat,
          });
        });
        $(this).click(function(){
          $(this).focus();
        });
      });
    }
  }
})(jQuery);
