(function ($) {

Drupal.behaviors.watchdog = {
  // Attach functionality.
  attach: function() {
    // Grab settings.
    Drupal.watchdog.interval = $('#edit-watchdog-live-interval').val();
    Drupal.watchdog.disabled = $('#edit-watchdog-live-disabled').attr('checked') || 0;

    // Interval setting.
    $('#edit-watchdog-live-interval').bind('change', function(event) {
      Drupal.watchdog.saveSetting('interval', this.value);
    });

    // Disabled state.
    $('#edit-watchdog-live-disabled').bind('click', function(event) {
      Drupal.watchdog.saveSetting('disabled', this.checked);
      if (!this.checked) {
        Drupal.watchdog.start();
      }
    });

    // Attach tooltip if BeatyTips plugin is loaded.
    if (jQuery.bt && !Drupal.settings.watchdogLive.beautytips_module) {
        Drupal.watchdog.attachTooltips();
    }

    // Handle updating.
    Drupal.watchdog.start();
  }
};

Drupal.watchdog = {
  // Check callback for update and display new content if any.
  load: function() {
    $.get(Drupal.settings.watchdogLive.callback_url, function(data) {
      var result = jQuery.parseJSON(data);

      // Only if we get content back.
      if (result.content) {

        var animated = false;
        $('#dblog-filter-form').parent().children().not('#dblog-filter-form, #dblog-clear-log-form').fadeOut('fast', function() {
          // Since we have multiple children, the callback will be called multiple times.
          // Force a stop.
          if (animated) {
           return false;
          }
          animated = true;

          // Remove old table.
          $('#dblog-filter-form').parent().children().not('#dblog-filter-form, #dblog-clear-log-form').remove();

          // Bring in new content.
          $('#dblog-filter-form').parent().append(result.content);
          $('#dblog-filter-form').parent().children().not('#dblog-filter-form, #dblog-clear-log-form').fadeIn('fast');

          // Attach tooltip if BeatyTips plugin is loaded.
          if (jQuery.bt && !Drupal.settings.watchdogLive.beautytips_module) {
            Drupal.watchdog.attachTooltips();
          }
        });
      }

      // Continue the loop.
      Drupal.watchdog.start();
    });
  },

  // Save a setting for the configurable options.
  saveSetting: function(name, value) {
    var setting = {};
    setting[name] = value;
    $.post(Drupal.settings.watchdogLive.setting_url, setting);
    Drupal.watchdog[name] = value;
  },

  // Start the process.
  start: function() {
    if (!Drupal.watchdog.disabled) {
      Drupal.watchdog.timeoutId = setTimeout(Drupal.watchdog.load, Drupal.watchdog.interval);
    }
  },

  // Attach tooltips to anchors in watchdog table.
  attachTooltips: function() {
    $('table#admin-dblog tr a:not([class])').bt({
      ajaxPath: ["$(this).attr('href')", 'table.dblog-event'],
      ajaxCache: false,
      trigger: 'hoverIntent',
      hoverIntentOpts: {
          interval: 300,
          timeout: 150
      },
      width: 'auto',
      fill: '#F7F7F7',
      strokeStyle: '#B7B7B7',
      spikeLength: 10,
      spikeGirth: 10,
      padding: 8,
      windowMargin: 70,
      cornerRadius: 0,
      positions: ['top', 'bottom']
    });
  }

};

})(jQuery);