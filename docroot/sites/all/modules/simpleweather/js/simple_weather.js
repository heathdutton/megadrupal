/**
 * @file
 * JavaScript settings for jQuery simpleWeather plugin.
 *
 * Download simpleWeather jQuery plugin: http://simpleweatherjs.com.
 * Read README.txt for installation instructions.
 */

(function($) {
  $(document).ready(function() {
    /* Block Settings */
    var zipCode = Drupal.settings.simple_weather.zipCode;
    var woeid = Drupal.settings.simple_weather.woeid;
    var scale = Drupal.settings.simple_weather.scale;

    /* The simple weather widget settings */
    $.simpleWeather({      
      woeid: woeid,
      location: zipCode,
      unit: scale,
      success: function(weather) {
        html = '<h2>' + weather.city + ', ' + weather.region + '</h2>';
        html += '<img style="float:left;" width="125px" src="' + weather.image + '">';
        html += '<div class="report">' + weather.temp + '&deg; ' + weather.units.temp + '<br /><span>' + weather.currently + '</span></div>';
        html += '<a href="' + weather.link + '" title="Yahoo! Weather Forcast">View Forecast &raquo;</a>';
        $("#simple-weather").html(html);
      },
      error: function(error) {
        $("#simple-weather").html('<p>' + error + '</p>');
      }
    });
  });
})(jQuery);
