/**
 * @file
 * Contains a jQuery World Calendars Datepicker demo javascript code.
 */

(function($) {
  /**
   * Attaches jQuery World Calendars demonstration behavior.
   */
  Drupal.behaviors.jqueryCalendarDemo = {
    attach: function(context) {
      $lang = Drupal.settings.jquery_calendar.lang;
      $picker = $('#datepicker-demo', context);
      $calendars = $('#datepicker-demo-calendars', context);
      if ($lang == 'en') $lang = '';
      $.calendars.picker.setDefaults($.calendars.picker.regional[$lang]);

      // Initialize the library.
      $picker.calendarsPicker({
        monthsToShow: [2, 3],
        showOtherMonths: false,
        calendar: $.calendars.instance($calendars.val(), $lang),
        onShow: $.calendars.picker.showStatus,
        renderer: $.calendars.picker.weekOfYearRenderer,
        onSelect: function(date) {
          alert(Drupal.t('You picked @date.',
            {'@date': date[0].formatDate()}
          ));
        }
      });

      $('.calendar_systems_js_date_picker_regular').calendarsPicker({calendar: $.calendars.instance($calendars.val(), $lang)});

      // Bind onchange handler.
      $calendars.bind('change', function() {
        try {
          var calendar = $.calendars.instance($calendars.val(), $lang);
          $picker
            .calendarsPicker('option', $.extend(
               {calendar: $.calendars.instance($calendars.val(), $lang)},
               $.calendars.picker.regional[$lang]))
            .calendarsPicker('showMonth');

            $('.calendar_systems_js_date_picker_regular').calendarsPicker('option', $.extend(
                {calendar: $.calendars.instance($calendars.val(), $lang)},
                $.calendars.picker.regional[$lang]));

            $.calendars.picker.setDefaults($.calendars.picker.regional['']);
        }
        catch (e) {
          alert(e);
        }
      });
    }
  };
})(jQuery);
