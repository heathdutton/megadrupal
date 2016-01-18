<?php
// $Id: simple_contest.pages.inc,v 1.1.2.1 2010/07/09 04:36:13 vinoth.3v Exp $

/**
 * @file
 * user pages for simeple contest
 */


/**
 * simple_contest_finalize_page
 */
function simple_contest_finalize_page($node) {
  $output = array();

  drupal_set_title(t('Choose winners for @title', array('@title' => $node->title)));

  if (!simple_contest_is_simple_contest($node)){
    return $output;
  }

  $settings = simple_contest_get_settings($node->type);
  $status = $node->simple_contest['status'];

  // status
  $output['status'] = array(
		'#markup' => "<h2>" . t('Contest status') . "</h2>" . t(simple_contest_status_name($status)), 
		'#weight' => 0,
  );


  // winners
  if($node->simple_contest['status'] == SIMPLE_CONTEST_STATUS_COMPLETED){
      
    $results_display_view = $settings['results_display_view'];
    $results = views_embed_view($results_display_view, 'default', $node->nid);
      
    $output['winners'] = array(
			'#markup' => theme('simple_contest_results_display', array(
          'node' => $node, 
          'results' => $results,
        )), 
			'#weight' => 1,
    );
  }
  // simple_contest_choose_winners_form
  if($node->simple_contest['status'] == SIMPLE_CONTEST_STATUS_ACTIVE){
    $output['selector'] = array(
			'#markup' => simple_contest_choose_winners_form($node), 
			'#weight' => 2,
    );
  }
  
  return $output;
}

function simple_contest_choose_winners_form($node){

  $settings = simple_contest_get_settings($node->type);

  $view_name = $settings['view'];
  
  $view = views_get_view($view_name, FALSE);
  $view->override_path = 'node/' . $node->nid . '/simple_contest';
  
  $args = array($node->nid);
  
  $date_field = $settings['date_field'];
  if (isset($node->{$date_field}) && !empty($node->{$date_field}['und'][0]['value']) && !empty($node->{$date_field}['und'][0]['value2'])) {
    $args[] = urlencode($node->{$date_field}['und'][0]['value']);
    $args[] = urlencode($node->{$date_field}['und'][0]['value2']);
  }
  
  $output = $view->execute_display('default', $args);
  
  return $output;

}

