<?php

/**
 * by requiring this file, we get our calling page called again, but with
 * a cookie set containing our client screen height and width.
 * Eg:
 * <?php
 *   require('cookieFactory.php'); 
 *   list($height, $width) = explode('x', $_COOKIE[$about_cookie_name]);
 * ?>
 * 
 */


/**
 * check/set the cookie for screen res
 */
$about_cookie_name = '_about_hxw';
if (! isset($_COOKIE[$about_cookie_name])) {
  $location = $_SERVER['REQUEST_URI'];
  print "<html><head></head><body><script language=\"javascript\">\n  var cName = '$about_cookie_name';\n  var gotoLocation = '$location';\n";

  print <<<ENDJS
  var w = 1000;
  var h = 600;
  if (parseInt(navigator.appVersion) > 3) {
   if (window.innerWidth || navigator.userAgent.indexOf("MSIE 9.0") != -1) {
    w = window.innerWidth;
    h = window.innerHeight;
   }
   else if (navigator.appName.indexOf("Microsoft") != -1) {
    w = document.body.offsetWidth;
    h = document.body.offsetHeight;
   }
  }

  var date = new Date();
  date.setTime(date.getTime() + 3000); // 3 second lifespan
  var expires = '; expires=' + date.toGMTString();
  document.cookie = cName + '=' + h + 'x' + w + expires + '; path=/';
  window.location.href = gotoLocation;

</script></body></html>
ENDJS;

  exit;
} 
