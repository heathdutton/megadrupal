(function($) {
  "use strict";

  // This filter may appear on pages without any calendar, thus without any
  // other js from this module: assure Drupal.availabilityCalendar is defined.
  Drupal.availabilityCalendar = Drupal.availabilityCalendar || {};

  /**
   * @class Drupal.availabilityCalendar.datePopupCoupler
   *
   * Date popup pairs are coupled such that the maxDate or minDate setting is
   * changed when the other date field is set or changed.
   *
   * As the date popup module only creates the datepickers when the input fields
   * get the focus, we must set the maxValue and minValue options both in the
   * date popup module settings (drupal.settings.datePopup) (if not already
   * created) and in the datepicker object (if already created).
   *
   * @constructor
   *   Creates a new date popup coupler object.
   * @param {Element} datePopup1
   * @param {Element} datePopup2
   * @param {Object} settings
   */
  Drupal.availabilityCalendar.DatePopupCoupler = function(datePopup1, datePopup2, settings) {
    var isTo1 = datePopup2.name.indexOf("[to1]") !== -1;

    // Event handler for when the from date gets set or changed: change the
    // minDate of the to(1) date.
    function fromDateChanged(fromDate) {
      // Check if from date has a correct value (not e.g. a hint text).
      // $.datepicker.parseDate() can throw as well as return null or false...
      try {
        // Use the dateFormat options (or date popup setting if not yet created)
        // from both date popup fields to parse resp. format the date. This allows
        // for different date formats in these fields.
        fromDate = $.datepicker.parseDate($(datePopup1).datepicker("option", "dateFormat") || settings.datePopup[datePopup1.id].settings.dateFormat, fromDate);
        if (fromDate) {
          if (isTo1) {
            // We have an arrival and departure date. The departure date should
            // be greater than the arrival date. Add 1 day.
            fromDate.setDate(fromDate.getDate() + 1);
          }
          fromDate = $.datepicker.formatDate($(datePopup2).datepicker("option", "dateFormat") || settings.datePopup[datePopup2.id].settings.dateFormat, fromDate);

          // Change both the settings and the actual option, as the instance may not
          // yet exist.
          settings.datePopup[datePopup2.id].settings.minDate = fromDate;
          $(datePopup2).datepicker("option", "minDate", fromDate);
        }
      }
      catch (e) {}
    }

    // Event handler for when the to date gets set or changed: change the
    // maxDate of the from date.
    function toDateChanged(toDate) {
      // Check if to date has a correct value (not e.g. a hint text).
      try {
        toDate = $.datepicker.parseDate($(datePopup2).datepicker("option", "dateFormat") || settings.datePopup[datePopup2.id].settings.dateFormat, toDate);
        if (toDate) {
          if (isTo1) {
            toDate.setDate(toDate.getDate() - 1);
          }
          toDate = $.datepicker.formatDate($(datePopup1).datepicker("option", "dateFormat") || settings.datePopup[datePopup1.id].settings.dateFormat, toDate);

          // Change both the settings and the actual option, as the instance may not
          // yet exist.
          settings.datePopup[datePopup1.id].settings.maxDate = toDate;
          $(datePopup1).datepicker("option", "maxDate", toDate);
        }
      }
      catch (e) {}
    }

    // Set event handler and call it directly.
    fromDateChanged($(datePopup1).val());
    settings.datePopup[datePopup1.id].settings.onClose = fromDateChanged;

    toDateChanged($(datePopup2).val());
    // Set event handler.
    settings.datePopup[datePopup2.id].settings.onClose = toDateChanged;
  };

  /**
   * @type {Array.<Drupal.availabilityCalendar.DatePopupCoupler>} Collection of
   *   date popup coupler instances.
   */
  Drupal.availabilityCalendar.datePopupCouplers = [];

  /**
   * Initialization code based on the Drupal behaviors paradigm.
   *
   * Date popup pairs are coupled such that the maxDate or minDate setting is
   * changed when the other date field is changed.
   *
   */
  Drupal.behaviors.availabilityCalendarHandlerFilterAvailabilityCoupleDatePopups = {
    attach: function(context, settings) {
      if (settings.datePopup) {
        // Find date popup pairs, ie. 2 date-popups within the same filter.
        // Date popups that form a pair will have names like:
        // <views filter name>[from][date] and <views filter name>[to(1)][date].
        $(".views-widget-filter-available").once('availability-calendar-handler-filter-availability-couple-date-popups', function() {
          var inputs = $(".form-type-date-popup input", $(this));
          // Check that we have 2 date popups, not 1 with a duration drop down.
          if (inputs.size() === 2) {
            // Couple these date popups.
            Drupal.availabilityCalendar.datePopupCouplers.push(new Drupal.availabilityCalendar.DatePopupCoupler(inputs[0], inputs[1], settings));
          }
        });
      }
    }
  };

  /**
   * Initialization code based on the Drupal behaviors paradigm.
   *
   * If the form uses auto submit, we prevent that it does so on coupled
   * elements that together define a date range, by adding the
   * ctools-auto-submit-exclude class to the date inputs.
   * However, that means that we have to take that task over by triggering the
   * auto submit if both elements have a (correct) value.
   */
  Drupal.behaviors.availabilityCalendarHandlerFilterAvailabilityHandleAutoSubmit = {
    attach: function(context, settings) {
      // Copied (and adapted) from ctools/js/auto-submit.js.
      function triggerSubmit(form) {
        form = $(form);
        if (!form.hasClass('ctools-ajaxing')) {
          form.find('.ctools-auto-submit-click').click();
        }
      }

      function validate(value, format) {
        jQuery.ui.Datepicker.parseDate(format, value);
        return true;
      }

      $('.availability-calendar-auto-submit', context)
        .once('availability-calendar-handler-filter-availability-handle-auto-submit', function () {
          $(this)
            // This unbinds all events, also others not set by ctools'
            // auto-submit.js. But there's no way to only unbind that event.
            .unbind('keydown keyup')
            .unbind('keyup')
            .unbind('change')
            .bind('change', function(e) {
              if ($(e.target).is(':not(.ctools-auto-submit-exclude)')) {
                var elt = $(this);
                var value = elt.val();
                var format = elt.parents('[data-date-format]').attr('data-date-format');
                if (validate(value, format)) {
                  triggerSubmit(this.form);
                }
              }
            });
        });
    }
  };

}(jQuery));
