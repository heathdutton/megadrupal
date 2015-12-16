(function ($) {
  Drupal.behaviors.roleHide = {
    attach: function (context, settings) {
      $('#permissions .checkbox:empty').hide();
    }
  };
})(jQuery);
