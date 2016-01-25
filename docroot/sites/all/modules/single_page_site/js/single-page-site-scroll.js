/**
 * Smooth scrolling to anchor item.
 */

(function ($) {
  Drupal.behaviors.scrollToAnchor = {
    init: function (context, settings) {
      // On page load, smooth scroll.
      var hash = window.location.hash;
      var heightDifference = $(document).height() - $(window).height();
      if (hash && $(hash).length > 0) {
        Drupal.behaviors.scrollToAnchor.scrollToDestination($(hash).offset().top, heightDifference);
      }

      // On click, smooth scroll this baby!
      $('a[href^="#"], a[href^="/#"]').click(function (event) {
        if ($(document).find('#single-page-overall-wrapper').length) {
          // Only do this on the single page.
          event.preventDefault();
        }
        var hrefValue = $(this).attr('href').replace('/', '');
        var strippedHref = hrefValue.replace('#', '').replace('/', '');

        if (Drupal.behaviors.scrollToAnchor.validateSelector(hrefValue)) {
          if ($(hrefValue).length > 0) {
            var linkOffset = $(this.hash).offset().top;
            Drupal.behaviors.scrollToAnchor.scrollToDestination(linkOffset, heightDifference);
          }
          else if ($('a[name=' + strippedHref + ']').length > 0) {
            var linkOffset = $('a[name=' + strippedHref + ']').offset().top;
            Drupal.behaviors.scrollToAnchor.scrollToDestination(linkOffset, heightDifference);
          }
        }
      });
    },
    validateSelector: function (a) {
      return /^#[a-z]{1}[a-z0-9_-]*$/i.test(a);
    },
    scrollToDestination: function (a, b) {
      if (a > b) {
        var destination = b;
      } else {
        var destination = a;
      }
      if (Drupal.settings.singlePage.offsetSelector) {
        // Take offset selector height into account.
        if ($(document).find(Drupal.settings.singlePage.offsetSelector).length) {
          destination -= $(Drupal.settings.singlePage.offsetSelector).height();
        }
      }
      $('html,body').stop().animate({scrollTop: destination}, 800, 'swing');
    }
  };

  $(function () {
    // Init scroll behaviour.
    Drupal.behaviors.scrollToAnchor.init();
  });
}(jQuery));
