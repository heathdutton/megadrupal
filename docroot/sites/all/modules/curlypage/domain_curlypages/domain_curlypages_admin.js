(function ($) {

/**
 * Provide the summary information for the curlypage settings vertical tabs.
 */
Drupal.behaviors.domainCurlypagesSettingsSummary = {
  attach: function (context) {
    // The drupalSetSummary method required for this behavior is not available
    // on the Curlypages administration page, so we need to make sure this
    // behavior is processed only if drupalSetSummary is defined.
    if (typeof jQuery.fn.drupalSetSummary == 'undefined') {
      return;
    }

    $('fieldset#edit-domain-vis-settings', context).drupalSetSummary(function (context) {
      var vals = [];
      $('input[type="checkbox"]:checked', context).each(function () {
        vals.push($.trim($(this).next('label').text()));
      });
      if (!vals.length) {
        return Drupal.t('Not restricted');
      }
      var $radio = $('input[name="domain_visibility"]:checked', context);
      if ($radio.val() == 0) {
        return Drupal.t('Show on:') + " " + vals.join(', ');
      }
      else {
        return Drupal.t('Show except on:') + " " + vals.join(', ');
      }
    });

  }
};

})(jQuery);
