(function ($) {
/**
 * Handle width and position for the ad, while page is scrolling.
 */
Drupal.behaviors.slideAd = {
  attach: function (context) {
    if (Drupal.settings && Drupal.settings.slideAd) {

      var winHeight = $(document).height();
      var popup = $('#slidead-popup', context);
      var popWidth = Drupal.settings.slideAd.width;
      var popHidden = popWidth+22; /* 10 + 10 (padding) + 1 + 1 (border) */

      popup.css('width', popWidth + 'px');
      popup.css('right', '-' + popHidden + 'px');

      var handlePopup = function() {
        var scrollTop = $(document).scrollTop();
        if(scrollTop > (winHeight/4)){
          popup.show('500').animate({right: '0'});
        } else {
          //popup.animate({right: '-' + popHidden + 'px' }, 'slow');
          popup.hide('500');
        }
      }

      var myInterval = setInterval(handlePopup, 500);

      $('#pop-close span', context).click(function() {
        popup.hide();
        popup = null;
        clearInterval(myInterval);
      });
    }
  }
};

})(jQuery);
