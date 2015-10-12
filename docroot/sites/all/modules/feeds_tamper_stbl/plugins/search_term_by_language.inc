<?php

/**
 * @file
 * Search term by language.
 */

$plugin = array(
  'form' => 'feeds_tamper_search_term_by_language_form',
  'callback' => 'feeds_tamper_search_term_by_language_callback',
  //'validate' => 'feeds_tamper_search_term_by_language_validate',
  'name' => 'Search term by language',
  'multi' => 'loop',
  'category' => 'Text',
);

function feeds_tamper_search_term_by_language_form($importer, $element_key, $settings) {
  $form = array();
  
  $vocabularies = taxonomy_get_vocabularies();
  $options = array();
  foreach ($vocabularies as $vocabulary) {
    $options[$vocabulary->vid] = $vocabulary->name;
  }
  $form['vocabulary'] = array(
    '#type' => 'select',
    '#title' => t('Vocabulary'),
    '#default_value' => isset($settings['vocabulary']) ? $settings['vocabulary'] : '',
    '#options' => $options,
    '#required' => TRUE,
    '#description' => t('The vocabulary which this field attached to.'),
  );
  
  $form['language'] = array(
    '#type' => 'textfield',
    '#title' => t('Language'),
    '#default_value' => isset($settings['language']) ? $settings['language'] : '',
	'#required' => TRUE,
  );
/*
  $form['auto_create'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto create'),
    '#default_value' => isset($settings['auto_create']) ? $settings['auto_create'] : FALSE,
    '#description' => t('If checked, auto creating a term base on field value.')
  );
*/
  return $form;
}


function feeds_tamper_search_term_by_language_callback($result, $item_key, $element_key, &$field, $settings) {
  //drupal_set_message(var_export($field, TRUE));
	$query  = db_select('locales_target', 'lt');
	$query->join('locales_source', 'ls', 'lt.lid = ls.lid');
	$query->join('taxonomy_term_data', 'ttd', 'ls.source = ttd.name');
	$query->fields('ttd', array(tid));
	$query->condition('lt.translation', $field);
	$query->condition('lt.language', $settings['language']);
	$query->condition('ttd.vid', $settings['vocabulary']);
	$tid = $query->execute()->fetchField();
	$field = $tid;
}
