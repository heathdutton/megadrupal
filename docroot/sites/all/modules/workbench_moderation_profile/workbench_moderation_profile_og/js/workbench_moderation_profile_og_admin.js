/**
 * @file
 * Update default states when profiles are selected.
 */
(function ($) {

Drupal.behaviors.workbenchModerationProfiles = {
  attach: function(context, settings) {
    var profileSelect = $('select[name="workbench_moderation_profile_og"]', context);
    var state = $('select[name="workbench_moderation_profile_og_default_state"]');
    if (profileSelect.val() == 0) {
      state.empty();
    }

    // On change of profile...
    profileSelect.change(function(context){
      var profile = context.target.value;

      // Ajax call to get available states from the profile's transitions.
      $.getJSON("/workbench_moderation_profile/" + profile + "/states", function(data){
        // Empty and populate with new values from ajax.
        state.empty();
        $.each(data, function(key, val){
          state.append("<option value='" + key + "'>" + val + "</option>");
        });
      });
    });
  }
}

})(jQuery);
