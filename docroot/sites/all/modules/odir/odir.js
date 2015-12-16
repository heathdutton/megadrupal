(function ($) {
Drupal.behaviors.odir = {
  attach: function (context, settings) {
    jQuery("#edit-files").attr("multiple", "yes");

    /**
     * Display upload status.
     */
    jQuery("#odir-multiple-file-upload-form").submit(function() {
      jQuery("#odir-uploading-status").css('visibility', 'visible');
    });

    /**
     * Deleting files
     */
    jQuery(".draggable_directory").once().draggable({opacity: 0.35, revert: true, zIndex: 1000});
    jQuery(".draggable_file").once().draggable({revert: true, zIndex: 1000, opacity: 0.35, revert: true, zIndex: 1000});

    jQuery("#odirtrashcan").once().droppable({
      greedy: true,
      background: 'silver',
      tolerance: 'pointer',
      zIndex: 1000,
      drop: function(event, ui) {
        var divid = ui.draggable[0].id;
        var path = jQuery('#'+divid+'').attr('file');
        if (jQuery('#'+divid+'').hasClass('draggable_file')) {
          if(! window.confirm(Drupal.t("File will be deleted! Continue?"))) {
            return false;
          }
          jQuery.post(Drupal.settings.odir.file_removal,
             {
              odir_path: path
             },
             function(data2) {
               jQuery('#odir_ajax_feedbacks').innerHTML += data2;
               Drupal.attachBehaviors(data2);
             }
          );
        }
        else if (jQuery('#'+divid+'').hasClass('draggable_directory')) {
          if(! window.confirm(Drupal.t("Directory will be deleted! Continue?"))) {
            return false;
          }
          jQuery.post(Drupal.settings.odir.directory_removal,
             {
              odir_path: path
             },
             function(data2) {
               jQuery("#odir_ajax_feedbacks").innerHTML += data2;
               Drupal.attachBehaviors(data2);
             }
           );
        }
        jQuery('#'+divid+'').remove();
      }
    });

    /**
     * Attach functions to events
     */
    jQuery('#odir_file_upload_ajax_link').once().click(odir.showFileUploadMask);
    jQuery('#odir_inline_directory_mask_selector_btn').once().click(odir.showInlineDirectoryMask);

    /* Ajax directory creation */
    jQuery('#odir_inline_directory_mask_selector_div').once().show(100);
    jQuery('#odir-create-directory-form').once().submit(odir.create_dir_form);

    }
  };
})(jQuery);


var odir = {};

/**
 * Show directory creation mask
 */
odir.showInlineDirectoryMask = function() {
  jQuery('#odir_inline_directory_mask').show(100);
  jQuery('#odir_inline_directory_mask_selector_div').hide(300);
};

/**
 * Show file upload form
 */
odir.showFileUploadMask = function() {
  if (jQuery("#odir_upload_form").is(":visible")) {
    jQuery('#odir_upload_form').hide(200);
  }
  else {
    jQuery('#odir_upload_form').show(200);
  }
};

/**
 * Create directory with Ajax
 */
odir.create_dir_form = function(event) {
  event.preventDefault();
  jQuery.post(Drupal.settings.odir.json_url_dir_creation + "/" + jQuery("input[name=odir_parent]").val(),
    {
      odir_currentdir: jQuery("input[name=odir_parent]").val(),
      odir_new_dir: jQuery("input[name=odir_new_dir]").val(),
      odir_div_filecounter: Drupal.settings.odir.odir_div_filecounter
    },
    function(data2) {
      odir_div_filecounter = Drupal.settings.odir.odir_div_filecounter;
      jQuery("#odir_inline_directory_placeholder").before(data2);
    }
  );
  jQuery('#odir_inline_directory_mask_selector_div').show(300);
  jQuery('#odir_inline_directory_mask').hide(100);
};

/**
 * Adds uploaded files into placeholders
 */
odir.show_uploaded_files = function (data) {
  jQuery("#odir_image_files_placeholder").before(data.main);
  jQuery("#odir_files_placeholder").before(data.block);
  jQuery("#odir-uploading-status").css('visibility', 'hidden');
};

/**
 * Load images with Ajax
 */
odir.album_load_directory = function (dir, div) {
  jQuery(document).ready(
    jQuery.get(Drupal.settings.odir.ajax_images_url + "/" + dir, function(data) {
      jQuery("#"+div).html(data);
      jQuery("#"+div).show();
    })
  );
};

/**
 * Check if directory exists
 */
odir.checkIfDirectoryExists = function(directory) {
  jQuery.get(Drupal.settings.odir.ajax_check_file_info + "/" + directory, function(data) {
    file_info = Drupal.parseJson(data);
    if (file_info.file_exists == 1) {
      return true;
    }
    else {
      return false;
    }
  });
};
