/**
 * @file
 * Update default states when profiles are selected.
 */
(function ($) {

Drupal.behaviors.workbenchModerationProfiles = {
  attach: function(context, settings) {
    // On change of profile...
    $('select[name="workbench_moderation_profile_node"]', context).change(function(context){
      var profile = context.target.value;
      var state = $('select[name="workbench_moderation_default_state"]');

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
