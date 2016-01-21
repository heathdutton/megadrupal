(function($) {
  Drupal.behaviors.cmis_browser = {
    attach : function(context, settings) {
      // Attach a close button to the folder dialog box.
      $('.cmis_views_browse_folder').dialog("option", "buttons", {
        "Select This Folder" : function() {
          var path = $(this).dialog("option", "title");
          if($('.edit-cmis_views_field_browser').length){
	        $('.edit-cmis_views_field_browser')[0].value = path.substr(1,path.length-1);
          }
          if($('#edit-filepath').length){
            $('#edit-filepath')[0].value = path.substr(1,path.length-1);
          }
          $(this).dialog("close");
        }
      });
    }
  }
})(jQuery);
