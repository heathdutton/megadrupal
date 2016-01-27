(function($) {
  Drupal.behaviors.skypeStatus = {
    attach: function (context, settings) {
      var currentStatus = 2;

      /**
       * 0 - Unknown
       * 1 - Offline
       * 2 - Online
       * 3 - Away
       * 4 - Not available
       * 5 - Do not disturb
       * 6 - Invisible
       * 7 - Skype me
      **/

      function incStatus(status) {
        // Skip 'invisible'.
        if (status === 5) {
          return 7;
        }
        // Start again from 'offline'.
        else if (status === 7) {
          return 1;
        }
        else {
          return status + 1;
        }
      }

      setInterval(function() {
        $('.skype-status-img-rotate').each(function () {
            var src = $(this).attr('src');
            $(this).attr('src', src.replace(/^([^\d]+)([\d])\.png$/, "$1" + currentStatus + '.png'));
        });
        currentStatus = incStatus(currentStatus);
      }, 2000);

    }
  }
})(jQuery);