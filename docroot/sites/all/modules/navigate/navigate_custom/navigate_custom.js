
/**
 * Save the content and reload it
 */
function navigate_custom_save(wid) {
  var callback = 'navigate_custom_msg';
  
  jQuery('.navigate-custom-output-' + wid).animate({'opacity': 0.4});
  
  var query_array = new Array();
  query_array['module'] = 'navigate_custom';
  query_array['action'] = 'save';
  query_array['content'] = jQuery('#navigate-content-input_' + wid).val();
  Navigate.processAjax(wid, query_array, callback);
}


/**
 * Callback for positive result from ajax query
 */
function navigate_custom_msg(msg, wid) {
  jQuery('.navigate-custom-output-' + wid).html(msg);
  jQuery('.navigate-custom-output-' + wid).animate({'opacity': 1});
}
