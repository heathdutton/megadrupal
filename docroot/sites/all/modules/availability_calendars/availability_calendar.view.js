(function($) {
"use strict";

/**
 * Javascript API for Availability Calendar field module.
 *
 * @class Drupal.availabilityCalendar.View
 *   Represents a client-side view for a given availability calendar. It is
 *   possible to have multiple calendars on the same page and each calendar can
 *   have multiple views attached. The cvid parameter is used to distinguish
 *   calendar views.
 *
 * @constructor
 *   Creates a new availability calendar view object.
 * @param {Object} settings
 * @param {number} settings.cvid
 *   The id for this calendar view.
 * @param {number|string} settings.cid
 *   The calendar id for the calendar we want to interact with.
 *   A {string} starting with 'new' for not yet existing calendars.
 * @param {string} settings.name
 *   The (localized) name of this calendar.
 * @param {boolean} settings.splitDay
 *   Indicates whether this calendar should be visualized using split days.
 * @param {number} settings.firstDayOfWeek
 *   The ISO day number for the first day of the week. 1 = monday, 7 = sunday
 * @param {string} settings.selectMode
 *   none|all|available|not-available
 *   Indicates whether this calendar allows interaction by selecting dates,
 *   and if so, what states may be selected
 */
Drupal.availabilityCalendar.View = function(settings) {
  var that = this;
  /** @type {jQuery} the calendar view as jQuery object. */
  var view;
  /** @type {jQuery} set of all months in this calendar view. */
  var months;
  /** @type {Drupal.availabilityCalendar.Calendar} */
  var calendar;
  var viewSettings;
  /** @type {Object.<string, {cell: HTMLElement, extraStates: Array.<string>}>} */
  var cells;

  /**
   * Initializes the calendar:
   * - Gives selectable cells the class selectable
   * - Initializes the custom calendarClick event on these cells
   */
  this.init = function (settings) {
    viewSettings = settings;
    view = $("#cal-view-" + viewSettings.cvid);
    months = $(".cal-month[data-cal-month]", view);
    calendar =  Drupal.availabilityCalendar.getCalendar(viewSettings.cid);
    view.once("Drupal-availabilityCalendar-View-init", function() {
      initDaysAdministration();
      calendar.attachView(that);
      initSelectable();
      initEvents();
    });
  };

  /**
   * Creates a list of all days and their DOM element.
   */
  function initDaysAdministration() {
    cells = {};
    var firstMonth = true;
    // Get all calendar months.
    months
      .each(function() {
        // Get year and month of this calendar month.
        var monthElement = $(this);
        var year = monthElement.attr("data-cal-year");
        var month = monthElement.attr("data-cal-month");
        var yearMonth = year + "-" + month + "-";
        // Get all day cells of this calendar month.
        var day = null;
        $("tbody td", monthElement)
          .not(".cal-other, .cal-pastdate, .cal-empty")
          .each(function() {
            // Using Number(cell.text()) is bad for performance.
            // Using the fact that the set is ordered we only have to use it for
            // the first selected cell in the first month (as for other months
            // it will be day 1).
            // http://docs.jquery.com/Release:jQuery_1.3.2#Elements_Returned_in_Document_Order
            day = day !== null ? day + 1 :
              firstMonth ? Number(this.textContent || $(this).text()) :
              1;

            /** @type {Array.<string>} */
            var extraStates = [];
            // Split the class property in separate classes.
            $.each(this.className.split(/\s+/), function (i, cssClass) {
              // Ignore classes ending with -am, -pm and defined states.
              if (cssClass.substr(cssClass.length - 3) !== "-am" &&
                  cssClass.substr(cssClass.length - 3) !== "-pm" &&
                  Drupal.availabilityCalendar.getStates().get(cssClass) === null) {
                extraStates.push(cssClass);
              }
            });

            cells[yearMonth + (day < 10 ? "0" : "") + day] = {cell: this, extraStates: extraStates};
          });
          firstMonth = false;
      });
  }

  /**
   * Makes certain cells selectable. Note that on overnight calendars, people
   * click on the departure date, a date that itself may be unavailable.
   */
  function initSelectable() {
    if (viewSettings.selectMode !== "none") {
      // Make (available) day cells selectable.
      var isPreviousSelectable = false;
      var day;
      for (day in cells) {
        if (cells.hasOwnProperty(day)) {
          if (viewSettings.selectMode === "all" ||
              (viewSettings.selectMode === "available" && calendar.isAvailable(day)) ||
              (viewSettings.selectMode === "not-available" && !calendar.isAvailable(day))) {
            that.addExtraState(null, day, "cal-selectable");
            isPreviousSelectable = true;
          }
          else {
            if (isPreviousSelectable && calendar.isOvernight()) {
              that.addExtraState(null, day, "cal-selectable");
            }
            isPreviousSelectable = false;
          }
        }
      }

      if (viewSettings.selectMode === "all") {
        // Make other elements selectable.
        months.find("caption").addClass("cal-selectable");
        initWeekNumberHeaders();
        initDayOfWeekHeaders();
      }
    }
  }

  function initWeekNumberHeaders() {
    months.each(function() {
      // Get year and month of this calendar month.
      var monthElement = $(this);
      var year = monthElement.attr("data-cal-year");
      var month = monthElement.attr("data-cal-month");
      // Get all cells with a week number.
      $("tbody th", monthElement)
        .not(".cal-empty")
        .addClass("cal-selectable")
        .each(function() {
          var day = null;
          var monthOffset = 1;
          $(this).next().each(function () {
            var td = $(this);
            day = td.text();
            monthOffset = td.hasClass("cal-other") ? 2 : 1;
          })
          .end()
          .attr("data-cal-week-num", $(this).text())
          .attr("data-cal-first-day", Drupal.availabilityCalendar.formatIsoDate(new Date(year, month - monthOffset, day)));
        });
    });
  }

  function initDayOfWeekHeaders() {
   months.each(function() {
      // Get year and month of this calendar month.
      var monthElement = $(this);
      var year = monthElement.attr("data-cal-year");
      var month = monthElement.attr("data-cal-month");
      // Keep track of the (ISO) day of the week.
      var dow = viewSettings.firstDayOfWeek;
      // Get all day cells of this calendar month.
      $("thead th", monthElement)
        .not(".cal-weekno-header")
        .addClass("cal-selectable")
        .each(function() {
          $(this).attr("data-cal-dow", dow);
          dow = dow === 7 ? 1 : dow + 1;
        });
    });
  }

  /**
   * Initializes the event handling for this calendar.
   *
   * Currently, we react to clicking on a day cell in the calendar, and we bind
   * to the state changing events from Drupal.availabilityCalendar.Calendar.
   */
  function initEvents() {
    // Bind to the events defined by the calendar controller.
    $(calendar).bind("stateChange", that.changeState)
      .bind("extraStateAdd", that.addExtraState)
      .bind("extraStateRemove", that.removeExtraState);

    // Bind to click events on the calendar.
    view.click(viewClicked);
  }

  /**
   * @returns {number}
   *   The id of the calendar view.
   */
  this.getCvid = function() {
    return viewSettings.cvid;
  };

  /**
   * @returns {string}
   *   The name of the calendar.
   */
  this.getName = function() {
    return viewSettings.name;
  };

  /**
   * @returns {Drupal.availabilityCalendar.Calendar}
   *   The calendar that this view represents.
   */
  this.getCalendar = function() {
    return calendar;
  };

  /**
   * @returns {jQuery}
   *   The calendar view element.
   */
  this.getView = function() {
    return view;
  };

  /**
   * @returns {boolean}
   *   If the calendar is shown using split days.
   */
  this.isSplitDay = function() {
    return calendar.isOvernight() && viewSettings.splitDay;
  };

  /**
   * @returns {Object}
   *   A list of calendar cells indexed by their date.
   */
  this.getCells = function() {
    return cells;
  };

  /**
   * @returns {number}
   *   The number of months in the current calendar view.
   */
  this.getNumberOfMonths = function() {
    return months.length;
  };

  /**
   * Handles a click on the calendar view.
   *
   * @param {jQuery.Event} event
   */
  function viewClicked(event) {
    // Find out if the event originated from a selectable element.
    var selectable = $(event.target).closest(".cal-selectable");
    if (selectable.size() > 0) {
      // Extract the year and month, we need that for all cases.
      var calMonth = selectable.closest(".cal-month[data-cal-month]");
      var year = Number(calMonth.attr("data-cal-year"));
      var month = Number(calMonth.attr("data-cal-month"));
      // Find out what type of element was clicked: a day, a week number, a
      // day of week, or the month caption.
      var tagName = selectable.get(0).tagName.toLowerCase();
      switch (tagName) {
        case "td":
          // Day.
          calendar.dateClicked(new Date(year, month - 1, Number(selectable.text())), that);
          break;
        case "th":
          // Week number or day of week.
          if (selectable.closest("tbody").size() === 1) {
            // Week number.
            calendar.weekClicked({year: year, month: month, week: selectable.attr("data-cal-week-num"), firstDay: new Date(selectable.attr("data-cal-first-day"))}, that);
          }
          else {
            // Day of week.
            calendar.dayOfWeekClicked({year: year, month: month, dayOfWeek: Number(selectable.attr("data-cal-dow"))}, that);
          }
          break;
        case "caption":
          // Month.
          calendar.monthClicked({year: year, month: month}, that);
          break;
        default:
          break;
      }
    }
  }

  /**
   * Internal function to combine the state and any extra states
   * for the given day to 1 value for the className property.
   *
   * @param {string} day
   */
  function setCellClass(day) {
    if (cells.hasOwnProperty(day)) {
      var state = calendar.getState(day);
      var cssClasses = [];
      if (that.isSplitDay()) {
        var previousDay = new Date(day.substr(0, 4), day.substr(5, 2) - 1, day.substr(8, 2));
        previousDay.setDate(previousDay.getDate() - 1);
        var statePreviousDay = calendar.getState(previousDay);
        if (statePreviousDay !== "") {
          cssClasses.push(statePreviousDay + "-am");
        }
        else {
          // If day is today, we want to set the am state to pastdate-am.
          if (day === Drupal.availabilityCalendar.formatIsoDate(new Date())) {
            cssClasses.push("cal-pastdate-am");
          }
        }
        if (state !== "") {
          cssClasses.push(state + "-pm");
        }
      }
      else {
        if (state !== "") {
          cssClasses.push(state);
        }
      }
      cssClasses = cssClasses.concat(cells[day].extraStates);
      cells[day].cell.className = cssClasses.join(" ");
    }
  }

  /**
   * Event handler that reacts to a stateChange event of a calendar controller.
   *
   * @param {Event} event
   * @param {Date} day
   */
  this.changeState = function(event, day) {
    var date = Drupal.availabilityCalendar.formatIsoDate(day);
    setCellClass(date);
    if (that.isSplitDay()) {
      var nextDay = new Date(day.getTime());
      nextDay.setDate(nextDay.getDate() + 1);
      setCellClass(Drupal.availabilityCalendar.formatIsoDate(nextDay));
    }
  };

  /**
   * Checks whether a date is selectable.
   *
   * @param {Date|string} day
   *
   * @returns {boolean} whether the given date has the given state.
   */
  this.isSelectable = function(day) {
    var date = Drupal.availabilityCalendar.formatIsoDate(day);
    return cells.hasOwnProperty(date) && cells[date].extraStates.indexOf("cal-selectable") >= 0;
  };

  /**
   * Sets an extra state on the given day.
   *
   * Extra states do not mix with or replace the availability settings.
   * An extra state is not added twice.
   *
   * @param {Event} event
   * @param {Date|string} day
   * @param {string} extraState
   */
  this.addExtraState = function(event, day, extraState) {
    var date = Drupal.availabilityCalendar.formatIsoDate(day);
    if (cells.hasOwnProperty(date)) {
      // Only add if extra state is not already set.
      if (cells[date].extraStates.indexOf(extraState) < 0) {
        cells[date].extraStates.push(extraState);
        setCellClass(date);
      }
    }
  };

  /**
   * Removes an extra state of the given day.
   *
   * Extra states do not mix with or replace the availability settings.
   *
   * @param {Event} event
   * @param {Date|string} day
   * @param {string} extraState
   */
  this.removeExtraState = function(event, day, extraState) {
    var date = Drupal.availabilityCalendar.formatIsoDate(day);
    if (cells.hasOwnProperty(date)) {
      // Only remove if extra state is set.
      var index = cells[date].extraStates.indexOf(extraState);
      if (index >= 0) {
        cells[date].extraStates.splice(index, 1);
        setCellClass(date);
      }
    }
  };

  /**
   * Adds the given extra state to all days in the from - to range.
   *
   * Extra states do not mix with or replace the availability settings.
   *
   * @param {Date} from
   * @param {Date} to
   * @param {string} extraState
   */
  this.addRangeExtraState = function(from, to, extraState) {
    // Loop through range of dates.
    var date = new Date(from.getTime());
    while (date <= to) {
      that.addExtraState(null, date, extraState);
      date.setDate(date.getDate() + 1);
    }
  };

  /**
   * Removes the given state from all days in the from - to range.
   *
   * Extra states do not mix with or replace the availability settings.
   *
   * @param {Date} from
   * @param {Date} to
   * @param {string} extraState
   */
  this.removeRangeExtraState = function(from, to, extraState) {
    // Loop through range of dates.
    var date = new Date(from.getTime());
    while (date <= to) {
      that.removeExtraState(null, date, extraState);
      date.setDate(date.getDate() + 1);
    }
  };

  this.init(settings);
};

/**
 * @type {Object} Collection of calendar view instances.
 */
Drupal.availabilityCalendar.views = {};

  /**
   * Multiton implementation for accessing views on the page.
   *
   * @param {number} cvid
   * @returns {?Drupal.availabilityCalendar.View}
   */
Drupal.availabilityCalendar.getCalendarView = function(cvid) {
  // We can only return an existing calendar,
  return Drupal.availabilityCalendar.views[cvid] !== undefined ? Drupal.availabilityCalendar.views[cvid] : null;
};

/**
 * Initialization code that works by implementing the Drupal behaviors paradigm.
 *
 * Based on the information in the settings, the calendar views are created.
 */
Drupal.behaviors.availabilityCalendarView = {
  attach: function(context, settings) {
    if (settings.availabilityCalendar && settings.availabilityCalendar.views) {
      var key;
      for (key in settings.availabilityCalendar.views) {
        if (settings.availabilityCalendar.views.hasOwnProperty(key)) {
          var viewSettings = settings.availabilityCalendar.views[key];
          if (Drupal.availabilityCalendar.views[viewSettings.cvid] === undefined) {
            Drupal.availabilityCalendar.views[viewSettings.cvid] = new Drupal.availabilityCalendar.View(viewSettings);
          }
          else {
            Drupal.availabilityCalendar.views[viewSettings.cvid].init(viewSettings);
          }
        }
      }
    }
  }
};

}(jQuery));
