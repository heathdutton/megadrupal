(function ($) {

Drupal.behaviors.l10nUpdateCollapse = {
  attach: function (context, settings) {
    $('.w3c_validator-report .w3c_validator-wrapper', context).once('w3cvalidatorcollapse', function () {
      var wrapper = $(this);

      // Turn the project title into a clickable link.
      // Add an event to toggle the content visibiltiy.
      var $legend = $('.page-title', wrapper);
      var $link = $('<a href="#"></a>')
        .prepend($legend.contents())
        .appendTo($legend)
        .click(function () {
          Drupal.toggleFieldset(wrapper);
          return false;
        });
    });
  }
};

})(jQuery);
