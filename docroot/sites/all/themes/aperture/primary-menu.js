// Suckerfish menu
(function ($) {
  
Drupal.behaviors.primary_menu = {
  attach: function (context, settings) {
    // Hover emulation for IE 6.
    if ($.browser.msie && parseInt(jQuery.browser.version) == 6) {
      $('#primary-menu li').hover(function() {
        $(this).addClass('iehover');
      }, function() {
        $(this).removeClass('iehover');
      });
    }

    // Delayed mouseout.
    $('#primary-menu li').hover(function() {
      // Stop the timer.
      clearTimeout(this.sfTimer);
      // Display child lists.
      $('> ul', this).css({left: '0' /* auto */, display: 'block'})
        // Immediately hide nephew lists.
        .parent().siblings('li').children('ul').css({left: '-999em', display: 'none'});
    }, function() {
      // Start the timer.
      var uls = $('> ul', this);
      this.sfTimer = setTimeout(function() {
        uls.css({left: '-999em', display: 'none'});
      }, 400);
    });
  }
};

})(jQuery);
