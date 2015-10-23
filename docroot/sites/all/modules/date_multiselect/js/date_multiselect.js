/**
 * @file
 * Attaches the calendar behavior to all required fields.
 */

(function ($) {
  Drupal.behaviors.date_multiselect = {
    attach: function (context) {
      for (var id in Drupal.settings.dateMultiselect) {
        var input = $('input#' + id);
        if (!input.hasClass('date-multiselect-init')) {
          var settings = Drupal.settings.dateMultiselect[id].settings;
          if (input.val()) {
            settings.addDates = input.val().split(', ');
            settings.minDate = this.getMinDate(settings.minDate, settings.addDates[0], settings.dateFormat);
          }
          if ($(window).width() <= settings.smartphoneWidth) {
            settings.numberOfMonths = 1;
          }
          input
            .addClass('date-multiselect-init')
            .wrap('<div id="' + id + '-wrapper" />')
            .parent().multiDatesPicker(settings);
          input.hide();
        }
      }
    },

    /**
     * Return the minimum date, either the minDate or the firstDate.
     *
     * @param minDate is either an date string or an int offset of the current
     *    date in days.
     */
    getMinDate: function (minDate, firstDate, format) {
      var minDateObj = null;
      if (typeof minDate == 'string') {
        minDateObj = $.datepicker.parseDate(format, minDate);
      }
      else if (typeof minDate == 'number') {
        minDateObj = new Date();
        minDateObj.setDate(minDateObj.getDate() + minDate);
      }

      var firstDateObj = $.datepicker.parseDate(format, firstDate);

      return (firstDateObj > minDateObj) ? minDate : firstDate;
    }
  };
})(jQuery);
