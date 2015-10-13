/**
 * @file
 * A jQuery file to make the menu more mobile on small screens.
 */

(function ($, Drupal, window, document, undefined) {
Drupal.behaviors.my_custom_behavior = {
  attach: function(context, settings) {

    // Begin basic mobile menu support here
    // Set this 481 to the number of pixels you want for when the mobile menu
    // becomes a desktop menu
    if ($(window).width() < 481) {
      //Add your jQuery for mobile menu on small screens here

      // Hide the main menu items for mobile views initially
      $('#main-menu ul').hide();

      // Remove the element-invisible class for the #main-menu title on
      // small screens so we can use the menu title as our trigger
      // Add an "unclicked" class to the trigger (handy for theming)
      $('#main-menu h2').removeClass('element-invisible').addClass('menu-trigger-unclicked');

      // Toggle list on/off when title is clicked
      $('#main-menu h2').click(function() {
        // When clicked, add a "clicked" class to the trigger (handy for theming)
        // and remove the "unclicked" class and vice versa.
        $('#main-menu h2').toggleClass('menu-trigger-unclicked menu-trigger-clicked');
        // Slide the menu in to/out of view when trigger is clicked
        $('#main-menu ul').slideToggle();
        // Set each menu item to 100% so they are no longer set side-by-side
        $('#navigation .links li').css('width','100%');
      });

      // Make the cursor a pointer when hovered
      $('#main-menu h2').hover(function() {
        $(this).css('cursor','pointer');
      });
    }
  }
};

})(jQuery, Drupal, this, this.document);
