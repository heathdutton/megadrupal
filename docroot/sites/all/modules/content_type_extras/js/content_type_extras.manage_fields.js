(function ($) {
  Drupal.behaviors.content_type_extras_manage_fields = {
    attach: function(context, settings) {
      $('#edit-fields-add-new-field-label').blur(function() {
        var label = processLabel($(this));
        $('#edit-fields-add-new-field-field-name').val(label);
      });
      
      $('#edit-fields-add-existing-field-field-name').change(function() {
        var label = ($('#edit-fields-add-existing-field-field-name :selected').text());
        var name = label.match(/.+\((.*)\).*/);
        if (name) {
          $('#edit-fields-add-existing-field-label').val(name[1]);
        }
        else {
          $('#edit-fields-add-existing-field-label').val('');
        }
      });
      
      $('#edit-fields-add-new-group-label').blur(function() {
        var label = processLabel($(this));
        $('#edit-fields-add-new-group-group-name').val(label);
      });
      
      function processLabel($this) {
        return $this.val().toLowerCase()
                 .replace(/[\/\& ]+/g, '_')
                 .replace(/[^a-zA-Z 0-9_]+/g, '')
                 .substring(0, 26);
      }
    }
  }
})(jQuery);
