/**
 * @file
 * JavaScript behaviors for the Implied Consent module.
 */

(function ($) {

/**
 * Custom summary for the module vertical tab.
 */
Drupal.behaviors.impliedConsentVerticalTabs = {
  attach: function (context) {

    if (!String.prototype.trim) {
      String.prototype.trim = function() {
        return this.replace(/^\s+|\s+$/g,'');
      };
    }

    var checkmark = '&#10003;';
    var exmark = '&#10008;';

    $('fieldset#edit-advanced', context).drupalSetSummary(function (context) {
      var variant = Drupal.checkPlain($('input[name="impliedconsent_variant"]:checked', context).val());
      var method = Drupal.checkPlain($('input[name="impliedconsent_method"]:checked', context).val());
      var async = Drupal.checkPlain($('#edit-impliedconsent-async', context).is(':checked'));

      summary = 'Variant: ' + variant + '<br/>';
      summary += 'Method: ' + method + '<br/>';
      summary += (async == "true" ? checkmark : exmark) + ' Skip autoloading' + '<br/>';

      return summary;
    });

    $('fieldset#edit-page-vis-settings', context).drupalSetSummary(function (context) {
      var mode = Drupal.checkPlain($('input[name="impliedconsent_visibility_pages"]:checked', context).val());
      var list = Drupal.checkPlain($('textarea[name="impliedconsent_pages"]', context).val().trim());

      summary = (list.length > 0) ? 'Restricted to certain pages' : 'Not restricted';

      return summary;
    });

    $('fieldset#edit-role-vis-settings', context).drupalSetSummary(function (context) {
      var mode = Drupal.checkPlain($('input[name="impliedconsent_visibility_roles"]:checked', context).val());
      var roles = [], roleList;

      $('input[name^="impliedconsent_roles"]', context).each(function(key) {
        if ($(this).is(':checked')) {
          roles.push($(this).next('label').text().trim());
        }
      });
      roleList = Drupal.checkPlain(roles.join(", "));

      if (roles.length > 0) {
        summary = 'Visible to ';
        summary += (mode == '0') ? 'the following roles' : 'every role except';
        summary += '<br/>';
        summary += roleList;
      }
      else {
        summary = 'Not restricted';
      }

      return summary;
    });

  }
};

})(jQuery);
