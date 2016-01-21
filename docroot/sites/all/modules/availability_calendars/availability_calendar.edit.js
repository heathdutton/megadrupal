(function($) {
"use strict";

/**
 * @class Drupal.availabilityCalendar.Command represents a calendar state
 * changing command during the whole creation phase, i.e. from click on a state
 * to the click on the end date.
 *
 * @param {Drupal.availabilityCalendar.Calendar} calendar
 * @param {*} fieldContext
 * @constructor
 */
Drupal.availabilityCalendar.Command = function(calendar, fieldContext) {
  this.state = "";
  this.from = null;
  this.to = null;
  this.elt = $(".availability-changes", fieldContext);

  /**
   * Sets the state of the current command and, as this is supposed to be the
   * first parameter to be set, cleans the from and to dates.
   *
   * @param {string} [selectedState]
   */
  this.start = function(selectedState) {
    if (selectedState !== undefined) {
      this.state = selectedState;
    }
    this.from = null;
    this.to = null;
    this.show();
  };

  /**
   * Adds a date to the command. If it is the 1st date it will be the from date.
   * If it is the 2nd date it will be the to date, if necessary, swapping the
   * from and to dates.
   *
   * @param {Date} date
   */
  this.addDate = function(date) {
    if (this.from === null) {
      this.from = date;
    }
    else if (this.to === null) {
      this.to = date;
      if (this.to < this.from) {
        var from = this.from;
        this.from = this.to;
        this.to = from;
      }
      if (calendar.isOvernight()) {
        // Overnight rental: to date = departure date = am only: store as "from"
        // to "to - 1 day". But in the case of clicking twice on the same day,
        // it will be handled as a 1 night booking.
        if (this.to > this.from) {
          this.to.setDate(this.to.getDate() - 1);
        }
      }
    }
    this.show();
  };

  /**
   * @returns {boolean} Whether the current command is complete.
   */
  this.isComplete = function() {
    return this.to !== null && this.from !== null && this.state !== "";
  };

  /**
   * Replaces the current command in the accompanying hidden field.
   */
  this.show = function() {
    var val = this.elt.val();
    var pos = val.lastIndexOf("\n") + 1;
    val = val.substr(0, pos) + this.toString();
    this.elt.val(val);
  };

  /**
   * Finishes the current command and starts a new one.
   */
  this.finish = function() {
    this.show();
    this.elt.val(this.elt.val() + "\n");
    this.start();
  };

  /**
   * Returns the history as an array
   * @returns {Array}
   */
  this.getHistory = function() {
    var commands = this.elt.val().split("\n");
    var history = [];
    for (var i = 0; i < commands.length; i++) {
      var command = commands[i].split(',');
      if (command.length === 3 && command[2] !== "" && command[1] !== "" && command[0] !== "") {
        history.push({
          state: command[0],
          from: Drupal.availabilityCalendar.parseIsoDate(command[1]),
          to: Drupal.availabilityCalendar.parseIsoDate(command[2])
        });
      }
    }
    return history;
  };

  /**
   * @returns string
   *   A string representation of the current command.
   */
  this.toString = function() {
    var result = "";
    result += this.state;
    result += ",";
    result += this.from !== null ? Drupal.availabilityCalendar.formatIsoDate(this.from) : "";
    result += ",";
    result += this.to !== null ? Drupal.availabilityCalendar.formatIsoDate(this.to) : "";
    return result;
  };
};

/**
 * @class Drupal.availabilityCalendar.Editor
 *   Provides the glueing code that connects the form elements on entity edit
 *   forms (for entities with an availability calendar field) with the
 *   Drupal.availabilityCalendar.Calendar API object and the
 *   Drupal.availabilityCalendar.Command class.
 *
 * @param {Object} settings
 * @param {number} settings.cvid
 */
Drupal.availabilityCalendar.Editor = function(settings) {
  /** @type {Drupal.availabilityCalendar.Calendar} */
  var calendar;
  var view;
  var editorSettings;
  var fieldContext;
  var formRadios;
  var command;
  var calSelectedDay;

  this.init = function(settings) {
    editorSettings = settings;
    view = Drupal.availabilityCalendar.getCalendarView(editorSettings.cvid);
    calendar = view.getCalendar();
    fieldContext = view.getView().parents(".form-wrapper").first();
    fieldContext.once("Drupal-availabilityCalendar-Editor-init", function () {
      formRadios = $(".form-radios.availability-states", fieldContext);
      initCommand();
      attach2Checkbox();
      attach2States();
      bind2CalendarEvents();
      replayHistory();
    });
  };

  /**
   * Initialize a new Drupal.availabilityCalendar.Command object.
   */
  function initCommand() {
    command = new Drupal.availabilityCalendar.Command(calendar, fieldContext);
    command.start($("input[type=radio]:checked", formRadios).val());
    // Add css_class of states as class to wrapper elements around the radios.
    $("[type=radio]", formRadios).parent().addClass(function() {
      return Drupal.availabilityCalendar.getStates().get(($(this).children("[type=radio]").val())).cssClass;
    });
  }

  /**
   * Replays the history from the Drupal.availabilityCalendar.Command object.
   *
   * Repaying the command history is necessary in 2 cases:
   * - clicking preview instead of save
   * - after form validation errors
   * In both cases the form is redrawn, but the calendar view is redrawn in its
   * original state, not showing any changes made to the calendar. As the list
   * of changes is available in the hidden field, accessible via the command
   * object, we can replay all these changes.
   */
  function replayHistory() {
    var history = command.getHistory();
    for(var i = 0; i < history.length; i++) {
      calendar.changeRangeState(history[i].from, history[i].to, history[i].state);
    }
  }

  /**
   * Attach to "enable calendar" checkbox (if it exists).
   */
  function attach2Checkbox() {
    var enable = $(".availability-enable", fieldContext);
    if (enable.size() > 0 ) {
      $(".availability-details", fieldContext).toggle(enable.filter(":checked").size() > 0);
      enable.click(function () {
        $(".availability-details", fieldContext).toggle("fast");
      });
    }
  }

  /**
   * Attach to state radios events.
   */
  function attach2States() {
    $("input[type=radio]", formRadios).click(function() {
      // State clicked: remove cal-selected and restart current command.
      removeCalSelected();
      command.start($("input[type=radio]:checked", formRadios).val());
    });
  }

  /**
   * Attach to the calendar calendarClick event.
   */
  function bind2CalendarEvents() {
    $(calendar)
      .bind("calendarClick", selectDay)
      .bind("calendarMonthClick", selectMonth)
      .bind("calendarWeekClick", selectWeek)
      .bind("calendarDayOfWeekClick", selectDayOfWeek);
  }

  /**
   * Event handler that processes a click on a date in the calendar.
   *
   * @param {jQuery.Event} event
   * @param {Date} date
   */
  function selectDay(event, date/*, view*/) {
    command.addDate(date);
    if (!command.isComplete()) {
      setCalSelected(command.from);
    }
    else {
      removeCalSelected();
      event.target.changeRangeState(command.from, command.to, command.state);
      command.finish();
    }
  }

  /**
   * Changes a whole month at once.
   *
   * It does so by resetting the command and simulating 2 clicks:
   * - on the 1st day of the given month.
   * - in case of a full day calendar: on the last day of the given month.
   * - in case of an overnight calendar: on the 1st day of the next month.
   * The calendar will adjust this range to the currently administered range.
   *
   * @param {jQuery.Event} event
   * @param {{year:number, month:number}} month
   */
  function selectMonth(event, month/*, view*/) {
    removeCalSelected();
    command.start();
    command.addDate(new Date(month.year, month.month - 1, 1));
    command.addDate(new Date(month.year, month.month, event.target.isOvernight() ? 1 : 0));
    var realRange = event.target.changeRangeState(command.from, command.to, command.state);
    command.from = realRange.from;
    command.to = realRange.to;
    command.finish();
  }

  /**
   * Changes a whole month at once.
   *
   * It does so by resetting the command and simulating 2 clicks:
   * - on the 1st day of the given month.
   * - in case of a full day calendar: on the last day of the given month.
   * - in case of an overnight calendar: on the 1st day of the next month.
   * The calendar will adjust this range to the currently administered range.
   *
   * @param {jQuery.Event} event
   * @param {{year: number, month: number, week: number, firstDay: string}} week
   */
  function selectWeek(event, week/*, view*/) {
    // Simulate 2 clicks:
    // - on the 1st day of the given week.
    // - in case of a full day calendar: on the last day of the given week.
    // - in case of an overnight calendar: on the 1st day of the next week.
    removeCalSelected();
    command.start();
    command.addDate(new Date(week.firstDay.getTime()));
    var to = new Date(week.firstDay.getTime());
    to.setDate(to.getDate() + (event.target.isOvernight() ? 7 : 6));
    command.addDate(to);
    var realRange = event.target.changeRangeState(command.from, command.to, command.state);
    if (realRange.from !== null) {
      command.from = realRange.from;
      command.to = realRange.to;
      command.finish();
    }
    else {
      command.start();
    }
  }

  /**
   * Changes all the same day of the week days in a month at once.
   *
   * @param {jQuery.Event} event
   * @param {{year: number, month: number, dayOfWeek: number}} dayOfWeek
   *   The day of the week clicked. As this is always within the context of 1
   *   month, the year and month number should also be passed in.
   *   The dayOfWeek property is an number between 1 (monday) and 7 (sunday).
   */
  function selectDayOfWeek(event, dayOfWeek/*, view*/) {
    removeCalSelected();
    command.start();
    // Start at the 1st day of the given month,
    var date = new Date(dayOfWeek.year, dayOfWeek.month - 1, 1);
    // and advance to the 1st given day of the week in that month
    while (date.getDay() !== dayOfWeek.dayOfWeek % 7) {
      date.setDate(date.getDate() + 1);
    }
    // Loop over all that days of the week in the given month.
    while (date.getMonth() < dayOfWeek.month) {
      command.addDate(date);
      command.addDate(date);
      var realRange = event.target.changeRangeState(command.from, command.to, command.state);
      if (realRange.from !== null) {
        command.finish();
      }
      else {
        command.start();
      }
      // Go to the same day next week.
      date.setDate(date.getDate() + 7);
    }
  }

  /**
   * Set the cal-selected class on the day cell of the given date.
   *
   * @param {Date} day
   */
  function setCalSelected(day) {
    removeCalSelected();
    calSelectedDay = new Date(day.getTime());
    calendar.addExtraState(calSelectedDay, "cal-selected");
  }

  /**
   * Removes the cal-selected class on the day cell where it was last set on.
   */
  function removeCalSelected() {
    if (calSelectedDay !== null) {
      calendar.removeExtraState(calSelectedDay, "cal-selected");
      calSelectedDay = null;
    }
  }

  this.init(settings);
};

/**
 * @type {Object} Collection of calendar editor instances.
 */
Drupal.availabilityCalendar.editors = {};

/**
 * Multiton implementation for accessing calendar editors on the page.
 *
 * @param {number} cvid
 * @return {?Drupal.availabilityCalendar.Editor}
 */
Drupal.availabilityCalendar.getEditor = function(cvid) {
  return Drupal.availabilityCalendar.editors[cvid] !== undefined ? Drupal.availabilityCalendar.editors[cvid] : null;
};

/**
 * Initialization code that works by implementing the Drupal behaviors paradigm.
 *
 * Based on the information in the settings, the calendar editors are created.
 */
Drupal.behaviors.availabilityCalendarEditor = {
  attach: function(context, settings) {
    if (settings.availabilityCalendar && settings.availabilityCalendar.editors) {
      var key;
      for (key in settings.availabilityCalendar.editors) {
        if (settings.availabilityCalendar.editors.hasOwnProperty(key)) {
          var editorSettings = settings.availabilityCalendar.editors[key];
          if (Drupal.availabilityCalendar.editors[editorSettings.cvid] === undefined) {
            Drupal.availabilityCalendar.editors[editorSettings.cvid] = new Drupal.availabilityCalendar.Editor(editorSettings);
          }
          else {
            Drupal.availabilityCalendar.editors[editorSettings.cvid].init(editorSettings);
          }
        }
      }
    }
  }
};

}(jQuery));
