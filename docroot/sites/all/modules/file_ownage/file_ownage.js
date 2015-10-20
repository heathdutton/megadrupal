/**
 * @file
 * UI addition to enhance the inport source switcher.
 */

Drupal.behaviors.image_ownage = function(context) {
  $('#image-ownage-admin-settings:not(.image_ownage-filtered)', context).each(
        function() {
            var form = $(this);
            form.addClass('image_ownage-filtered');
            /*
            When the select changes, we hide the unwanted sub-fieldsets
            */
            $('#edit-image-ownage-settings-attach-method', form).change(
                function(){
                    // Hide most options.
                    $("fieldset.image-attachment-method-options").hide();
                    // The one to show is, eg: attach-as-a-filefield-wrapper
                    // "image-ownage-attach-imageholder-nodereference-wrapper"
                    // "fieldset#image-ownage-attach-imageholder-wysiwyg-wrapper"
                    var show_fieldset = this.value;
                    $("fieldset.image-attachment-method-options." + show_fieldset).show();
                }
            );
            $('#edit-image-ownage-settings-file-ownage-attach-method', form).change(
                function(){
                    // Hide most options.
                    $("fieldset.file-attachment-method-options").hide();
                    // The one to show is, eg: attach-as-a-filefield-wrapper
                    // "image-ownage-attach-imageholder-nodereference-wrapper"
                    // "fieldset#image-ownage-attach-imageholder-wysiwyg-wrapper"
                    var show_fieldset = this.value;
                    $("fieldset.file-attachment-method-options." + show_fieldset).show();
                }
            );
        }
    );
  // Trigger the filter to update the current display.
  $('#edit-image-ownage-settings-attach-method').trigger('change');
  $('#edit-image-ownage-settings-file-ownage-attach-method').trigger('change');
};
