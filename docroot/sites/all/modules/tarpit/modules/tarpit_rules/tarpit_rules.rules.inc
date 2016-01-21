<?php

/**
 * Implements hook_rules_event_info().
 */
function tarpit_rules_rules_event_info() {
  return array(
    'tarpit_view' => array(
      'label' => 'When someone is viewing a tarpit.',
      'group' => 'Tarpit',
    ),
    'tarpit_reaction' => array(
      'label' => 'When someone crossed the maximum depth of links in the tarpit.',
      'group' => 'Tarpit',
    )

  );
}

/**
 * Implements hook_rules_action_info().
 */
function tarpit_rules_rules_action_info() {
  return array(
    'tarpit_sleep' => array(
      'label' => t('Slow down the render of the page.'),
      'base' => 'tarpit_rules_rules_action_sleep',
      'parameter' => array(
        'sleep_min' => array(
          'type' => 'integer',
          'label' => t('Sleep minimum time in second'),
          'save' => TRUE,
          'default value' => 0
        ),
        'sleep_max' => array(
          'type' => 'integer',
          'label' => t('Sleep maximum time in second'),
          'save' => TRUE,
          'default value' => 2
        ),
      ),
      'group' => t('Tarpit'),
    ),
    'tarpit_generate_content' => array(
      'label' => t('Generate Tarpit random content'),
      'base' => 'tarpit_rules_rules_action_generate_content',
      'parameter' => array(
        'words' => array(
          'type' => 'text',
          'label' => t('Number of words'),
          'save' => TRUE,
          'default value' => 150
        ),
        'links' => array(
          'type' => 'text',
          'label' => t('Number of links'),
          'save' => TRUE,
          'default value' => 40
        ),
        'file' => array(
          'type' => 'text',
          'label' => t('Words source file'),
          'save' => TRUE,
          'default value' => drupal_get_path('module', 'tarpit') . '/assets/words.txt'
        ),
      ),
      'group' => t('Tarpit'),
    ),
    'tarpit_disable_blocks' => array(
      'label' => t('Disable blocks'),
      'base' => 'tarpit_rules_rules_action_disable_blocks',
      'parameter' => array(
        'blocks_to_disable' => array(
          'type' => 'text',
          'label' => t('Blocks'),
          'save' => TRUE,
          'optional' => TRUE,
          'restriction' => 'input',
          'default mode' => 'input',
          'description' => 'Write here the blocks to disable. The syntax is: <em>MODULE::DELTA</em>. You may use wildcards: <em>system::*</em> or if you want to remove all blocks: <em>*::*</em>'
        ),
      ),
      'group' => t('Tarpit'),
    )
  );
}
