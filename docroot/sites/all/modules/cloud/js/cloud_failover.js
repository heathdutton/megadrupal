// $Id$

/**
 * @file
 * Used for this module
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 * 
 */

function switchOptions() {

  var fAction = document.getElementById("fAction").value;

  if (fAction == 1) {
    document.getElementById("fScript").style.display = "";
  }
  else{
    document.getElementById("fScript").style.display = "none";
  }
}