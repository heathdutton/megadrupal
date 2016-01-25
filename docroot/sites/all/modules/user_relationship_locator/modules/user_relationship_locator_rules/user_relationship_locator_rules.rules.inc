<?php
/**
 * @file
 * Rules integration.
 */

/**
 * Implements hook_rules_event_info().
 */
function user_relationship_locator_rules_rules_event_info() {
  return array(
    'user_relationships_locator_import_data' => array(
      'label' => t('Data has been imported'),
      'group' => t('Relationships Locator'),
      'variables' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('User who imported data.'),
        ),
        'suggestions' => array(
          'type' => 'list<user>',
          'label' => t('Suggested users'),
        ),
        'module' => array(
          'type' => 'text',
          'label' => t('Module name'),
        ),
      ),
    ),
  );
}
