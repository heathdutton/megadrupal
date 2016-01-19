<?
/*
 * Created on Jan 27, 2012
 * Created by Mark Libkuman
 * libkuman@openflows.com
 * www.openflows.com
 * 
 * Created for Manhattan Neighborhood Network
 */



function cm_airing_grid_extract_grid_data(&$airings_grid_array, $column_time, 
  $start_hour, $start_time, $channel, $grid_mode) {
  global $base_url;
  $airing_id = filter_input(INPUT_GET, 'airing_id');
  $channel_number = preg_replace("/Channel /", "", $channel);
  $data = isset($airings_grid_array[$column_time]) ? 
    $airings_grid_array[$column_time] : NULL;

  $block_bump_count = 0;
  $orig_column_time = $column_time;
  if (!$data && (date('H',$column_time) == $start_hour) && (date('i',$column_time) == 0)) {
    $column_time = $column_time - 30*60;
    $data = isset($airings_grid_array[$column_time]) ? 
      $airings_grid_array[$column_time] : NULL;
    $block_bump_count ++;
    if (!$data && (date('H',$column_time) == $start_hour) && (date('i',$column_time) == 0)) {
      $column_time = $column_time - 30*60;
      $data = isset($airings_grid_array[$column_time]) ? 
	$airings_grid_array[$column_time] : NULL;
      $block_bump_count ++;
      if (!$data && (date('H',$column_time) == $start_hour) && (date('i',$column_time) == 0)) {
	$column_time = $column_time - 30*60;
	$data = isset($airings_grid_array[$column_time]) ? 
	  $airings_grid_array[$column_time] : NULL;
	$block_bump_count ++;
      }
    }
  }

  $count = 1;
  if ($data) {
    if ($block_bump_count) {
      $data[0]->event_start = date('Y-m-d H:i:s',
				   cm_airing_grid_add_to_date(strtotime($data[0]->event_start),
							      'hour', 
							      ($block_bump_count/2)));
      //FIXME, MAKE WORK FOR SHOWS LONGER THAN A HOUR 
      $data[0]->count = 1;
    }
    
    $text = "<table class = 'cm_airing_grid_internal_grid'>";

    foreach($data as $airing) {
      $airing_class = ($airing_id==$airing->airing_id) ? 
	'cm_airing_grid_selected_airing' : '';

      if ($airing->event_start == date("Y-m-d H:i:s", $orig_column_time)) { 
        $display_class = cm_airing_grid_grid_display_class_dictionary($airing);
        //FIXME, MAKE BUTTON EITHER SHOW PID OR TITLE BY USING A CONFIG SETTING
	$link = '<input type="button" id="'.$airing->airing_id.
	  '" class="cmag use-ajax form-submit '.$airing_class.'" value="'.
	  substr($airing->title, 0, 15).'">';
 
	  
        if ($grid_mode == 'show_all') {
          $link .= "<br/>".
	    l("(remove)", "airing/".$airing->airing_id."/delete",
	      array('query'=>drupal_get_destination()));
        }
        $text .= "<tr><td class='$display_class'>".$link.
            "</td></tr>";
        $display_class = 'cm_airing_grid_blank';
        if ($airing->count > $count) {
          $count = $airing->count;
        }
      }
    }
    if ($grid_mode == 'show_all') {
      $column_time = strtotime($airing->event_end)+1;
      $url = 
	url($base_url."/airing/add?timestamp=$column_time".
	    "&channel=$channel_number&mode=at-fill") ;

	$img_path = drupal_get_path('module', 'cm_airing_grid')."/images/".
	  'green_square_plus.gif';
	  
        $add_link = '<a href="'.$url.'">'.
            "<img class='cm_airing_grid_add_img' src='/$img_path' 
              height=30 width=30></a>";
        $text .= "<tr><td>".$add_link.
          "</td></tr>";
    }
    $text .= "</table>";
    return array('text'=>$text, "colspan"=>$count, 
		 'grid_data_class' => $display_class);
  }
  return null;         
}

function cm_airing_grid_format_status_row
    (&$rows, $data, $data_classes = null, $row_class = null) {
  if (!$data_classes){
    $data_classes = array('cm-airing-grid-empty-cell', 
			  'cm-airing-grid-empty-cell');
  }
  if (!$row_class) {
    $row_class = "cm_airing_grid_grid";
  }
  if (count($data) == 1) {
    $cells[] = array('data' => $data[0],
                     'class' => array($data_classes[0]),
                     'colspan' => 2,
                     );
    $rows[] = array('data' => $cells, 'class' => $row_class);
  }
  else if (count($data) == 2) {
    $cells[] = array('data' => $data[0],
                     'class' => array($data_classes[0]),
                    );
    $cells[] = array('data' => $data[1],
                     'class' => array($data_classes[1]),
                    );
    $rows[] = array('data' => $cells, 'class' => array($row_class));
  }
  else if (count($data) > 2) {
    foreach ($data as $key=>$value) {
      if (is_array($data_classes)) {
        $data_class = $data_classes[$key];
      }
      else {
        $data_class = $data_classes;
      }
      if (is_array($value)) {
        $colspan = $value[1];
        $value = $value[0];
      }
      else {
        $colspan = 1;
      }
      $cells[] = array('data' => $value,
                        'class' => array($data_class),
                        'colspan' => $colspan,
                       );
      
    }
    $rows[] = array('data' => $cells, 'class' => array($row_class));
  }
}


/*
 * try to remove the last element from a url
 */
function cm_airing_grid_remove_last_url_element($url) {
  $a = explode("/", $url);
  if ( $unused = array_pop($a) ) {
    return implode("/", $a);
  }
  else {
    return $url;
  }
}

function cm_airing_grid_get_grid_mode_for_airing($airing) {
  $detail_airing_type_tid= $airing->field_airing_type[LANGUAGE_NONE][0]['tid'];
  $airing_type_series_tid = cm_airing_grid_get_airing_series_tid();
  if($detail_airing_type_tid &&
     $airing_type_series_tid  != $detail_airing_type_tid) {
    $grid_mode = "show_all";
  }
  else {
    $grid_mode = "filter_end";
  }
  return $grid_mode;
}

function cm_airing_grid_is_live_airing($airing) {
  //THIS IS THE NEW WAY PCM IS SETTING THIS FIELD, WITH THE LIVE SOURCE/
  //LIVE FEED SET DIRECTLY ON THE AIRING
  if (isset($airing->live_source) && $airing->live_source ) {
    return TRUE;
  }
  //FIXM, remove this and figure out how to split the logic
  return FALSE;

  //THIS IS THE LEGACY WAY OF SETTING THE FIELD
  $format_vid = cm_airing_grid_get_format_vid();
  if (isset($airing->episode_nid) && $airing->episode_nid) {
    $format_tid = cm_airing_grid_get_project_format_tid($airing->episode_nid);
    if ($format_tid) { 
      $format_term_name = cm_airing_grid_get_term_name_for_tid($format_tid);
    }
  }
  else {
    $format_tid = cm_airing_grid_get_project_format_tid($airing->project_nid);
    if ($format_tid) { 
      $format_term_name = cm_airing_grid_get_term_name_for_tid($format_tid);
    }

  }
  if (isset($format_term_name) && ($format_term_name == "LIVE")) {
    return TRUE;
  }

  return FALSE;
}

/**
 * @param $show_nid the show to find a set of airings for
 * @param $ignore_airing_id the airing to ignore in the set
 *
 * @return $show_airings the set of all the airings for a particular show
 *
 */
function cm_airing_grid_get_future_show_airings($show_nid, $ignore_airing_id) {
  $show_airings = array();

  $view = views_get_view(CM_AIRING_GRID_SHOW_AIRINGS_VIEW);
  if (!$view) {
    drupal_set_message("Error Code 59128: Missing view:".
		       CM_AIRING_GRID_SHOW_AIRINGS_VIEW, 'error');
    return;
  }
  
  //set show nid as argument
  $view->set_arguments(array($show_nid));
  $view->execute('future');
  $results = $view->result;

  while ($result = array_shift($results)) {
    $show_airings[] = $result->airing_id;
  }

  return $show_airings;
}