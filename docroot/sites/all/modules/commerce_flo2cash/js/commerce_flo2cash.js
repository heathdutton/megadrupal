/**
 * @file
 * jQuery for Commerce Flo2Cash.
 */
;(function ($) {
  Drupal.behaviors.commerce_flo2cash = {
    attach: function (context, settings) {
      // @todo Use AJAX so we can keep count and not reload forever.
      $(window).load(function() {
        if ($('.commerce-flo2cash-verify').length) {
          setTimeout(function() {
            location.reload()
          }, 2000)
        }
      })
    }
  }
})(jQuery);
