/* EqualHeights - Thanks to Omega */
(function($) {
  Drupal.behaviors.asyncEqualHeights = {
    attach: function (context) {
      $('body', context).once('async-equalheights', function () {
        $(window).bind('resize.asyncequalheights', function () {
          $($('.equal-height-container').get().reverse()).each(function () {
            var elements = $(this).children('.equal-height-element').css('height', '');

            var tallest = 0;

            elements.each(function () {
              if ($(this).height() > tallest) {
                tallest = $(this).height();
              }
            }).each(function() {
              if ($(this).height() < tallest) {
                $(this).css('height', tallest);
              }
            });
          });
        }).load(function () {
          $(this).trigger('resize.asyncequalheights');
        });
      });
    }
  };
})(jQuery);