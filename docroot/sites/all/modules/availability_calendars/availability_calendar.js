(function($) {
"use strict";

/*
 * IE8- hack. It turns out that IE8 and lower do not support the indexOf method
 * on arrays. Define it here.
 */
if (!Array.indexOf) {
  Array.prototype.indexOf = function (obj, start) {
    var i;
    for (i = (start || 0); i < this.length; i++) {
      if (this[i] === obj) {
        return i;
      }
    }
    return -1;
  };
}

Drupal.availabilityCalendar = Drupal.availabilityCalendar || {};

/**
 * Helper method to format a date according to the date format defined for user
 * presented dates.
 *
 * @param {string|Date} date
 * @return {string}
 */
Drupal.availabilityCalendar.formatDate = function(date) {
  return date instanceof Date ? $.datepicker.formatDate(Drupal.settings.availabilityCalendar.displayDateFormat, date) : date;
};

/**
 * Formats a date according to the ISO 8601 date format (yyyy-mm-dd).
 *
 * @param {string|Date} date
 * @return {string}
 */
Drupal.availabilityCalendar.formatIsoDate = function(date) {
  return date instanceof Date ? $.datepicker.formatDate('yy-mm-dd', date) : date;
};

/**
 * Helper function to parse a date string in ISO format (yyyy-mm-dd).
 *
 * @param {string|Date} date
 * @returns {?Date}
 */
Drupal.availabilityCalendar.parseIsoDate = function(date) {
  var result;
  if (typeof date === "string") {
    result = new Date(date.substr(0, 4), date.substr(5, 2) - 1, date.substr(8, 2));
    if (isNaN(result.getTime())) {
      result = null;
    }
  }
  else if (date instanceof Date) {
    result = date;
  }
  else {
    result = null;
  }
  return result;
};

/**
 * @class Drupal.availabilityCalendar.States
 *   Represents the collection of all availability states. Per state some
 *   information is kept:
 *   - {number} sid,
 *   - {string} cssClass
 *   - {boolean} isAvailable
 *
 * @constructor
 *   Creates a new collection of all availability states.
 * @param {Object[]} [states]
 *   Array with information about all possible availability states.
 */
Drupal.availabilityCalendar.States = function(states) {
  var statesById = {};
  var statesByClass = {};

  this.init = function(states) {
    var key;
    // Add empty state (easiest way to prevent javascript errors).
    states[""] = {sid: 0, cssClass: "", isAvailable: false};
    statesById = {};
    statesByClass = {};
    for (key in states) {
      if (states.hasOwnProperty(key)) {
        statesById[states[key].sid] = states[key];
        statesByClass[states[key].cssClass] = states[key];
      }
    }
  };

  /**
   * Returns the state for the given class or state id.
   *
   * @param {number|string} stateOrId
   * @returns {?Object}
   *   {cid:number, cssClass:String, isAvailable:boolean}
   */
  this.get = function(stateOrId) {
    if (isNaN(stateOrId) && statesByClass[stateOrId] !== undefined) {
      return statesByClass[stateOrId];
    }
    else if (!isNaN(stateOrId) && statesById[stateOrId] !== undefined) {
      return statesById[stateOrId];
    }
    return null;
  };

  this.init(states);
};

/**
 * @class Drupal.availabilityCalendar.Calendar
 *   Represents the controller for a given availability calendar.
 *
 * It is possible to have multiple calendars on the same page, each will get
 * its own controller instance. The cid parameter is used to distinguish
 * calendar controllers.
 *
 * A calendar can be displayed multiple times on the same page. each display
 * will be represented by a Drupal.availabilityCalendar.CalendarView that
 * connects to a controller.
 *
 * The calendar controller class offers the following features and events:
 * - Methods to query properties from the calendar: cid, isOvernight.
 * - Methods to retrieve information about the calendar: getState, isAvailable,
 *   isRangeAvailable.
 * - Methods to change the (visual) status of calendar days. However, note that
 *   it does not update the server-side calendar: changeState, restoreState,
 *   addExtraState, removeExtraState, changeRangeState, removeRangeState
 * - getViews: get a list of connected Drupal.availabilityCalendar.CalendarView
 *   objects.
 * - Event calendarClick: triggered when the visitor clicks on a day cell in
 *   1 of the attached views. The 'calendarClick' event passes in a Date
 *   object, representing the day that was clicked on, and the cid, to
 *   identify which calendar was clicked on.
 * - dateClicked: method to trigger the calendarClick event.
 *
 * @constructor
 *   Creates a new AvailabilityCalendar controller object.
 * @param {Object} settings
 * @param {number|string} settings.cid
 *   The calendar id for the calendar we want to interact with.
 *   A string starting with 'new' for not yet existing calendars.
 * @param {boolean} settings.overnight
 *   Indicates whether this calendar is used for overnight or for day bookings.
 */
Drupal.availabilityCalendar.Calendar = function(settings) {
  var that = this;
  var calendarSettings;
  /**
   * @type {Object} List of states (string: CSS class) indexed by date
   *   (string: yyyy-mm-dd format).
   */
  var days = {};
  /** @type {number[]} List of attached view ids. */
  var views = [];

  this.init = function (settings) {
    calendarSettings = settings;
  };

  /**
   * Creates an overview of all days and their states.
   *
   * @param {Object} cells
   *   A list of cells representing the dates in the calendar range and indexed
   *   by date (string in yyyy-mm-dd format).
   */
  function initDaysAdministration(cells) {
    // Extract the state information from each cell of the view. Each cell is an
    // object with 2 properties: cell and (ignored here) extraStates.
    $.each(cells, function (date, cell) {
      // Define the date in our administration (if not already set).
      if (days[date] === undefined) {
        days[date] = [];
      }
      // Split the class property in separate classes.
      $.each(cell.cell.className.split(/\s+/), function (i, cssClass) {
        // Ignore classes ending with -am.
        if (cssClass.substr(cssClass.length - 3) !== "-am") {
          // Remove any -pm at the end.
          if (cssClass.substr(cssClass.length - 3) === "-pm") {
            cssClass = cssClass.substring(0, cssClass.length - 3);
          }
          // Only add defined states (not extra states).
          if (Drupal.availabilityCalendar.getStates().get(cssClass) !== null) {
            // Overwrite any existing state.
            days[date] = [cssClass];
          }
        }
      });
    });
  }

  /**
   * @returns {number} The cid of the calendar.
   */
  this.getCid = function () {
    return calendarSettings.cid;
  };

  /**
   * @returns {boolean}
   *   If the calendar is used for overnight or for day rental.
   */
  this.isOvernight = function() {
    return calendarSettings.overnight;
  };

  /**
   * Attaches a calendar view to this controller.
   *
   * @param {Drupal.availabilityCalendar.View} calendarView
   */
  this.attachView = function(calendarView) {
    if (views.indexOf(calendarView.getCvid()) < 0) {
      views.push(calendarView.getCvid());
    }
    initDaysAdministration(calendarView.getCells());
  };

  /**
   * @returns {Array.<number>} A list of ids of attached views.
   */
  this.getViews = function() {
    return views;
  };

  /**
   * Returns the state id for the given date.
   *
   * @param {Date|string} date
   * @returns {string}
   *   The CSS class for the given date, or the empty string if the date is not
   *   within the calendar range.
   */
  this.getState = function(date) {
    date = Drupal.availabilityCalendar.formatIsoDate(date);
    return days[date] !== undefined ? days[date][days[date].length - 1] : "";
  };

  /**
   * Returns whether the given day is available.
   *
   * @param {Date|string} date
   * @returns {?boolean}
   *   true if the date is available.
   *   false if the date is not available.
   *   null if the date is not within the calendar range.
   */
  this.isAvailable = function(date) {
    var state = this.getState(date);
    return state !== "" ? Drupal.availabilityCalendar.getStates().get(state).isAvailable : null;
  };

  /**
   * Informs this calendar controller that a day has been clicked.
   *
   * @param {Date} date
   *   The date clicked.
   * @param {Drupal.availabilityCalendar.View} calendarView
   *   The originating calendarView.
   */
  this.dateClicked = function(date, calendarView) {
    var dateIso = Drupal.availabilityCalendar.formatIsoDate(date);
    if (days[dateIso] !== undefined) {
      $(that).trigger("calendarClick", [date, calendarSettings.cid, calendarView]);
    }
  };

  /**
   * Informs this calendar controller that a month has been clicked.
   *
   * @param {{year:number, month:number}} month
   *   The month clicked.
   * @param {Drupal.availabilityCalendar.View} calendarView
   *   The originating calendarView.
   */
  this.monthClicked = function(month, calendarView) {
    $(that).trigger("calendarMonthClick", [month, calendarView]);
  };

  /**
   * Informs this calendar controller that a week number has been clicked.
   *
   * @param {{year: number, month: number, week: number, firstDay: Date}} week
   * @param {Drupal.availabilityCalendar.View} calendarView
   *   The originating calendarView.
   */
  this.weekClicked = function(week, calendarView) {
    $(that).trigger("calendarWeekClick", [week, calendarView]);
  };

  /**
   * Informs this calendar controller that a day of week label has been clicked.
   *
   * @param {{year: number, month: number, dayOfWeek: number}} dayOfWeek
   *   The day of the week clicked. As this is always within the context of 1
   *   month, the year and month number should also be passed in.
   *   The dayOfWeek property is an number between 1 (monday) and 7 (sunday).
   * @param {Drupal.availabilityCalendar.View} calendarView
   *   The originating calendarView.
   */
  this.dayOfWeekClicked = function(dayOfWeek, calendarView) {
    $(that).trigger("calendarDayOfWeekClick", [dayOfWeek, calendarView]);
  };

  /**
   * Changes the availability state of the given day.
   *
   * @param {Date} date
   * @param {number|string} state
   *
   * @return {boolean}
   *   True if the date is in the current calendar range, false otherwise.
   */
  this.changeState = function(date, state) {
    var dateIso = Drupal.availabilityCalendar.formatIsoDate(date);
    if (days[dateIso] !== undefined) {
      days[dateIso].push(Drupal.availabilityCalendar.getStates().get(state).cssClass);
      $(that).trigger("stateChange", [date]);
      return true;
    }
     return false;
  };

  /**
   * Restores the availability state of the given day to its previous value.
   *
   * @param {Date} date
   *
   * @return {boolean}
   *   True if the date is in the current calendar range, false otherwise.
   */
  this.restoreState = function(date) {
    var dateIso = Drupal.availabilityCalendar.formatIsoDate(date);
    if (days[dateIso] !== undefined) {
      // Remove current state (if not the original state).
      if (days[dateIso].length > 1) {
        days[dateIso].pop();
        $(that).trigger("stateChange", [date]);
      }
      return true;
    }
    return false;
  };

  /**
   * Informs views to set an extra state on the given day.
   *
   * Extra states do not mix with or replace the availability settings.
   * An extra state is not added twice.
   *
   * @param {Date|string} date
   * @param {string} extraState
   */
  this.addExtraState = function(date, extraState) {
    var dateIso = Drupal.availabilityCalendar.formatIsoDate(date);
    if (days[dateIso] !== undefined) {
      $(that).trigger("extraStateAdd", [date, extraState]);
    }
  };

  /**
   * Informs views to remove an extra state on the given day.
   *
   * Extra states do not mix with or replace the availability settings.
   * An extra state is not added twice.
   *
   * @param {Date|string} date
   * @param {string} extraState
   */
  this.removeExtraState = function(date, extraState) {
    var dateIso = Drupal.availabilityCalendar.formatIsoDate(date);
    if (days[dateIso] !== undefined) {
      $(that).trigger("extraStateRemove", [date, extraState]);
    }
  };

  /**
   * Returns whether all dates in the given range are available.
   *
   * @param {Date} from
   * @param {Date} to
   * @returns {?boolean}
   *   true if the whole range is available,
   *   false if not the whole range is available,
   *   null if the given date range is not fully within the calendar range.
   */
  this.isRangeAvailable = function(from, to) {
    var available = true;
    var date = new Date(from.getTime());
    while (available === true && date <= to) {
      available = this.isAvailable(date);
      date.setDate(date.getDate() + 1);
    }
    return available;
  };

  /**
   * Sets all days in the from - to range to the given state.
   *
   * @param {Date} from
   * @param {Date} to
   * @param {number|string} state
   *
   * @return {{from: Date, to: Date}}
   *   The range as was actually changed. This will be a smaller range when not
   *   the whole range is currently visible/administered.
   */
  this.changeRangeState = function(from, to, state) {
    // Loop through range of dates.
    var first = null;
    var last = null;
    var date = new Date(from.getTime());
    while (date <= to) {
      if (this.changeState(date, state)) {
        first = first || new Date(date.getTime());
        last = date.getTime();
      }
      date.setDate(date.getDate() + 1);
    }
    return {from: first, to: new Date(last)};
  };

  /**
   * Restores all days in the from - to range to their previous state.
   *
   * @param {Date} from
   * @param {Date} to
   */
  this.restoreRangeState = function(from, to) {
    // Loop through range of dates.
    var date = new Date(from.getTime());
    while (date <= to) {
      this.restoreState(date);
      date.setDate(date.getDate() + 1);
    }
  };

  this.init(settings);
};

/**
 * @type {Drupal.availabilityCalendar.States} Collection of calendar states.
 */
Drupal.availabilityCalendar.states = null;

/**
 * Singleton implementation for accessing calendar states.
 *
 * @returns Drupal.availabilityCalendar.States
 */
Drupal.availabilityCalendar.getStates = function() {
  return Drupal.availabilityCalendar.states;
};

/**
 * @type {Object} Collection of calendar instances.
 */
Drupal.availabilityCalendar.calendars = {};

/**
 *
 * Multiton implementation for accessing calendars on the page.
 *
 * @param {number} cid
 * @returns {?Drupal.availabilityCalendar.Calendar}
 */
Drupal.availabilityCalendar.getCalendar = function(cid) {
  return Drupal.availabilityCalendar.calendars[cid] !== undefined ? Drupal.availabilityCalendar.calendars[cid] : null;
};

/**
 * @returns {Object} A list of all calendars on the current page indexed by cid.
 */
Drupal.availabilityCalendar.getCalendars = function() {
  return Drupal.availabilityCalendar.calendars;
};

/**
 * Initialization code that works by implementing the Drupal behaviors paradigm.
 *
 * - The collection of availability states is created.
 * - The calendars that are defined in the settings are created.
 */
Drupal.behaviors.availabilityCalendar = {
    attach: function(context, settings) {
      if (settings.availabilityCalendar) {
        if (settings.availabilityCalendar.states) {
          if (Drupal.availabilityCalendar.states === null) {
            Drupal.availabilityCalendar.states = new Drupal.availabilityCalendar.States(settings.availabilityCalendar.states);
          }
          else {
            // Reinitialize.
            Drupal.availabilityCalendar.states.init(settings.availabilityCalendar.states);
          }
        }

        if (settings.availabilityCalendar.calendars) {
          var key;
          for (key in settings.availabilityCalendar.calendars) {
            if (settings.availabilityCalendar.calendars.hasOwnProperty(key)) {
              var calendarSettings = settings.availabilityCalendar.calendars[key];
              if (Drupal.availabilityCalendar.calendars[calendarSettings.cid] === undefined) {
                Drupal.availabilityCalendar.calendars[calendarSettings.cid] = new Drupal.availabilityCalendar.Calendar(calendarSettings);
              }
              else {
                Drupal.availabilityCalendar.calendars[calendarSettings.cid].init(calendarSettings);
              }
            }
          }
        }
      }
    }
};

}(jQuery));
