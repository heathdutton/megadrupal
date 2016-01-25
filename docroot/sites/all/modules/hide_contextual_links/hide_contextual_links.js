(function ($) {
  Drupal.behaviors.hide_contextual_links = {
    attach: function (context, settings) {

      // The drupalSetSummary method required for this behavior is not available
      // on the Blocks administration page, so we need to make sure this
      // behavior is processed only if drupalSetSummary is defined.
      if (typeof jQuery.fn.drupalSetSummary == 'undefined') {
        return;
      }

      $('fieldset#edit-hide-contextual-links', context).drupalSetSummary(function (context) {
        switch ($('#edit-hide-contextual-links-type').val()) {
          case '1':
            $summary = 'Hidden';
            break;
          case '2':
            $summary = 'Hidden from All';
            break;
          default:
            $summary = 'Visible';
        }
        return $summary;
      });
    }
  };
})(jQuery);