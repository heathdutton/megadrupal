
jQuery(document).ready(function($){
  $('#edit-qp-selector').keyup(function() {
    querypath_filter_rule_preview();
  });
  $('#edit-qp-action').change(function() {
    querypath_filter_rule_preview();
  });
  $('#edit-qp-value').keyup(function() {
    querypath_filter_rule_preview();
  });
});


function querypath_filter_rule_preview() {
  var qp_selector = jQuery('#edit-qp-selector').val();
  var qp_action = jQuery('#edit-qp-action').val();
  var qp_value = jQuery('#edit-qp-value').val();

  var result = '';

  result += "$('" + qp_selector + "')";
  if (qp_action != '') {
    result += "." + qp_action + "(";
    if (qp_value != '') {
      result += "'" + qp_value + "'";
    }
    result += ")";
  }
  result += ';';

  jQuery('#querypath-filter-preview span').html(htmlEncode(result));
}

function htmlEncode(value){
  if (value) {
    return jQuery('<div />').text(value).html();
  } else {
    return '';
  }
}

function htmlDecode(value) {
  if (value) {
    return $('<div />').html(value).text();
  } else {
    return '';
  }
}
