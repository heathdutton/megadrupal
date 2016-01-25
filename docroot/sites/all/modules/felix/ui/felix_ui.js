(function ($) {
  Drupal.behaviors.felixUI = {
    attach: function(context) {
      $('.felix-ui-hash', context).each(function() {
        var input = $('input:first', this);
        var config = $('.felix-ui-hash-config', this);
        input.change(function() {
          if ($(this).is(':checked')) {
            config.show();
          }
          else {
            config.hide();
          }
        });
        input.change();
      });
    }
  };
})(jQuery);
