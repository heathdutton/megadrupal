<?
/*
 * Created on Jan 27, 2012
 * Created by Mark Libkuman
 * libkuman@openflows.com
 * www.openflows.com
 * 
 * Created for Manhattan Neighborhood Network
 */
 
/**
 * This is the main function for displaying the entire grid page, including the
 * header (which includes the grid filters), the timeblock tabs, the grid
 * itself, and the key (legend explaining the meaning of the cell colors)
 *
 * Here are the steps of the function
 *
 * 1. Takes passed in filter values and validates them.
 * 2. Takes the filter values and calculates the time parameters for the columns
 *    and rows of the grid.
 * 3. Passes filter values and calculated time parameters to
 *    programming_get_grid_header() which returns the header html
 * 4. If the grid page is to show a detail block for an airing, it calls the
 *    programming_airing_detail_form() function which returns html displaying
 *    the airing detail info/form
 * 5. Passes the filter values and calculated time parameters to
 *    programming_get_tabs() which returns the html displaying the 4 time block
 *    tabs (i.e. 12am-6am,6am=12pm,12pm-6pm,6pm-12am)
 * 6. Passes the filter values and calculated time parameters to the
 *    programming_get_grid_airings() function which returns the mysql result
 *    of the airings query
 * 7. Passes the mysql result of airings to programming_get_grid_array() which
 *    processes the airings into an array that can be used to populate the grid
 * 8. Passes the grid airings array into programming_get_grid() which returns
 *    the html for the actual grid
 * 9. Calls the programming_color_key() function which returns the html for
 *    displaying the key (i.e. a legend which shows the color scheme for the
 *    cells of the grid.)
 */
function cm_airing_grid_show_grid ($channel = null, $day=null, 
				   $start_hour=null,
				   $grid_mode) {  


  //SEE IF WE NEED TO DISPLAY THE AIRING CONFLICT BLOCK AS A DRUPAL SET MESSAGE
  $view = 
    views_get_view('airing_conflict_block');
  if ($view) {
    //build and execute the view using the content type of the bucket
    $output = $view->execute_display('block_1');   
    $result_count = count($view->result);
    if ($result_count > 0) {
      drupal_set_message("<h2>".$output['subject']."</h2>".
			 t("There are airings that overlap! ".
			   "Please fix them using the links below.").
			 $output['content'], 'error');
    }
    

  }
					    





  $output = NULL;
  $airing_id = filter_input(INPUT_GET, 'airing_id');
  $var = filter_input(INPUT_GET, 'grid_mode');
  if ($var) {
    $grid_mode = filter_input(INPUT_GET, 'grid_mode');
  }

  if (isset($airing_id) && $airing_id) {
    $airing = entity_load('airing', array($airing_id));
    if (isset($airing) && isset($airing[$airing_id])) {
      $airing = $airing[$airing_id];
      $channel_tid = $airing->field_airing_channel[LANGUAGE_NONE][0]['tid'];
      $channel_term = cm_airing_grid_get_term_name_for_tid($channel_tid);
      $channel = cm_airing_grid_get_channel_number($channel_term);        
      $timestamp = 
	strtotime($airing->field_airing_date[LANGUAGE_NONE][0]['value']);

      $day = CM_AIRING_GRID_TIMEZONE_SHIFT ? 
	$timestamp + date('Z', $timestamp) : $timestamp;

      $start_hour = date('H', $day);
      if (!$grid_mode) {
	$grid_mode = cm_airing_grid_get_grid_mode_for_airing($airing);
      }
    }
    else {
      $airing_id = NULL;
      $airing = NULL;
    }
  }
  $channel = cm_airing_grid_validate_channel($channel);
  
  $start_hour = cm_airing_grid_validate_start_hour($start_hour);
  
  cm_airing_grid_get_grid_time_constraints($day, $start_hour, 
					   $mysql_day_of_week,
					   $start_time, $end_time);
  


  //this is the last time for the last row, since there are 13 weeks
  //displayed we will add 12 weeks to $end_time which stores the
  //time of the last airing end for the first week of the grid
  $last_grid_airing_time = $end_time + (12*7*24*60*60);
  
  //This conditional checks to see if the DST change happens within
  //the timeblock for any row in the grid. IF so it either adds or
  //subtracts an hour from the end time of the entire grid.
  if(date('I', $end_time) != date('I', $last_grid_airing_time)) {
    if(date('I', $start_time) == 1) {
      $last_grid_airing_time = $last_grid_airing_time + 60*60;
    }
    else {
      $last_grid_airing_time = $last_grid_airing_time - 60*60;
    }
  }
  $channels = 
    cm_airing_grid_get_vocab_terms_array(cm_airing_grid_get_channel_vid());

  $channel_tid = array_search($channel, $channels);
  $channel_tid = $channel_tid ? $channel_tid : 
    array_search($channel, $channels);
  
  $collapse_navigation = isset($airing_id);
  
  $output .= cm_airing_grid_get_grid_header($channel, $start_time, 
					    $end_time, $mysql_day_of_week, 
					    $collapse_navigation, $grid_mode);
  
  $view_display = "";
  $view_class = "";
  if (isset($airing)) {
    $view_display = cm_airing_grid_get_airing_detail_block_view($airing);

  }

  $view_class = 'airing_detail_filled';
  $output .= "<div id='airing_detail_div' class='$view_class'>".
    $view_display.'</div>';

  $airings = cm_airing_grid_get_grid_airings($channel_tid, $start_time, 
					     $last_grid_airing_time, 
					     $mysql_day_of_week, NULL);

  $airings_grid_array = cm_airing_grid_get_grid_array($airings, $start_time, 
						      $end_time, $grid_mode);

  $output .= cm_airing_grid_get_grid($airings_grid_array, $channel, 
				     $start_time, $end_time,
				     $last_grid_airing_time, 
				     $mysql_day_of_week, $start_hour, 
				     $grid_mode);

  $output .= '<br/><br/><div align="center">'. cm_airing_grid_color_key() .
    '</div>';

  return $output;
}

function cm_airing_grid_get_tabs($channel, $start_time, $end_time, 
    $mysql_day_of_week, $grid_mode = "filter_end") {

  $channel_number = preg_replace("/Channel /", "", $channel);
  $start_hour = date('H', $start_time);     

  global $base_url;
  
  $yesterday = cm_airing_grid_add_to_date($start_time, 'day', -1);
  $tomorrow = cm_airing_grid_add_to_date($start_time, 'day', 1);

  $last_week = cm_airing_grid_add_to_date($start_time, 'week', -1);
  $next_week = cm_airing_grid_add_to_date($start_time, 'week', 1);

  $left_links = l("<<<Yesterday", 
		  "admin/manage/scheduling/$channel_number/" .
		  "$yesterday/".date('H',$start_time)."/$grid_mode")."</br>";
  $left_links .= l("<<<Last Week", 
		  "admin/manage/scheduling/$channel_number/" .
		  "$last_week/".date('H',$start_time)."/$grid_mode");

  $right_links = l("Tomorrow>>>", 
		  "admin/manage/scheduling/$channel_number/" .
		  "$tomorrow/".date('H',$start_time)."/$grid_mode")."</br>";
  $right_links .= l("Next Week>>>", 
		  "admin/manage/scheduling/$channel_number/" .
		  "$last_week/".date('H',$start_time)."/$grid_mode");


  $spacer = '<td class="spacer">&nbsp;</td>';
  $url =url($base_url."/admin/manage/scheduling/$channel_number/$start_time/");
  if ($start_hour < 6) {
    $item1_prefix = '<td class="selected-tab">';
    $item1_postfix = '</td>'.$spacer;
    $item2_prefix = '<td class="unselected-tab"><a href="'.$url.'6/'.
      $grid_mode.'">';
    $item2_postfix = '</a></td>'.$spacer;
    $item3_prefix = '<td class="unselected-tab"><a href="'.$url.'12/'.
      $grid_mode.'">';
    $item3_postfix = '</a></td>';
    $item4_prefix = '<td class="unselected-tab"><a href="'.$url.'18/'.
      $grid_mode.'">';
    $item4_postfix = '</a></td>';
  }
  else if ($start_hour < 12) {
    $item1_prefix = '<td class="unselected-tab"><a href="'.$url.'0/'.
      $grid_mode.'">';
    $item1_postfix = '</a></td>'.$spacer;
    $item2_prefix = '<td class="selected-tab">';
    $item2_postfix = '</td>'.$spacer;
    $item3_prefix = '<td class="unselected-tab"><a href="'.$url.'12/'.
      $grid_mode.'">';
    $item3_postfix = '</a></td>';
    $item4_prefix = '<td class="unselected-tab"><a href="'.$url.'18/'.
      $grid_mode.'">';
    $item4_postfix = '</a></td>';
  } 
  else if ($start_hour < 18) {
    $item1_prefix = '<td class="unselected-tab"><a href="'.$url.'0/'.
      $grid_mode.'">';
    $item1_postfix = '</a></td>'.$spacer;
    $item2_prefix = '<td class="unselected-tab"><a href="'.$url.'6/'.
      $grid_mode.'">';
    $item2_postfix = '</a></td>'.$spacer;
    $item3_prefix = '<td class="selected-tab">';
    $item3_postfix = '</td>';
    $item4_prefix = '<td class="unselected-tab"><a href="'.$url.'18/'.
      $grid_mode.'">';
    $item4_postfix = '</a></td>';
  }
  else {
    $item1_prefix = '<td class="unselected-tab"><a href="'.$url.'0/'.
      $grid_mode.'">';
    $item1_postfix = '</a></td>'.$spacer;
    $item2_prefix = '<td class="unselected-tab"><a href="'.$url.'6/'.
      $grid_mode.'">';
    $item2_postfix = '</a></td>'.$spacer;
    $item3_prefix = '<td class="unselected-tab"><a href="'.$url.'12/'.
      $grid_mode.'">';
    $item3_postfix = '</a></td>';
    $item4_prefix = '<td class="selected-tab">';
    $item4_postfix = '</td>';
  }

  $item1 = $item1_prefix."12am-6am".$item1_postfix;
  $item2 = $item2_prefix."6am-12pm".$item2_postfix;
  $item3 = $item3_prefix."12pm-6pm".$item3_postfix;
  $item4 = $item4_prefix."6pm-12am".$item4_postfix;  

  $output = '<table class="cm_airing_grid_grid_header">'.
    "<tr><td class='cm_airing_grid_left_links'>$left_links</td>".
    $item1.$item2.$item3.$spacer.$item4.$spacer.
    "<td class='cm_airing_grid_right_links'>$right_links</td></tr></table>";
  return $output;      
  
} 

function cm_airing_grid_get_grid_array($airings, 
				       $start_time, $end_time, $grid_mode) {
  $data = array();
  $ids = array();
  foreach ($airings as $airing) {
    if (!in_array($airing->airing_id, $ids)) {
      $start = strtotime($airing->event_start);
      $end = strtotime($airing->event_end);
      $airing->count = ceil(($end - $start) / 60 / 30);
      $data[$start][] = $airing; 
      $ids[] = $airing->airing_id;
    }
  } 
  return $data;    

}

function cm_airing_grid_get_grid($airings_grid_array, $channel, 
				 $start_time,$end_time, 
				 $last_grid_airing_time, $mysql_day_of_week, 
				 $start_hour, $grid_mode) {
  
  $grid_contains_skip_dst = FALSE;
  $grid_contains_doubled_dst = FALSE;
  // the logic used with this variable is flawed
  // setting to null to address #1732714
  $have_passed_doubled_dst = NULL;
 
  //SET VARIABLES IF:
  //1. THERE IS A DST BETWEEN THE FIRST ROW DATE AND THE LAST ROW DATE
  //2. IT IS A SUNDAY
  if(date('I', $start_time) != date('I', $last_grid_airing_time) &&
     date('w', $start_time) == 0) {
  
     if(date('I', $start_time) == 1){
       $grid_contains_doubled_dst = true;
     }
     else {
       $grid_contains_skip_dst = true;
     }
  }

  if(date('I', $start_time) != date('I', $start_time + 60*60*6) &&
     date('w', $start_time) == 0) {
 
     if(date('I', $start_time) == 1){
       $row_contains_doubled_dst = TRUE;
       $have_passed_doubled_dst = FALSE;
       
       // The variable names/logic seems off
       // Shouldn't need to be this redundant
       $row_contains_skip_dst = FALSE;
       $have_passed_skip_dst = TRUE;
     }
     else {
       $row_contains_skip_dst = TRUE;
       $have_passed_skip_dst = FALSE;
       
       // The variable names/logic seems off
       // Shouldn't need to be this redundant
       $row_contains_doubled_dst = FALSE;
       $have_passed_doubled_dst = TRUE;
       
     }
  }


  //initialize variables for looping 
  $space = '&nbsp;';
  $channel_number = preg_replace("/Channel /", "", $channel);
  global $base_url;  
      
  $rows = array();
  $column_index = 0;
  $col_time = $start_time;
  $max_column_index = 12;
  $start_col_time = $col_time;
  $double_hour = false;
  
  if (isset($row_contains_doubled_dst)) {
    $max_column_index = $max_column_index + 2;
  }

  //First column of first row is blank
  $data[] = $space;
  $data_classes[] = "cm_airing_grid_cell";
  $have_passed_doubled_dst = NULL;
  $row_contains_doubled_dst = NULL;
  while ($column_index < $max_column_index) {
    if (!$have_passed_doubled_dst && $grid_contains_doubled_dst && 
        !$row_contains_doubled_dst && date('H', $col_time) == 2) {
      $have_passed_doubled_dst = true;
      $col_time = $col_time - 60*60;    
      $data[] = "<b>".date("g:ia", $col_time)."</b>";
      $data_classes[] = "cm_airing_grid_cell";
      $column_index ++;
      $col_time = $col_time + 30 * 60;
      $data[] = "<b>".date("g:ia", $col_time)."</b>";
      $data_classes[] = "cm_airing_grid_cell";
      $column_index ++;
      $col_time = $col_time + 30 * 60;
    }
    if (!$have_passed_doubled_dst && $grid_contains_skip_dst && 
        $row_contains_skip_dst && date('H', $col_time) == 3) {
      $data[] = "<b>2:00am</b>";
      $data_classes[] = "cm_airing_grid_cell";
      $column_index ++;
      $data[] = "<b>2:30am</b>";
      $data_classes[] = "cm_airing_grid_cell";
      $column_index ++;
      $have_passed_doubled_dst = true;
    }
    $data[] = "<b>".date("g:ia", $col_time)."</b>";
    $data_classes[] = "cm_airing_grid_cell";
    $column_index ++;
    $col_time = $col_time + 30 * 60;
  }  
  if ($have_passed_doubled_dst && $grid_contains_doubled_dst) {
    $data[] = "<b>".date("g:ia", $col_time)."</b>";
    $data_classes[] = "cm_airing_grid_cell";
    $col_time = $col_time + 30 * 60;
    $data[] = "<b>".date("g:ia", $col_time)."</b>";
    $data_classes[] = "cm_airing_grid_cell";
    $max_column_index = $max_column_index + 2;
  }
  cm_airing_grid_format_status_row
    ($rows, $data, $data_classes, "cm_airing_grid_cell");

  $data = array();
  $data_classes = array();  
  $row_index = 0;
  $next_week_column_time = $start_time;
  $finished_black_out = false;
  
  while ($row_index < 13)  {
    $data = array();
    $column_index = 0;
    $data[] = "<b>".date("m/d/y", $next_week_column_time)."</b>";
    $data_classes[] = "cm_airing_grid_cell";
   
    $column_time = $next_week_column_time;
    $row_start_time = $column_time;
    $next_week_column_time = 
        cm_airing_grid_add_to_date($next_week_column_time, "week", 1);
    
    $have_passed_doubled_dst = false;
    $have_passed_skip_dst = false;
    $should_blackout_field = false;
    
    while ($column_index < $max_column_index) {
      $airing_data = cm_airing_grid_extract_grid_data($airings_grid_array, 
        $column_time, $start_hour, $start_time, $channel, $grid_mode);

      $row_contains_skip_dst = false;
      $row_contains_doubled_dst = false;    
      if(date('I', $row_start_time) != date('I', $row_start_time + 60*60*6) &&
         date('w', $row_start_time) == 0) {
        if(date('I', $start_time) == 1){
          $row_contains_doubled_dst = true;
        }
        else {
          $row_contains_skip_dst = true;
        }
      }
  
      if ($should_blackout_field){
        $column_index = $column_index + 4;
        $have_passed_doubled_dst = true;
      }
      $should_blackout_field = false;          
      if ($row_contains_skip_dst && (date('H', $column_time) == 3) &&
          !$have_passed_skip_dst) {
        $should_blackout_field = true;
        $have_passed_skip_dst = true;
      }
      if ($grid_contains_doubled_dst && !$row_contains_doubled_dst && 
          (date('H', $column_time) == 2) && !$have_passed_doubled_dst &&
          (date('i', $column_time) != 30)) {
           $have_passed_doubled_dst = true;
           $should_blackout_field = true;
      }    
      if ($airing_data) {

        if ($should_blackout_field) {
          $data[] = array($space, 2);
          $data_classes[] = "cm_airing_grid_blackout";
          $data[] = array($airing_data['text'], $airing_data['colspan']);
          //FIXME add class based on live/tape + episode/no episode
          $data_classes[] = "cm_airing_grid_data";
          $column_time = $column_time + 30*60*$airing_data['colspan'];
   
          if ($row_contains_skip_dst) {
            $have_passed_skip_dst = true;
          }
          $column_index = $column_index + $airing_data['colspan'] - 2;
          
        }
        else { 
          $data[] = array($airing_data['text'], $airing_data['colspan']);
          //FIXME add class based on live/tape + episode/no episode
          $data_classes[] = $airing_data['grid_data_class'];
          $column_time = $column_time + (30 * 60 * $airing_data['colspan']);
          $column_index = $column_index + $airing_data['colspan'];        
        }
      }
      else if ($should_blackout_field && $grid_contains_doubled_dst) {
        $data[] = array($space, 2);
        $data_classes[] = "cm_airing_grid_blackout";
        $column_index = $column_index - 2;
        $have_passed_doubled_dst = true;
      }
      else if ($row_contains_skip_dst && $should_blackout_field) {
        $data[] = array($space, 2);
        $data_classes[] = "cm_airing_grid_blackout";
        $column_index = $column_index -2;
        $have_passed_skipped_dst = true;
      }
      else {
        if ($grid_mode == 'show_all') {
          $fill_mode = "at-fill";
        }
        else {
          $fill_mode = "series";
        }
	
        $url = 
	  url($base_url."/airing/add?timestamp=$column_time".
	      "&channel=$channel_number&mode=$fill_mode") ;

	$img_path = drupal_get_path('module', 'cm_airing_grid')."/images/".
	  'green_square_plus.gif';
	  
        $space_link = '<a href="'.$url.'">'.
            "<img class='cm_airing_grid_add_img' src='/$img_path' 
              height=30 width=30>".'</a>';
        $data[] = array($space_link, 1);
        $data_classes[] = "cm_airing_grid_blank";
        $column_index++;
         $column_time = $column_time + 30 * 60;
      }
      $airing_data = null;
    }
    $row_index ++;
    cm_airing_grid_format_status_row
      ($rows, $data, $data_classes, "cm_airing_grid_cell");
    $data = array();
    $data_classes = array();
  }
  
  return theme('table', array('rows' => $rows, 
			      'attributes' => array('class' => 
						    'cm_airing_grid_cell')));

}

function cm_airing_grid_color_key() {

  $key = "
    <table><tr>
      <td class='cm_airing_grid_data'>Airing with Show</td>
      <td class='cm_airing_grid_no_episode'>Airing without Show</td>
      <td class='cm_airing_grid_live'>Live with Livefeed</td>
      <td class='cm_airing_grid_no_device'>Live without Livefeed</td>
      <td class='cm_airing_grid_short'>Less Than 20 Minutes</td>
    </tr></table>";

  return $key;
}

function cm_airing_grid_get_airing_detail_block_view($airing) {
  $airing_edit_mode = filter_input(INPUT_GET, 'edit-mode');
  $view = views_get_view('airing_detail_for_grid');
  $view->set_arguments(array($airing->airing_id));
  
  $show_id = 
    isset($airing->field_airing_show_ref[LANGUAGE_NONE]) &&
    isset($airing->field_airing_show_ref[LANGUAGE_NONE][0]) ?
    $airing->field_airing_show_ref[LANGUAGE_NONE][0]['target_id'] : NULL;

  $show_live_block = FALSE;
  if (!$show_id && $airing_edit_mode != 'live-swapout') {
    $is_live = cm_airing_grid_is_live_airing($airing);
    if ($is_live) {
      $show_live_block = TRUE;
    }
  }
  if ($show_live_block) {
    $view_display = $view->execute_display('airing_detail_block_livefeed');
  }
  else {
    $view_display = $view->execute_display('airing_detail_block_regular');
  }

  $view_display = $view_display['content'];

  return $view_display;
}
