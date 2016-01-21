(function ($) {

// Behavior for cookie preferences block.
Drupal.behaviors.cookiePreferencesBlock = {
  attach: function(context) {
    $('#block-ro-cookie-opt-in-ro-cookie-opt-in-preferences').once('ro-cookie', function() {
      $(this).find('.ck-change').click(function(e) {
        $.rocookiebar.build('change');
        e.preventDefault();
      });
      $(this).show();
    });
  }
};

}(jQuery));
