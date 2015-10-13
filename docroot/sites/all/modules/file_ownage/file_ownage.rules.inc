<?php

/**
 * @file
 * RULES based processing for processing embedded items.
 *
 * There are so many branches and ways this logic could go,
 * I'll expose the logic through rules.
 *
 * An example.
 * - When an entity is saved
 * -> scan the body text field for embeds
 * = hook_action_info() makes this available?
 *
 * This may trigger another event
 * - When a text field is being scanned
 * or
 * - When an element is found during a text scan
 * -> then do things to that domelement.
 *
// hook_rules_action_info
// hook_rules_condition_info
// hook_rules_data_type_info *
// hook_rules_event_info
 *
 */

/**
 * Advertise an event that fires when a suitable domelement element is found.
 *
 * Implements hook_rules_event_info().
 * Docs at: http://drupal.org/node/298549
 */
function file_ownage_rules_event_info() {

  $events = array();
  $events['embed_element_found'] = array(
    'label' => 'An embedded file link was found in the text',
    'group' => 'File Ownage',
    'variables' => array(
      'element' => array(
        'label' => 'Found DOM element',
        'type' => 'domelement',
        'description' => '',
        'options list' => array(),
      ),
      'entity' => array(
        'label' => 'Container entity',
        'type' => 'entity',
        'description' => '',
        'options list' => array(),
      ),
    ),
  );
  return $events;
}


/**
 * Implements hook_trigger_info().
 */
function file_ownage_trigger_info() {

  return array(
    'embed' => array(
      'embed_update' => array(
        'label' => t('An embedded link is found in the text'),
      ),
    ),
  );
}


/**
 * Advertises things that can be triggered through rules.
 *
 * Implements hook_rules_action_info().
 */
function file_ownage_rules_action_info() {

  $return['scan_text_for_markup'] = array(
    'label' => t('Scan text for markup'),
    'group' => t('Markup'),
    // Input and config values expected to be provided to this rule.
    'parameter' => array(
      'fieldvalue' => array(
        'type' => '*',
        'label' => t('Text Field'),
        'description' => t('Specifies the data field to be modified using a data selector, e.g. "node:body:value".'),
        'restriction' => 'selector',
        'wrapped' => TRUE,
        'allow null' => TRUE,
        'default mode' => 'selector',
      ),
      'elementtype' => array(
        'type' => 'text',
        'label' => t('Markup tag'),
        'description' => t('Tag to scan the markup for, eg "img".'),
        'allow null' => FALSE,
        'optional' => FALSE,
        'default mode' => 'input',
        'restriction' => 'input',
      ),
    ),
    // Data made available BY this rule for further processing.
    'provides' => array(
      'domelements' => array(
        'type' => 'list<domelement>',
        'label' => 'HTML elements',
        'description' => 'HTML tags extracted from the text',
      )
    ),
  );
  return $return;
}



/**
 * Settings for the scan embeds process.
 */
function file_ownage_scan_embeds_action_form() {

  dpm(func_get_args());

  $form = array();

  return $form;
}


/**
 * Advertise a new data type object that we take as an argument and manipulate.
 *
 * We deal with a new data type
 * - a domelement element found during text processing.
 *
 * By declaring a domelement, then the domelement becomes a 'thing'
 * that can be passed around and referred to throughout the Rules pipeline.
 *
 * Implements hook_rules_data_type_info().
 */
function file_ownage_rules_data_info() {

  return array(
    'domelement' => array(
      'label' => t('HTML Element'),
      'savable' => FALSE,
      'identifiable' => FALSE,
    ),
    'list<domelement>' => array(
      'label' => t('list of HTML elements'),
      #'ui class' => 'RulesDataUIListText',
      'wrap' => TRUE,
      'group' => t('List', array(), array('context' => 'data_types')),
    ),

  );
}

