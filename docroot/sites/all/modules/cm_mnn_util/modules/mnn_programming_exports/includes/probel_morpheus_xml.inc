<?php

function mnn_programming_exports_probel_morpheus_xml() {  
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
    $results = mnn_programming_exports_process_results_for_xml($results);
    $output .=  mnn_programming_exports_playout_xml_links($results, $date);
  }
  $output .= mnn_programming_exports_widgets($view);
  
  return $output;
}

function mnn_programming_exports_playout_xml_links($results, $date) {
  $links = array();
  $chvid = mnn_programming_exports_get_vid_for_name('Channel');
  $channel_terms = taxonomy_get_tree($chvid);
  $dir_date = date('Y_m_d',$date);

  foreach ( $channel_terms as $channel ) {
    //the function that builds the morpheus playout can handle "secondary"
    //channels, but since we aren't using them currently there will only be
    //primary. check AC 1.0 
    //sites/default/modules/programming/inc/programming.playout.form.inc 
    //to see how it was originally build
    $channels = array();
    $channels['primary'] = $channel->name;
    $xmldata = 
      mnn_programming_exports_build_morpheus_schedule($results[$channel->name],
						      $channels);
    
    $dir = "sites/default/files/probel/schedules/".
      urlencode($channel->name)
      ."/$dir_date";
    @mkdir($dir, 0775, TRUE);
    $filename = $dir . "/" . $dir_date . "_" . 
      preg_replace("/ /", "_", $channel->name) ."_". "probel.sch";
    mnn_programming_exports_write_xml_playout_file($xmldata, $filename);
    $links['xml_'.$channel->name] = $filename;
  }
  $import_xmldata = 
    mnn_programming_exports_build_import_xml_file($results['all'], 
						  $channel_map);

  //build the import xml file and store a link to it
  $dir = "sites/default/files/probel/import/$dir_date";
  @mkdir($dir, 0775, TRUE);
  $filename = "$dir/import.xml";
  mnn_programming_exports_write_xml_playout_file($import_xmldata, $filename);
  $links['xml_morpheus_import'] = $filename;

  $ret = "";
  foreach ($links as $title=>$path) {
    $ret .= l("Download $title file (Opens in new window or tab)", 
	      $path, array('attributes'=>array('target'=> "_blank")))."<br/>";
  }
  
  return $ret;
}

function mnn_programming_exports_build_import_xml_file($results,$channel_map) {
  $xw = new ProBelMorpheusImport();
  $xw->openMemory();
  $xw->setIndent(TRUE);
  $xw->setIndentString("\t");
  $xw->startDocument('1.0', 'utf-8');

  $xw->writeImportData($results);
  $xw->endDocument();
  return $xw->flush();
}
function mnn_programming_exports_build_morpheus_schedule($results, $channels){
  $xw = new ProBelSchedule();
  $xw->openMemory();
  $xw->setIndent(TRUE);
  $xw->setIndentString("\t");
  $xw->startDocument('1.0', 'utf-8');
  $xw->startElement('Schedule');
  $xw->writeAttribute('Name', 'MNN Playout Schedule');

  // start of the large <Events> tag
  $xw->startElement('Events');

  $xw->writeAttribute('Channel', 
  mnn_programming_exports_xml_channel_attribute($channels['primary']));
  $xw->endAttribute();

  $xw->buildDataTree($results, $channels);

  $xw->writeTranslatedData();

  // </Events>
  $xw->endElement();
  // </Schedule>
  $xw->endElement();

  $xw->endDocument();

  return $xw->flush();
}


function mnn_programming_exports_process_results_for_xml(&$results) {
  $ret = array();

  foreach ($results as $result) {
    $data = array();
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

    if (!isset($result->field_field_airing_show_ref[0])) {
      drupal_set_message("Missing Show for airing: ".
			 l($result->airing_id,"airing/".$result->airing_id),
			 "error");
      continue;
    }
    $show = $result->field_field_airing_show_ref[0]['raw']['entity'];

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

    $livefeed_tid = $show->field_device[LANGUAGE_NONE][0]['tid'];
    if ($livefeed_tid) {
      $livefeed_name = taxonomy_term_load($livefeed_tid)->name;
    }

    $data['title'] = $project->title;
    $data['project_id'] = $project->nid;

    $data['field_timecode_in_value'] = 
      $show->field_timecode_in[LANGUAGE_NONE][0]['value'];
    $data['field_timecode_out_value'] = 
      $show->field_timecode_out[LANGUAGE_NONE][0]['value'];

    $data['field_transcoded_timecode_in_value'] = 
      $show->field_trans_timecode_in[LANGUAGE_NONE][0]['value'];
    $data['field_transcoded_timecode_out_value'] = 
      $show->field_trans_timecode_out[LANGUAGE_NONE][0]['value'];

    $data['ep_number'] = 
      $show->field_episode_number[LANGUAGE_NONE][0]['value'];


    $data['event_start'] = strtotime($date['value']);
    $data['event_end'] = strtotime($date['value2']);
    $data['channel_name'] = $channel;
    $data['format_name'] = $format_name;
    $data['format_tid'] = $format_tid;
    $data['msn'] = $project->field_project_msn[LANGUAGE_NONE][0]['value'];
    $data['ep_nid'] = $show->nid;

    $data['livefeed_tid'] = $livefeed_tid;
    $data['livefeed_name'] = $livefeed_name;
    $ret[$channel][]= $data;
    $ret['all'][] = $data;
  }
  return $ret;
}

function mnn_programming_exports_write_xml_playout_file($data, $filename) {
  $xmlfile = fopen($filename, "w+b");
  if ( fwrite($xmlfile, $data) == FALSE ) {
    drupal_set_message('Unable to write to file: ' . $filename .
                  ". Please ensure that the filesystem is writable
                  and has sufficient available space.", 'error');
    return FALSE;
  }
  if ( fclose($xmlfile) ) {
  // TODO: this code will perform schema validation
//    $domdoc = new DOMDocument();
//    $domdoc->load($filename);
//    $valid = $domdoc->schemaValidate("./scripts/Schedule new.xsd");
    return TRUE;
  }
  else {
    trigger_error("Error closing file " . $filename . ".");
    return FALSE;
  }
}


/**
 *  to control the purge date, set the variable morpheus_purge_date_offset in
 *   drupal's settings.php
 *
 *  default is 30 days
 */
function mnn_programming_exports_xml_calculate_purge_date() {
  $now = time();
  // offset is in *days*
  $purge_date_offset = variable_get('morpheus_purge_date_offset', 30);
  $offset = 24 * 60 * 60 * $purge_date_offset;
  $purge_time = $now + $offset;
  return date('m/d/Y', $purge_time);
}


function mnn_programming_exports_xml_device_map($key) {
  $map = array(
    'ARCH' => 'ARCH', // Arch-Diocese feed.
    'BKLN' => 'BCAT', // Brooklyn Cable Access
    'BRNX' => 'BRONXNET', // Bronxnet
    'DCTV' => 'DCTV', // Downtown Community Television
    'FSTV' => 'FSTV', // Free Speech Television
    'STI' => 'ST', // Staten Island Cable Access
    'QPTV' => 'QPTV', // Queens Cable Access
    'ST1' => 'STU1-1', // Open Studio
    'ST2' => 'STU2-1', // Closed Studio
    'ST3' => 'STU3', // 2nd flr studio
    'ST4' => 'STU4', // 2nd flr studio
    'DNOW' => 'DEMOCRACY NOW', //Democracy Now!
    'FH1' => 'FH1', //Firehouse 1
    'FH2' => 'FH1', //Firehouse 1
    'FH3' => 'FH1', //Firehouse 1
    'FH4' => 'FH1', //Firehouse 1
  );
  return $map[$key];

}


/* helpers for programming_xml_format_data()
 */

/*
 *  change something that looks like 123467890 to something that looks like:
 *    "13-Feb-2009 18:31:30:00 LOCAL"
 */
function mnn_programming_exports_xml_StartTimeOffset($timestamp, $data) {
  if ( ! $data['#is_child'] ) {
    $month = date('F', $timestamp);
    $mon = strtoupper(substr($month, 0, 3));
    $day = date('d', $timestamp);
    $year = date('Y', $timestamp);
    $cal = $day ."-". $mon ."-". $year;
    $time = date('H:i:s', $timestamp);
    return "$cal $time;00 LOCAL";
  }
  else {
    return mnn_programming_exports_xml_Duration($timestamp);
  }
}

function mnn_programming_exports_xml_Duration($dur) {
  return mnn_programming_exports_theme_tape_time_colons($dur) .";00";
}

function mnn_programming_exports_xml_PlayoutDevice($chname) {
  //FIXME: these values were guessed
  $dict = array(
    'Channel 1' => 'MNN_Player_Ch1',
    'Channel 2' => 'MNN_Player_Ch2',
    'Channel 3' => 'MNN_Player_Ch3',
    'Channel 4' => 'MNN_Player_Ch4',
  );
  return $dict[$chname];
}

function mnn_programming_exports_xml_Type($chname) {
  return ( mnn_programming_exports_xml_channel_attribute($chname) . " Default Main Event" );
}

function mnn_programming_exports_xml_MaterialId($proj_id, $d) {
  if ( $d['#is_child'] ) {
    return NULL;
  }
  if ( $d['ep_number'] ) {
    return $proj_id ."_". $d['ep_number']; // "project_id.ep_number"
  }
  else {
    return NULL;
  }
}

function mnn_programming_exports_xml_Material_Id($proj_id, $d) {
  if ( $d['ep_number'] ) {
    return $proj_id .".". $d['ep_number']; // "project_id.ep_number"
  }
  else {
    return NULL;
  }
}

// just a series of digits: HHiiss00 (e.g. 00123000 for 12 min. 30 sec.)
function mnn_programming_exports_xml_Timecode_In($date) {
  return mnn_programming_exports_xml_format_timecode_for_morpheus($date);
}

function mnn_programming_exports_xml_Timecode_Out($date) {
  return mnn_programming_exports_xml_format_timecode_for_morpheus($date);
}

function mnn_programming_exports_xml_On_Air_Duration($date) {
  return mnn_programming_exports_xml_format_timecode_for_morpheus($date);
}

function mnn_programming_exports_xml_format_timecode_for_morpheus($date) {
  $time = gmdate('His', $date);
  return $time .'00';
}

function mnn_programming_exports_xml_channel_attribute($ch_term_name) {
  $dict = array(
    'Channel 1' => 'MNN 1 - Community',
    'Channel 2' => 'MNN 2 - Lifestyle',
    'Channel 3' => 'MNN 3 - Spirit',
    'Channel 4' => 'MNN 4 - Culture',
  );
  return $dict[$ch_term_name];
}

function mnn_programming_exports_xml_chyron_type($ch_term_name) {
  //FIXME: does this need to be a FullyQualifiedType instead of a Type?
  //FIXME: why are they all "Community"?
  $dict = array(
    'Channel 1 Chyron' =>
      'MNN 1 Community DSK1 &amp; CHY',

    'Channel 2 Chyron' =>
      'MNN 2 Community DSK1 &amp; CHY',

    'Channel 3 Chyron' =>
      'MNN 3 Community DSK1 &amp; CHY',

    'Channel 4 Chyron' =>
      'MNN 4 Community DSK1 &amp; CHY',
  );
  return $dict[$ch_term_name];
}

function mnn_programming_exports_xml_Material_Type($format_term_name) {
  $dict = array(
    'LIVE' => 'Live',
  );
  return $dict[$format_term_name];
}

function mnn_programming_exports_xml_EventName($title) {
  return strtoupper($title);
}

function mnn_programming_exports_xml_MainVideoSource($device_term_name) {
  $dict = array(
    'ARCH' => 'ARCH', // Arch-Diocese feed.
    'BKLN' => 'BCAT', // Brooklyn Cable Access
    'BRNX' => 'BRONXNET', // Bronxnet
    'DCTV' => 'DCTV', // Downtown Community Television
    'FSTV' => 'FSTV', // Free Speech Television
    'STI' => 'ST', // Staten Island Cable Access
    'QPTV' => 'QPTV', // Queens Cable Access
    'ST1' => 'STU 1-1', // Open Studio
    'ST2' => 'STU 2-1', // Closed Studio
    'ST3' => 'STU 3', // 2nd flr studio
    'ST4' => 'STU 4', // 2nd flr studio
    'DNOW' => 'DEMOCRACY NOW', //Democracy Now!
    'FH1' => 'FH1', //Firehouse 1
    'FH2' => 'FH2', //Firehouse 2
    'FH3' => 'FH3', //Firehouse 3
    'FH4' => 'FH4', //Firehouse 4
  );
  return $dict[$device_term_name];
}

function mnn_programming_exports_xml_MainAudioSource($device_term_name) {
  return mnn_programming_exports_xml_MainVideoSource($device_term_name);
}
/* end helpers for programming_xml_format_data()
 */

function mnn_programming_exports_theme_tape_time_colons($t) {
  mnn_programming_exports_broken_up_timstamp_display($t, $h, $i, $s);
  return implode(":", array(
			    str_pad($h, 2, '0', STR_PAD_LEFT),
			    str_pad($i, 2, '0', STR_PAD_LEFT),
			    str_pad($s, 2, '0', STR_PAD_LEFT),
			    )
		 );
}

function mnn_programming_exports_broken_up_timstamp_display($timecode, 
							    &$default_hour, 
							    &$default_min, 
							    &$default_sec) {
  $default_hour = floor($timecode/(60*60));
  $timecode = $timecode%(60*60);
  $default_min = floor($timecode/(60));
  $timecode = $timecode%60;
  $default_sec = $timecode;
}

