(function ($) {

// Unveil behavior code
Drupal.behaviors.Unveil = {
  attach: function() {
    // Load Unveil, and set the distance to unveil at.
    $('img').unveil(Drupal.settings.Unveil.unveil_distance);
  }
}

}(jQuery));
