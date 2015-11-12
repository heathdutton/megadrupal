// $Id$

/**
 * @file
 * Used for this module
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */


var init_refresh_in_sec = 10; 
var refresh_in_sec = init_refresh_in_sec * init_refresh_in_sec ;


(function ($) {
  $(document).ready(function() {
    setTimeout(autoupdate_all_instances_list, ( refresh_in_sec / init_refresh_in_sec ) * 1000);
 });

function autoupdate_all_instances_list() {
  refresh_in_sec++ ;
  setTimeout(autoupdate_all_instances_list, ( refresh_in_sec / init_refresh_in_sec ) * 1000);
  
  var img_tag = $("img[alt$='sort descending']");
  if ( !img_tag.length) {
      img_tag = $("img[alt$='sort ascending']");
  }

  var sort_col_val = img_tag.parent().text() ;
  var sort_img_src = img_tag.attr('src') ;
  var order        = sort_col_val.substr( 0 , sort_col_val.length / 2 ) ;
  var sort         = 'desc' ;
  if ( sort_img_src != ''  && sort_img_src.indexOf('desc') != -1 ) {
      sort = 'asc' ;
  }

  var tgt_url = img_tag.parent().attr('href');
  var opr_index = tgt_url.indexOf('operation=')  ; 
  var operation = '' ;
  var filter    = '' ;
  
  if ( opr_index != - 1) {
    operation = tgt_url.charAt( opr_index +  'operation='.length ) ;
    // Operation exists check filter value
    var fltr_start_index = tgt_url.indexOf('filter=')  ; 
    var fltr_end_index   = tgt_url.indexOf('&' , fltr_start_index )  ; 
    
    if ( fltr_start_index != -1
    &&   fltr_end_index   != -1 ) {
        filter = tgt_url.substring( fltr_start_index + 'filter='.length , fltr_end_index ) ;
        filter = unescape( filter) ; 
    } 
  }    
  
  $url = $('#all_instances_list_table').attr('autoupdate_url') ;
  $.get($url , { 'order'          : order, 
                 'sort'           : sort,
                'operation'      : operation,
                 'filter'         : filter, 
               } 
            , 
        update_all_instances_list );
          
 }
 
 
function update_all_instances_list(response) {

  var result = jQuery.parseJSON(response);
  var head_present = $('#all_instances_list_table').children('thead').length ;
  var body_present = $('#all_instances_list_table').children('tbody').length ;
  if (result.html != 'NULL' ) { // Returned empty string. skip the output
  
    if (head_present) { // Head already present
        
      if (body_present) {
        $('#all_instances_list_table tbody').replaceWith(result.html);    
      }
      else { // Body not present
      
        $('#all_instances_list_table').append(result.html);    
      }
    } 
    else {
    
      var head_str = $('#all_instances_list_table').html() ;
      head_str = head_str.replace( '<tbody>'  , '<thead>'  ) ; 
      head_str = head_str.replace( '</tbody>' , '</thead>' ) ;
      
      $('#all_instances_list_table tbody').replaceWith(head_str); 
      $('#all_instances_list_table tbody').append(result.html); 
    }    
    
    Drupal.behaviors.showActionButtons.attach() ;
  }
  else {
  
    if (head_present && body_present) {
    
      $('#all_instances_list_table tbody').remove() ;    
    }
  }
}

})(jQuery);