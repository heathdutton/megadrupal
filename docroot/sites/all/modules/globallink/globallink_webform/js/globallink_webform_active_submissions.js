(function($) {
  Drupal.behaviors.globallinkWebformActiveSubmissions = {
    attach: function(context, settings) {
      if ($('.globallink-webform-active-select-form', context).length == 0) {
        return;
      }

      $('.globallink-webform-select-active-submission', context).change(function() {
        $('.globallink-webform-active-select-form').submit();
      });
    }
  };
})(jQuery);
