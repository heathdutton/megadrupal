/**
 * Record custom activities from Drupal norms.
 */

/* global dataLayer, DataLayerHelper */

(function ($, window, dataLayer) {
  Drupal.behaviors.semiAnonymousFormSubmit = {
    attach: function (context, settings) {

      // Ajax protection.
      if (context !== document) return;

      // Record configured form submits.
      $('form').submit(function() {
        var forms = settings.semiAnonymous.recordForms,
            formId = $(this).find('input[name="form_id"]').val(),
            dlHelper = new DataLayerHelper(dataLayer);

        if (forms.length === 0 || $.inArray(formId, forms) >= 0) {
          groucho.createActivity('formSubmit', {
            formId: formId,
            url: window.location.href,
            entityBundle: dlHelper.get('entityBundle'),
            entityId: dlHelper.get('entityId'),
            entityTnid: dlHelper.get('entityTnid'),
          });
        }
      });

    }
  };
})(jQuery, window, dataLayer);
