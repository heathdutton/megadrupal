(function($){
  Drupal.behaviors.field_timer = {
    attach: function() {
      var settings = Drupal.settings.field_timer;
      for (var type in settings) {
        for (var id in settings[type]) {
          for (var delta in settings[type][id]) {
            switch (settings[type][id].plugin) {
              case 'county':
                var options = settings[type][id].options;
                $('#county-' + type + '-' + settings[type][id].nid + '-' + delta).not('.field-timer-processed').
                  county({
                    endDateTime: new Date(settings[type][id][delta] * 1000),
                    animation: options.animation,
                    speed: options.speed,
                    theme: options.county_theme,
                    reflection: options.reflection,
                    reflectionOpacity: options.reflectionOpacity,
                  }).addClass('field-timer-processed');
                break;

              case 'jquery.countdown':
                var options = settings[type][id].options;
                $('#jquery-countdown-' + type + '-' + settings[type][id].nid + '-' + delta).not('.field-timer-processed').
                  countdown({
                    until: options.until ? new Date(settings[type][id][delta] * 1000) : null,
                    since: options.since ? new Date(settings[type][id][delta] * 1000) : null,
                    format: options.format,
                    layout: options.layout,
                    compact: options.compact,
                    significant: options.significant,
                    timeSeparator: options.timeSeparator,
                    description: options.description,
                    expiryText: options.expiryText,
                    expiryUrl: options.expiryUrl,
                    padZeroes: options.padZeroes,
                  }).addClass('field-timer-processed');
                break;
                
              case 'jquery.countdown.led':
                var options = settings[type][id].options;
                $('#jquery-countdown-led-' + type + '-' + settings[type][id].nid + '-' + delta).not('.field-timer-processed').
                  countdown({
                    until: options.until ? new Date(settings[type][id][delta] * 1000) : null,
                    since: options.since ? new Date(settings[type][id][delta] * 1000) : null,
                    layout: $('#jquery-countdown-led-' + type + '-' + settings[type][id].nid + '-' + delta).html(),
                    description: options.description,
                    expiryText: options.expiryText,
                    expiryUrl: options.expiryUrl,
                  }).addClass('field-timer-processed');
            }
          }
        }
      }
    }
  }
})(jQuery);
