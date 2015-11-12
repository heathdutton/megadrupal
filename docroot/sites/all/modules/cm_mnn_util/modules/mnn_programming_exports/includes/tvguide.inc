<?php


function mnn_programming_exports_tvguide() {
  //fetch our view
  $view = views_get_view(MNN_TVGUIDE_VIEW);

  //execute the query and grab the results
  $view->execute();
  $results = $view->result;

  if (isset($view->exposed_input['field_airing_date_value'])) {
    $start_date = ($view->exposed_input['field_airing_date_value']) ?
      strtotime($view->exposed_input
		['field_airing_date_value']['min']['date'])
      : strtotime('today');
    $end_date = ($view->exposed_input['field_airing_date_value']) ?
      strtotime($view->exposed_input
		['field_airing_date_value']['max']['date'])
      : strtotime('today');
    mnn_programming_exports_tvguide_data($results,$start_date,$end_date);
    $output .= "<h1>".l("Download TV Guide",
		 'admin/shows/tvguide/download')."</h1>";
  }
  $old_action = 'action="/'.MNN_TVGUIDE_VIEW_PATH_RAW;
  $new_action = 'action="/'.MNN_TVGUIDE_VIEW_PATH;

  $output .= mnn_programming_exports_widgets($view,$old_action,$new_action);
  
  return $output;
}

function mnn_programming_exports_tvguide_data($results,
					      $start_date,
					      $end_date) {
  $airings_array = array();

  foreach($results as $result) {
    if (!isset($result->field_field_airing_project_ref[0])) {
      drupal_set_message("Missing project for airing: ".
			 l($result->airing_id, "airing/".$result->airing_id), 
			 "error");
      continue;
    }
    $project = $result->field_field_airing_project_ref[0]['raw']['entity'];

    if (!isset($result->field_field_airing_date[0])) {
      drupal_set_message("Missing Date for airing: ".
			 l($result->airing_id,"airing/".$result->airing_id),
			 "error");
      continue;
    }
    $date = $result->field_field_airing_date[0]['raw'];
    $start_timestamp = strtotime($date['value']);
    $end_timestamp = strtotime($date['value2']);

    if (!isset($result->field_field_airing_channel[0])) {
      drupal_set_message("Missing Channel for airing: ".
			 l($result->airing_id,"airing/".$result->airing_id),
			 "error");
      continue;
    }
    $channel = $result->field_field_airing_channel[0]['rendered']['#title'];
    $channel = str_replace('Channel ', 'MNN',$channel);

    $airings_array[$channel] = 
      isset($airings_array[$channel]) ? 
      $airings_array[$channel] : array();

    $airings_array[$channel][$date] = 
      isset($airings_array[$channel][$date]) ? 
      $airings_array[$channel][$date] : array();

    $data['AIRDATE'] = date('m/d/Y',strtotime($date['value']));
    $data['AIRTIME'] = date('h:i A',strtotime($date['value']));
    $data['CH'] = $channel;
    $data['PROGRAM'] = $project->title;
    $data['start_timestamp'] = $start_timestamp;
    $data['end_timestamp'] = $end_timestamp;
    $airings_array[$channel][date('Y-m-d', $start_timestamp)][] = $data;
  }

  $_SESSION['mnn_tvguide_data'] = $airings_array;
  $_SESSION['mnn_tvguide_start_date'] = $start_date;
  $_SESSION['mnn_tvguide_end_date'] = $end_date;
}


function mnn_programming_exports_tvguide_download() {
  $airings_array = $_SESSION['mnn_tvguide_data'];
  $start_date = $_SESSION['mnn_tvguide_start_date'];
  $end_date = $_SESSION['mnn_tvguide_end_date'];

  //start creating the file
  // Include PEAR::Spreadsheet_Excel_Writer
  require_once "Spreadsheet/Excel/Writer.php";

  // Create an instance
  $workbook =& new Spreadsheet_Excel_Writer();

  // Send HTTP headers to tell the browser what's coming
  $workbook->send("MNN_schedule.xls");

  //add a color definition in this case at color index 12 whatever that means  
  $workbook->setCustomColor(12, 109, 255, 145);

  //create a bold format to apply to the headers
  $format_bold =& $workbook->addFormat();
  $format_bold->setBold();
  $format_bold->setFgColor(12);
  //create bold, large type format to apply to the initial channel/date display
  $format_title =& $workbook->addFormat();
  $format_title->setBold();
  $format_title->setSize('16');
  $format_title->setFgColor(12);

  //format for centering text
  $format_center =& $workbook->addFormat();
  $format_center->setAlign('center');
  
  //loop for each channel
  foreach($airings_array as $channel=>$channel_airings) {
    $rowcounter=0;
    $worksheet =& $workbook->addWorksheet($channel);
    $worksheet->setHeader("Manhattan Neighborhood Network Schedule");
    $worksheet->setFooter("problems? call the MNN Scheduling Coordinator 
      @ 212-757-2670 x334");

    foreach($channel_airings as $airdate=>$channel_airdate_airings) {
      //add in an extra row for all but the first day
      if ($rowcounter >1){
        $rowcounter =$rowcounter+1;
        //add in a page break
        $worksheet->setHPageBreaks(array($rowcounter));
      }
      //set width on col 0,2 and 3
      $worksheet->setColumn(0,0,11);
      $worksheet->setColumn(2,2,6);
      $worksheet->setColumn(3,3,50);

      $worksheet->write($rowcounter, 0, $channel, $format_title);
      //add in null rows for bg color (eric turned off the extra rows as 
      //they fuckup the printout)

      $worksheet->write($rowcounter, 1, "", $format_title);
      $worksheet->write($rowcounter, 2, "", $format_title);
      $worksheet->write($rowcounter, 3, date('l F jS Y', strtotime($airdate)), 
			$format_title);

      $rowcounter= $rowcounter+1;
      $worksheet->write($rowcounter, 0, "AIRDATE", $format_bold);
      $worksheet->write($rowcounter, 1, "AIRTIME", $format_bold);
      $worksheet->write($rowcounter, 2, "CH", $format_bold);
      $worksheet->write($rowcounter, 3, "PROGRAM", $format_bold);

      $rowcounter=$rowcounter+1;

      //execute the query and output results one row per row
      //leave out the timestamp that was necessary for sort order
      $last_end_time = strtotime($airdate);
      
      foreach($channel_airdate_airings as $program_info) {
        //set values from array
        $channel_for_printout =$program_info['CH'];
        $program_for_printout =$program_info['PROGRAM'];
        $airdate_for_printout =$program_info['AIRDATE'];
        $airtime_for_printout =$program_info['AIRTIME'];
        $airtime_start_seconds = $program_info['start_timestamp'];
        $airtime_end_seconds = $program_info['end_timestamp'];

        if (($airtime_start_seconds - $last_end_time) > (5 * 60)) {
          $worksheet->write($rowcounter, 0, $airdate_for_printout);
	  if ((date('i', $last_end_time) == 28) || 
	      (date('i', $last_end_time) == 58)) {
	    $last_end_time += 2*60;
	  } 
          $worksheet->write($rowcounter, 1, date('g:i A', $last_end_time));
          $worksheet->write($rowcounter, 2, $channel_for_printout,
			    $format_center);

	  $worksheet->write($rowcounter, 3, "www.mnn.org");
          $rowcounter++;
        }
        $worksheet->write($rowcounter, 0, $airdate_for_printout);
        $worksheet->write($rowcounter, 1, $airtime_for_printout);
        $worksheet->write($rowcounter, 2, $channel_for_printout,
			  $format_center);
        $worksheet->write($rowcounter, 3, $program_for_printout);
        $rowcounter++;
        $last_end_time = $airtime_end_seconds;
      }
    }
  }
  // Finish the spreadsheet, dumping it to the browser
  $workbook->close();

}