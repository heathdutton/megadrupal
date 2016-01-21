/**
 * @file
 * Behaviours for the Baidu Analytics admin settings form page's vertical tabs.
 */

(function ($) {

/**
 * Provide the summary information for the tracking settings vertical tabs.
 */
Drupal.behaviors.trackingSettingsSummary = {
  attach: function (context) {
    // Make sure this behavior is processed only if drupalSetSummary is defined.
    if (typeof jQuery.fn.drupalSetSummary == 'undefined') {
      return;
    }

    // Page visibility settings tabs summary.
    $('fieldset#edit-page-vis-settings', context).drupalSetSummary(function (context) {
      var $radio = $('input[name="baidu_analytics_visibility_pages"]:checked', context);
      if ($radio.val() == 0) {
        if (!$('textarea[name="baidu_analytics_pages"]', context).val()) {
          return Drupal.t('Not restricted');
        }
        else {
          return Drupal.t('All pages with exceptions');
        }
      }
      else {
        return Drupal.t('Restricted to certain pages');
      }
    });

    // Pages visibility settings vertical tab summary.
    $('fieldset#edit-role-vis-settings', context).drupalSetSummary(function (context) {
      var vals = [];
      $('input[type="checkbox"]:checked', context).each(function () {
        vals.push($.trim($(this).next('label').text()));
      });
      if (!vals.length) {
        return Drupal.t('Not restricted');
      }
      else if ($('input[name="baidu_analytics_visibility_roles"]:checked', context).val() == 1) {
        return Drupal.t('Excepted: @roles', {'@roles' : vals.join(', ')});
      }
      else {
        return vals.join(', ');
      }
    });

    // Roles visibility settings vertical tab summary.
    $('fieldset#edit-user-vis-settings', context).drupalSetSummary(function (context) {
      var $radio = $('input[name="baidu_analytics_custom"]:checked', context);
      if ($radio.val() == 0) {
        return Drupal.t('Not customizable');
      }
      else if ($radio.val() == 1) {
        return Drupal.t('On by default with opt out');
      }
      else {
        return Drupal.t('Off by default with opt in');
      }
    });

    // Links and Downloads Tracking settings vertical tab summary.
    $('fieldset#edit-linktracking', context).drupalSetSummary(function (context) {
      var vals = [];
      if ($('input#edit-baidu-analytics-trackoutbound', context).is(':checked')) {
        vals.push(Drupal.t('Outbound links'));
      }
      if ($('input#edit-baidu-analytics-trackmailto', context).is(':checked')) {
        vals.push(Drupal.t('Mailto links'));
      }
      if ($('input#edit-baidu-analytics-trackfiles', context).is(':checked')) {
        vals.push(Drupal.t('Downloads'));
      }
      if (!vals.length) {
        return Drupal.t('Not tracked');
      }
      return Drupal.t('@items enabled', {'@items' : vals.join(', ')});
    });

    // Messages Tracking settings vertical tab summary.
    $('fieldset#edit-messagetracking', context).drupalSetSummary(function (context) {
      var vals = [];
      $('input[type="checkbox"]:checked', context).each(function () {
        vals.push($.trim($(this).next('label').text()));
      });
      if (!vals.length) {
        return Drupal.t('Not tracked');
      }
      return Drupal.t('@items enabled', {'@items' : vals.join(', ')});
    });

    // Search and Advertising settings vertical tab summary.
    $('fieldset#edit-search-and-advertising', context).drupalSetSummary(function (context) {
      var vals = [];
      if ($('input#edit-baidu-analytics-site-search', context).is(':checked')) {
        vals.push(Drupal.t('Site search'));
      }
      if (!vals.length) {
        return Drupal.t('Not tracked');
      }
      return Drupal.t('@items enabled', {'@items' : vals.join(', ')});
    });

    // Privacy settings vertical tab summary.
    $('fieldset#edit-privacy', context).drupalSetSummary(function (context) {
      var vals = [];
      if ($('input#edit-baidu-analytics-privacy-donottrack', context).is(':checked')) {
        vals.push(Drupal.t('Universal web tracking opt-out'));
      }
      if (!vals.length) {
        return Drupal.t('No privacy');
      }
      return Drupal.t('@items enabled', {'@items' : vals.join(', ')});
    });
  }
};

})(jQuery);
