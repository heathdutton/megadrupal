(function($) {
    $(document).ready(function() {	
      yatt = function() {
        if($('#yatt-days').length > 0) {
          var d1 = new Date(Drupal.settings.Yatt.yatt_expiry_time);
          var d2 = new Date();

          if(d2 > d1){
            // Expiry text
            $('#yatt-days').remove();
            $('#yatt-hours').remove();
            $('#yatt-minutes').remove();
            $('#yatt-seconds').remove();

            $('.yatt-wrapper').append('<div class="yatt-expiry-text">' + Drupal.settings.Yatt.yatt_expiry_text + '</div>');
          }
          else {
            days = Math.floor((d1-d2) / (1000*60*60*24));
            restOfDaysInMilliSeconds = Math.round((	((d1-d2) / (1000*60*60*24))		-		(Math.floor((d1-d2) / (1000*60*60*24)))	)	*	(1000*60*60*24));

            hours = Math.floor((restOfDaysInMilliSeconds) / (1000*60*60));
            restOfHoursInMilliSeconds = ((	((restOfDaysInMilliSeconds) / (1000*60*60))		-		(Math.floor((restOfDaysInMilliSeconds) / (1000*60*60)))	)	*	(1000*60*60));

            minutes = Math.floor((restOfHoursInMilliSeconds) / (1000*60));
            restOfMinutesInMilliSeconds = ((	((restOfHoursInMilliSeconds) / (1000*60))		-		(Math.floor((restOfHoursInMilliSeconds) / (1000*60)))	)	*	(1000*60));

            seconds = Math.floor((restOfMinutesInMilliSeconds) / (1000));

            //document.write(days + ' dagen, ' + hours + ' uren, ' + minutes + ' minuten, ' + seconds + ' seconden');
            $('#yatt-days .yatt-element-value').html('' + days + '');
            $('#yatt-hours .yatt-element-value').html('' + hours + '');
            $('#yatt-minutes .yatt-element-value').html('' + minutes + '');
            $('#yatt-seconds .yatt-element-value').html('' + seconds + '');

            setTimeout(yatt, 1000);
          }
        }   
      }
      yatt();
    });
})(jQuery);
