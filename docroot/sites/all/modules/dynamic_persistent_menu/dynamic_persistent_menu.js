// $Id: dynamic_persistent_menu.js,v 1.2.2.6 2010/03/22 13:34:00 yrocq Exp $

/**
 * @file dynamic_persistent_menu.js
 * The Javascript code for Dynamic Persistent Menu
 */

/**
 * Initialize the module's JS functions
 */
(function ($) {

Drupal.behaviors.dynamic_persistent_menu = {
attach: function() {
  
  dpms = Drupal.settings.dynamic_persistent_menu;
  // Initialize the default settings
  dynamic_persistent_menu_load_settings(Drupal.settings.dynamic_persistent_menu);
  for (i in dpms.menus) {
    dpms.boolStatus[i] = 0;
  } 
  // Set mouseover function to show current submenu, hide previous submenu, if any.
  $(".dynamic-persistent-menu-menu-item").mouseover(
    function () {
      dpms.overStatus = 1 ;
      //var parentId=$(this).parent().attr("id");
      dpms.settings_id = dynamic_persistent_menu_menu_settings_index($(this));
      if(dpms.boolStatus[dpms.settings_id] == 0) {
        dpms.overMenu[dpms.settings_id] = 'dynamic-persistent-menu-menu' + dpms.menus[dpms.settings_id]['over'];
        dpms.boolStatus[dpms.settings_id] = 1;
      }
      // If there was a previous menu which received a mouseover, hide the submenu.
      if (dpms.overMenu[dpms.settings_id]) {
        subMenu = dynamic_persistent_menu_get_sub_menu(dpms.overMenu[dpms.settings_id]);
        $('#' + subMenu).hide();
        $('#' + dpms.overMenu[dpms.settings_id]).removeClass('dynamic-persistent-menu-children-active');
      }
      // Show the submenu of the current menu.
      dpms.overMenu[dpms.settings_id] = this.id;
      subMenu = dynamic_persistent_menu_get_sub_menu(dpms.overMenu[dpms.settings_id]);
      $('#' + subMenu).show();
      $('#' + this.id).addClass('dynamic-persistent-menu-children-active');
    }
  ).mouseout(
    function() {
      // Set timeout for the hide function
      dynamic_persistent_menu_set_timeout(dpms.settings_id);
    }
  );
      
  // Set mouseover function to keep overStatus as long as mouse is over the current menu.
  $(".dynamic-persistent-menu-sub-menu").mouseover(
    function() {
      if (dynamic_persistent_menu_get_sub_menu(dpms.overMenu[dpms.settings_id]) == this.id) {
        dpms.overStatus = 1;
      }
    }
  ).mouseout(
    function() {
      dynamic_persistent_menu_set_timeout(dpms.settings_id);
    }
  );
  $(".dynamic-persistent-menu-sub-menu-item a").click(function() {
    dpms.clicked = 1;
  });
  $(".dynamic-persistent-menu-menu-item a").click(function() {
    dpms.clicked = 1;
  });
}
};
})(jQuery);

/**
 * Get the id of the submenu of a menu item, given its id.
 */
function dynamic_persistent_menu_get_sub_menu(menu_id) {
  if(menu_id != null) {
    return menu_id.replace('dynamic-persistent-menu-menu','dynamic-persistent-menu-sub-menu');
  }
}

/**
 * Get the index of the settings array for a given menu.
 */
function dynamic_persistent_menu_menu_settings_index(menu_item) {
  id = menu_item.parent().attr('id').replace('dynamic-persistent-menu-', '');
  return Number(id);
}

/**
 * Reset all submenus and the corresponding statuses.
 */
function dynamic_persistent_menu_reset() {
  dpms = Drupal.settings.dynamic_persistent_menu;
  // Hide menu's only if the mouse is not over one of the submenus
  // and if we are waiting to process a click on one of the links
  if (!dpms.overStatus && !dpms.clicked) {
    jQuery('#' + dynamic_persistent_menu_get_sub_menu(dpms.overMenu[dpms.settings_id])).hide();
    jQuery('#' + dpms.overMenu[dpms.settings_id]).removeClass('dynamic-persistent-menu-children-active');
    dpms.overMenu[dpms.settings_id] = 'dynamic-persistent-menu-menu' + dpms.menus[dpms.settings_id]['default'];
    jQuery('#' + dynamic_persistent_menu_get_sub_menu(dpms.overMenu[dpms.settings_id])).show();
  }
}

/**
 * Call timeout function.
 */
function dynamic_persistent_menu_set_timeout(settings_id) {
  dpms = Drupal.settings.dynamic_persistent_menu;
  subMenuTimeout = dpms.menus[settings_id]['timeout'];
  dpms.overStatus = 0 ;
  setTimeout('dynamic_persistent_menu_reset()', subMenuTimeout);
}
 
/**
 * Load global variables in the module context into the settings array
 * without polluting the javascript global namespace
 */
function dynamic_persistent_menu_load_settings(settings) {
  settings.overMenu = new Array();
  settings.settings_id = 0;
  settings.overStatus = 1;
  settings.boolStatus = new Array();
  settings.clicked = 0;
}


