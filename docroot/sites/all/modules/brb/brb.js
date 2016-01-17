(function ($) {
  Drupal.behaviors.brb = {
    attach: function (context, settings) {
      // Load brb settings under Drupal.settings
      settings.brb = brb;  // Remove this line once drupal_add_js() supports the browser option.
      
      $('body').prepend(settings.brb.widget);
      
      $('#brb-wrap').dialog({
        modal: settings.brb.overlay,
        title: settings.brb.title,
        width: $('#brb-wrap').width()
      });
    }
  };
})(jQuery);