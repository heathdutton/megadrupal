(function ($) {
  var hideElements = function(formatter, form) {
    $('.form-item-settings-image-style', form).hide();
    $('#edit-settings-size', form).hide();
    switch (formatter) {
      case 'node_field_file_image':
    $('.form-item-settings-image-style', form).show();
        break;
      case 'node_field_file_audio':
      case 'node_field_file_video':
        $('#edit-settings-size', form).show();
        break;
    }
  }

  Drupal.behaviors.node_field_file = {
    attach: function (context, settings) {
      var form = $('#node-field-node-field-edit-form');
      var defaultItem = $('#edit-settings-formatter', form).val();
      hideElements(defaultItem, form);
      $('#edit-settings-formatter', form).change(function(){
        var item = $(this).val();
        hideElements(item, form);
      });

    }
  };

})(jQuery);