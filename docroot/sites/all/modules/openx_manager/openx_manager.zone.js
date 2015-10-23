/**
 * Use the Zone size presets.
 */
/*global Drupal, $ */

Drupal.behaviors.openx_manager_zone = function() {
  var once, sizes;
  if (!once) {
    once = true;
    $("#edit-size-preset").change(Drupal.behaviors.openx_manager_zone);
  }

  if ($("#edit-size-preset").val() != "") {
    sizes = $("#edit-size-preset").val().split("x");
    $("#edit-size-width").val(sizes[0]);
    $("#edit-size-height").val(sizes[1]);
    $("#edit-size-width").attr("disabled", "disabled");
    $("#edit-size-height").attr("disabled", "disabled");
  }
  else {
    $("#edit-size-width").removeAttr("disabled");
    $("#edit-size-height").removeAttr("disabled");
  }
};
