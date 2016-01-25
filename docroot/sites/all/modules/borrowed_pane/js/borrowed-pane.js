(function ($) {
  Drupal.behaviors.borrowedPane = {
    attach: function (context, settings) {
      $('.borrowed-pane-container').not('.no-refresh').each(function () {
        $(this).html('');
        var lender = $($(this).data('selector'));
        if (lender.length === 0) {
          $(this).closest('.pane-borrowed-pane').hide();
        }
        else {
          lender.clone($(this).data('events')).appendTo($(this));
          $(this).closest('.pane-borrowed-pane').show();
          if ($(this).data('flag')) {
            lender.closest($(this).data('flag')).addClass('borrowed-pane-lender');
          }
          else {
            lender.addClass('borrowed-pane-lender');
          }
        }
        if (!$(this).data('refresh')) {
          $(this).addClass('no-refresh');
        }
      });
    }
  };
}(jQuery));
