(function($) {
  Drupal.behaviors.globallinkFieldConfiguration = {
    attach: function(context, settings) {
      if ($('.globallink-field', context).length == 0) {
        return;
      }

      $('.globallink-field-select-type', context).change(function() {
        $('.globallink-field').submit();
      });
    }
  };
})(jQuery);
