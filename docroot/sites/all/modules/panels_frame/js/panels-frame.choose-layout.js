(function($) {

  Drupal.behaviors.panelsFrameChooseLayout = {
    attach: function(context, settings) {

      // Create jQuery Tabs for every "choose-layout" element known by the system.
      for (id in settings.panelsFrame.chooseLayout.tabs) {
        $('#' + id, context).once('panels-frame-choose-layout', function() {
          $(this).tabs(settings.panelsFrame.chooseLayout.tabs[id]);
            // .buttonset();
        });
      }
    }
  }

})(jQuery);
