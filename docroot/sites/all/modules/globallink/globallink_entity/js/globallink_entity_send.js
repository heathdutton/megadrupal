(function($) {
  Drupal.behaviors.globallinkEntitySend = {
    attach: function(context, settings) {
      if ($('.globallink-entity-form-clear', context).length == 0) {
        return;
      }

      $('.globallink-entity-form-clear-submit', context).click(function(e) {
        if (!confirm(Drupal.t('Are you sure you want to clear the changed status for the selected content(s)?'))) {
          e.preventDefault();
        }
      });
    }
  };
})(jQuery);
