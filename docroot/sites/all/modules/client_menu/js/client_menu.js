(function ($) {

/**
 * Add dropdown class to list-items containing submenus.
 */
Drupal.behaviors.client_menu_dropdown = {
  attach: function (context, settings) {
		$( "#client_menu_wrapper li" ).has( "ul" ).addClass( "dropdown" );
  }
};

/**
 * Add active class to list-items containing active links.
 */
Drupal.behaviors.client_menu_active_parent = {
  attach: function (context, settings) {
		$( "#client_menu_wrapper li" ).has( "li a.active" ).addClass( "active-parent" );
  }
};

/**
 * Add root class to the first parent list-item containing active link.
 */
Drupal.behaviors.client_menu_active_parent_root = {
  attach: function (context, settings) {
		$( "#client_menu_wrapper li" ).has( "li a.active" ).first().addClass( "root" );
  }
};

})(jQuery);
