(function($) {
  Drupal.behaviors.block_remove_files = {
    attach : function(context) {
      $.each($('.delete-files tr:not(:first)'), function() { 
      var source = $('td:last span a', this).attr('href');
      var type = $('td:last', this).attr('field_type');
      if (type == 'image') {
        var $item = $('.field-item ').find('img[src$="' + source + '"]');
      }
      else {
        var $item = $('.field-item span.file').find('a[href$="' + source + '"]');
      }
      $('td:first input', this).click(function() {
        if ($(this).attr('checked')) {
          $item.parent().hide();
          }
        else
        {
            $item.parent().show();
        }
        });
        $(this).addClass('block-upload-table');
        $(this).hover(
          function() {
            $item.parent().addClass('block-upload-field');
        }, 
        function () {
          $item.parent().removeClass('block-upload-field');
        }
        );
      });
      $block_button = $('.form-managed-file').find('input[name="block_upload_file_upload_button"]');
      $block_button.hide();
    }
  }
})(jQuery);
