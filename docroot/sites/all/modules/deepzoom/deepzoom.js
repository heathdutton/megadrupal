(function($) {
  Drupal.behaviors.deepzoom = {
    attach: function(context) {
      $('.deepzoom').each(function() {
        id = $(this).attr('id').split('-');
        Seadragon.Utils.addEvent(window, 'load', function() {
          viewer = new Seadragon.Viewer('deepzoom-' + id[1]);
          viewer.openDzi(Drupal.settings.deepzoom + '/' + id[1] + '.dzi');
        });
      });
    }
  }
})(jQuery);
