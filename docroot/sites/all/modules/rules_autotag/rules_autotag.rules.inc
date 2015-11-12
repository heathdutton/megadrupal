<?php

/**
* Implements hook_rules_action_info().
*/
function rules_autotag_rules_action_info() {
  return array(
    'rules_autotag_extract' => array(
      'base' => 'rules_autotag_extract_action',
      'label' => t('Extract tags'),
      'group' => t('Rules Autotag'),
      'parameter' => array(
        'text' => array('type' => 'text', 'label' => t('Text to extract terms from')),
        'vocabulary' => array('type' => 'taxonomy_vocabulary', 'label' => t('Vocabulary used for extraction')),
      ),
      'provides' => array(
        'extraced_terms' => array(
          'type' => 'list<taxonomy_term>',
          'label' => t('Extracted terms'),
        ),
      ),
    ),
  );
}

/**
 * Rules action for extracting terms.
 */
function rules_autotag_extract_action($text, $vocabulary) {
  $extracted_tids = rules_autotag_extract($text, $vocabulary);
  return array('extraced_terms' => $extracted_tids);
}