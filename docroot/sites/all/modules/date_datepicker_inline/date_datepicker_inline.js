/**
 * Attaches the calendar behavior to all required fields.
 */
(function ($) {
  Drupal.behaviors.date_datepicker_inline = {
    attach: function (context) {
      for (var id in Drupal.settings.datePopup) {
        $('#'+ id).each(function() {
          if (!$(this).hasClass('date-popup-init')) {
            var datePopup = Drupal.settings.datePopup[this.id];
            if (datePopup.func == 'datepicker-inline') {
              // Hide the original date element.
              $('#'+ this.id + '-owned').parent().children().hide();

              // Inline datepicker.
              $(this)
                .datepicker(datePopup.settings)
                .addClass('date-popup-init');

              // Trigger the original date element when selection changes.
              $('#'+ this.id).bind('change', function() {
                $('#'+ this.id + '-owned').trigger('change');
              });
            }
          }
        });
      }
    }
  };
})(jQuery);
