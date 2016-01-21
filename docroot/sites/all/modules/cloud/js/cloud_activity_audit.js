// $Id$

/**
 * @file
 * Used for this module
 * 
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

function switchOptions()
{
  var filterOption = document.getElementById("filterOption").value;

  if (filterOption=="location") {
    document.getElementById("moduleFilter").style.display="";
    document.getElementById("otherFilter").style.display="none";
  }
  else{
    document.getElementById("otherFilter").style.display="";
    document.getElementById("moduleFilter").style.display="none";
  }
}