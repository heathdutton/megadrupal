/**
 * @file
 * Sticky Sharrre Bar UI.
 */

(function ($) {

  'use strict';

  jQuery.exists = function(selector) {
    return ($(selector).length > 0);
  };

  /**
   * Attaches the Sticky Sharrre Bar behavior to each block element.
   */
  Drupal.behaviors.stickySharrreBarRender = {
    attach: function (context, settings) {
      var enableTracking = (settings.googleanalytics && settings.stickySharrreBar.useGoogleAnalyticsTracking == 1) ? true : false,
        blockRegion = settings.stickySharrreBar.blockRegion,
        isCustomSelector = settings.stickySharrreBar.isCustomSelector,
        providerIsEnabled = {},
        selector = '';

      if (isCustomSelector) {
        selector = blockRegion;
      } else {
        // Try to find class, id or tag of region.
        if ($.exists('.' + blockRegion)) {
          selector = '.' + blockRegion + ':first';
        } else if ($.exists('#' + blockRegion)) {
          selector = '#' + blockRegion;
        } else if ($.exists(blockRegion)) {
          selector = blockRegion + ':first';
        } else {
          return;
        }
      }

      $('.sticky_sharrre_bar', context).insertAfter(selector).waypoint('sticky');

      $.each(settings.stickySharrreBar.providers, function (provider) {
        if (provider) {
          providerIsEnabled[provider] = true;

          $('#' + provider, context).sharrre({
            share: providerIsEnabled,
            template: '<a class="share ' + provider + '" href="#">' + Drupal.t('Share on <span class="provider_name">!provider</span>', {'!provider': provider}, {}) + '</a></div><span class="count"><a href="#">{total}</a></span>',
            enableHover: false,
            enableTracking: enableTracking,
            urlCurl: (provider == 'stumbleupon' || provider == 'googlePlus') ? '/sharrre' : '',
            click: function (api, options) {
              api.simulateClick();
              api.openPopup(provider);
            }
          });
        }
      });
    }
  };

})(jQuery);
