(function($) {
  Drupal.behaviors.globallinkBlockActiveSubmissions = {
    attach: function(context, settings) {
      if ($('.globallink-block-active-select-form', context).length == 0) {
        return;
      }

      $('.globallink-block-select-active-submission', context).change(function() {
        $('.globallink-block-active-select-form').submit();
      });
    }
  };
})(jQuery);
