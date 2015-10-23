/**
 * Disable some form fields, depending on what banner type is selected.
 */
/*global Drupal, $ */

Drupal.behaviors.openx_manager_banner = function() {
  var once, fields_mask, field;
  if (!once) {
    once = true;
    $("#edit-btype").change(Drupal.behaviors.openx_manager_banner);
  }

  var fields = ["#edit-size-height", "#edit-size-width",
    "#edit-text-all", "#edit-text-below", "#edit-text-status",
    "#edit-data-text", "#edit-new-data-image", "#edit-data-url"];

  // Disable only the forbidden ones (marked as 1)
  if ($("#edit-btype").val() == "sql") {
    fields_mask = [0, 0, 1, 1, 1, 0, 1, 0];
  }
  else if ($("#edit-btype").val() == "web") {
    fields_mask = [0, 0, 1, 1, 1, 0, 1, 0];
  }
  else if ($("#edit-btype").val() == "url") {
    fields_mask = [1, 1, 1, 1, 1, 0, 0, 1];
  }
  else if ($("#edit-btype").val() == "url2") {
    fields_mask = [0, 0, 1, 1, 1, 0, 1, 0];
  }
  else if ($("#edit-btype").val() == "html") {
    fields_mask = [1, 1, 0, 0, 0, 1, 0, 0];
  }
  else if ($("#edit-btype").val() == "txt") {
    fields_mask = [0, 0, 0, 0, 1, 1, 0, 0];
  }

  for (field in fields) {
    if (fields_mask[field]) {
      $(fields[field] +"-wrapper").show();
      $(fields[field]).removeAttr("disabled");
    }
    else {
      $(fields[field]).attr("disabled", "disabled");
      $(fields[field] +"-wrapper").hide();
    }
  }
};
