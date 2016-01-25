/**
 * @file
 * Update default states when groups (and their profiles) are selected.
 */

// Quick little caching layer to support switching around without ajax.
var wmpCache = new Object;

(function ($) {
Drupal.behaviors.workbenchModerationProfiles = {
  attach: function(context, settings) {
    var groupSelect = $('select[name="og_group_ref[und][0][default]"]', context);
    var state = $('select[name="workbench_moderation_state_new"]');
    var workbenchBlock = $("#workbench_moderation_profile_label");

    // Prime cache with starting values.
    wmpCache[groupSelect.val()] = new Object;
    wmpCache[groupSelect.val()].options = state.html();
    wmpCache[groupSelect.val()].title = workbenchBlock.html();

    // On change of profile...
    groupSelect.change(function(context){
      var group = context.target.value;

      // Check caching.
      if (typeof wmpCache[group] == 'undefined') {
        // Ajax call to get available states from the profile's transitions.
        $.getJSON("/workbench_moderation_profile_og/" + group + "/" + state.val() + "/profile", function(data){
          // Update Workbench info block.
          workbenchBlock.html(data.profile.label);

          // Empty and populate possible states form element with new values.
          state.empty();
          $.each(data.states, function(key, val){
            if (key == data.current_state) {
              state.append("<option value='" + key + "' selected='selected'>Current: " + val + "</option>");
            }
            else {
              state.append("<option value='" + key + "'>" + val + "</option>");
            }
          });
          // Trigger change event on state form element; this updates vert. tab.
          state.trigger("change");
        });
        wmpCache[group] = new Object;
        wmpCache[group].options = state.html();
        wmpCache[group].title = workbenchBlock.html();
      }
      else {
        // Empty and populate possible states form element with cached values.
        state.empty();
        state.append(wmpCache[group].options);
        // Update Workbench info block.
        workbenchBlock.html(wmpCache[group].title);
        // Trigger change event on state form element; this updates vert. tab.
        state.trigger("change");
      }
    });
  }
}

})(jQuery);
