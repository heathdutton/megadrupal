/**
 * Hide/Show search row in submissions table.
 */
function form_maker_show_hide_search_row() {  if (document.getElementById("search_row_id")) {    
    var search_row_id = document.getElementById("search_row_id");
    var filter_img = document.getElementById("filter_img");
    if (search_row_id.style.display == "none") {
      search_row_id.style.display = "";
      filter_img.src = "" + Drupal.settings.form_maker.filter_img_src + "filter_hide.png";
    }
    else {
      search_row_id.style.display = "none";
      filter_img.src = "" + Drupal.settings.form_maker.filter_img_src + "filter_show.png";
    }  }
}
