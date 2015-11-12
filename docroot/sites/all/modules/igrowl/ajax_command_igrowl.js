(function ($, Drupal) {
  // add a custom ajax command to spawn an iGrowl alert.
  if (typeof Drupal.ajax !== 'undefined') {
    Drupal.ajax.prototype.commands.igrowl = function (ajax, response, status) {
      $.iGrowl(response.options);
    }
  }
})(jQuery, Drupal);