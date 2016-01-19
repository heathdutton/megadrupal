<?php 

/**
 * @file
 * Sets the page title for online petition page reports.
 */
// define the page title
$plugin = array(
  'single' => TRUE,  
  'title' => t('Petition Page: Set Page Title'),  
  'description' => t('Simply sets the page title when being used on a petition page report.'), 
  'category' => t('CRM Core Petition'), 
  'edit form' => 'crm_core_petition_set_page_page_title_form', 
  'render callback' => 'crm_core_petition_set_page_page_title', 
  'admin info' => 'crm_core_petition_set_page_page_title_info', 
  'defaults' => array(),
  'all contexts' => TRUE, 
);

/**
 * admin info
 */
function crm_core_petition_set_page_page_title_info ($subtype, $conf, $contexts){
  $block = new stdClass;
  $block->title = t('Petition Page Report: Set Page Title');
  $block->content = t('Sets the page title within a panel.');
  return $block;
}

/**
 * Settings form
 */
function crm_core_petition_set_page_page_title_form ($form, &$form_state){
  $conf = $form_state['conf'];
  return $form;
}
/**
 * Validation for settings form
 */
function crm_core_petition_set_page_page_title_form_submit ($form, &$form_state){  
}

/**
 * Render callback
 */
function crm_core_petition_set_page_page_title($subtype, $conf, $panel_args, $context = NULL){
  
  $title = '';
  
  // we want to use this to handle various situations
  if(!isset($panel_args[0])){
    $title = 'Petition summary';
  } else if(is_numeric($panel_args[0])) {
    $sql = "SELECT title from {node} WHERE nid = :nid";
    $check = db_query($sql, array(':nid' => $panel_args[0]));
    foreach ($check as $item){
      $title = 'Petition summary for <em>' . $item->title . '</em>';
    }
  } else {
    $title = 'Petition summary for <em>' . filter_xss($panel_args[0]) . '</em>';
  }
  
  drupal_set_title($title, PASS_THROUGH);

  $block = new stdClass();
  $block->title = '';
  $block->content = '';
  
  return $block;
  
}








