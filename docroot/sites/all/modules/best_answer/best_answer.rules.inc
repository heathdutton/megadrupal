<?php

/**
* Implementation of hook_rules_event_info().
* @ingroup rules
*/
function best_answer_rules_event_info() {
  $rules = array();

  $rules['best_answer_set'] = array(
    'label'      => t('A best answer has been selected'),
    'group'      => t('Best Answer'),
    'variables'  => array(
      'question'   => array('type' => 'node', 'label' => t('The question which the answer addressed')),
      'answer'     => array('type' => 'node', 'label' => t('The answer selected')),
    ),
  );

  $rules['best_answer_unset'] = array(
    'label'     => t('A prior best answer has been unselected'),
    'group'      => t('Best Answer'),
    'variables' => array(
      'question'  => array('type' => 'node', 'label' => t('The question which the answer addressed')),
      'answer'    => array('type' => 'node', 'label' => t('The answer that was unselected')),
    ),
  );

  return $rules;

}
