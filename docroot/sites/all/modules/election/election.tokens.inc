<?php
/**
 * @file
 * Token integration for the Election module.
 */

/**
 * Implements hook_token_info().
 */
function election_token_info() {
  $info = array();
  $info['types'] = array(
    'election' => array(
      'name' => t('Elections'),
      'description' => t('Tokens related to elections.'),
      'needs-data' => 'election',
    ),
    'election_type' => array(
      'name' => t('Election types'),
      'description' => t('Tokens related to election types.'),
      'needs-data' => 'election_type_mn',
    ),
  );
  $info['tokens'] = array(
    'election' => array(
      'id' => array(
        'name' => t('Election ID'),
        'description' => t('The unique ID of the election.'),
      ),
      'type' => array(
        'name' => t('Election type'),
        'description' => t('The type of the election.'),
      ),
      'title' => array(
        'name' => t('Title'),
        'description' => t('The title of the election.'),
      ),
      'created' => array(
        'name' => t('Date created'),
        'description' => t('The date the election was created.'),
        'type' => 'date',
      ),
    ),
    'election_type' => array(
      'machine-name' => array(
        'name' => t('Machine name'),
        'description' => t('The machine name of the election type (default).'),
      ),
      'name' => array(
        'name' => t('Name'),
        'description' => t('The human-readable name of the election type.'),
      ),
      'name-ucfirst' => array(
        'name' => t('Name (ucfirst)'),
        'description' => t('The election type name run through drupal_ucfirst().'),
      ),
      'post-name' => array(
        'name' => t('Post name'),
        'description' => t('The human-readable post name for the election type.'),
      ),
      'post-name-ucfirst' => array(
        'name' => t('Post name (ucfirst)'),
        'description' => t('The post name run through drupal_ucfirst().'),
      ),
    ),
  );
  return $info;
}

/**
 * Implements hook_tokens().
 */
function election_tokens($type, array $tokens, array $data = array(), array $options = array()) {
  $sanitize = !empty($options['sanitize']);
  $replacements = array();
  if ($type == 'election' && !empty($data['election'])) {
    $election = $data['election'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'id':
          $replacements[$original] = $election->election_id;
          break;

        case 'type':
          $replacements[$original] = $sanitize ? check_plain($election->type) : $election->type;
          break;

        case 'title':
          $replacements[$original] = $sanitize ? check_plain($election->title) : $election->title;
          break;

        case 'created':
          $replacements[$original] = format_date($election->created, 'medium');
          break;

      }
    }
    if ($election_tokens = token_find_with_prefix($tokens, 'type')) {
      $replacements += token_generate('election_type', $election_tokens, array('election_type_mn' => $election->type), $options);
    }
  }
  elseif ($type == 'election_type' && !empty($data['election_type_mn'])) {
    $type_machine_name = $data['election_type_mn'];
    $type = _election_type_get_info($type_machine_name);
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'name':
          $replacements[$original] = $sanitize ? check_plain($type['name']) : $type['name'];
          break;

        case 'name-ucfirst':
          $replacements[$original] = $sanitize ? check_plain(drupal_ucfirst($type['name'])) : drupal_ucfirst($type['name']);
          break;

        case 'machine-name':
          $replacements[$original] = $sanitize ? check_plain($type_machine_name) : $type_machine_name;
          break;

        case 'post-name':
          $post_name = election_get_post_name($type_machine_name);
          $replacements[$original] = $sanitize ? check_plain($post_name) : $post_name;
          break;

        case 'post-name-ucfirst':
          $post_name = election_get_post_name($type_machine_name);
          $replacements[$original] = $sanitize ? check_plain(drupal_ucfirst($post_name)) : drupal_ucfirst($post_name);
          break;

      }
    }
  }
  return $replacements;
}
