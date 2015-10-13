(function($) {
  Drupal.behaviors.field_countdown = {
    attach: function(context, settings) {
      var i = 0;
      while (Drupal.settings.field_countdown["coundownSetting" + i])
      {
        var field_time = Drupal.settings.field_countdown["coundownSetting" + i]['coundownSetting_time'];
        var field_suffix = Drupal.settings.field_countdown["coundownSetting" + i]['coundownSetting_suffix'];
        var note = jQuery('#field-countdown-timer-note-' + field_suffix);
        ts = new Date(field_time * 1000);
        $('#field-countdown-timer-' + field_suffix).
                not('.jquery-countdown-timer-processed')
                .addClass('jquery-countdown-timer-processed').countdown({
          timestamp: ts,
          callback: function(days, hours, minutes, seconds) {
            var date_time_str = new Array();
            date_time_str['@days'] = Drupal.formatPlural(
                    days, '1 day', '@count days'
                    );
            date_time_str['@hours'] = Drupal.formatPlural(
                    hours, '1 hour', '@count hours'
                    );
            date_time_str['@minutes'] = Drupal.formatPlural(
                    minutes, '1 minute', '@count minutes'
                    );
            date_time_str['@seconds'] = Drupal.formatPlural(
                    seconds, '1 second', '@count seconds'
                    );
            var message = Drupal.t(
                    '@days, @hours, @minutes and @seconds left',
                    date_time_str
                    );
            note.html(message);
          }
        });

        i++;
      }
    }
  };
})(jQuery);
