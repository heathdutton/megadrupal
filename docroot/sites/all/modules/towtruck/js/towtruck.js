(function(window, $, undefined) {
  Drupal.behaviors.TowTruck = {
    attach: function(context, settings) {
      for(var prop in settings.TowTruck) {
        window[prop] = settings.TowTruck[prop];
      }
    }
  }
})(window, jQuery);
