var odir_field = {};
var odir_behavior_count = 0;
(function ($) {
Drupal.behaviors.odir_fields = {
  attach: function (context, settings) {
    odir_behavior_count++;
    jQuery('#edit-odir-path').after("\n<DIV ID=\"odir_field_navigator\" />\n");
    jQuery('#edit-odir-path').after("\n<h4 id=\"odir_field_navigator_preview_heading\">Preview: </h4><DIV ID=\"odir_field_navigator_preview\"></div>\n");
    jQuery('#edit-odir-path').change(odir_field.retrieveDirectories);
    if (odir_behavior_count == 1) {
      odir_field_retrieveDirectories();
    }
  }
};
})(jQuery);

odir_field.retrieveDirectories = function (dir2) {
  dir_field_retrieveDirectories (dir2);
};

function odir_field_retrieveDirectories (dir2) {
  if (typeof  Drupal.settings.odir == 'undefined') {
    return;
  }
  var dir = "";
  if (dir2) {
    dir = dir2;
  }
  else {
    dir = jQuery(this).val();
  }
  if (dir != "<none>") {
    jQuery('#odir_field_navigator_newdir_info').hide();
    jQuery('#odir_path_selector').show();
    jQuery.get(Drupal.settings.odir.ajax_directories_url + "/" + dir, function(data) {
      jQuery("#odir_field_navigator").html(data);
      Drupal.attachBehaviors("#odir_field_navigator");
      jQuery('#odir_field_navigator').show();

      jQuery('.odir_filepath_chooser_link').click(function(event) {
        event.preventDefault();
        $('#edit-odir-path').val(jQuery(this).attr("href"));
        odir_field.retrieveDirectories(jQuery('#edit-odir-path').val());
      });
    });
    jQuery.get(Drupal.settings.odir.ajax_images_url + "/" + dir, function(data) {
      jQuery("#odir_field_navigator_preview").html(data);
      Drupal.attachBehaviors("#odir_field_navigator_preview");
      jQuery('#odir_field_navigator_preview').show();
    });
  }
  else {
    jQuery('#odir_field_navigator_newdir_info').show();
    jQuery('#odir_path_selector').hide();
    jQuery('#odir_field_navigator').hide();
  }
}
