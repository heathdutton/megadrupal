(function($) {
  Drupal.behaviors.dirtyform = {
    attach: function(context, settings) {

      settings.dirtyform = settings.dirtyform || Drupal.settings.dirtyform;

      $.each(settings.dirtyform, function(id, conf) {
        $('form#'+id).once('dirtyform', function() {
            $(this).areYouSure(conf);
        });
      });

    }
  };
})(jQuery);
