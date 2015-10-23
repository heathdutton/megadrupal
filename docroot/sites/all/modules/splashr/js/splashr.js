(function ($) {

  Drupal.behaviors.splashr = {
    attach:function (context) {

      // Fetch splash settings.
      var settings = Drupal.settings.splash;

      var splashr_inter_open = function () {

        if (settings['no_cookie'] == 1 || !$.cookie('splashr_inter')) {
          if (settings['no_cookie'] != 1) {
            if (settings['num_day_cookie'] <= 0) {
              $.cookie('splashr_inter', 'okay');
            }
            else {
              $.cookie('splashr_inter', 'okay', { expires:settings['num_day_cookie'] });
            }
          }

          // Show splash and overlay if required.
          $('body').css('overflow', 'hidden');
          $('html').css('overflow', 'hidden');

          if (settings['show_overlay']) {
            $('#splashr_overlay').show();
          }
          $('#splashr').show();

          // Hide splash and overlay on mouse click.
          if (settings['dismiss_click'] > 0) {
            $('#splashr, #splashr_overlay').live('click', function () {
              splashr_inter_close();
            });
          }

          // Hide splash and overlay on timeout.
          if (settings['dismiss_timeout'] > 0) {
            setTimeout(function () {
              splashr_inter_close();
            }, settings['dismiss_timeout'] * 1000);
          }

          // Hide splash and overlay on dismiss link.
          $('#splashr_close').live('click', function () {
            splashr_inter_close();
          });

        }
      }

      var splashr_inter_close = function () {
        var settings = Drupal.settings.splash;
        // Hide splash and overlay if enabled.
        $('#splashr').hide();
        if (settings['show_overlay'] > 0) {
          $('#splashr_overlay').hide();
        }
        // Reset overflows.
        $('body').css('overflow', '');
        $('html').css('overflow', '');
      }

      // Ensure the splash has been called once.
      if (!settings['called']) {
        splashr_inter_open();
        settings['called'] = true;
      }
    }
  };

})(jQuery);
