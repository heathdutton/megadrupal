(function($) {

Drupal.timeEntry = Drupal.timeEntry || {};
Drupal.timeEntry.fullCalendar = {};

/**
 * FullCalendar plugin implementation.
 */
Drupal.fullcalendar.plugins.time_entry = {

  /**
   * Add in FullCalendar options.
   *
   * @param fullcalendar
   *   The fullcalendar object.
   *
   * @see http://arshaw.com/fullcalendar/docs
   */
  options: function (fullcalendar) {
    var settings = Drupal.settings.fullcalendar[fullcalendar.dom_id].time_entry,
        time_entry_options = {
          selectable: true,
          selectHelper: true,
          select: Drupal.timeEntry.fullCalendar.select
        };

    return $.extend(time_entry_options, settings);
  }
};

/**
 * Callback called after select a new date.
 *
 * @see http://arshaw.com/fullcalendar/docs/selection/select_callback
 */
Drupal.timeEntry.fullCalendar.select = function(startDate, endDate, allDay, jsEvent, view) {
  var calendar = view.calendar,
      $calendar = $(view.element).parents('.fullcalendar');

  calendar.renderEvent({
    title: '',
    start: startDate,
    end: endDate,
    allDay: allDay,
    className: 'fc-event-default',
    editable: true
  }, true);

  $calendar.trigger('timeEntry.create', [startDate, endDate]);

  calendar.unselect();
};

/**
 * Converts a date to a format compatible with time entry form.
 *
 * @param date
 *   The date object to be converted.
 *
 * @return
 *   String with the formatted date.
 */
Drupal.timeEntry.fullCalendar.formatDate = function(date) {
  var utimezone = date.getTimezoneOffset() * 60000,
      timezone = new Date(utimezone),
      datestr = {},
      timestampstr = {},
      formatted = '';

  datestr = {
      year: date.getFullYear(),
     month: ('0' + (date.getMonth() + 1)).slice(-2), // months starts in 0, so we must add 1
       day: ('0' + date.getDate()).slice(-2), // add leading zeros
      hour: ('0' + date.getHours()).slice(-2),
    minute: ('0' + date.getMinutes()).slice(-2),
    second: ('0' + date.getSeconds()).slice(-2)
  };

  timestampstr = {
      hour: ('0' + timezone.getHours()).slice(-2),
    minute: ('0' + timezone.getMinutes()).slice(-2)
  };

  formatted += datestr.year;
  formatted += '-';
  formatted += datestr.month;
  formatted += '-';
  formatted += datestr.day;
  formatted += 'T';
  formatted += datestr.hour;
  formatted += ':';
  formatted += datestr.minute;
  formatted += ':';
  formatted += datestr.second;
  formatted += utimezone > 0 ? '-' : '';
  formatted += timestampstr.hour;
  formatted += ':';
  formatted += timestampstr.minute;

  return formatted;
};

})(jQuery);
