(function($) {
  Drupal.behaviors.globallinkFieldablePanelsActiveSubmissions = {
    attach: function(context, settings) {
      if ($('.globallink-fieldable-panels-active-select-form', context).length == 0) {
        return;
      }

      $('.globallink-fieldable-panels-select-active-submission', context).change(function() {
        $('.globallink-fieldable-panels-active-select-form').submit();
      });
    }
  };
})(jQuery);
