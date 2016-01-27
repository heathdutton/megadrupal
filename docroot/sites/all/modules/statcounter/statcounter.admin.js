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

    $('fieldset#edit-page-vis-settings', context).drupalSetSummary(function (context) {
      var $radio = $('input[name="statcounter_visibility_pages"]:checked', context);
      if ($radio.val() == 0) {
        if (!$('textarea[name="statcounter_pages"]', context).val()) {
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

    $('fieldset#edit-role-vis-settings', context).drupalSetSummary(function (context) {
      var vals = [];
      $('input[type="checkbox"]:checked', context).each(function () {
        vals.push($.trim($(this).next('label').text()));
      });
      if (!vals.length) {
        return Drupal.t('Not restricted');
      }
      else if ($('input[name="statcounter_visibility_roles"]:checked', context).val() == 1) {
        return Drupal.t('Excepted: @roles', {'@roles' : vals.join(', ')});
      }
      else {
        return vals.join(', ');
      }
    });

    $('fieldset#edit-user-vis-settings', context).drupalSetSummary(function (context) {
      var $radio = $('input[name="statcounter_custom"]:checked', context);
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

    $('fieldset#edit-privacy', context).drupalSetSummary(function (context) {
      var vals = [];
      if ($('input#edit-statcounter-privacy-donottrack', context).is(':checked')) {
        vals.push('Universal web tracking opt-out');
      }
      if (!vals.length) {
        return Drupal.t('No privacy');
      }
      return Drupal.t('@items enabled', {'@items' : vals.join(', ')});
    });
  }
};

})(jQuery);