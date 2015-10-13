;(function ($) {
  Drupal.behaviors.slidebox = {
    attach: function(context, settings) {

      // Check for exising slidebox cookie
      if ($.cookie('slidebox') == null) {
        // Set to default of 'open' if not previously set
        $.cookie('slidebox', 'open', { path: '/' });
      }

      $('#slidebox_cookie').hide();

      $(window).scroll(function() {
        var distanceTop = $('#slidebox_trigger').offset().top - $(window).height();
        if  ($(window).scrollTop() >= distanceTop) {
          if ($.cookie('slidebox') == 'open') {
            $('#slidebox').animate({'right':'0px'}, settings.slidebox.showTime);
            $('#slidebox_manual').hide();
            $('#slidebox_cookie').hide();
          }
        }
        else {
          $('#slidebox').stop(true).animate({'right': '-' + ($('#slidebox').width() + 30) + 'px'}, settings.slidebox.hideTime);
          $('#slidebox_manual').show();
          $('#slidebox_cookie').hide();
        }
      });

      $('#slidebox .close').click(function(){
        $('#slidebox').stop(true).animate({'right': '-' + ($('#slidebox').width() + 30) + 'px'}, settings.slidebox.hideTime);
        $('#slidebox_manual').show();
        $(window).unbind();
        if ($.cookie('slidebox') != 'closed') {
          $('#slidebox_cookie').show();
        }
      });

      $('#slidebox_manual .open').click(function() {
        $('#slidebox').animate({'right':'0px'}, settings.slidebox.showTime);
        $('#slidebox_manual').hide();
        $('#slidebox_cookie').hide();
      });

      // Offer the user the ability to set the Slidebox to be persistently closed
      $('#slidebox_cookie .set').click(function() {
        $.cookie('slidebox', 'closed', { path: '/' });
        $('#slidebox_cookie').hide();
      });
    }
  };
})(jQuery);

