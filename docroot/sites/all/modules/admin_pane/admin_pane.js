// $Id: admin_pane.js,v 1.1.2.1 2011/01/28 14:11:50 blixxxa Exp $

(function ($) {

Drupal.admin_pane = Drupal.admin_pane || {};

/**
 * Init administration pane.
 */
Drupal.behaviors.admin_pane = {
  attach: function(context) {
    // Set the inital state of the admin pane.
    $('#admin-pane', context).once('admin-pane', Drupal.admin_pane.init);

    // Toggling admin pane.
    $('#toolbar a.toggle', context).bind('click', function(e) {
      Drupal.admin_pane.toggle();
      return false;
    });

    // Activate tabs.
    $("#menu-tabs").tabs({ cookie: { expires: 30 } });

    // Respond to overlay loading.
    $(document).bind('drupalOverlayLoad', function(e) {
      Drupal.admin_pane.overlay(e);
    });

    // Respons to overlay closing.
    $(document).bind('drupalOverlayClose', function(e) {
      Drupal.admin_pane.overlay(e);
    });

    // Activate jsTree.
    $("#admin-pane .ui-tabs-panel").jstree({
      "core" : {
        "animation" : 0
      },
      "themes" : {
        "theme" : "classic",
        "icons" : false,
        "url" : "/hip/sites/all/modules/admin_pane/jstree/themes/classic/style.css"
      },
      "plugins" : [ "themes", "html_data", "cookies"]
    });
  }
};

/**
 * Respond to overlay opening and closing.
 */
Drupal.admin_pane.overlay = function(e) {
  var $pane = $("#admin-pane > div");
  switch (e.type) {
    case 'drupalOverlayLoad':
      $pane.css('right', 15);
      break;

    case 'drupalOverlayClose':
      $pane.css('right', 0);
      break;
  }
}

/**
 * Retrieve last saved cookie settings and set up the initial admin pane state.
 */
Drupal.admin_pane.init = function() {
  // Retrieve the collapsed status for the toolbar from a stored cookie.
  var collapsed = $.cookie('Drupal.toolbar.collapsed');

  // Expand or collapse the admin pane based on the cookie value.
  if (collapsed == 1) {
    Drupal.admin_pane.collapse();
  }
  else {
    Drupal.admin_pane.expand();
  }
};

/**
 * Collapse the admin pane.
 */
Drupal.admin_pane.collapse = function() {
  $('#admin-pane').addClass('collapsed');
  $('body').removeClass('admin-pane').css('padding-right', 0);
  $('.overlay-element').contents().find('body').css('padding-right', 0);
};

/**
 * Expand the admin pane.
 */
Drupal.admin_pane.expand = function() {
  $('#admin-pane').removeClass('collapsed');
  $('body').addClass('admin-pane').css('padding-right', 300);
  $('.overlay-element').contents().find('body').css('padding-right', 300);
};

/**
 * Toggle the admin pane.
 */
Drupal.admin_pane.toggle = function() {
  if ($('#admin-pane').hasClass('collapsed')) {
    Drupal.admin_pane.expand();
  }
  else {
    Drupal.admin_pane.collapse();
  }
};

})(jQuery);
