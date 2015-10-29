(function($) {
  Drupal.behaviors.skypeStatus = {
    attach: function (context, settings) {

      var profile = $('a.skype-status-profile-link');
      var block = $('a.skype-status-block-link');

      $.fn.getSkypeStatus = function(style) {
        var self = $(this);
        var id = self.attr('rel');
        var url = settings.skypeStatus.ajaxURL + '/' + id + '/' + style;
        $.get(url, function(data){
          self.html(data);
        });
      }

      if (profile.length) {
        profile.each(function() {
          $(this).getSkypeStatus(settings.skypeStatus.profileStyle);
        });
      }

      if (block.length) {
        block.getSkypeStatus(settings.skypeStatus.blockStyle);
      }

    }
  }
})(jQuery);