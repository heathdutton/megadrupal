function spider_calendar_check_all(check_all_id, id_col_string, id) {
  var if_checked;
  if (document.getElementById(check_all_id).checked == true) {
    if_checked = true;
  }
  else {
    if_checked = false;
  }
  var ids = id_col_string.split(",");
  for (key in ids) {
    if (document.getElementById(id + ids[key]) && (document.getElementById(id + ids[key]).disabled == false)) {
      document.getElementById(id + ids[key]).checked = if_checked;
    }
  }
}
