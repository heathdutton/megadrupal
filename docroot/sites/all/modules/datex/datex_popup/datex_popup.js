/**
 * Attaches the calendar behavior to all required fields
 */
(function ($) {
datexPopupGetInfo = function (id) {
  for (i in Drupal.settings.datex.fields) {
    if (i.length < id.length && id.substr(0, i.length) == i) {
      return Drupal.settings.datex.fields[i];
    }
  }

  return {'calendar' : 'default'};
}
Drupal.behaviors.date_popup = {
  attach: function (context) {

  for (var id in Drupal.settings.datePopup) {
    $('#'+ id).bind('focus', Drupal.settings.datePopup[id], function(e) {
      if (!$(this).hasClass('date-popup-init')) {
        var datePopup = e.data;
        // Explicitely filter the methods we accept.
        switch (datePopup.func) {
          case 'datepicker':
            var info = datexPopupGetInfo(this.id);
            switch (info.calendar) {
              case 'jalali':
                var lang = Drupal.settings.datex.global.langcode;
                if (lang == 'en') lang = '';

                var calendar = $.calendars.instance('jalali', lang);
                var extend = $.extend(
                    {calendar: calendar},
                    $.calendars.picker.regional[lang]);

                extend.yearRange = info.yearRange;
                extend.dateFormat = info.format;
                extend.defaultDate = calendar.newDate(info.DefaultYear, info.DefaultMonth, info.DefaultDay);
                extend.altField = '#' + info.altField;
                extend.altFormat = info.altFormat;

                $(this).calendarsPicker(extend);
                break;

              default:
                $(this)
                  .datepicker(datePopup.settings)
                  .addClass('date-popup-init')
                break;
            }

            $(this).click(function(){
              $(this).focus();
            });
            $(this).addClass('date-popup-init');
            break;

          case 'timeEntry':
            $(this)
              .timeEntry(datePopup.settings)
              .addClass('date-popup-init')
            $(this).click(function(){
              $(this).focus();
            });
            break;
          case 'timepicker':
            // Translate the PHP date format into the style the timepicker uses.
            datePopup.settings.timeFormat = datePopup.settings.timeFormat
              // 12-hour, leading zero,
              .replace('h', 'hh')
              // 12-hour, no leading zero.
              .replace('g', 'h')
              // 24-hour, leading zero.
              .replace('H', 'HH')
              // 24-hour, no leading zero.
              .replace('G', 'H')
              // AM/PM.
              .replace('A', 'p')
              // Minutes with leading zero.
              .replace('i', 'mm')
              // Seconds with leading zero.
              .replace('s', 'ss');

            datePopup.settings.startTime = new Date(datePopup.settings.startTime);
            $(this)
              .timepicker(datePopup.settings)
              .addClass('date-popup-init');
            $(this).click(function(){
              $(this).focus();
            });
            break;
        }
      }
    });
  }
  }
};
})(jQuery);
