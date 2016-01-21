/*
 * @file hotel_booking.js
 * Provides js to make settings rates easier
 * @copyright Copyright(c) 2010 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands leerowlands at rowlands-bcs dot com
 *
 */

(function ($) {
  Drupal.behaviors.hotel_booking_calendars = {
    attach: function (context) {
      $('input[type=submit].uc_hotel_set_all:not(.uc_hotel_set_all-processed)', context).addClass('uc_hotel_set_all-processed').each(function () {
        $(this).click(function(){
          var rel = $(this).attr('rel');
          var val = $('.uc_hotel_set_all.' + rel).attr('value');
          $('.' + rel).each(function(){$(this).attr('value',val)});
          return false;
        })
      });
    }
  };
})(jQuery);
