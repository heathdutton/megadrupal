<?php
/**
 * @file
 * Rules {profanity} file.
 */

/**
 * Implements hook_rules_condition_info().
 */
function profanity_rules_condition_info() {
  return array(
    'profanity_rules_match_lists' => array(
      'label' => t('Detect profanity'),
      'parameter' => array(
        'lists' => array(
          'label' => t('Lists to apply'),
          'type' => 'list<text>',
          'options list' => 'profanity_get_lists_flat',
        ),
        'text' => array(
          'label' => t('Text'),
          'type' => 'text',
        ),
      ),
      'group' => t('Profanity'),
    ),
  );
}

/**
 * Run the lists against the text.
 */
function profanity_rules_match_lists($lists, $text) {
  $found = FALSE;

  foreach ($lists as $list_name) {
    $found = profanity_list_execute($list_name, $text, FALSE);

    if ($found) {
      break;
    }
  }

  return $found;
}
