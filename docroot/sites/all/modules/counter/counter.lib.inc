<?php

function counter_get_browser() {
  $user_agent      = $_SERVER['HTTP_USER_AGENT'];
  $browser_name    = 'Unknown';
  $browser_version = '';
  $platform        = 'Unknown';
  $ub              = 'Unknown';
  
  //First get the platform?
  if (preg_match('/linux/i', $user_agent)) {
    $platform = 'Linux';
  }
  elseif (preg_match('/macintosh|mac os x/i', $user_agent)) {
    $platform = 'Mac';
  }
  elseif (preg_match('/windows|win32/i', $user_agent)) {
    $platform = 'Windows';
  }
   
  // Next get the name of the useragent yes seperately and for good reason
  if(preg_match('/MSIE/i',$user_agent) && !preg_match('/Opera/i',$user_agent))
  {
    $browser_name = 'Internet Explorer';
    $ub = "IE";
  }
  elseif(preg_match('/Firefox/i',$user_agent))
  {
    $browser_name = 'Mozilla Firefox';
    $ub = "Firefox";
  }
  elseif(preg_match('/Chrome/i',$user_agent))
  {
    $browser_name = 'Google Chrome';
    $ub = "Chrome";
  }
  elseif(preg_match('/Safari/i',$user_agent))
  {
    $browser_name = 'Apple Safari';
    $ub = "Safari";
  }
  elseif(preg_match('/Opera/i',$user_agent))
  {
    $browser_name = 'Opera';
    $ub = "Opera";
  }
  elseif(preg_match('/Netscape/i',$user_agent))
  {
    $browser_name = 'Netscape';
    $ub = "Netscape";
  }
   
  // finally get the correct browser_version number
  $known = array('browser_version', $ub, 'other');
  $pattern = '#(?<browser>' . join('|', $known) .
  ')[/ ]+(?<browser_version>[0-9.|a-zA-Z.]*)#';
  if (!preg_match_all($pattern, $user_agent, $matches)) {
    // we have no matching number just continue
  }
   
  // see how many we have
  $i = count($matches['browser']);
  if ($i != 1) {
    //we will have two since we are not using 'other' argument yet
    //see if browser_version is before or after the name
    if (strripos($user_agent,"browser_version") < strripos($user_agent,$ub)){
      $browser_version= $matches['browser_version'][0];
    }
    else {
      $browser_version= $matches['browser_version'][1];
    }
  }
  else {
    $browser_version= $matches['browser_version'][0];
  }
   
  // check if we have a number
  if ($browser_version==null || $browser_version=="") {$browser_version="?";}
   
  return array(
    'browser_name'    => $browser_name,
    'browser_version' => $browser_version,
    'platform'        => $platform,
    'pattern'         => $pattern,
  );
}
