(function ($) {

  Drupal.behaviors.crm_core_profileNodeTypeFieldsetSummaries = {
    attach: function (context) {
      // Summaries on node type edit pages.
      $('fieldset.crm-core-profile-node-settings-form', context).drupalSetSummary(function (context) {
        var profile_enabled = $('.form-item-crm-core-profile-node-type-enabled input:checked', context);
        var vals = [];
        if (profile_enabled.length != 0) {
          var profile_label = $('#edit-crm-core-profile-node-type-label', context).val();
          if (profile_label) {
            vals.push('Profile label: ' + Drupal.checkPlain(profile_label));
          }
          // CRM Core Profile machine name.
          var profile_mname = $('#edit-crm-core-profile-node-type-profile option:selected', context).val();
          if (profile_mname) {
            var profile_name = $('#edit-crm-core-profile-node-type-profile option:selected', context).text();
            vals.push('Profile name: ' + Drupal.checkPlain(profile_name));
          }
        }
        return vals.join('<br />');
      });
    }
  };

})(jQuery);
