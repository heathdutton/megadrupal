(function ($) {
  Drupal.behaviors.openlucius_timetracker = {
    attach: function (context, settings) {
      if (context == document) {

        // The colors to be used for progress.
        var red = { r: 244, g: 31, b: 27 },
            blue = { r: 8, g: 138, b: 181 };

        /**
         * Function to interpolate between numeric values.
         *
         * @param start
         *   The starting value.
         * @param end
         *   The ending value.
         * @param steps
         *   The amount of steps.
         * @param count
         *   The current step.
         * @returns {number}
         */
        function Interpolate(start, end, steps, count) {
          var s = start,
              e = end,
              final = s + (((e - s) / steps) * count);
          return Math.floor(final);
        }

        function updateClocks() {
          var internalTime = active2 ? time2 : time;
          var percentage  = (active2 ? settings.openlucius_timetracker.max2 : settings.openlucius_timetracker.max1) / 100;
          var currentPercentage = internalTime / percentage;

          // Get current color for the timer.
          var r = Interpolate(blue.r, red.r, 100, currentPercentage);
          var g = Interpolate(blue.g, red.g, 100, currentPercentage);
          var b = Interpolate(blue.b, red.b, 100, currentPercentage);

          // Calculate the raw values.
          hours = Math.floor(internalTime / 3600);
          remaining = internalTime % 3600;
          minutes = Math.floor(remaining / 60);
          seconds = remaining % 60;
          // Add leading zeros.
          if (minutes < 10) {
            minutes = '0' + minutes;
          }
          if (seconds < 10) {
            seconds = '0' + seconds;
          }

          if (active2) {
            // Update the html with the new time.
            $('.current-time2').html(hours + ':' + minutes + ':' + seconds);
            $('.current-time2, .current-active-todo2 a').css({ "color": "rgb(" + r + "," + g + "," + b + ")" });
            time2 += 1;
          }
          else {
            // Update the html with the new time.
            $('.current-time').html(hours + ':' + minutes + ':' + seconds);
            $('.current-time, .current-active-todo a').css({ "color": "rgb(" + r + "," + g + "," + b + ")" });
            time += 1;
          }
          if (active || active2) {
            window.setTimeout(updateClocks, 1000);
          }
        }

        // Check if the openlucius_timetracker is available.
        if (settings.hasOwnProperty('openlucius_timetracker')) {

          // Init variables.
          var time = 0,
              time2 = 0,
              active = false,
              active2 = false,
              hours,
              minutes,
              seconds,
              remaining;

          // Check if there is a start time.
          if (settings.openlucius_timetracker.hasOwnProperty('start_time1')) {
            time = settings.openlucius_timetracker.start_time1;
          }
          // Check if there is a secondary start time.
          if (settings.openlucius_timetracker.hasOwnProperty('start_time2')) {
            time2 = settings.openlucius_timetracker.start_time2;
          }
          // Check if the first timer is active.
          if (settings.openlucius_timetracker.hasOwnProperty('active1')) {
            active = settings.openlucius_timetracker.active1;
          }
          // Check if the second timer is active.
          if (settings.openlucius_timetracker.hasOwnProperty('active2')) {
            active2 = settings.openlucius_timetracker.active2;
          }

          // Check if we have an active timer.
          if (active || active2) {
            updateClocks();
          }

          // Fetch the start button.
          var start_timer = $('.start-node-timer'), real_submit = $('#timetracker-form .main-ajax-submit');

          // Check if the start button exist.
          if (start_timer.length > 0) {

            start_timer.on('mousedown', function (e) {
              e.preventDefault();
              real_submit.trigger('mousedown');
              $('#openlucius-timetracker-timer-form').show();
              $(this).remove();
            });
          }

          var secondary_submit = $('.other-timer .secondary-submit');

          // Check if the secondary button exists.
          if (secondary_submit.length > 0) {

            // On mousedown.
            secondary_submit.on('mousedown', function(e) {
              e.preventDefault();
              e.stopPropagation();

              // Switch active case off.
              $.post(Drupal.settings.basePath + 'timetracker/finish-active', {
                'token': $(this).attr('data-token')
              }, function (data) {
                if (data == true) {
                  active2 = false;
                  $('.other-timer').slideUp(100, function() {
                    $('#openlucius-timetracker-timer-form').attr('style', 'display:none');
                    $('.hide-initially').removeClass('hide-initially');
                  });
                }
              });

              return false;
            });
          }

          // When the start button is clicked.
          real_submit.on('mousedown', function (e) {

            // Check if the active time is running.
            if (!active) {
              active = true;

              // Remove button if exist.
              if (start_timer.length > 0) {
                start_timer.remove();
              }

              // If there's another timer running only change the boolean.
              if (active2) {
                active2 = false;
                $('.other-timer').slideUp("slow", function() {
                  $('.hide-initially').removeClass('hide-initially');
                });
              }
              else {
                updateClocks();
              }
              $(this).html('<span class="glyphicon glyphicon-stop"></span>');
            }
            else if (active) {
              active = false;
              $(this).html('<span class="glyphicon glyphicon-play"></span>');
            }
          });
        }
      }
    }
  }
})(jQuery);
