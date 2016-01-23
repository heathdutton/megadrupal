<?php
// $Id: simple_contest_repeat.form.inc,v 1.1.2.1 2010/07/09 04:36:13 vinoth.3v Exp $

/**
 * @file
 *
 * simple_contest_repeat custom module
 * mostly adopted from date/date/date_repeat.inc, thanks to karens.
 */

/**
 * Implementation of hook_form_FORM_ID_alter().
 */
function _simple_contest_repeat_content_type_settings_form_alter(&$form, &$form_state) {

  if ( !$form['is_sc']['#default_value'] ) {
    return;
  }
  
  $type = $form['type']['#value'];
  
  $settings = simple_contest_get_settings($type);

  module_load_include('inc', 'simple_contest_repeat', 'simple_contest_repeat.helper');
  //$def_node = simple_contest_repeat_get_def_node($type, TRUE); // always refresh here?
  
  $rrule_desc = 'None';
  if (isset($settings['rrule']) && $settings['rrule']) {
    $rrule_desc = date_repeat_rrule_description($settings['rrule']);
  }
  
  $form['sc']['repeat'] = array(
    '#type'        => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
    '#title'       => t('Contest repeat scheduler'),
    '#group'       => 'additional_settings',
    '#description' => t('You can set the repeat rules below to schedule the creation of contests automatically.
    <br><b>Current state: ' . $rrule_desc. '</b>
    '),
    '#tree' => TRUE,
  );

  //The default node template for this contest is !node.", array('!node' => l($def_node->title, 'node/' . $def_node->nid))  

  $form['sc']['repeat']['schedule'] = array(
    '#type' => 'container',
  );
  
  // date repeate theme function needs this naming. :)
  $form['sc']['repeat']['schedule']['options']['show_repeat_settings'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable contest auto create using repeat rule.'),
    '#default_value' => $settings['auto_create'],
    '#options' => array(0 => 0, 1 => 1),
    '#required' => FALSE,
  );
  
  // time zone
  $tz = '';
  if($settings['tz_handling'] != ''){
    $tz = date_get_timezone_db($settings['tz_handling']);
  }
  
  $form['sc']['repeat']['schedule']['options']['rrule'] = array(
    '#type' => 'date_repeat_rrule',
    '#theme_wrappers' => array('date_repeat_rrule'),
    '#default_value' => isset($settings['rrule']) ? $settings['rrule'] : '',
    '#date_timezone' => $tz,
    '#date_format'      => date_limit_format('Y-m-d', array('year', 'month', 'day')),
    '#date_text_parts'  => array(),
    '#date_increment'   => 1,
    '#date_year_range'  => '-1:+3',
    '#date_label_position' => 'within',
    '#prev_value' => isset($settings['rrule']['value']) ? $settings['rrule']['value'] : '',
    '#prev_value2' => isset($settings['rrule']['value2']) ? $settings['rrule']['value2'] : '',
    '#prev_rrule' => isset($settings['rrule']) ? $settings['rrule'] : '',
    '#date_repeat_widget' => module_exists('date_popup') ? 'date_popup' : 'date_select',
    '#date_repeat_collapsed' => TRUE,
    '#date_flexible' => 0,
  );

  $options = array(
    'site' => t("Site's time zone"),
    'date' => t("Date's time zone"),
    'user' => t("User's time zone"),
    'utc' => 'UTC',
    '' => t('No time zone conversion'),
  );

  $form['sc']['repeat']['schedule']['tz_handling'] = array(
    '#type' => 'select',
    '#title' => t('Time zone handling'),
    '#default_value' => $settings['tz_handling'],
    '#options' => $options,
    '#description' => t('Select the timezone handling method to be used for this date.'),
  );
  
  /*$form['sc']['repeat']['schedule']['contest_defaults_nid'] = array(
    '#value' => $def_node->nid,
    '#type' => 'hidden',
  );*/
  
  /*if (!empty($settings['contest_defaults'])) {
    $node = (object) $settings['contest_defaults'];
  } else {
    $node = (object) array(
      'type' => $type, 
      'language' => LANGUAGE_NONE,
    );
  }
  
  $node_form_state['build_info']['args'] = array($node);
  //$node_form_state['build_info']['base_form_id'] = $form_state['build_info']['#form_id'];
  
  //$node_form_state['no_cache'] = TRUE; 
  //$node_form_state['groups'] = array();
  form_load_include($node_form_state, 'inc', 'node', 'node.pages');
  
  $node_form_id = $type . '_node_form';
  
  $node_form = drupal_retrieve_form($node_form_id, $node_form_state);
  
  $form_state = array_merge($form_state, $node_form_state);
  
  $unset = array('form_token', '#form_id', 'form_build_id',
    //'type', '#type', '#entity_type', '#bundle', 
    //'#node', '#node_edit_form', 
    //'#build_id', //'#pre_render', '#after_build', 
    'actions', '#submit', '#validate', 
    '#id', '#action', '#method',
    //'nid', 'vid', 'uid', 
    'menu', 'path', 'author', 'options', 
    //'#theme', //'#theme_wrappers', 
    'comment_settings', 'additional_settings', 'revision_information',
    '#array_parents', '#processed', '#parents', 
  );
  
  foreach($unset as $key) {
    if (isset($node_form[$key])) {
      if (is_array($node_form[$key])) {
        $node_form[$key]['#access'] = FALSE;
      } else {
        unset($node_form[$key]);
      }
    }
  }
  
  // remove date field
  if (isset($node_form[$settings['date_field']])) {
    $node_form[$settings['date_field']]['#access'] = FALSE;
  }
  
  
  $form['sc']['repeat']['def_node']['contest_defaults'] = $node_form;
  */
  
  $form['sc']['repeat']['contest_defaults'] = array(
    '#type'        => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
    '#title'       => t('Contest template defaults'),
    '#group'       => 'template',
    '#description' => t('Following values will be used while creating contests automatically.'),
    '#tree' => TRUE,
  );
  
  $form['sc']['repeat']['contest_defaults']['node_title'] = array(
    '#type' => 'textfield',
    '#title'  => t('Contest node title'),
    '#default_value' => isset($settings['contest_defaults']['node_title'])?$settings['contest_defaults']['node_title']:'',
    '#required' => TRUE,
  );
  
  $form['sc']['repeat']['contest_defaults']['node_body'] = array(
    '#type' => 'textarea',
    '#title'  => t('Contest node body'),
    '#default_value' => isset($settings['contest_defaults']['node_body'])?$settings['contest_defaults']['node_body']:'',
    '#required' => TRUE,
  );
  
  $form['#validate'][] = 'simple_contest_repeat_date_repeat_widget_validate';
  $form['#submit'][] = 'simple_contest_repeat_date_repeat_widget_submit';

}

/**
 * Validation for date repeat form element.
 *
 * Create multiple values from the RRULE results.
 * 
 */
function _simple_contest_repeat_date_repeat_widget_validate($form, &$form_state) {
  
  $form_values = $form_state['values'];

  $form_state['values']['tz_handling'] = $form_values['repeat']['schedule']['tz_handling'];
  $form_state['values']['contest_defaults'] = $form_values['repeat']['contest_defaults'];
  $form_state['values']['auto_create'] = $form_values['repeat']['schedule']['options']['show_repeat_settings'];
  
  if (!$form_values['repeat']['schedule']['options']['show_repeat_settings']) {
    return;
  }

  $input = $form_state['input'];
    
  $rrule_values = $input['repeat']['schedule']['options']['rrule'];
  //print_r($rrule_values);
  //exit();
  $error_field_base = 'repeat][schedule][options';
  $error_field_until =  $error_field_base . '][rrule][until_child][datetime][';
  if ($rrule_values['range_of_repeat'] === 'UNTIL' && empty($rrule_values['UNTIL']['datetime'])) {
    if (module_exists('date_popup')) {
      form_set_error($error_field_until . 'date', t("Missing value in 'Range of repeat'. (UNTIL).", array(), array('context' => 'Date repeat')));
    } else {
      form_set_error($error_field_until . 'year', t("Missing value in 'Range of repeat': Year (UNTIL)", array(), array('context' => 'Date repeat')));
      form_set_error($error_field_until . 'month', t("Missing value in 'Range of repeat': Month (UNTIL)", array(), array('context' => 'Date repeat')));
      form_set_error($error_field_until . 'day', t("Missing value in 'Range of repeat': Day (UNTIL)", array(), array('context' => 'Date repeat')));
    }
  }

  $error_field_count = $error_field_base . '][rrule][count_child';
  if ($rrule_values['range_of_repeat'] === 'COUNT') {
    if (empty($rrule_values['count_child'])) {
      form_set_error($error_field_count, t("Missing value in 'Range of repeat'. (COUNT).", array(), array('context' => 'Date repeat')));
    } else if (!is_numeric($rrule_values['count_child']) || intval($rrule_values['count_child']) <=0) {
      form_set_error($error_field_count, t("'Range of repeat'. (COUNT) ust be integer value.", array(), array('context' => 'Date repeat')));
    }
  }

  if (form_get_errors()) {
    return;
  }
  
  $form_state['values']['rrule'] = $form_values['repeat']['schedule']['options']['rrule'];
    
}

/**
 * Submit handler for simple_contest_repeat_date_repeat_widget
 *
 */
function _simple_contest_repeat_date_repeat_widget_submit($form, &$form_state) {

  $values = $form_state['values'];
  
  if (isset($values['auto_create']) && $values['auto_create']) {
  
    $type = $values['type'];
    
    $parents = array('repeat','schedule','options');
    
    $input = drupal_array_get_nested_value($form_state['input'], $parents, $input_exists);
    
    $rrule_values = date_repeat_merge($input['rrule'], array('#date_repeat_widget' => module_exists('date_popup') ? 'date_popup' : 'date_select' ));
    
    module_load_include('inc', 'simple_contest_repeat', 'simple_contest_repeat.helper');
    simple_contest_repeat_date_repeat_build_dates($type, $values, $values['rrule'], $rrule_values);
  }
  
}

