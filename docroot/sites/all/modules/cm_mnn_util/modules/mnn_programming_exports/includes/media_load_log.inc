<?php


function mnn_programming_exports_load_log() {
  //fetch our view
  $view = views_get_view(MNN_DAILY_SHOWS_VIEW);

  //execute the query and grab the results
  $view->execute();
  $results = $view->result;
  if (isset($view->exposed_input['field_airing_date_value'])) {
    $date = ($view->exposed_input['field_airing_date_value']) ?
      strtotime($view->exposed_input
		['field_airing_date_value']['value']['date'])
      : strtotime('today');
    $results = mnn_programming_exports_load_log_html($results,$date);
    $output .= l("Download Media Load Log",
		 'admin/shows/load-log/download');
  }
  $old_action = 'action="/'.MNN_DAILY_SHOWS_VIEW_PATH_RAW;
  $new_action = 'action="/'.MNN_MEDIA_LOAD_LOG_VIEW_PATH;

  $output .= mnn_programming_exports_widgets($view,$old_action,$new_action);
  
  return $output;
}

function mnn_programming_exports_load_log_html($results,$log_date) {
  $need_array = array();
  $have_array = array();
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
    
    if (!isset($result->field_field_airing_channel[0])) {
      drupal_set_message("Missing Channel for airing: ".
			 l($result->airing_id,"airing/".$result->airing_id),
			 "error");
      continue;
    }
    $channel = $result->field_field_airing_channel[0]['rendered']['#title'];

    if (!isset($project->taxonomy_cm_project_types[LANGUAGE_NONE])) {
      drupal_set_message("Missing Project Type for Project:".
			 l($project->nid,"node/".$project->nid),
			 "error");
      continue;
    }
    $project_type_tid = 
      $project->taxonomy_cm_project_types[LANGUAGE_NONE][0]['tid'];
    
    $project_type = taxonomy_term_load($project_type_tid);
    if(!$project_type) {
       drupal_set_message("Missing Type for tid :".$project_type_tid,"error");
       continue;
    }
    $project_type = $project_type->name;
    switch (strtolower($project_type)) {
    case 'series':
      $project_prefix = "P-";
      break;
    case 'special':
      $project_prefix = "S-";
      break;
    case 'default':
      $project_prefix = "F-";
      break;
    }
    $show = null;
    if (isset($result->field_field_airing_show_ref[0])) {
      $show = $result->field_field_airing_show_ref[0]['raw']['entity'];
    }

    if (!$show) {
      continue;
    }

    if (!isset($show->field_format[LANGUAGE_NONE])) {
      drupal_set_message("Missing Format for Show:".
			 l($show->nid,"node/".$show->nid),
			 "error");
      continue;
    }
    $format_tid = $show->field_format[LANGUAGE_NONE][0]['tid'];
    $format_name = taxonomy_term_load($format_tid);
    if(!$format_name) {
      drupal_set_message("Missing Format for tid :".$format_tid,"error");
      continue;
    }
    $format_name = $format_name->name;
    
    $data['episode_nid'] = $show->nid;
    $data['event_end'] = strtotime($date['value2']);
    $data['event_start'] = strtotime($date['value']);
    $data['AirTime'] = date('H:i:s', $data['event_start']);
    $data['Ch.'] = str_replace('Channel ', '',$channel);
    $data['Program'] = $project_prefix.$project->nid;
    $data['Episode'] = 
      $show->field_episode_number[LANGUAGE_NONE][0]['value'];
				      $data['Title'] = $project->title;
    $data['Format'] = $format_name;
    //FIXME, IF TIMECODES GET PUT BACK INTO PLACE SET USING 
    //ACTUAL TIMECODES
    $data['timecode_in'] = 0;
    $data['timecode_out'] = 0;
    $media_array[] = $data;
   
  }
				    
  $log_table = 
    mnn_programming_exports_load_log_table($media_array);
  
  $_SESSION['mnn_media_log_html'] = $log_table;
  $_SESSION['mnn_media_log_date'] = $log_date;
}

function mnn_programming_exports_load_log_table($results,$have=TRUE) {

  $first_row = TRUE; // distinguish header from other rows
  foreach ($results as $row) {
    $row_vals=array();
    foreach ($row as $key => $value) {
      if ($key == 'timecode_in' || $key == 'timecode_out' || 
	  $key == 'timecode_in' || $key=='episode_nid' || 
	  $key == 'event_end' || $key == 'event_start') {
	continue;
      }
      if ($first_row) {
	if ( 'Title' == $key ) {
	  $header[]="<u><b>".$key."</u></b>";
	  $header[]=" ";
	}
       
	else {
	  $header[]="<u><b>".$key."</u></b>";
	}
      }
      if ( $key == 'Title' ) {
	$title = ereg_replace("\n","",$value);
	$row_vals[] = array('data' => $title, 'colspan' => 2);
	continue;
      }
      else {
	$row_vals[]=array('data' => ereg_replace("\n","",$value));
      }
    }
    
    $len_start = 0;
    $len_end = 0;
  
    mnn_programming_exports_broken_up_timstamp_display($row['timecode_in'], 
						       $display_hour, 
						       $display_min, 
						       $display_sec);
    
    $start = sprintf('%02d', $display_hour).":".
      sprintf('%02d', $display_min).":".
      sprintf('%02d', $display_sec);
    $row_vals[] = array('data' => $start);
    
    if ( $row['timecode_out'] ) {
      $len_end = $row['timecode_out'];
      $len_start = $row['timecode_in'];
    }
    else {
      $len_end = $row['event_end'];
      $len_start = $row['event_start'];
    }
    
    $len = $len_end - $len_start;
    if ( $len > 0 ) {
      mnn_programming_exports_broken_up_timstamp_display(($len_end - 
							  $len_start), 
							 $display_hour, 
							 $display_min, 
							 $display_sec);
    }
    else {
      $display_hour = $display_min = $display_sec = 0;
    }
    $length = sprintf('%02d', $display_hour).":".
      sprintf('%02d', $display_min).":".
      sprintf('%02d', $display_sec);
    
    $row_vals[]=array('data' => $length);

    $row_vals[]=array('data' => '[  ]');
    $table_array[]=$row_vals;
    $first_row = FALSE;
    $prev_start = $row['event_start'];
  }

  $header[]="<u><b>StartTime</u></b>";
  $header[]="<u><b>Length</u></b>";
  $header[]='<u><b>Load</u></b>';
  
  //$tape_log_output=theme_table($header, $table_array,array(width=>'250px'));
  //$header = array("test");
  //$table_array = array(array("y0"));
  $html = theme_table(array('header'=>$header, 
			   'rows'=>$table_array,
			   'attributes'=>array()));
 
  return $html;
}


function mnn_programming_exports_load_log_download() {
  require_once(TCPDF_ENG_LANG_PATH);
  require_once(TCPDF_PATH);

  $html_content = $_SESSION['mnn_media_log_html'];
  $log_date = $_SESSION['mnn_media_log_date'];
  if (!$html_content || !$log_date) {
    return "Sorry, something bad happend, error code 45322";
  }

  // create new PDF document
  $pdf = new TCPDF(PDF_PAGE_ORIENTATION, PDF_UNIT, PDF_PAGE_FORMAT, true); 

  // set document information
  $pdf->SetCreator(PDF_CREATOR);
  $pdf->SetAuthor("MNN");
  $pdf->SetTitle("Media Load Log");
  $pdf->SetSubject("Media Load Log");
  $pdf->SetKeywords("MNN");

  $pdf->SetHeaderData(NULL,NULL,'Manhattan Neighborhood Network                                   '.date('m/d/Y', $log_date).'   MEDIA LOAD LOG ',
		      date('l, F j, Y', $log_date));
  
  // set header and footer fonts
  $pdf->setHeaderFont(Array(PDF_FONT_NAME_MAIN, '', PDF_FONT_SIZE_MAIN));
  $pdf->setFooterFont(Array(PDF_FONT_NAME_DATA, '', PDF_FONT_SIZE_DATA));

  //set margins
  $pdf->SetMargins(PDF_MARGIN_LEFT, PDF_MARGIN_TOP, PDF_MARGIN_RIGHT);
  $pdf->SetHeaderMargin(PDF_MARGIN_HEADER);
  $pdf->SetFooterMargin(PDF_MARGIN_FOOTER);

  //set auto page breaks
  $pdf->SetAutoPageBreak(TRUE, PDF_MARGIN_BOTTOM);


  //set some language-dependent strings
  $pdf->setLanguageArray($l); 

  //initialize document
  $pdf->AliasNbPages();

  $pdf->SetFont(PDF_FONT_NAME_MAIN, "", 11);
  // add a page
  $pdf->AddPage();


  $pdf->setDefaultTableColumns(9);
  $pdf->setCellHeightRatio(1.0);
  $pdf->writeHTML($html_content, false, 0, false, 0);


  $pdf->lastPage();


  //Close and output PDF document
  $pdf->Output("mnn_media_load_log.pdf", "I", "I");

}


