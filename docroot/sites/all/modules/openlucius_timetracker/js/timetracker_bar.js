(function ($) {
  Drupal.behaviors.openlucius_timetracker_bar = {
    attach: function (context, settings) {
      if (context == document) {
        var raw_data = settings.openlucius_timetracker.data;
        var ctx = document.getElementById("timeTrackerOverview").getContext("2d");
        var labels = [];
        var time_data = [];
        if (raw_data) {
          $.each(raw_data, function(index, value) {
            labels.push(index);
            time_data.push(value);
          });
        }

        var data = {
          labels: labels,
          datasets: [
            {
              label: "Clocked time",
              fillColor: "rgba(220,220,220,0.5)",
              strokeColor: "rgba(220,220,220,0.8)",
              highlightFill: "rgba(220,220,220,0.75)",
              highlightStroke: "rgba(220,220,220,1)",
              data: time_data
            }
          ]
        };
        var scaleBeginAtZero = false;
        if (settings.openlucius_timetracker.zero_start) {
          scaleBeginAtZero = true;
        }
        var options = {
          scaleBeginAtZero: scaleBeginAtZero
        };
        var myBarChart = new Chart(ctx).Bar(data, options);
      }

    }
  }
})(jQuery);
