(function($) {

/**
 * Attach create event to calendar.
 */
Drupal.behaviors.timeEntryCreate = {
  attach: function(context, settings) {
    $.each(settings.fullcalendar, function(selector, calendar) {
      var $calendar = $(selector),
          $form = $calendar.find('.time-entry-form');

      /**
       * Attach into the calendar a event to create a new time entry.
       *
       * Example usage:
       * @code
       *   var start = new Date(),
       *       end = new Date();
       *
       *   end.setHours(end.getHours() + 1);
       *
       *   $('#calendar-id').trigger('timeEntry.create', [start, end]);
       * @endcode
       */
      $calendar.once('timeEntry.create')
        .bind('timeEntry.create', function(event, start, end) {
          var $start = $form.find('input[name="start"]'),
              $end = $form.find('input[name="end"]'),
              $submit = $form.find('button[type="submit"]');

          $start.val(Drupal.timeEntry.fullCalendar.formatDate(start));
          $end.val(Drupal.timeEntry.fullCalendar.formatDate(end));

          $submit.trigger('mousedown');
        });

      $form.hide();
    });
  }
};

})(jQuery);