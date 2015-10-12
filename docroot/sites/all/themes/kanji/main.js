(function ($) {
  Drupal.behaviors.kanji = {
    attach: function (context, settings) {
      $('#primaryMenu').prepend('<div id="menu-icon">Menu</div>');

      $("#menu-icon").click(function() {
        $("#primaryMenu").toggleClass("active");
      });
    }
  };  
}(jQuery));
