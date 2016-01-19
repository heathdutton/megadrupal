
/**
 * @file
 * Initiate Tooltip(s).
 */

(function($) {
  Drupal.behaviors.tips = {
    attach: function(context, settings) {
      $.each(settings.tips, function(i, val) {
        $.each(settings.tips[i], function(key) {
          // Support basic content param position.
          if (settings.tips[i][key].content == 'param') {
            if (settings.tips[i][key].content_param_first) {
              $(i)[settings.tips[i][key].library](settings.tips[i][key].content_param, settings.tips[i][key].settings);
            } else {
              $(i)[settings.tips[i][key].library](settings.tips[i][key].settings, settings.tips[i][key].content_param);
            }
          } else {
            // Default implementation.
            $(i)[settings.tips[i][key].library](settings.tips[i][key].settings);
          }
        });
      });
    }
  };
}(jQuery));
