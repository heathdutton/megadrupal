/**
 * @file
 * Add custom ajax command, move the scroll to the top of the browser window.
 */

(function($) {
  Drupal.ajax.prototype.commands.read_more_ajax = function(ajax, response, status) {
    var item = $('.more-load').offset().top;
    $('body,html').animate({
       scrollTop: (item - 10)
    }, response.speed);
    $('.more-load').removeClass('more-load');
  }
})(jQuery);
