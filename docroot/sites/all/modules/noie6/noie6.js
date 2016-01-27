
// Avoid library namespace conflict
(function ($) {
  Drupal.behaviors.noie6 = {
    attach: function (context, settings) {
      // Load NoIE6 settings under Drupal.settings
      settings.noie6 = noie6;  // Remove this line once drupal_add_js() supports the browser option.
      
      $('body').prepend(settings.noie6.widget);
      
      // Set height of overlay if exist
      if ($('#noie6-overlay').length) {
        $('#noie6-overlay').height($(document).height());
      }
      
      var noie6_center = $('#noie6-wrap').width() / (-2); // make sure the windows is centered even is the width change in css.
      var noie6_wrap_bg = $('#noie6-wrap').css('background-image');
      $('#noie6-wrap').
        css('margin-left', noie6_center).
        css('background-image', 'none').
        slideDown(
          function() {
            // avoid flickering
            $(this).css('background-image', noie6_wrap_bg);
          }
        );
        
      $('#noie6-close').click(function(){
        $('#noie6-wrap').slideUp('normal', function(){
          if ($('#noie6-overlay').length) {
            $('#noie6-overlay').hide();
          }
        });
        return false;
      });
    }
  };
})(jQuery);