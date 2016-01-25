/**
 * @file
 * The JavaScript that controls the beer o'clock countdown timer.
 */

(function ($) {

/**
 * Pad a number to be a certain width.
 *
 * @param int number A number you want to pad
 * @param int width  The amount of characters you want the number to be padded
 *                   to.
 *
 * @see  http://stackoverflow.com/questions/1267283/how-can-i-create-a-zerofilled-value-using-javascript
 *
 * @return string    The padded string.
 */
function zeroFill(number, width) {
  width -= number.toString().length;
  if ( width > 0 ) {
    return new Array(width + (/\./.test( number ) ? 2 : 1)).join('0') + number;
  }
  // Always return a string.
  return number + "";
}

Drupal.behaviors.beer_o_clock_countdown = {
  attach: function(context) {
    var countdown_timer;
    var currentdate = new Date();
    var now = Math.round(new Date().getTime() / 1000.0);
    var beer_o_clock = Drupal.settings.beer_o_clock.boc_timer;
    var diff = beer_o_clock - now;

    // Countdown every second.
    countdown_timer = setInterval(function () {

      // Math is awesome.
      var seconds = diff % 60;
      var seconds_min = Math.floor(diff / 60);
      var minutes = seconds_min % 60;
      var hour_min = Math.floor(seconds_min / 60);
      var hours = hour_min % 24;
      var hour_day = Math.floor(hour_min / 24);
      var days = hour_day % 365;

      // Not yet beer o'clock.
      if (diff >= 0) {
        $("#boc_timer").text(zeroFill(days, 2) + ":" + zeroFill(hours, 2) + ":" + zeroFill(minutes, 2) + ":" + zeroFill(seconds, 2));
        diff--;
      }

      // Now it is beer o'clock.
      else {
        $("#boc_text").text(Drupal.settings.beer_o_clock.boc_mess);
        $("#boc_timer").remove();
        clearInterval(countdown_timer);
      }
    }, 1000);
  }
};

})(jQuery);
