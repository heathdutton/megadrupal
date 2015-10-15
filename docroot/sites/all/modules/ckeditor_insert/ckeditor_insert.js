(function($) {
  Drupal.behaviors.ckeditor_insert = {
    attach : function(context, settings) {
      ckeditor_insert_update();
      $('#ckeditor_file_insert_close_button').click(function() {
        $('#ckeditor_insert_fade').removeClass('file-dialog-is-open');
        ckeditor_insert_update();
      });
      $('#ckeditor_image_insert_close_button').click(function() {
        $('#ckeditor_insert_fade').removeClass('image-dialog-is-open');
        ckeditor_insert_update();
      });
    }
  };

  function ckeditor_insert_update() {
    if ($('#ckeditor_insert_fade').hasClass('file-dialog-is-open')) {
      $('#ckeditor_file_insert_light,#ckeditor_insert_fade').show();
    }
    else if ($('#ckeditor_insert_fade').hasClass('image-dialog-is-open')) {
      $('#ckeditor_image_insert_light,#ckeditor_insert_fade').show();
    }
    else {
      $('#ckeditor_file_insert_light,#ckeditor_image_insert_light,#ckeditor_insert_fade').hide();
    }
  }

})(jQuery);

