<?php

function mnn_programming_exports_widgets(&$view, 
					 $old_action=NULL, 
					 $new_action=NULL) {
  
  $exposed_widgets = $view->exposed_widgets;
  
  if (!$old_action) {
    $old_action = 'action="/'.MNN_DAILY_SHOWS_VIEW_PATH_RAW;
  }
  if (!$new_action) {
    $new_action = 'action="/'.MNN_DAILY_SHOWS_VIEW_PATH;
  }
  //changed the form action so we point to the chart, rather than the raw view
  $exposed_widgets = str_replace($old_action, $new_action, $exposed_widgets);

  return $exposed_widgets;
}

function mnn_programming_exports_get_vid_for_name($name) {
  $global_var_name = 'vid_cache_'.$name;
  global $$global_var_name;
  if ($$global_var_name) {
    return $$global_var_name;
  }
  
  if ( $name ) {
    $query = "SELECT vid FROM {taxonomy_vocabulary} WHERE name=:s LIMIT 1";
    $db_result = db_query($query, array(':s' => $name))->fetchField();

    $$global_var_name = $db_result;
    return $db_result;
  }
  else {
    return false;
  }
}
