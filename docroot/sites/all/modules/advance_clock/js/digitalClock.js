
(function($) {
  Drupal.behaviors.digitalClock = {
    attach: function(context, settings) {
      // Used as key in each function.
      if ( !Drupal.settings.advance_clock) {
        return;
      }
      var i = Drupal.settings.advance_clock.count;
      // Key - Value pair ("clock number" - "GMT Value").
      var clock_offset = Drupal.settings.advance_clock.clock_offset;
      var clock_type = Drupal.settings.advance_clock.clock_type;

      // Create two variable with the names of the months and days in an array.
      var monthNames = ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"];
      var dayNames = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"];

      return $.each(clock_offset, function(i, val) {

        // Create a newDate() object
        var newDate = new Date(new Date().getTime() + (new Date().getTimezoneOffset() * 60000) + (3600000 * val));
        // These functions need to be called for each and every clock. Say if there are 3 clocks this will be called 3 times.

        // Extract the current date from Date object
        newDate.setDate(newDate.getDate());
        // Output the day, date, month and year    
        $('#date-' + i).html(dayNames[newDate.getDay()] + " " + newDate.getDate() + ' ' + monthNames[newDate.getMonth()] + ' ' + newDate.getFullYear());

        // Seconds.
        setInterval(function() {
          // Create a newDate() object and extract the seconds of the current time on the visitor's
          var seconds = new Date(new Date().getTime() + (new Date().getTimezoneOffset() * 60000) + (3600000 * val)).getSeconds();
          // Add a leading zero to seconds value
          $("#sec-" + i).html((seconds < 10 ? "0" : "") + seconds);
        }, 1000);

        // Minutes.
        setInterval(function() {
          // Create a newDate() object and extract the minutes of the current time on the visitor's
          var minutes = new Date(new Date().getTime() + (new Date().getTimezoneOffset() * 60000) + (3600000 * val)).getMinutes();
          // Add a leading zero to the minutes value
          $("#min-" + i).html((minutes < 10 ? "0" : "") + minutes);
        }, 1000);

        // Hours.
        setInterval(function() {
          // Create a newDate() object and extract the hours of the current time on the visitor's
          var hours = new Date(new Date().getTime() + (new Date().getTimezoneOffset() * 60000) + (3600000 * val)).getHours();

          // Clock Type.
          if ( clock_type == 0 )
          {
            // Choose either "AM" or "PM" as appropriate
            var timeOfDay = (hours < 12) ? "AM" : "PM";
            // Convert the hours component to 12-hour format if needed
            currentHoursAP = (hours > 12) ? hours - 12 : hours;
            // Convert an hours component of "0" to "12"
            currentHoursAP = (currentHoursAP == 0) ? 12 : currentHoursAP;
            // Add a leading zero to the hours value
            $("#hours-" + i).html((currentHoursAP < 10 ? "0" : "") + currentHoursAP);
            $("#format-" + i).html(timeOfDay);

          }
          else
          {
            $("#hours-" + i).html((hours < 10 ? "0" : "") + hours);
          }
        }, 1000);
        // Ends.
      });
    }
  };
})(jQuery);
