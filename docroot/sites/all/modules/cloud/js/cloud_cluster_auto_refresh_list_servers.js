// $Id$

/**
 * @file
 * Used for this module
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */


var refresh_in_sec = 10 ;
 
(function ($) {
  $(document).ready(function() {
    setInterval(autoupdate_cluster_server_list, refresh_in_sec * 1000);
 });

  

function autoupdate_cluster_server_list() {

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
    
    var cluster_id = $('input[name=cluster_id]').val();
    
    var $url = $('#cluster_servers_list_table').attr('autoupdate_url');
    $.get($url , { 'order'          : order, 
                   'sort'           : sort,
                   'cluster_id'     : cluster_id,
                 } 
              , 
          update_cluster_servers_list );
     

 }
 
 
function update_cluster_servers_list(response) {

  var result = jQuery.parseJSON(response);
  
  var head_present   = $('#cluster_servers_list_table').children('thead').length ;
  var body_present   = $('#cluster_servers_list_table').children('tbody').length ;
  
  if (result.html != 'NULL' ) { // Returned empty string. skip the output
  
    if (head_present) { // Head already present
        
            if (body_present) {
                
                $('#cluster_servers_list_table tbody').replaceWith(result.html);    
            }
            else { // Body not present
            
                $('#cluster_servers_list_table').append(result.html);    
            }
    } 
    else {
    
        var head_str = $('#cluster_servers_list_table').html() ;
        head_str = head_str.replace( '<tbody>' , '<thead>' ) ; 
        head_str = head_str.replace( '</tbody>' , '</thead>' ) ;
        
        $('#cluster_servers_list_table tbody').replaceWith(head_str); 
        $('#cluster_servers_list_table tbody').append(result.html); 
    }    
    
    Drupal.behaviors.showActionButtons.attach() ;
  }
  else {
  
    if (head_present && body_present) {
    
        $('#cluster_servers_list_table tbody').remove() ;    
    }
  }
}

})(jQuery);

