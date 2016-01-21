// $Id$

/**
 * @file
 * Used for this module
 * 
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

// $Id: 
if (Drupal.jsEnabled) {
  $(document).ready(function () {
  
  setTimeout("array_callback()", 2000);
  });
}  


function array_callback() {
  
  var images = document.getElementsByName('monitor_image') ;
  var timeoutPeriod = -1 ;

  for ( var i = 0 ; i < images.length ; i++ ) {

    var img   = images[i] ;
    var src   = img.src ;
    var index = src.search(/TmpParam/ ) ;
    var res   = src.split(';TmpParam=') ; 
    var cnt   = ( res[1] * 1 ) + 1 ;

    if ( cnt == 32768 ) 
      cnt = 0 ;
      
    img.src = res[0] + ";TmpParam=" + cnt ;
    
    
    var beginArr = res[0].split(';begin=-') ; 
    var begin = beginArr[1] ;
    timeoutPeriod = begin/600 ;

    if ( timeoutPeriod > 5 )
      timeoutPeriod = 5 ;
  }
  
  if ( timeoutPeriod > 0 )
    setTimeout("array_callback()", timeoutPeriod * 1000);
}