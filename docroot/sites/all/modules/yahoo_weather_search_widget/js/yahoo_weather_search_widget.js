/**
 * @file
 * Scripts.
 */

(function($){

  /**
   * Toggle the temperatures scales
   */
  Drupal.behaviors.yahooWeatherTempscale = {
    attach: function (context, settings) {
      $('.tempscale-button', context).once('yahooWeather', function () {
        $(this).click(function(){
          var temp = $('.temp');
          $('.tempscale-button.active').removeClass('active');
          $(this).toggleClass('active');
          if ($(this).hasClass('fahrenheit')) {
            temp.find('.celsius').hide();
            temp.find('.fahrenheit').show();
          } else if ($(this).hasClass('celsius')) {
            temp.find('.fahrenheit').hide();
            temp.find('.celsius').show();
          }

        });
      });
    }
  };

})(jQuery);
