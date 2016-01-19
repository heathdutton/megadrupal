/**
 * @file
 * Swaps out the datepicker with HTML date inputs on mobile resolutions.
 */

(function ($) {
  if ($(window).width() < 768) {

    Drupal.behaviors.date_popup = {
      attach: function (context, settings) {
        // There are two phases to this process, which only run if the current screen width is < 768px:
        // 1. On page load, change all the datepicker text inputs to date inputs, and format the initial value of the field to
        //    be the format that date inputs expect, which is YYYY-MM-DD.
        // 2. On form submit, change all of the altered date inputs back into text fields, and change the format of them
        //    to match the original format of the field, which luckily gets passed into Drupal.settings per-field already.
        // Cycle through all of the fields originally set by the date_popup module to have the popup on them,
        // so that we can make sure that we only operate on the right fields.
        var dateFields = this.getDateFields(context, settings);
        for (var id in dateFields) {

          var $input = $('#' + id, context); // The input itself.
          var initialDate = $input.val();

          // If there's a default value in the field, then we have to reformat it to be in the YYYY-MM-DD format that
          // HTML5 date fields expect. Otherwise it would just be ignored altogether, which is bad bad bad.
          if (initialDate) {
            var timestamp = Date.parse(initialDate);
            $input.val($.datepicker.formatDate('yy-mm-dd', new Date(timestamp)));
          }

          // Change the input type to date. Can't use jQuery's .attr() because jQuery doesn't allow you to change "type",
          // so we have to fall back to regular old JS for this one line.
          $input[0].type = 'date';

          // Ensure inputs reverted on form submission.
          $input.parents('form').once('mobile-popup').bind('submit', function () {
            Drupal.behaviors.date_popup.revertInputs(context, settings);
          });
        }
      },

      /**
       * Revert inputs on AJAX detach.
       *
       * @param context
       * @param settings
       */
      detach: function (context, settings) {
        this.revertInputs(context, settings);
      },

      /**
       * Change inputs back to original format.
       *
       * @param context
       * @param settings
       */
      revertInputs: function (context, settings) {
        var dateFields = this.getDateFields(context, settings);
        for (var id in dateFields) {
          Drupal.behaviors.date_popup.inputDateToText(id);
        }
      },

      /**
       * Helper function to get valid date fields from settings
       *
       * @param context
       * @param settings
       * @returns {{}|*}
       */
      getDateFields: function (context, settings) {
        var dateFields = {};
        for (var id in settings.datePopup) {
          var $input = $('#' + id, context); // The input itself.
          // Verify element exists.
          if ($input.length != 0) {
            // The date_popup module also handles the time fields, and we want to leave those alone, so we short
            // circuit and continue the loop here if we're not currently looking at a date field.
            if (settings.datePopup[id].func !== 'datepicker') {
              continue;
            }
            dateFields[id] = settings.datePopup[id];
          }
        }
        return dateFields;
      },

      /**
       * Changes altered date inputs back to text fields.
       * @param thisId
       */
      inputDateToText: function (thisId) {
        var $thisInput = $('#' + thisId);

        // If this input isn't of type date or datepicker.
        if ($thisInput[0].type !== 'date' || Drupal.settings.datePopup[thisId].func !== 'datepicker') {
          return;
        }

        // The original expected format of the field is provided in Drupal.settings for us.
        var format = Drupal.settings.datePopup[thisId].settings.dateFormat;
        var formattedDate;

        if ($thisInput.val()) {

          // We have the value in the format YYYY-MM-DD, most likely, since that's what HTML5 date inputs pass over
          // the wire, so we have to check to make sure that's really the format, and then reformat it.
          var dateParts = $thisInput.val().split('-');

          if (dateParts.length === 1) {
            // Hey, look at that, it's not in YYYY-MM-DD format after all, so just leave as-is.
            formattedDate = $thisInput.val();

          } else {
            // This is the common case. We're looking at YYYY-MM-DD date input format, and we have to reformat it
            // to match the expected format for that particular field, which can be many different things.
            // Luckily, $.datepicker.formatDate() exists to make that easy for us.
            var year = parseInt(dateParts[0]);
            var month = parseInt(dateParts[1]);
            var day = parseInt(dateParts[2]);
            formattedDate = $.datepicker.formatDate(format, new Date(year, month - 1, day));
          }
        } else {
          // This is an empty field, so just set it to an empty string.
          formattedDate = '';
        }

        $thisInput[0].type = 'text'; // Back to 'text' we go.
        $thisInput.val(formattedDate); // Set the value, and we're done, and ready to submit the form.
      }
    };
  }
})(jQuery);
