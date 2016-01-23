(function ($) {

  Drupal.behaviors.contextlySettingsForm = {
    attach: function (context, settings) {
      // Submit "Contextly settings" button in new tab/window.
      $('input.contextly-settings-button', context)
        .once()
        .click(function() {
          var form = $(this).closest('form');
          form.attr('target', '_blank');

          // Remove attribute a bit later.
          setTimeout(function() {
            form.removeAttr('target');
          }, 100);
        });
    }
  };

})(jQuery);
