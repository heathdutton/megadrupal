/**
 * Check/Uncheck all checkboxes depend on "All" checkbox.
 */
function form_maker_check_all(check_all_id, id_col_string, id) {
  var if_checked;
  if (document.getElementById(check_all_id).checked == true) {
    if_checked = true;
  }
  else {
    if_checked = false;
  }
  var ids = id_col_string.split("@@&&@@");
  for (key in ids) {
    if (document.getElementById(id + ids[key]) && (document.getElementById(id + ids[key]).disabled == false)) {
      document.getElementById(id + ids[key]).checked = if_checked;
    }
  }
}

/**
 * Check/Uncheck "All" checkbox depend on checkboxes.
 */
function form_maker_checked_one(checked_id, id_col_string) {
  var ids = id_col_string.split("@@&&@@");
  // Check "All" checkbox when all checkboxes are checked.
  if ((document.getElementById("edit-check-all-box").checked == false) && (document.getElementById("checkbox_" + checked_id).checked == true)) {
    var all_is_checked = true;
    for (key in ids) {
      if (document.getElementById("checkbox_" + ids[key]).checked == false) {
        all_is_checked = false;
      }
    }
    if (all_is_checked == true) {
      document.getElementById("edit-check-all-box").checked = true;
    }
  }
  // Uncheck "All" checkbox when all checkboxes are unchecked.
  if ((document.getElementById("edit-check-all-box").checked == true) && (document.getElementById("checkbox_" + checked_id).checked == false)) {
    document.getElementById("edit-check-all-box").checked = false;
  }
}
