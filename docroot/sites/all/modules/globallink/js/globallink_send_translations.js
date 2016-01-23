(function($) {
  Drupal.behaviors.globallinkSendTranslations = {
    attach: function(context, settings) {
      if ($('.globallink-node-form-clear', context).length == 0) {
        return;
      }

      $('.globallink-node-form-clear-submit', context).click(function(e) {
        if (!confirm(Drupal.t('Are you sure you want to clear the changed status for the selected content(s)?'))) {
          e.preventDefault();
        }
      });
    }
  };
})(jQuery);
