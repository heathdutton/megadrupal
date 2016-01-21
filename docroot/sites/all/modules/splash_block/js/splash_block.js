(function ($) {
  Drupal.behaviors.splashBlock = {
    attach: function (context, settings) {
      var splashed = false;
      $.each(Drupal.settings.splashBlock, function() {
        if (!splashed) {
          var id = this.id;
          var value = $.jStorage.get(id);
          var ttl = $.jStorage.getTTL(id);

          if (!value || ttl == 0) {
            value = 1;

            var splash = $('#' + this.id).clone().wrap('<div>').parent().html();
            var time = this.time;
            var delay = this.delay;
            var width = this.size + 'px';
            setTimeout(function() {
              splashBlockSplash.open({
                content: splash,
                width: width
              });

              $.jStorage.set(id,value);
              $.jStorage.setTTL(id,time);

              splashed = true;
            }, delay);
          }
        }
      });
      $("#splash-content").remove();
    }
  };
})(jQuery);
