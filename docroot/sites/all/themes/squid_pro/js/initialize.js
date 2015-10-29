(function($) {
Drupal.behaviors.squid_pro_initialize = {
  attach: function (context, settings) {

      // superFish
       $('#main-menu ul.menu').supersubs({
       minWidth:    16, // minimum width of sub-menus in em units
       maxWidth:    40, // maximum width of sub-menus in em units
       extraWidth:  1 // extra width can ensure lines don't sometimes turn over
     })
    .superfish(); // call supersubs first, then superfish
  }
};
})(jQuery);
