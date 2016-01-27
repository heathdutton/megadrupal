(function($) {
Drupal.behaviors.royal_olive_initialize = {
  attach: function (context, settings) {

    //SuperFish
      $("#main-menu > ul.menu").superfish({
        delay: 100,
        autoArrows: false,
        dropShadows: false,
        animation: {opacity:'show', height:'show'},
        speed: 'fast'
      });
  }
};
})(jQuery);
