/**
 * @file
 * JavaScript file for the ac_newsletter module.
 */

(function ($) {

/**
 * Provide the summary information for the block settings vertical tabs.
 */
Drupal.behaviors.ac_newsletterBlockSettingsSummary = {
  attach: function (context) {
    // The drupalSetSummary method required for this behavior is not available
    // on the Blocks administration page, so we need to make sure this
    // behavior is processed only if drupalSetSummary is defined.
    if (typeof jQuery.fn.drupalSetSummary == 'undefined') {
      return;
    }

    // Sets the summary description for the firstname field veritcal tab.
    $('fieldset#edit-firstname', context).drupalSetSummary(function (context) {
      var vals = [];
      if ($('input[name="ac_newsletter_en_firstname"]:checked').length) {
        $('input[type="checkbox"]:checked', context).each(function () {
          vals.push(Drupal.t($.trim($(this).next('label').text())));
        });
        var vals2 = [];
        $('input[type="text"]', context).each(function () {
          if ($(this).val()) {
            if (!vals2.length) {
              vals2.push(Drupal.t($.trim('Customized: ' + $(this).prev('label').text())));
            }
            else {
              vals2.push(Drupal.t($.trim($(this).prev('label').text())));
            }
          }
        });
        vals = $.merge(vals, vals2);
      }
      if (!vals.length) {
        vals.push(Drupal.t('Not enabled'));
      }
      return vals.join(', ');
    });

    // Sets the summary description for the lastname field veritcal tab.
    $('fieldset#edit-lastname', context).drupalSetSummary(function (context) {
      var vals = [];
      if ($('input[name="ac_newsletter_en_lastname"]:checked').length) {
        $('input[type="checkbox"]:checked', context).each(function () {
          vals.push(Drupal.t($.trim($(this).next('label').text())));
        });
        var vals2 = [];
        $('input[type="text"]', context).each(function () {
          if ($(this).val()) {
            if (!vals2.length) {
              vals2.push(Drupal.t($.trim('Customized: ' + $(this).prev('label').text())));
            }
            else {
              vals2.push(Drupal.t($.trim($(this).prev('label').text())));
            }
          }
        });
        vals = $.merge(vals, vals2);
      }
      if (!vals.length) {
        vals.push(Drupal.t('Not enabled'));
      }
      return vals.join(', ');
    });

    // Sets the summary description for the email field veritcal tab.
    $('fieldset#edit-email', context).drupalSetSummary(function (context) {
      var vals = [];
      $('input[type="text"]', context).each(function () {
        if ($(this).val()) {
          if (!vals.length) {
            vals.push(Drupal.t($.trim('Customized: ' + $(this).prev('label').text())));
          }
          else {
            vals.push(Drupal.t($.trim($(this).prev('label').text())));
          }
        }
      });
      if (!vals.length) {
        vals.push(Drupal.t('Not customizable'));
      }
      return vals.join(', ');
    });

    // Sets the summary description for the unsubscribe field veritcal tab.
    $('fieldset#edit-unsubscribe', context).drupalSetSummary(function (context) {
      var vals = [];
      if ($('input[name="ac_newsletter_en_unsubscribe"]:checked').length) {
        $('input[type="checkbox"]:checked', context).each(function () {
          vals.push(Drupal.t($.trim($(this).next('label').text())));
        });
        var vals2 = [];
        $('input[type="text"]', context).each(function () {
          if ($(this).val()) {
            if (!vals2.length) {
              vals2.push(Drupal.t($.trim('Customized: ' + $(this).prev('label').text())));
            }
            else {
              vals2.push(Drupal.t($.trim($(this).prev('label').text())));
            }
          }
        });
        vals = $.merge(vals, vals2);
      }
      if (!vals.length) {
        vals.push(Drupal.t('Not enabled'));
      }
      return vals.join(', ');
    });

    // Sets the summary description for the list options field veritcal tab.
    $('fieldset#edit-list-options', context).drupalSetSummary(function (context) {
      var vals = [];
      if ($('input[name="ac_newsletter_en_lt_options"]:checked').length) {
        $('input[type="checkbox"]:checked', context).each(function () {
          vals.push(Drupal.t($.trim($(this).next('label').text())));
        });
        var vals2 = [];
        $('input[type="text"]', context).each(function () {
          if ($(this).val()) {
            if (!vals2.length) {
              vals2.push(Drupal.t($.trim('Customized: ' + $(this).prev('label').text())));
            }
            else {
              vals2.push(Drupal.t($.trim($(this).prev('label').text())));
            }
          }
        });
        vals = $.merge(vals, vals2);
      }
      if (!vals.length) {
        vals.push(Drupal.t('Not enabled'));
      }
      return vals.join(', ');
    });

  }
};

})(jQuery);
