(function ($) {
  Drupal.behaviors.responsive_menu = {
    attach: function (context, settings) {

      // Attach the swipe hammer events to the body.
      $('body', context).once('responsive-menu', function() {
        if (Drupal.settings.responsive_menu.breakpoint !== "undefined") {
          var mc = new Hammer($('body').get(0));
          mc.get('swipe').set({
            velocity: 0.3,
            threshold: 5
          });
          mc.on("swipeleft swiperight", function(e) {
            // Only do something if we're below our breakpoint.
            if ($(document).width() <= Drupal.settings.responsive_menu.breakpoint) {
              var menuToggle = $('input#main-nav-check');
              // Check if the swipe was to the right and the menu is currently
              // closed.
              if (e.type == 'swiperight' && menuToggle.is(':checked') == false) {
                // Open the menu.
                menuToggle.click();
              }
              // Check if the swipe was to the left and the menu is currently
              // open.
              if (e.type == 'swipeleft' && menuToggle.is(':checked')) {
                // Close the menu.
                menuToggle.click();
              }
            }
          });
        }

      });
    }
  };

})(jQuery);
