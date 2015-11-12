(function($) {

Drupal.behaviors.buttonStyle = {
  attach: function (context, settings) {
    $('span.form-button-wrapper input, li.button a, a.button').mousedown(function () {
      $(this).blur();
      return true;
    });
  }
}

})(jQuery);
