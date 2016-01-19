(function($) {
  Drupal.behaviors.globallinkEntityActiveSubmissions = {
    attach: function(context, settings) {
      if ($('.globallink-entity-active-select-form', context).length == 0) {
        return;
      }

      $('.globallink-entity-select-active-submission', context).change(function() {
        $('.globallink-entity-active-select-form').submit();
      });
    }
  };
})(jQuery);
