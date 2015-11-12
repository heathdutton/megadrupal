// $Id$

/**
 * @file
 * Used for this module
 * 
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

//$Id$



$(document).ready(function() {
var  urlStr = String(window.location);
//Convert.ToString(urlStr);
var urlObj = urlStr.split("?"); 
var hostName = urlObj[0];
var stopAjax = 'false';
var qStr = window.location.search.substring(1);
var completed = 'No';
//AJAX call
if (qStr.indexOf("jobs")!=-1) {
  //alert('here');
  var strObj = qStr.split("&", 3);
  var jdIdObj = strObj[1].split("=");
  var jdId = jdIdObj[1];
  //var hostName = $("#host").val();
  //hostName = (hostName)?hostName:'localhost/drupal';
  //alert(hostName);
  var url = hostName+'/?q=design/je2/r_jobdefinitions/jobdetails&id='+jdId;
  //alert(url);
}


// Call back function for AJAX call
var handleRProgress = function(responseText) {
    var myJSONObject = responseText;
  myJSONObject = JSON.parse(myJSONObject);
  //alert(myJSONObject.start);
  if (qStr.indexOf("jobs")!=-1) {
    complete = myJSONObject.complete;
    start = myJSONObject.start;
    end = myJSONObject.end;
    stopAjax = myJSONObject.stopAjax;

    completeList = complete.toString();
    startList = start.toString();
    endList = end.toString();
    //alert(endList);
    completeList = completeList.split(", "); 
    startList = startList.split(", ");
    endList = endList.split(", ");
    
    for (i=0;i<completeList.length;i++)
    {
      $("#complete_"+i).html(completeList[i]);
      $("#start_"+i).html(startList[i]);
      $("#end_"+i).html(endList[i]);
    }
  }
  
}

var m=0;
var funcInterval;
function rTimer() {

  if (completed == 'No') {

    $.get( url,  null,  handleRProgress);
    funcInterval = window.setInterval(rTimer, 15000);
    //alert(funcInterval);
    if (stopAjax == 'true') {
      clearInterval(funcInterval);
    }
    m++;  
  }
}

rTimer();

// preventing entire page from reloading
return false;
});


