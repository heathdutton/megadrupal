/**
 * @file
 * 
 * Provides vertical tabs summaries for theme settings form.
 */

(function ($) {

Drupal.behaviors.themesettingsFieldsetSummaries =  {

  attach: function (context) {
    $('fieldset#edit-theme-settings', context).drupalSetSummary(function (context) {
      var vals = [];

      $('input:checked', context).parent().each(function () {
        vals.push(Drupal.checkPlain($.trim($(this).text())));
      });

      return vals.join(', ');
    });

    $('fieldset#edit-logo', context).drupalSetSummary(function (context) {
      // handle checkbox settings
      if ( $('.form-type-checkbox', context).length > 0) {
        return $('#edit-default-logo', context).is(':checked') ?
          Drupal.t('Default logo') :
          Drupal.t('Custom logo');
      }
      // handle radio button settings
      else if ( $('.form-type-radios', context).length > 0) {
        return $('#edit-logo-display .form-item', context)
          .has(':radio:checked')
          .find('label')
          .text();
      }
    });

    $('fieldset#edit-favicon', context).drupalSetSummary(function (context) {
      // handle checkbox settings
      if ( $('.form-type-checkbox', context).length > 0) {
        return $('#edit-default-favicon', context).is(':checked') ?
          Drupal.t('Default logo') :
          Drupal.t('Custom logo');
      }
      // handle radio button settings
      else if ( $('.form-type-radios', context).length > 0) {
        return $('#edit-favicon-display .form-item', context)
          .has(':radio:checked')
          .find('label')
          .text();
      }
    });

    $('fieldset#color_scheme_form', context).drupalSetSummary(function (context) {
      return $('#edit-scheme option:selected', context).text();
    });

  }
};

})(jQuery);
