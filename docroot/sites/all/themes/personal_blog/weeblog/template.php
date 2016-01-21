<?php

function weeblog_preprocess_views_view_field(&$vars) {
  if ($vars['field']->field == 'created') {
  	$field_alias = $vars['field']->field_alias; 
  	$time = $vars['row']->$field_alias;
  	$day = date('d');
  	$month = date('M');
  	$year = date('Y');
  	/*
  	$value = $vars['field']->original_value;
  	$parts = explode(" ", $value);
  	$day = $parts[0];
    $month = $parts[1];
    $year = $parts[2];
    */
  	$vars['output'] = "<span class='date-wrapper'>
  	  <span class='date-left'><span class='day'>$day</span></span>
  	  <span class='date-right'><span class='month'>$month</span><span class='year'>$year</span></span>
  	</span>";
  }
  else {
  	$vars['output'] = $vars['field']->advanced_render($vars['row']);
  }
}

function weeblog_preprocess_node(&$vars) {
  $vars['created_day'] = date('d', $vars['created']);
  $vars['created_month'] = date('M', $vars['created']);
  $vars['created_year'] = date('Y', $vars['created']);
}

