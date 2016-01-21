(function($) {
"use strict";

/**
 * @class Drupal.availabilityCalendar.BookingFormlet
 *   Provides the glueing code that connects the reservation formlet with the
 *   availability calendar fields on the page.
 *
 *   This class should be able to operate on its own if no calendar is loaded
 *   on the page, though availability_calendars.js remains needed for some
 *   helper functions.
 *
 * @param {Object} settings
 * @param {string} settings.formId
 *   The id of the form(let) instance that this object instance is linked to.
 *   The id has to be prefixed with a '#', so it can serve as a jQuery selector.
 * @param {number[]} settings.cids
 *   The list of calendar id's to listen to.
 * @param {number} settings.bookedState
 *   The state id (sid) to visually change the state to after the user has
 *   clicked both an arrival and departure date. This gives the visitor visual
 *   feedback and suggests that the state changes to "optionally booked".
 * @param {boolean} settings.singleDay
 *   true if this formlet should accept only 1 date (for both start and end
 *   date), false if it should accept different dates.
 * @param {string} settings.displayDuration
 *   not|nights|days|nights_and_days.
 *
 * @see AvailabilityCalendars API object.
 */
Drupal.availabilityCalendar.BookingFormlet = function(settings) {
  var that = this;
  var cid;
  /** @type {Drupal.availabilityCalendar.Calendar} */
  var calendar;
  var bookingFormletSettings;
  /** @type {Date} */
  var from;
  /** @type {Date} */
  var to;
  var calSelectedDay;

  /**
   * Initializes this object.
   */
  this.init = function(settings) {
    bookingFormletSettings = settings;
    $(bookingFormletSettings.formId).once("Drupal-availabilityCalendar-BookingFormlet", function () {
      // Check cids:
      if (Drupal.availabilityCalendar.getCalendar === undefined) {
        bookingFormletSettings.cids = [];
      }
      // Extract values from form elements if preset by the server.
      cid = $('[name="cid"]', bookingFormletSettings.formId).val();
      if (cid === '') {
        cid = null;
      }
      calendar = cid !== null ? Drupal.availabilityCalendar.getCalendar(cid) : null;
      from  = null;
      to = null;
      calSelectedDay = null;

      // from and to are empty or in ISO format (yyyy-mm-dd).
      var fromIso = Drupal.availabilityCalendar.parseIsoDate($('[name="from_iso"]', bookingFormletSettings.formId).val());
      var toIso = Drupal.availabilityCalendar.parseIsoDate($('[name="to_iso"]', bookingFormletSettings.formId).val());

      // Show form in correct state. If we have preset values we use the addDate
      // event handler to also make changes to the calendar.
      if (cid !== null && fromIso !== null) {
        that.addDate(null, fromIso, cid);
        if (toIso !== null && !bookingFormletSettings.singleDay) {
          if (calendar.isOvernight()) {
            toIso.setDate(toIso.getDate() + 1);
          }
          that.addDate(null, toIso, cid);
        }
      }
      else {
        show();
      }

      // Attach to the calendar click from the calendar controller.
      var i, calendar1;
      for (i = 0; i < bookingFormletSettings.cids.length; i++) {
        calendar1 = Drupal.availabilityCalendar.getCalendar(bookingFormletSettings.cids[i]);
        if (calendar1 !== null) {
          $(calendar1).bind("calendarClick", that.addDate);
        }
      }

      // Attach to the click events of the reset buttons.
      $(".acbf-reset-from", bookingFormletSettings.formId).click(that.resetDates);
      $(".acbf-reset-both", bookingFormletSettings.formId).click(that.resetDates);
    });
  };

  /**
   * Adds a date to the command.
   *
   * - If it is the first date, it will be the from date.
   * - If it is the 2nd date, it will be the to date, swapping the from and to
   *   dates if needed.
   * - If it is a 3rd date, either the from or to date will be changed,
   *   depending on whether the 3rd date is before the current from or not.
   *
   * @param {?jQuery.Event} event
   *   The event object.
   * @param {Date} date
   *   The clicked date.
   * @param {number} eventCid
   *   The id of the calendar where the click originated from.
   * @param {Drupal.availabilityCalendar.View} [eventView]
   *   The calendar view where the click originated from.
   */
  this.addDate = function(event, date, eventCid, eventView) {
    // Create a clone.
    date = new Date(date.getTime());
    // Does this event came from a different calendar on the page?
    if (cid !== eventCid) {
      if (cid !== null) {
        // Reset the old calendar, both the settings and visually.
        that.resetDates();
        show();
      }
      // Assign info from the new calendar and view to the internal members.
      cid = eventCid;
      calendar = Drupal.availabilityCalendar.getCalendar(eventCid);

      $('[name="cid"]', bookingFormletSettings.formId).val(cid);
      if (eventView) {
        $('[name="calendar_label"]', bookingFormletSettings.formId).val(eventView.getName());
      }
    }

    // Get new from and to, but store the old ones so we can cancel the
    // operation.
    var oldFrom = from;
    var oldTo = to;

    if (bookingFormletSettings.singleDay) {
      // Assign the same date but different instances to both from and to.
      from = new Date(date.getTime());
      to = new Date(date.getTime());
    }
    else {
      if (from === null) {
        // 1st date: assign to from.
        from = new Date(date.getTime());
      }
      else {
        // 2nd date. Assign to from or to depending on order.
        if (date >= from) {
          // 2nd date is after the 1st date: assign to to.
          to = new Date(date.getTime());
        }
        else {
          // 2nd date is before the 1st date: assign to from.
          // If to does not already have a value it gets the from date assigned,
          // otherwise it remains unchanged.
          to = to || from;
          from = new Date(date.getTime());
        }
      }
    }

    // Check if the selection made is valid. If we are still initializing, no
    // eventView is passed in, so we skip this check and assume that the server
    // side did its work correctly and thus that the selection is valid.
    if (typeof(eventView) === 'undefined' || that.isValidSelection(eventView)) {
      // Restore the old selection (if any) and show the new selection.
      if (oldTo !== null) {
        // Change of range: reset the currently selected range.
        calendar.restoreRangeState(oldFrom, oldTo);
      }
      show();
    }
    else {
      // Invalid: restore old values, no state changes needed.
      // Show an error sign on the selected cell (for 5 seconds).
      calendar.addExtraState(date, 'cal-error');
      var errorDate = new Date(date.getTime());
      setTimeout(function() {
        calendar.removeExtraState(errorDate, 'cal-error');
      }, 5000);
      from = oldFrom;
      to = oldTo;
    }
  };

  /**
   * Resets both dates and restores the calendar.
   *
   * If just 1 date is set this works as well.
   */
  this.resetDates = function() {
    if (from !== null && to !== null) {
      calendar.restoreRangeState(from, to);
    }
    from = to = null;
    show();
    // Stop propagating the event.
    return false;
  };

  /**
   * @returns {boolean} Whether the current form values are valid.
   */
  this.isValid = function() {
    return to !== null && from !== null && cid !== null;
  };

  /**
   * Checks if the new selection is valid.
   *
   * The selection is valid if:
   * - only 1 date has been selected. It is the responsibility of the
   *   calendar view that only clicks on selectable dates are passed.
   * - if 2 dates has been selected, the whole range must be selectable.
   *
   * @param {Drupal.availabilityCalendar.View} calendarView
   *   The calendar view where the interaction originated from.
   *
   * @returns {boolean} Whether the new selection is valid.
   */
  this.isValidSelection = function(calendarView) {
    var result = true;
    if (from !== null && to !== null) {
      var date = new Date(from.getTime());
      while (date <= to && calendarView.isSelectable(date)) {
        date.setDate(date.getDate() + 1);
      }
      result = date > to;
    }
    return result;
  };

  /**
   * Shows the current values, help texts, and enables the submit button.
   */
  function show() {
    if (from === null) {
      // No dates:
      // - Remove the "selected state" from the calendar.
      removeCalSelected();
      // - Hide reset buttons.
      $(".form-reset", bookingFormletSettings.formId).css("display", "none");
      // - Disable arrival date field and set help text in it.
      $('[name="from_display"]', bookingFormletSettings.formId)
        .attr("disabled", "disabled")
        .addClass("form-button-disabled")
        .val(Drupal.t("Click on an available date in the calendar"));
      // - Disable and empty departure date field.
      $('[name="to_display"]', bookingFormletSettings.formId)
        .val("")
        .attr("disabled", "disabled")
        .addClass("form-button-disabled");
      // - Reset iso dates.
      $('[name="from_iso"]', bookingFormletSettings.formId).val("");
      $('[name="to_iso"]', bookingFormletSettings.formId).val("");
      // - Reset duration.
      $('[name="duration"]', bookingFormletSettings.formId).val(0);
      $('[name="duration_display"]', bookingFormletSettings.formId)
        .val("")
        .attr("disabled", "disabled")
        .addClass("form-button-disabled");
    }
    else {
      // 1 or 2 dates set:
      // - Set value in arrival date field and remove its disabled attribute.
      $('[name="from_display"]', bookingFormletSettings.formId)
        .val(Drupal.availabilityCalendar.formatDate(from))
        .removeAttr("disabled")
        .removeClass("form-button-disabled");
      // Set iso from date.
      $('[name="from_iso"]', bookingFormletSettings.formId).val(Drupal.availabilityCalendar.formatIsoDate(from));
      if (to === null) {
        // 1 date only:
        // - Show "clear arrival date" button, hide "Clear both dates" button.
        $(".acbf-reset-from", bookingFormletSettings.formId).css("display", "inline-block");
        $(".acbf-reset-both", bookingFormletSettings.formId).css("display", "none");
        // - Disable departure date field and set help text in it.
        $('[name="to_display"]', bookingFormletSettings.formId)
          .val(Drupal.t("Click on your departure date"))
          .attr("disabled", "disabled")
          .addClass("form-button-disabled");
        // - Reset iso to date.
        $('[name="to_iso"]', bookingFormletSettings.formId).val("");
        // - Reset duration.
        $('[name="duration"]', bookingFormletSettings.formId).val(0);
        $('[name="duration_display"]', bookingFormletSettings.formId)
          .val("")
          .attr("disabled", "disabled")
          .addClass("form-button-disabled");
        // - Set this single date to the "selected state".
        setCalSelected(from);
      }
      else {
        // 2 dates set:
        // - Remove the "selected state" from the calendar.
        removeCalSelected();
        // - Set iso to date.
        //
        // In day rental this will be the same as the clicked date (= to).
        // In overnight rental situations people click on the departure date and
        // the iso to date will be set to the day before.
        // But in the case of clicking twice on the same day, it will also in
        // the overnight rental situation be handled as a 1 night booking.
        var toIso = new Date(to.getTime());
        if (calendar.isOvernight()) {
          if (to > from) {
            // Adjust isoTo by going back 1 day.
            toIso.setDate(toIso.getDate() - 1);
          }
          else {
            // Clicked twice on the same day: handle as if the 2nd click was on
            // the next day.
            to.setDate(to.getDate() + 1);
          }
        }
        $('[name="to_iso"]', bookingFormletSettings.formId).val(Drupal.availabilityCalendar.formatIsoDate(toIso));
        // - Set value in departure date field and remove disabled attribute.
        $('[name="to_display"]', bookingFormletSettings.formId)
          .val(Drupal.availabilityCalendar.formatDate(to))
          .removeAttr("disabled")
          .removeClass("form-button-disabled");
        // - Set (hidden) duration.
        var duration = Math.round((to.getTime() - from.getTime()) / 86400000);
        if (!calendar.isOvernight()) {
          duration += 1;
        }
        $('[name="duration"]', bookingFormletSettings.formId).val(duration);
        // - Set display_duration (if available).
        $('[name="duration_display"]', bookingFormletSettings.formId)
          .removeAttr("disabled")
          .removeClass("form-button-disabled")
          .val(function() {
            var duration_display = '';
            switch (bookingFormletSettings.displayDuration) {
              case 'days':
                duration_display = Drupal.formatPlural(duration, '1 day', '@count days');
                break;
              case 'nights':
                duration_display = Drupal.formatPlural(duration, '1 night', '@count nights');
                break;
              case 'nights_and_days':
                duration_display = Drupal.formatPlural(duration, '1 night', '@count nights');
                duration_display += ' / ';
                duration_display += Drupal.formatPlural(duration + 1, '1 day', '@count days');
                break;
            }
            return duration_display;
          });
        // - Hide "clear arrival date" button, show  "Clear both dates" button.
        $(".acbf-reset-from", bookingFormletSettings.formId).css("display", "none");
        $(".acbf-reset-both", bookingFormletSettings.formId).css("display", "inline-block");
        // - Visually update the calendar.
        if (calendar !== null && that.isValid()) {
          calendar.changeRangeState(from, toIso, bookingFormletSettings.bookedState);
        }
      }
    }
    // Enable or disable the submit button depending on the validity of the form.
    if (that.isValid()) {
      $(".form-submit", bookingFormletSettings.formId).removeAttr("disabled").removeClass("form-button-disabled");
    }
    else {
      $(".form-submit", bookingFormletSettings.formId).attr("disabled", "disabled").addClass("form-button-disabled");
    }

  }

  /**
   * Marks the given day as selected (by adding the class cal-selected).
   *
   * @param {Date} day
   */
  function setCalSelected(day) {
    if (calendar !== null) {
      removeCalSelected();
      calSelectedDay = new Date(day.getTime());
      calendar.addExtraState(calSelectedDay, "cal-selected");
    }
  }

  /**
   * Removes the selected mark form the day it was previously set on.
   */
  function removeCalSelected() {
    if (calSelectedDay !== null && calendar !== null) {
      calendar.removeExtraState(calSelectedDay, "cal-selected");
      calSelectedDay = null;
    }
  }

  this.init(settings);
};

/**
 * @type {Object} Collection of booking formlet instances.
 */
Drupal.availabilityCalendar.bookingFormlets = {};

/**
 * Multiton implementation for accessing booking formlets on the page.
 *
 * @param {string} formId
 * @return {?Drupal.availabilityCalendar.BookingFormlet}
 */
Drupal.availabilityCalendar.getBookingFormlet = function(formId) {
  return Drupal.availabilityCalendar.bookingFormlets[formId] !== undefined  ? Drupal.availabilityCalendar.bookingFormlets[formId] : null;
};

/**
 * Initialization code that works by implementing the Drupal behaviors paradigm.
 *
 * Based on the information in the settings, the booking formlets are created.
 */
Drupal.behaviors.availabilityCalendarBookingFormlet = {
  attach: function(context, settings) {
    if (settings.availabilityCalendar && settings.availabilityCalendar.bookingFormlets) {
      var key;
      for (key in settings.availabilityCalendar.bookingFormlets) {
        if (settings.availabilityCalendar.bookingFormlets.hasOwnProperty(key)) {
          var bookingFormletSettings = settings.availabilityCalendar.bookingFormlets[key];
          if (Drupal.availabilityCalendar.bookingFormlets[bookingFormletSettings.formId] === undefined) {
            Drupal.availabilityCalendar.bookingFormlets[bookingFormletSettings.formId] = new Drupal.availabilityCalendar.BookingFormlet(bookingFormletSettings);
          }
          else {
            Drupal.availabilityCalendar.bookingFormlets[bookingFormletSettings.formId].init(bookingFormletSettings);
          }
        }
      }
    }
  }
};

}(jQuery));
