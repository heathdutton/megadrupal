
var NavigateSearch = NavigateSearch || {};

Drupal.behaviors.navigate_search = function () {
  jQuery(document).bind('keydown', {combi:'ctrl+shift+s'}, function () { Navigate.show();jQuery('.navigate-search-phrase').focus();});
  if (jQuery('.navigate-key-button').hasClass('key-disabled')) {
    jQuery(document).unbind('keydown', {combi:'ctrl+shift+s'});
  }

  // Highlight all content types when clicked
  jQuery('.content-type-search-all:not(.content-type-search-all-processed)').each(function () {
    jQuery(this).click(function () {
      //alert(jQuery(this).parents('.navigate-widget-settings').html());
      var $settings = jQuery(this).parents('.navigate-widget-settings').find('.navigate-search-content-types');
      $settings.find('.navigate-button').addClass('navigate-button-on');
      var wid = jQuery(this).parents('.navigate-widget-outer').children('.wid').val();
      var query_array = new Array();
      query_array['module'] = 'navigate_search';
      query_array['action'] = 'search_content_type_all';
      Navigate.processAjax(wid, query_array);
    });
  });

  // De-select all content types when clicked
  jQuery('.content-type-search-none:not(.content-type-search-none-processed)').each(function () {
    jQuery(this).click(function () {
      //alert(jQuery(this).parents('.navigate-widget-settings').html());
      var $settings = jQuery(this).parents('.navigate-widget-settings').find('.navigate-search-content-types');
      $settings.find('.navigate-button').removeClass('navigate-button-on');
      //var wid = Navigate.getWid(this);
      var wid = jQuery(this).parents('.navigate-widget-outer').children('.wid').val();
      var query_array = new Array();
      query_array['module'] = 'navigate_search';
      query_array['action'] = 'search_content_type_all';
      query_array['none'] = 1;
      Navigate.processAjax(wid, query_array);
    });
  });


  // Bind keyboard shortcut enable / disable
  jQuery('.navigate-key-button:not(.navigate-key-button-search-processed)').each(function () {
    jQuery(this).addClass('navigate-key-button-search-processed');

    jQuery(this).click(function () {
      if (jQuery(this).hasClass('key-disabled')) {
        jQuery(document).bind('keydown', {combi:'ctrl+shift+s'});
      } else {
        jQuery(document).bind('keydown', {combi:'ctrl+shift+s'});
      }
    });
  });
}

/**
 * Grab search variables and send to be processed
 */
function navigate_search_process(wid) {
  var callback = 'navigate_search_process_msg';

  var query_array = new Array();
  query_array['module'] = 'navigate_search';
  query_array['action'] = 'search';
  query_array['phrase'] = jQuery('#navigate-search-phrase_' + wid).val();
  jQuery('.navigate-search-results-' + wid).slideUp('fast', function () {

    // Run through Navigate.processAjax so the API can handle the query string
    Navigate.processAjax(wid, query_array, callback);
  });
}


/**
 * Callback for positive result from ajax query
 */
function navigate_search_process_msg(msg, wid) {
  jQuery('.navigate-search-results-' + wid).html(msg);
  jQuery('.navigate-search-results-' + wid).slideDown('fast', function() {
    // SlideUp and slideDown add some styles we need to get rid of before saving
    jQuery('.navigate-search-results-' + wid).removeAttr('style');
    Drupal.attachBehaviors();
  });
}
