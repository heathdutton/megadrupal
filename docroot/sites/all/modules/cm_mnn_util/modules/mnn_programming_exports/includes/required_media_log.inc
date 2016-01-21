<?php


function mnn_programming_exports_required_log() {
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
    $results = mnn_programming_exports_required_media_html($results,$date);
    $output .= l("Download Required Media Log",
		 'admin/shows/required-log/download');
  }
  $old_action = 'action="/'.MNN_DAILY_SHOWS_VIEW_PATH_RAW;
  $new_action = 'action="/'.MNN_REQUIRED_LOG_VIEW_PATH;

  $output .= mnn_programming_exports_widgets($view,$old_action,$new_action);
  
  return $output;
}

function mnn_programming_exports_required_media_html($results,$log_date) {
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
    
    if (isset($result->field_field_airing_show_ref[0])) {
      $show = $result->field_field_airing_show_ref[0]['raw']['entity'];
    }
    else {
      $show = NULL;
    }
    if ($show) {
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
    }
    else {
      if (!isset($project->field_format[LANGUAGE_NONE])) {
	drupal_set_message("Missing Format for Project:".
			   l($project->nid,"node/".$project->nid),
			   "error");
	continue;
      }
      $format_tid = $project->field_format[LANGUAGE_NONE][0]['tid'];
      
      $format = taxonomy_term_load($format_tid);
      if(!$format) {
	drupal_set_message("Missing Type for tid :".$format_tid,"error");
	continue;
      }
      $format_name = $format->name;
      
    }
    
    if (strtolower($format_name) == 'live') {
      continue;
    }

    $data['Project'] = $project_prefix.$project->nid;

    $data['Ep'] = 
      $show->field_episode_number[LANGUAGE_NONE][0]['value'];
    $data['title'] = $project->title;

    $data['AirTime'] = date('H:i:s',strtotime($date['value']));
    $data['Ch.'] = str_replace('Channel ', '', $channel);
    $data['Format'] = $format_name;
    $array_index = $project->nid.'.'.$data['Ep'].$data['Ch.'].
      date('His',strtotime($date['value']));

    if ($show) {
      $have_array[$array_index] = $data;
    }
    else {
      $need_array[$array_index] = $data;
    }
  }
  ksort($have_array);
  ksort($need_array);
  $need_media_table = 
    mnn_programming_exports_req_media_table($need_array,FALSE);

  $have_media_table = 
    mnn_programming_exports_req_media_table($have_array);
  
  $req_media_output.= "<h1>".t("Needed (not in Lib)")."</h1>";
  $req_media_output.=$need_media_table;
  $req_media_output.= "<hr><h1>".t("Already On Cart")."</h1>";
  $req_media_output.=$have_media_table;
  $_SESSION['mnn_req_media_output'] = $req_media_output;
  $_SESSION['mnn_req_media_date'] = $log_date;
}

function mnn_programming_exports_req_media_table($results,$have=TRUE) {

  $cnt=1;
  $header[]='<u>Cart</u><br/>';

  foreach($results as $row) {
    $row_vals=array();
    $row_vals[]='[  ]';
    foreach ($row as $key => $value) {
      if ( 'AirTime' == $key ) {
	$value = '&nbsp;&nbsp;'.$value;
      }
      if ($key == 'project_nid') {
	$proj_nid = $value;
	continue;
      }
      if ($cnt==1) {
	$header[]="<u><b>$key</b></u><br/>";
      }
      $row_vals[]=array('data' => ereg_replace("\n","",$value)."<br/>");
    }
    if ($cnt == 1) {
      $header[]="<u><b>Alt Episode</b></u>";
    }
    $alt_airing_times = array();
    //$ep_nid_results = programming_get_recent_checked_in_episode_nids_for_project($proj_nid, (time() - (24 * 3600)));
    //$ep_nid_arr = db_fetch_array($ep_nid_results);
    if ( $ep_nid_arr['event_start']) {
      $row_vals[] = date('Y-m-d', $ep_nid_arr['event_start']);
    }
    else {
      $row_vals[] = "none";
    }
    $row_vals[]='[  ]';
    $table_array[]=$row_vals;
    $cnt++;
  }
  $header[]='<u><b>Problem</b></u><br/>';
  $html = theme_table(array('header'=>$header, 
			   'rows'=>$table_array,
			   'attributes'=>array()));
 
  return $html;
}


function mnn_programming_exports_required_log_download() {
  require_once(TCPDF_ENG_LANG_PATH);
  require_once(TCPDF_PATH);

  $html_content = $_SESSION['mnn_req_media_output'];
  $date = $_SESSION['mnn_req_media_date'];
  if (!$html_content) {
    return "Sorry, something bad happend, error code 45322";
  }
  // create new PDF document
  $pdf = new TCPDF('Landscape', PDF_UNIT, PDF_PAGE_FORMAT, true); 

  // set document information
  $pdf->SetCreator(PDF_CREATOR);
  $pdf->SetAuthor("MNN");
  $pdf->SetTitle("Required Media Log");
  $pdf->SetSubject("Required Media Log");
  $pdf->SetKeywords("TCPDF, PDF, example, test, guide");

  $pdf->SetHeaderData(NULL,NULL,'Required Media Log', 
		      date('l, F j, Y', $date));

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

  $pdf->SetFont("courier", "", 9);
  // add a page
  $pdf->AddPage();

  //require_once('./mnn_req_media_log.inc.php');
  $pdf->setDefaultTableColumns(9);
  $pdf->setCellHeightRatio(1.0);
  $pdf->writeHTML($html_content, false, 0, false, 0);

  $pdf->lastPage();

  //Close and output PDF document
  $pdf->Output("mnn_req_media_log.pdf", "I", "I");
}


