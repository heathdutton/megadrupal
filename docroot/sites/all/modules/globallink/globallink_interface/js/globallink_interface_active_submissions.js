(function($) {
  Drupal.behaviors.globallinkInterfaceActiveSubmissions = {
    attach: function(context, settings) {
      if ($('.globallink-interface-active-select-form', context).length == 0) {
        return;
      }

      $('.globallink-interface-select-active-submission', context).change(function() {
        $('.globallink-interface-active-select-form').submit();
      });
    }
  };
})(jQuery);
