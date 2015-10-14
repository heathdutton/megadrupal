(function ($) {
  Drupal.behaviors.pickadate = {
    attach: function(context, settings) {
      if (typeof settings.pickadate != 'undefined') {
        if (typeof settings.pickadate.dateSettings != 'undefined') {
          $.each(settings.pickadate.dateSettings, function(index, value) {
            $('#' + index).pickadate(JSON.parse(value));
          });
        }

        if (typeof settings.pickadate.timeSettings != 'undefined') {
          $.each(settings.pickadate.timeSettings, function(index, value) {
            $('#' + index).pickatime(JSON.parse(value));
          });
        }
      }
    }
  };
})(jQuery);