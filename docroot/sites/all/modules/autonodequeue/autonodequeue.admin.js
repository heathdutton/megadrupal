
(function($) {

  // Fieldset summariy handlers.
  Drupal.behaviors.autonodequeue_fieldsetSummary = {
    attach: function(context, settings) {

      if ($.fn.drupalSetSummary) {

        // Node form.
        $('.node-form #edit-autonodequeue', context).drupalSetSummary(function(context) {
          return Drupal.t('Add to: ') + ($(':checkbox:checked', context).parent().find('em').map(function() {
            return $(this).text();
          }).get().join(', ') || '-');
        });

      }

    }
  };

})(jQuery);
