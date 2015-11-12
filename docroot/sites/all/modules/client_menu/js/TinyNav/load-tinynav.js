(function ($) {

/**
 * Add dropdown class to list elements with child menus.
 */
Drupal.behaviors.client_menu_responsive = {
  attach: function (context, settings) {
    $("#client-menu ul:first").tinyNav({
      active: 'active', // String: Set the "active" class
      //header: ' ', // String: Specify text for "header" and show header instead of the active item
      indent: '- ', // String: Specify text for indenting sub-items
      //label: '' // String: Sets the <label> text for the <select> (if not set, no label will be added)
    });
    //$("#client-menu").after($(".home-icon"));
    $( ".home-icon" ).once('home-icon', function(){
      $(this).clone().appendTo( "#client_menu_wrapper" ).addClass( "duplicate" );
    });
  }
};

})(jQuery);
