<?php 
////////////////////////////////////////////////////
// GAT_Parse - PHP Parser Class for Custom Cookies
//
// Version 0.1
// Date: 15 December 2013
//
// Author: Brian Stevenson, yadaDROP LLC - http://yadadrop.com
//
////////////////////////////////////////////////////

class GAT_Parse
{

  var $first_referrer;    	// First referrer to site
  var $first_landing;  		// Landing page of first referrer
  var $cur_referrer;    	// Current referrer to site
  var $cur_landing;   		// Landing page of current referrer
  var $recent_referrer;     // Most Recent referrer of page load
  var $recent_landing;      // Most Recent landing page of page load (current page)
  
  function __construct($cookie) {
     if (isset($cookie["gat_first"])) {
       $this->gatFirst = json_decode($cookie["gat_first"]);
     }
     if (isset($cookie["gat_cur"])) {
       $this->gatCur = json_decode($cookie["gat_cur"]);
     }
     if (isset($cookie["gat_recent"])) {
       $this->gatRecent = json_decode($cookie["gat_recent"]);
     }
       $this->ParseCookies();
  }

  function ParseCookies(){
    // Parse gat_first cookie
    if (isset($this->gatFirst)) {
      $this->first_referrer = $this->gatFirst->referrer;
      $this->first_landing = $this->gatFirst->landing;
    }
    
    // Parse gat_cur cookie
    if (isset($this->gatCur)) {
      $this->cur_referrer = $this->gatCur->referrer;
      $this->cur_landing = $this->gatCur->landing;
    }

    // Parse gat_recent cookie
    if (isset($this->gatRecent)) {
      $this->recent_referrer = $this->gatRecent->referrer;
      $this->recent_landing = $this->gatRecent->landing;
    }

 // End ParseCookies
 }  

// End GAT_Parse
}
