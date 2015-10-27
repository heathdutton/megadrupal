(function ($) {

/**
 * Add dropdown class to list elements with child menus.
 */
Drupal.behaviors.client_menu_mobile = {
  attach: function (context, settings) {
		$("#client_menu_wrapper.mobile #mobile-menu").mmenu({"offCanvas": {"zposition": "front"}});
  }
};

})(jQueryClientMenu);
