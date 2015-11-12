<?php

////////////////////////////////////////////////////////////////////////////
//FORM TO GENERATE PDFS FOR A SEASON OF ACTIVE SERIES PROJECT(s)
function mnn_programming_exports_quarter_pdfs_form($form, &$form_state,
						   $project=NULL) {
  $quarters = mnn_programming_exports_quarter_field_options();

  if ($project) {
    $var = 'mnn_pnid_'.$project->nid."_airing_data";
    $airing_data = $_SESSION[$var]; 

    $var = 'mnn_pnid_'.$project->nid."_project_data";
    $project_data = $_SESSION[$var]; 

    $var = 'mnn_pnid_'.$project->nid."_fieldset_header";
    $fieldset_header = $_SESSION[$var];

    $var = 'mnn_pnid_'.$project->nid."_links";
    $links = $_SESSION[$var];

    $form['project_nid'] = array(
				 '#type' => 'hidden',
				 '#value' => $project->nid,
				 );
  }
  else {
    $airing_data = $_SESSION['mnn_project_airing_data']; 
    $project_data = $_SESSION['mnn_active_series_project_data']; 
    $fieldset_header = $_SESSION['mnn_project_pdf_fieldset_header'];
    $links = $_SESSION['mnn_project_pdf_links'];
  }
    
  if ($project_data) {
    $form['download_fieldset'] = array(
				       '#type'=>'fieldset',
				       '#title'=>$fieldset_header,
				       );
  
    if (count($airing_data) == 0) {
      $form['download_fieldset']['no_results'] = 
	array(
	      '#type' => 'item',
	      '#title'=>t("There are no airings for this quarter."),
	      );
    }
    else {
      foreach ($links as $type=>$dest) {
	$form['download_fieldset'][$type] = 
	  array(
		'#type' => 'item',
		'#title' => l($type,$dest),
		);
      }
    }
  }
  $form['select_quarter_form_fieldset'] = array(
					     '#type'=>'fieldset',
					     '#title'=>"Generate PDF's",
					     );
  $form['select_quarter_form_fieldset']['header'] = array(
			  '#type' => 'item',
			  
			  '#markup' => 
			  t('Select a quarter from the dropdown below and hit
                             submit to generate downloadable PDFs of letters 
                             and labels.'),
			  );


  $form['select_quarter_form_fieldset']['quarter'] = array(
			    '#type' => 'select',
			    '#title' => t('Quarter'),
			    '#options'=> $quarters,
			    );
  $form['submit'] = array(
			  '#type' => 'submit',
			  '#value' => 'Submit',
			  );
  return $form;
}
/////////////////////////////////////////////////////////////////////////////
//FORM SUBMIT FUNCTION FOR SELECTING A QUARTER TO GENEATE PDFS FOR
function mnn_programming_exports_quarter_pdfs_form_submit($form, 
							  &$form_state){
  //global $mnn_task_time;
  //$mnn_task_time = strtotime('now');
  //dsm((strtotime('now') - $mnn_task_time)/60);

  $project_data = array();
  $airing_data = array();
  $links = array();

  $date_string = $form_state['input']['quarter'];
  $dates = explode('--',  $date_string);
  $start = strtotime(array_shift($dates));
  $end = strtotime(array_shift($dates));
  $submitted_project_id = $form_state['input']['project_nid'];
  //set date string to be from now or the start of the submitted quarter, 
  //whatever is earliest
  if (strtotime(date('r')) < $start) {
    $date_string = date('Y-m-d').'--'.date('Y-m-d', $end);
  } 

  //fetch to get all airings for a quarter
  $view = views_get_view(MNN_QUARTERLY_AIRINGS_VIEW);
  
  //SET ARGUMENTS
  $args = array($date_string);
  if ($submitted_project_id) {
    $args[] = $submitted_project_id ;
  }
  $view->set_arguments($args);

  //execute view
  $view->execute();
  $results = $view->result;
  
  mnn_programming_exports_set_project_pdf_data($results,
					       $project_data,
					       $airing_data,
					       $date_string,
					       $start);
  
  if ($submitted_project_id) {
    $links['Download Show Labels PDF'] = 
      "node/".$submitted_project_id."/pdfs/labels/show/download";
    $links['Download Address Labels PDF'] = 
      "node/".$submitted_project_id."/pdfs/labels/address/download";
    $links['Download Letter PDF'] = 
      "node/".$submitted_project_id."/pdfs/letter/download";

    $var = 'mnn_pnid_'.$submitted_project_id."_project_data";
    $_SESSION[$var] = $project_data;

    $var = 'mnn_pnid_'.$submitted_project_id."_airing_data";
    $_SESSION[$var] = $airing_data;
   
    $var = 'mnn_pnid_'.$submitted_project_id."_fieldset_header";
    $_SESSION[$var] = "Quarter: ".date('m/d/Y', $start)." to ".
      date('m/d/Y', $end);
    
    $var = 'mnn_pnid_'.$submitted_project_id."_links";
    $_SESSION[$var] = $links;
  }
  else {
    $links['Download Show Labels PDF'] = 
      "admin/projects/pdfs/labels/show/download";
    $links['Download Address Labels PDF'] = 
      "admin/projects/pdfs/labels/address/download";
    $links['Download Letters PDF'] = 
      "admin/projects/pdfs/letters/download";
    $fieldset_header = "Quarter: ".date('m/d/Y', $start)." to ".
      date('m/d/Y', $end);
    
    $_SESSION['mnn_project_airing_data'] = $airing_data; 
    $_SESSION['mnn_active_series_project_data'] = $project_data; 
    $_SESSION['mnn_project_pdf_fieldset_header'] = $fieldset_header;
    $_SESSION['mnn_project_pdf_links'] = $links;
  }
}
/////////////////////////////////////////////////////////////////////////////
//GET DATA FOR ALL SERIES AIRINGS AND PROJECTS IN A QUARTER
function mnn_programming_exports_set_project_pdf_data(&$result_airings,
						      &$project_data,
						      &$airing_data,
						      $date_string,
						      $start){

  $assigned_episode_number = 0;

  $tmp_dates=explode('--', $date_string);
  $date_string = date('Y-m-d', $start)."--".$tmp_dates[1];

  foreach($result_airings as $result) {
    unset($contact_id);
    $project = $result->field_field_airing_project_ref[0]['raw']['entity'];

    if (!$project_data[$project->nid] && $project) {
      $project_data[$project->nid] = array();
    

      civicrm_initialize();
      $results=civicrm_api("UFMatch","get", array ('version' => '3',
						   'sequential' =>'1', 
						   'uf_id' => $project->uid));
      if ($results['values']) {
	$uf_match = $results['values'][0];
	$contact_id = $uf_match['contact_id'];
      }
    
      if ($contact_id) {
	$results=civicrm_api("Contact","get", 
			     array (version => '3',
				    'sequential' =>'1', 
				    'contact_id' =>$contact_id));      
	if ($results['values']) {
	  $contact = $results['values'][0];
	  
	  $project_data[$project->nid]['display_name'] = 
	    $contact['display_name'];
	  $project_data[$project->nid]['contact_id'] = $contact_id;
	  $project_data[$project->nid]['street_address'] = 
	    $contact['street_address'];
	  $project_data[$project->nid]['supplemental_address'] = 
	    $contact['supplemental_address_1'];
	  $project_data[$project->nid]['city'] = $contact['city'];
	  $project_data[$project->nid]['state_province'] = 
	    $contact['state_province'];
	  $project_data[$project->nid]['postal_code'] = 
	    $contact['postal_code'];
	  $project_data[$project->nid]['project_id'] = $project->nid;
	  $project_data[$project->nid]['quarter'] = $date_string;
	  $project_data[$project->nid]['title'] = $project->title;

	  if (!$project_data[$project->nid]['street_address']) {
	    $user = user_load(array($project->uid));
	    drupal_set_message("WARNING No address for executive producer ".
			       l($user->name, 'user/'.$project->uid), 'ERROR');
	  }
	}
      }
      else {
	drupal_set_message("Missing contact for $project->nid!".
			   " Error Code 121-223", 'ERROR');
	return "ERROR!";
      }
    }
    $airdate = strtotime($result->field_field_airing_date[0]['raw']['value']);
    if ($result->field_field_airing_show_ref) {
      $show = $result->field_field_airing_show_ref[0]['raw']['entity'];
      $episode_number = $show->field_episode_number[LANGUAGE_NONE][0]['value'];
    }
    else {
      $episode_number = NULL;
    }
    
    $variable_name = 'assigned_project_episode_number_'.$project->nid;
    if (!$episode_number) {
      if (!$$variable_name) {
	$$variable_name = mnn_show_get_next_episode_number($project->nid);
      }
      else {
	$$variable_name++;
      }
      $episode_number = $$variable_name;
    }
    else {
      $$variable_name = ($episode_number > $$variable_name) ?
	$episode_number : $$variable_name;
    }

    //FIXME, SHOULD REMOVE ABOVE CODE AS WE SEEM TO NO LONGER NEED 
    //EPISODE NUMBERS
    $episode_number = '_____ (Office Use Only)';

    if ($airdate < $start) {
      continue;
    }


    $orig_channel = 
      $result->field_field_airing_channel[0]['rendered']['#markup'];

    $channel = str_replace('Channel ','CH.', $orig_channel);
      
    $project_title_line1 = substr($project->title, 0, 28);
    
    $project_title = $project->title;
    if (strlen($project_title) > 28) {
      $project_title_line1 .= '-';
      $project_title_line2 = substr($project_title, 28, 25);
      if (strlen(substr($project_title, 28)) > 25) {
	$project_title_line2 .= "...";
      }
    }
    else {
      $project_title_line2 = " ";
    }

    switch($project->field_runtime[LANGUAGE_NONE][0]['tid']) {
    case MNN_28_MINS:
      $runtime = "28 min";
      break;
    case MNN_58_MINS:
      $runtime = "58 min";
      break;
    case MNN_88_MINS:
      $runtime = "88 min";
      break;
    case MNN_118_MINS:
      $runtime = "118 min";
      break;
    default:
      $runtime = "";
      break;
    }
    
    $footer = date('m/d/Y', $airdate).' '.date('h:ia', $airdate).
      '  '.$channel.' '.str_replace('mins', 'min',$runtime);
    
    $airing_data[]= array (
			   'project_title_line1'=>$project_title_line1,
			   'project_title_line2'=>$project_title_line2,
			   'project_id' => $project->nid,
			   'episode_number' => $episode_number,
			   'footer' => $footer,
			   'runtime' => $runtime,
			   'channel' => $channel,
			   'orig_channel' => $orig_channel,
			   'airdate' => date('m/d/Y', $airdate),
			   'airtime' => date('h:i A', $airdate),
			   'title' => $project_title,
			   );
  }


}

/////////////////////////////////////////////////////////////////////////////
//UTIL FUNCTIONS
function mnn_programming_exports_quarter_to_views_arg($quarter) {
  $start = strtotime($quarter['start']);
  $end = strtotime($quarter['end']);
  
  return date('Y-m-d', $start)."--".date('Y-m-d', $end);
}
function mnn_programming_exports_quarter_to_option_label($quarter) {
  $start = strtotime($quarter['start']);
  $end = strtotime($quarter['end']);
  
  return date('m-d-Y', $start)." to ".date('m-d-Y', $end);
}
function mnn_programming_exports_quarter_field_options() {
  $quarters = array();
  $quarter = timespan_current('mnnquarter',date('Y-m-d'));
  $start = date('Y-m-d',strtotime($quarter['start']));
  $arg = mnn_programming_exports_quarter_to_views_arg($quarter);
  $label = mnn_programming_exports_quarter_to_option_label($quarter);
  $quarters[$arg] = $label;

  $quarter = timespan_next('mnnquarter',$start);
  $start = date('Y-m-d',strtotime($quarter['start']));
  $arg = mnn_programming_exports_quarter_to_views_arg($quarter);
  $label = mnn_programming_exports_quarter_to_option_label($quarter);
  $quarters[$arg] = $label;

  $quarter = timespan_next('mnnquarter',$start);
  $start = date('Y-m-d',strtotime($quarter['start']));
  $arg = mnn_programming_exports_quarter_to_views_arg($quarter);
  $label = mnn_programming_exports_quarter_to_option_label($quarter);
  $quarters[$arg] = $label;

  $quarter = timespan_next('mnnquarter',$start);
  $start = date('Y-m-d',strtotime($quarter['start']));
  $arg = mnn_programming_exports_quarter_to_views_arg($quarter);
  $label = mnn_programming_exports_quarter_to_option_label($quarter);
  $quarters[$arg] = $label;
  return $quarters;
}
