(function ($) {
  Drupal.behaviors.protest = {
    attach: function (context, settings) {
      $('body').prepend(Drupal.settings.protest.widget);

      // Set height of overlay if exist
      if ($('#protest-overlay').length) {
        $('#protest-overlay').height($(document).height());
      }

      var protest_center_h = $('#protest-wrap').width() / (-2); // Center horizontally: make sure the windows is centered even is the width change in css.
      var protest_center_v = ($(window).height() - $('#protest-wrap').height()) / 2; // Center vertically
      var protest_wrap_bg = $('#protest-wrap').css('background-image');


      $('#protest-wrap').
        css({'margin-left':protest_center_h, 'background-image':'none', 'top':protest_center_v}).
        slideDown(
          function() {
            // avoid flickering
            $(this).css('background-image', protest_wrap_bg);
          }
        );

      $('#protest-close').click(function(){
        $('#protest-wrap').slideUp('normal', function(){
          if ($('#protest-overlay').length) {
            $('#protest-overlay').hide();
          }
        });
        return false;
      });

      $('#protest-overlay').click(function() {
        $('#protest-close').trigger('click');
      });
    }
  };
})(jQuery);