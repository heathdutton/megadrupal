(function ($) {

Drupal.behaviors.blinkyMessages = {
  attach: function (context) {
    $('.messages').once('blinky-messages', function() {
      for (i=0; i < 5; i++) {
        // For some reason I can't get the jQuery UI effect to actually work.
        //$(this).effect('pulsate');
        $(this).fadeTo(400, 0).fadeTo(400, 1);
      }
    });
  }
};

})(jQuery);
