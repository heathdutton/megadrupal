(function($) {
  Drupal.behaviors.globallinkTaxonomyActiveSubmissions = {
    attach: function(context, settings) {
      if ($('.globallink-taxonomy-active-select-form', context).length == 0) {
        return;
      }

      $('.globallink-taxonomy-select-active-submission', context).change(function() {
        $('.globallink-taxonomy-active-select-form').submit();
      });
    }
  };
})(jQuery);
