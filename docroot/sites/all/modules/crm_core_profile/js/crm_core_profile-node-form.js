(function ($) {

  Drupal.behaviors.crm_core_profileFieldsetSummaries = {
    attach: function (context) {
      // Summaries on node edit pages.
      $('fieldset#edit-crm-core-profile', context).drupalSetSummary(function (context) {
        var profile_enabled = $('.form-item-crm-core-profile-node-enabled input:checked', context);
        var profile_label = '';
        if (profile_enabled.length != 0 && $('#edit-crm-core-profile-node-profile  option:selected', context).val()) {
          profile_label = $('#edit-crm-core-profile-node-profile  option:selected', context).text();
        }
        return Drupal.checkPlain(profile_label);
      });
    }
  };

})(jQuery);
