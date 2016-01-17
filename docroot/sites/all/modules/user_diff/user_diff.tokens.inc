<?php

/**
 * @file
 * Builds placeholder replacement tokens for diff-related data.
 */

/**
 * Implements hook_token_info().
 */
function user_diff_token_info() {
  $user['diff'] = array(
    'name' => t('Latest differences'),
    'description' => t('The differences between the current revision and the previous revision of this user.'),
  );
  $user['diff-markdown'] = array(
    'name' => t('Latest differences (marked down)'),
    'description' => t('The differences between the current revision and the previous revision of this user, but with a marked-down form of each revision used for comparison.'),
  );

  return array(
    'tokens' => array('user' => $user),
  );
}

/**
 * Implements hook_tokens().
 */
function user_diff_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if ($type == 'user' && !empty($data['user'])) {
    $user = $data['user'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        // Basic diff standard comparison information.
        case 'diff':
        case 'diff-markdown':
          $revisions = user_revision_list($user);
          if (count($revisions) == 1) {
            $replacements[$original] = t('(No previous revision available.)');
          }
          else {
            module_load_include('inc', 'diff', 'diff.pages');
            module_load_include('inc', 'user_diff', 'user_diff.pages');
            $old_vid = _diff_get_previous_vid($revisions, $user->vid);
            $state = $name == 'diff' ? 'raw' : 'raw_plain';
            $output = user_diff_diffs_show($user, $old_vid, $user->vid, $state);

            $replacements[$original] = $sanitize ? check_plain($output) : $output;
          }
          break;

      }
    }
  }
  return $replacements;
}
