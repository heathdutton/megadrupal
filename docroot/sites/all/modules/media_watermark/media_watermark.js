/**
 * @file
 * Media watermark module javascript file.
 * 
 * Contains javascript for admin interface of media watermark module.
 */

(function ($) {
  Drupal.behaviors.flex2_media_watermark = {
    attach: function (context, settings) {
      $('.hide-select-list', context).hide();
      $('#edit-watermarks-images .image-hidden', context).hide();
      $('.form-item-watermarks-names', context).hide();
      $("#edit-add-watermark", context).click(function(){
        if ($('input[name=add_watermark]', context).is(':checked')) {
          $('#edit-upload-upload-button, #edit-upload-remove-button', context).hide();
          $('#edit-watermarks-images .image-hidden', context).first().show();
          $('.form-item-watermarks-names', context).show();
          $('#edit-watermarks-names option', context).first().attr('selected', true);
          $('#edit-watermarks-names',context).siblings('span.custom-select')
            .find('span span')
            .text($('#edit-watermarks-names option', context).first().text());
        }
        else {
          $('#edit-upload-upload-button, #edit-upload-remove-button', context).show();
          $('.form-item-watermarks-names', context).hide();
          $('#edit-watermarks-images .image-hidden', context).hide();
        }
      });
      $('#edit-watermarks-names', context).change(function () {
        var val = $('#edit-watermarks-names :selected', context).val();
        $('#edit-watermarks-images .image-hidden', context).hide();
        $('#edit-watermarks-images #image-' + val, context).show();
      });
    }
  };
})(jQuery);
