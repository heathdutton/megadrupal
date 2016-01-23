<?php

/**
 * randomly select an image from the current directory and return a CSS style to reference it.
 */

$file_types = array( 'gif', 'jpg', 'jpeg', 'png', 'swf') ;

$regex = '/\.(' . implode('|',$file_types) . ')$/i' ;
$files = array() ;

$directory = opendir(".");
while ( FALSE !== ($file = readdir( $directory )) ) {
  if ( preg_match( $regex, $file ) ) {
    $files[] = $file ;
  }
}

if ( !empty( $files ) ) {

  $which   = rand(0,sizeof($files)-1) ;
    
  header( "Content-type: text/css" ) ;
  header( "Expires: Wed, 29 Jan 1975 04:15:00 GMT" );
  header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
  header( "Cache-Control: no-cache, must-revalidate" );
  header( "Pragma: no-cache" );

  print ".himg #headimg {background: #fff url(" . $files[$which] . ") no-repeat 0 100%;}";

}
