// $Id$

/**
 * @file
 * Used for this module
 * 
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */



function selectAllTemplateOptions() { // Select all the necessary options on Form submit

    selectAllOptions('boot_selected');
    selectAllOptions('operational_selected');
    selectAllOptions('termination_selected');
    selectAllOptions('alert_id_selected');
    selectAllOptions('failover_scenario_id_selected'); 
}

function moveElement(first,  second) {

  var availState    = (first)?first:document.getElementById(first);
  var selectedState = (second)?second:document.getElementById(second);
  moveSelectedOptions ( availState ,  selectedState ,  true ,  "" );
  return false;
}

function selectAllOptions(selStr) {
  
  var selObj = document.getElementById(selStr);
  if (selObj == null ) return ;
  for (var i=0; i<selObj.options.length; i++) {
    selObj.options[i].selected = true;
  }
}

function moveElementUp(obj) {

  var obj = (obj)?obj:document.getElementById(obj);
  //var selectedState = (second)?second:document.getElementById(second);
  moveOptionUp ( obj );
  return false;
}

function moveElementDown(obj) {
  
  var obj = (obj)?obj:document.getElementById(obj);
  //var selectedState = (second)?second:document.getElementById(second);
  moveOptionDown ( obj );
  return false;
}