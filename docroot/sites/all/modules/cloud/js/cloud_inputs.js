// $Id$

/**
 * @file
 * Used for this module
 * 
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */


function switchOptions(paramID)
{
  var inputType = document.getElementById(paramID + "_inputType").value;

  if (document.getElementById(paramID + "_inputEnv"))
    document.getElementById(paramID   + "_inputEnv").style.display = "none";

  if (inputType == "text") {

    //alert('here');
    document.getElementById(paramID + "_inputText").style.display       = "";
    document.getElementById(paramID + "_inputKey").style.display        = "none";
    document.getElementById(paramID + "_instance_select").style.display = "none";
  }
  elseif (inputType == 'key') {
    //alert(paramID+"_inputKey");
    document.getElementById(paramID + "_inputKey").style.display        = "";
    document.getElementById(paramID + "_inputText").style.display       = "none";
    document.getElementById(paramID + "_instance_select").style.display = "none";
  }  
  else {

    //alert('here');
    if (document.getElementById(paramID + "_inputEnv"))
    {
      document.getElementById(paramID + "_inputEnv").style.display        = "";    
      document.getElementById(paramID + "_instance_select").style.display = "";    
    }
    if (document.getElementById(paramID + "_inputText"))
      document.getElementById(paramID   + "_inputText").style.display = "none";
    if (document.getElementById(paramID + "_inputKey"))
      document.getElementById(paramID   + "_inputKey" ).style.display = "none";
  }
}