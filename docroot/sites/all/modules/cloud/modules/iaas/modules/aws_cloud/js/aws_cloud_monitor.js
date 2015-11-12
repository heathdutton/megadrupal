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
  
  setTimeout("callback()", 1000);
  });
}  


function callback() {

  var img   = document.getElementById('virt_cpu_total') ;
  var src   = img.src ;
  var index = src.search(/TmpParam/) ;
  
  var res   = src.split(';TmpParam=') ; 
  var cnt   = ( res[1] * 1 ) + 1 ;

  if ( cnt == 32768 ) 
    cnt = 0 ;
    
  img.src = res[0] + ";TmpParam="+ cnt ;
  
  /*img = document.getElementById('if_packets') ;
  res = img.src.split(';TmpParam=') ;
  img.src = res[0] + ";TmpParam="+ cnt ;
  
  img = document.getElementById('if_octets') ;
  res = img.src.split(';TmpParam=') ;
  img.src = res[0] + ";TmpParam="+ cnt ;
  
  img = document.getElementById('if_errors') ;
  res = img.src.split(';TmpParam=') ;
  img.src = res[0] + ";TmpParam="+ cnt ;
  
  img = document.getElementById('if_dropped') ;
  res = img.src.split(';TmpParam=') ;
  img.src = res[0] + ";TmpParam="+ cnt ;
  
  img = document.getElementById('disk_ops') ;
  res = img.src.split(';TmpParam=') ;
  img.src = res[0] + ";TmpParam="+ cnt ;
  
  img = document.getElementById('disk_octets') ;
  res = img.src.split(';TmpParam=') ;
  img.src = res[0] + ";TmpParam="+ cnt ;
  */
  
  var beginArr = res[0].split(';begin=-') ; 
  var begin = beginArr[1] ;
  var timeoutPeriod = begin/600 ;
  if ( timeoutPeriod > 5 )
    timeoutPeriod = 5 ;

  setTimeout("callback()", timeoutPeriod * 1000);
}