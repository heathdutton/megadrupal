/**
 * Record custom activities from Drupal norms.
 */

(function ($) {
  Drupal.behaviors.SemiAnonymousActivitiesFormSubmit = {
    attach: function (context, settings) {

      var submitData = {};
      $('form').submit(function(e) {
        submitData.formId = $(this).find('input[name="form_id"]').val();
        submitData.url = window.location.href;
        groucho.createActivity('form_submit', submitData);
      });

    }
  };
})(jQuery);
