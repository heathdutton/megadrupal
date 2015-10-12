(function ($) {
  Drupal.behaviors.conditional_text_fieldset = {
    attach: function (context) {
      $('div.condition-wrapper form:not(.processed)', context)
        .addClass('processed')
        .submit(function (event) {
          event.preventDefault();
        });
    },
    detach: function (context) {}
  };
})(jQuery);
