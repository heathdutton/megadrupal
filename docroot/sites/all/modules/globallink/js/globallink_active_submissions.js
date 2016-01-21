(function($) {
  Drupal.behaviors.globallinkActiveSubmissions = {
    attach: function(context, settings) {
      if ($('.globallink-node-select-submission-form', context).length == 0) {
        return;
      }

      $('.globallink-node-select-submission-edit', context).change(function() {
        $('.globallink-node-select-submission-form').submit();
      });
    }
  };
})(jQuery);
