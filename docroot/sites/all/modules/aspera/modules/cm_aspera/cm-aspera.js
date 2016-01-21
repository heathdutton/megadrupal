(function($) {
  Drupal.cm_aspera = {};
  var click = false; // Allow Submit/Edit button
  var edit = false; // Dirty form flag

  Drupal.behaviors.CMAspera = {
    attach : function(context) {
      
      // Let all form submit buttons through
      $(".node-form input[type='submit']").each(function() {
        $(this).addClass('cm-aspera-processed');
        $(this).click(function() {
          //console.log($("#progress_meter").text());
          if ($("#transfer_spec").text()) {
						var asperaTranser = jQuery.parseJSON($("#transfer_spec").text());
						//console.log(asperaTranser.paths[0].source);
						var fileNameIndex = asperaTranser.paths[0].source.lastIndexOf("/") + 1;
						var filename = asperaTranser.paths[0].source.substr(fileNameIndex);
						$("#edit-field-expected-filename-und-0-value").val(filename)
						//var asperaConnectionSettings = jQuery.parseJSON(asperaProgress.aspera_connect_settings);
						//$("#edit-field-aspera-session-id-und-0-value").val(asperaProgress.aspera_connect_settings.request_id);
          }
          click = true;
        });
      });

      // Catch all links and buttons EXCEPT for "#" links
      $("a, button, input[type='submit']:not(.cm-aspera-processed)")
          .each(function() {
            $(this).click(function() {
            
              // return when a "#" link is clicked so as to skip the
              // window.onbeforeunload function
              if (edit && $(this).attr("href") != "#") {
                return 0;
              }
              
            });
          });

      // Handle backbutton, exit etc.
      window.onbeforeunload = function() {
        
        // Assume they aren't uploading unless they click the Upload buton 
        if (!click && $("#aspera_status_message").text()) {
          return (Drupal.t("You must save the Show after starting an upload."));
        }
      }
    }
  };
})(jQuery);
