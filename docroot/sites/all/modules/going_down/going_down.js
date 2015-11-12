(function ($) {
  Drupal.behaviors.goingDown = {
    attach: function (context) {
      Drupal.goingDownUpdate = function() {
        var local = new Date();
        // time remaining in seconds
        var timeRemaining = Drupal.settings.goingDown.timeDown - Math.round(local.getTime() / 1000);

        if (timeRemaining > 0) {
          var days = Math.floor(timeRemaining / 86400);
          var hours = Math.floor((timeRemaining - (days * 86400)) / 3600);
          var minutes = Math.floor((timeRemaining - (days * 86400) - (hours * 3600)) / 60);
          var seconds = Math.floor(timeRemaining - (days * 86400) - (hours * 3600) - (minutes * 60));
          var output  = '';

          if (days > 0) {
            output += days + ' ' + Drupal.formatPlural(days, 'day', 'days') + ' ';
          }
          if (hours > 0) {
            output += hours + ' ' + Drupal.formatPlural(hours, 'hour', 'hours') + ' ';
          }
          if (minutes > 0) {
            output += minutes + ' ' + Drupal.formatPlural(minutes, 'minute', 'minutes') + ' ';
          }
          if (seconds > 0) {
            output += seconds + ' ' + Drupal.formatPlural(seconds, 'second', 'seconds') + ' ';
          }
          if (output !== '') {
            message = Drupal.settings.goingDown.messageGoingDown.replace('@time', output);
            $("div#going-down").html(message);
            timerId = setTimeout("Drupal.goingDownUpdate()", 1000);
          }
        }
        else {
          $("div#going-down").html(Drupal.settings.goingDown.messageOffline);
          if(timerId) {
            clearTimeout(timerId);
          }
        }
      };
      Drupal.goingDownUpdate();
    }
  };
})(jQuery);
