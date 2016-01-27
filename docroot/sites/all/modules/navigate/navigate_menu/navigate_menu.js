
var NavigateMenu = NavigateMenu || {};

Drupal.behaviors.navigate_menu = {
  attach: function () {
  jQuery('.navigate-menu-list').each(function () {
    if (jQuery(this).hasClass('navigate-menu-list-processed')) {
      return;
    }
    jQuery(this).addClass('navigate-menu-list-processed');
    var wid = jQuery(this).parents('.navigate-widget-outer').children('.wid').val();
    jQuery(this).treeview({
      animated: "fast",
      collapsed: true,
      persist: "cookie",
      cookieId: "navigationtree_" + wid
    });
  });
  }
}


/**
 * Save the content and reload it
 */
function navigate_menu_load_menu(wid) {
  var callback = 'navigate_menu_msg';

  jQuery('.navigate-menu-output-' + wid).animate({'opacity': 0.4});

  var query_array = new Array();
  query_array['module'] = 'navigate_menu';
  query_array['action'] = 'load';
  query_array['mid'] = jQuery('.navigate-menu-output-' + wid).find('select').val();
  Navigate.processAjax(wid, query_array, callback);
}


/**
 * Callback for positive result from ajax query
 */
function navigate_menu_msg(msg, wid) {
  jQuery('.navigate-menu-output-' + wid).html(msg);
  Drupal.attachBehaviors();
  jQuery('.navigate-menu-output-' + wid).animate({'opacity': 1});
}
