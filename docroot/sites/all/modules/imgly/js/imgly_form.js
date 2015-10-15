/* Fieldset summaries for Imgly node form */

(function ($) {
  "use strict";

  Drupal.behaviors.pathFieldsetSummaries = {
    attach: function (context) {
      $('fieldset.path-form', context).drupalSetSummary(function (context) {
        var path = $('.form-item-path-alias input').val();
        var automatic = $('.form-item-path-imgly input').attr('checked');

        if (automatic) {
          return Drupal.t('Automatic alias');
        }
        if (path) {
          return Drupal.t('Alias: @alias', {
            '@alias': path
          });
        }
        else {
          return Drupal.t('No alias');
        }
      });
    }
  };

})(jQuery);