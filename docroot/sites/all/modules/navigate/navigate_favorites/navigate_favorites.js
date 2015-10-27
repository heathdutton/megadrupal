
var NavigateFavorites = NavigateFavorites || {};

Drupal.behaviors.navigate_favorites = function () {
  
  // Add sortable
  jQuery(".navigate-favorites-list-sortable").each(function () {
    if (jQuery(this).hasClass('favorites-sortable-processed')) {
      return;
    }
    jQuery(this).addClass('favorites-sortable-processed');
    jQuery(this).sortable({
      cursor: "move",
      revert:true,
      distance: 4,
      helper:"clone",
      stop:function(e, ui) {
        var wid = jQuery(this).parents('.navigate-widget-outer').children('.wid').val();
        var query_array = new Array();
        query_array['module'] = 'navigate_favorites';
        query_array['action'] = 'sort';
        query_array[''] = jQuery(this).sortable('serialize');
        Navigate.processAjax(wid, query_array);
      }
    });
  });
  
  // Bind delete links
  jQuery(".navigate-favorites-delete").each(function () {
    if (jQuery(this).hasClass('favorites-delete-processed')) {
      return;
    }
    jQuery(this).addClass('favorites-delete-processed');
    jQuery(this).click(function() {
      var callback = 'navigate_favorites_msg';
      var wid = jQuery(this).parents('.navigate-widget-outer').children('.wid').val();
      jQuery('.navigate-favorites-list-' + wid).animate({'opacity': 0.4});
      var query_array = new Array();
      query_array['module'] = 'navigate_favorites';
      query_array['action'] = 'remove';
      query_array['fav_id'] = jQuery(this).children('.navigate-favorites-id').val();
      Navigate.processAjax(wid, query_array, callback);
    });
  });
  
  
  // Show delete links when hovering
  jQuery(".navigate-favorites-link-outer").each(function () {
    if (jQuery(this).hasClass('navigate-favorites-link-outer-processed')) {
      return;
    }
    jQuery(this).addClass('navigate-favorites-link-outer-processed');
    jQuery(this).hover(
      function() {
        jQuery(this).children('.navigate-favorites-delete').show();
      },
      function() {
        jQuery(this).children('.navigate-favorites-delete').hide();
      }
    );
  });
  
  jQuery('.navigate-favorites-export').focus(function () { jQuery(this).select();});
  jQuery('.navigate-favorites-import').focus(function () { jQuery(this).select();});
}

/**
 * Process adding of a favorite
 */
function navigate_favorites_add(wid) {
  var callback = 'navigate_favorites_msg';
  
  jQuery('.navigate-favorites-list-' + wid).animate({'opacity': 0.4});
  
  var query_array = new Array();
  query_array['module'] = 'navigate_favorites';
  query_array['action'] = 'add';
  query_array['name'] = jQuery('#navigate-favorite-name_' + wid).val();
  Navigate.processAjax(wid, query_array, callback);
}


/**
 * Callback for positive result from ajax query
 */
function navigate_favorites_msg(msg, wid) {
  jQuery('.navigate-favorites-list-' + wid).html(msg);
  jQuery('.navigate-favorites-list-' + wid).animate({'opacity': 1}, function() {
    Drupal.attachBehaviors('.navigate-favorites-list-' + wid);
    jQuery('.navigate-favorites-delete').hide();
  });
}

/**
 * Create export content and populate textarea
 */
function navigate_favorites_export(wid) {
  var callback = 'navigate_favorites_export_msg';
  
  jQuery('.navigate-favorite-settings-' + wid).animate({'opacity': 0.4});
  
  var query_array = new Array();
  query_array['module'] = 'navigate_favorites';
  query_array['action'] = 'export';
  query_array['content'] = jQuery('#navigate-favorites-export_' + wid).val();
  Navigate.processAjax(wid, query_array, callback);
}

/**
 * Callback for positive result from ajax query
 */
function navigate_favorites_export_msg(msg, wid) {
  jQuery('#navigate-favorites-export_' + wid).val(msg);
  jQuery('.navigate-favorite-settings-' + wid).animate({'opacity': 1.0});
}

/**
 * Create export content and populate textarea
 */
function navigate_favorites_import(wid) {
  if (jQuery('#navigate-favorites-import_' + wid).val() == '') {
    if (!confirm("You are about to import nothing, which will clear your favorites list. Are you sure you want to do this?")) {
      return;
    }
  }
  var callback = 'navigate_favorites_import_msg';
  
  jQuery('.navigate-favorite-settings-' + wid).animate({'opacity': 0.4});
  jQuery('.navigate-favorites-list-' + wid).animate({'opacity': 0.4});
  
  var query_array = new Array();
  query_array['module'] = 'navigate_favorites';
  query_array['action'] = 'import';
  query_array['content'] = jQuery('#navigate-favorites-import_' + wid).val();
  Navigate.processAjax(wid, query_array, callback);
}

/**
 * Callback for positive result from ajax query
 */
function navigate_favorites_import_msg(msg, wid) {
  jQuery('#navigate-favorites-import_' + wid).val('');
  jQuery('.navigate-favorite-settings-' + wid).animate({'opacity': 1.0});
  
  jQuery('.navigate-favorites-list-' + wid).html(msg);
  jQuery('.navigate-favorites-list-' + wid).animate({'opacity': 1}, function() {
    Drupal.attachBehaviors('.navigate-favorites-list-' + wid);
    jQuery('.navigate-favorites-delete').hide();
  });
}