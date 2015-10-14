(function ($) {

  /**
   * Attaches the Moxiemanager opener behavior to Field API File fields.
   */
  Drupal.behaviors.editField = {
    attach: function (context) {
      $('.filefield-moxiemanager-edit', context).once('filefield-moxiemanager-edit').each(function(e) {
        var $button = $(this);

        // If cardinality is multiple, the wrapper is a TR element.
        if ($button.hasClass('multiple')) {
          var $wrapper = $button.parents('tr.draggable');
        }
        else {
          var $wrapper = $button.parents('.form-managed-file');
        };

        $button.bind('click', function(e) {
          e.preventDefault();
          e.stopPropagation();
          var edit_path = $('input[name*="[moxiemanager_edit_path]"]', $wrapper).val();

          moxman.edit({
            path: edit_path,
            onsave: function(args) {
              filefieldFileManagerEdit.updateFile(args.file, $wrapper);
            }
          });
        });
      });
    }
  };

  var filefieldFileManagerEdit = {

    /**
     * After saving the picture in the moxiemanager with a ajax callback.
     */
    updateFile: function(file, $wrapper) {
      filefieldFileManagerEdit.addThrobber($wrapper);

      var data = {
        url: file.url,
        field_name: $('input[name*="[moxiemanager_field_name]"]', $wrapper).val(),
        bundle_name: $('input[name*="[moxiemanager_bundle_name]"]', $wrapper).val(),
        entity_type: $('input[name*="[moxiemanager_entity_type]"]', $wrapper).val()
      };

      $.ajax({
        type: 'post',
        url: Drupal.settings.basePath + 'moxiemanager_filefield_edit_ajax',
        data: data,
        dataType: 'json',
        success: function(data) {
          filefieldFileManagerEdit.replaceFile(data, $wrapper);
        }
      });
    },

    /**
     *
     */
    removeThrobber: function($wrapper) {
      $('.mm-throbber', $('.image-preview img', $wrapper).parent()).remove();
    },

    /**
     *
     */
    addThrobber: function($wrapper) {
      var $img = $('.image-preview img', $wrapper);
      $img.parent().css('position', 'relative');
      $img.after($('<div />').addClass('mm-throbber').css({
        'width': $img.width(),
        'height': $img.height(),
        'position': 'absolute',
        'top': '0',
        'left': '0',
      }));
    },

    /**
     *
     */
    replaceFile: function(data, $wrapper) {
      $('.image-preview img', $wrapper).unbind('load').bind('load', function() {
        filefieldFileManagerEdit.removeThrobber($wrapper);

        $(this).attr({
          'width': data.mm_thumbnail_width,
          'height': data.mm_thumbnail_height
        });

        $('input[name$="[fid]"]', $wrapper).val(data.fid);
        $('.file-size', $wrapper).text('(' + data.filesize / 1000 + ' kB)');
        $('.file a', $wrapper).attr('href', data.url);
        $('.file a', $wrapper).text(data.filename);

      }).attr({'src': data.mm_thumbnail_url});
    }
  };

})(jQuery);
