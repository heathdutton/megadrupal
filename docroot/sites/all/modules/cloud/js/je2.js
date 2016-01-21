// $Id$

/**
 * @file
 * Used for this module
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 * 
 */


$(document).ready(function() {
var  urlStr = String(window.location);
//Convert.ToString(urlStr);
var urlObj = urlStr.split("?"); 
var hostName = urlObj[0];

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
  var url = hostName+'/?q=design/je2/jobdefinitions/jobdetails&id='+jdId;
  //alert(url);
}
else{
  
  var strObj = qStr.split("&", 4);
  var jdIdObj = strObj[1].split("=");
  var jdId = jdIdObj[1];
  var jobIdObj = strObj[2].split("=");
  var jobId = jobIdObj[1];
  var completeObj = strObj[3].split("=");
  var completed = completeObj[1];
  var url = hostName+'/?q=design/je2/jobdefinitions/job/taskdetails&jobdefinition_id='+jdId+'&id='+jobId;
}

// Call back function for AJAX call
var handleProgress = function(responseText) {
    var myJSONObject = responseText;
  myJSONObject = JSON.parse(myJSONObject);
  if (qStr.indexOf("jobs")!=-1) {
    jobMapProgress = myJSONObject.jobMapProgress;
    //alert(jobMapProgress);
    jobReduceProgress = myJSONObject.jobReduceProgress;
    complete = myJSONObject.complete;
    created = myJSONObject.created;
    //alert(created);
    jobMapProgressList = jobMapProgress.toString();
    jobReduceProgressList = jobReduceProgress.toString();
    completeList = complete.toString();
    createdList = created.toString();
    jobMapProgressList = jobMapProgressList.split(", "); 
    jobReduceProgressList = jobReduceProgressList.split(", "); 
    completeList = completeList.split(", "); 
    createdList = createdList.split(", ");

    //var progressObj = responseText.split("|"); 
    //var mapProgress = progressObj[0].split(", ");  
    for (i=0;i<jobMapProgressList.length;i++)
    {
      $("#mapProgress_"+i).html(jobMapProgressList[i]);
      $("#reduceProgress_"+i).html(jobReduceProgressList[i]);
      $("#complete_"+i).html(completeList[i]);
      $("#created_"+i).html(createdList[i]);
    }
    /*
    var reduceProgress = progressObj[1].split(", ");  
    for (i=0;i<reduceProgress.length;i++)
    {
      $("#reduceProgress_"+i).html(reduceProgress[i]);
    }*/    
  }
  else{
    
    progress = myJSONObject.progress;
    status = myJSONObject.status;
    startTime = myJSONObject.startTime;
    finishTime = myJSONObject.finishTime;
    progressList = progress.toString();
    statusList = status.toString();
    startTimeList = startTime.toString();
    finishTimeList = finishTime.toString();
    progressList = progressList.split(", "); 
    statusList = statusList.split(", "); 
    startTimeList = startTimeList.split(", "); 
    finishTimeList = finishTimeList.split(", "); 
    
    for (i=0;i<progressList.length;i++)
    {
      $("#progress_"+i).html(progressList[i]);
      $("#status_"+i).html(statusList[i]);
      $("#startTime_"+i).html(startTimeList[i]);
      $("#finishTime_"+i).html(finishTimeList[i]);
    }
  }
}

var m=0;
function timer() {
  //alert('here');
  if (completed=='No')
  {
    //alert('here');
    $.get( url,  null,  handleProgress);

    var funcInterval = window.setInterval(timer, 15000);
    if (m>3) {
      clearInterval(funcInterval);
    }
    m++;  
  }
}

timer();

// preventing entire page from reloading
return false;
});


