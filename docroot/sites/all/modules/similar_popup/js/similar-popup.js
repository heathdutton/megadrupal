Drupal.similarPopup = Drupal.similarPopup || {};
Drupal.similarPopup.closed = Drupal.similarPopup.closed || false;

(function($) {
  Drupal.behaviors.similarPopup = {
    attach: function (context, settings) {
      if ($('.node', context).length) {
        var $similar = $('#similar-popup-wrapper', context);
        var nodeHeight = $('.node', context)[0].scrollHeight;
        var isVisible = false;

        if ($similar.hasClass('top-left') || $similar.hasClass('bottom-left')) {
          $similar.css({ 'left' : -$similar.outerWidth() });
        }
        else {
          $similar.css({ 'right' : -$similar.outerWidth() });
        }

        $(function() {
          $(window).scroll(function () {
            if (!Drupal.similarPopup.closed) {
              if (document.body.scrollHeight - $(document).scrollTop() <= $(this).height() || $(this).scrollTop() >= nodeHeight) {
                if (!isVisible) {
                  similarShow();
                }
              }
              else if (isVisible) {
                similarHide();
              }
            }
          });
        });

        $similar.find('span.close').click(function() {
          Drupal.similarPopup.closed = true;
          similarHide();
        });

        /**
         * Show similar popup.
         */
        function similarShow() {
          isVisible = true;

          if ($similar.hasClass('top-left') || $similar.hasClass('bottom-left')) {
            $similar.stop(true).animate({
              left: 0
            }, 500);
          }
          else {
            $similar.stop(true).animate({
              right: 0
            }, 500);
          }

          if ($similar.find('#similar-popup-sound-notification').length) {
            $similar.find('#similar-popup-sound-notification')[0].play();
          }
        }

        /**
         * Hide similar popup.
         */
        function similarHide() {
          isVisible = false;

          if ($similar.hasClass('top-left') || $similar.hasClass('bottom-left')) {
            $similar.stop(true).animate({
              left: -$similar.outerWidth()
            }, 500);
          }
          else {
            $similar.stop(true).animate({
              right: -$similar.outerWidth()
            }, 500);
          }
        }
      }
    }
  }
})(jQuery);
