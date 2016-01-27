(function ($) {
Drupal.behaviors.adSenseUnblock = {
  attach: function (context) {
    setTimeout(function() {
      if ($('.adsense ins').contents().length == 0) {
        $('.adsense').html(Drupal.t("Please, enable ads on this site. By using ad-blocking software, you're depriving this site of revenue that is needed to keep it free and current. Thank you."));
        $('.adsense').css({'overflow': 'hidden', 'font-size': 'smaller'});
      }
    }, 3000); // wait 3 seconds for adsense async to execute
  }
};

})(jQuery);
