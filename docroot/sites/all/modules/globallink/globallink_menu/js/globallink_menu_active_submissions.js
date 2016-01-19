(function($) {
  Drupal.behaviors.globallinkMenuActiveSubmissions = {
    attach: function(context, settings) {
      if ($('.globallink-menu-active-select-form', context).length == 0) {
        return;
      }

      $('.globallink-menu-active-select-form-edit', context).change(function() {
        $('.globallink-menu-active-select-form').submit();
      });
    }
  };
})(jQuery);
