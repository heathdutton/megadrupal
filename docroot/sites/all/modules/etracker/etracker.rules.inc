<?php

/**
 * Implements hook_rules_action_info().
 */
function etracker_rules_action_info() {
  return array(
    'etracker_action_pagename_set' => array(
      'label' => t('Set page name'),
      'parameter' => array(
        'title' => array(
          'type' => '*',
          'label' => t('Page name'),
        ),
      ),
      'group' => 'etracker',
    ),
    'etracker_action_area_set' => array(
      'label' => t('Set area'),
      'parameter' => array(
        'area' => array(
          'type' => '*',
          'label' => t('Area'),
        ),
      ),
      'group' => 'etracker',
    ),
    'etracker_action_target_set' => array(
      'label' => t('Set target'),
      'parameter' => array(
        'area' => array(
          'type' => 'text',
          'label' => t('Target'),
        ),
      ),
      'group' => 'etracker',
    ),
    'etracker_action_target_append' => array(
      'label' => t('Set a subsequent target'),
      'parameter' => array(
        'area' => array(
          'type' => 'text',
          'label' => t('Target'),
        ),
      ),
      'group' => 'etracker',
    ),
    'etracker_action_target_history_reset' => array(
      'label' => t('Reset target history'),
      'group' => 'etracker',
    ),
    'etracker_action_interest_level_set' => array(
      'label' => t('Set interest level'),
      'parameter' => array(
        'area' => array(
          'type' => 'text',
          'label' => t('Visitor interest level'),
        ),
      ),
      'group' => 'etracker',
    ),
  );
}
