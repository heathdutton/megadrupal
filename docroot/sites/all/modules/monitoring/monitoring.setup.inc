<?php
/**
 * @file
 * Set up dependencies for demo and tests.
 */

/**
 * Creates search_api default index.
 *
 * This is copied from search_api_install() where the code will not run in case
 * of active batch process which is the case of running tests.
 *
 * @see search_api_install()
 */
function monitoring_setup_search_api() {

  global $databases;

  $database = NULL;
  foreach ($databases as $key => $targets) {
    foreach ($targets as $target => $info) {
      $database = "$key:$target";
      break;
    }
  }

  /** @var SearchAPIServer $server */
  $server = entity_create('search_api_server', array(
    'name' => 'test_search_server',
    'machine_name' => 'test_search_server',
    'class' => 'search_api_db_service',
  ));
  $server->options['database'] = $database;
  $server->options['min_chars'] = 3;
  $server->save();

  $name = 'Monitoring test index';
  $values = array(
    'name' => $name,
    'machine_name' => preg_replace('/[^a-z0-9]+/', '_', drupal_strtolower($name)),
    'description' => 'An automatically created search index for indexing node data. Might be configured to specific needs.',
    'server' => $server->machine_name,
    'enabled' => TRUE,
    'item_type' => 'node',
    'options' => array(
      'index_directly' => 1,
      'cron_limit' => '50',
      'data_alter_callbacks' => array(
        'search_api_alter_node_access' => array(
          'status' => 1,
          'weight' => '0',
          'settings' => array(),
        ),
      ),
      'processors' => array(
        'search_api_case_ignore' => array(
          'status' => 1,
          'weight' => '0',
          'settings' => array(
            'strings' => 0,
          ),
        ),
        'search_api_html_filter' => array(
          'status' => 1,
          'weight' => '10',
          'settings' => array(
            'title' => 0,
            'alt' => 1,
            'tags' => "h1 = 5\n" .
                "h2 = 3\n" .
                "h3 = 2\n" .
                "strong = 2\n" .
                "b = 2\n" .
                "em = 1.5\n" .
                "u = 1.5",
          ),
        ),
        'search_api_tokenizer' => array(
          'status' => 1,
          'weight' => '20',
          'settings' => array(
            'spaces' => '[^\\p{L}\\p{N}]',
            'ignorable' => '[-]',
          ),
        ),
      ),
      'fields' => array(
        'type' => array(
          'type' => 'string',
        ),
        'title' => array(
          'type' => 'text',
          'boost' => '5.0',
        ),
        'promote' => array(
          'type' => 'boolean',
        ),
        'sticky' => array(
          'type' => 'boolean',
        ),
        'created' => array(
          'type' => 'date',
        ),
        'changed' => array(
          'type' => 'date',
        ),
        'author' => array(
          'type' => 'integer',
          'entity_type' => 'user',
        ),
        'comment_count' => array(
          'type' => 'integer',
        ),
        'search_api_language' => array(
          'type' => 'string',
        ),
        'body:value' => array(
          'type' => 'text',
        ),
      ),
    ),
  );

  search_api_index_insert($values);
}

/**
 * Creates comments for testing purposes.
 *
 * @param object $node
 *   Node to which the comment belongs.
 * @param array $settings
 *   Comment data.
 *
 * @return object
 *   Crated comment.
 */
function _monitoring_setup_create_comment($node, $settings = array()) {
  $settings += array(
    'status' => COMMENT_PUBLISHED,
    'node_type' => $node->type,
    'nid' => $node->nid,
  );
  $comment = entity_create('comment', $settings);
  comment_save($comment);
  return $comment;
}

/**
 * Creates nodes for testing purposes.
 *
 * @param array $settings
 *   Node data.
 *
 * @return object
 *   Created node.
 */
function _monitoring_setup_create_node($settings = array()) {
  // Populate defaults array.
  $settings += array(
    'body'      => array(LANGUAGE_NONE => array(array())),
    'title'     => _monitoring_setup_random_name(8),
    'comment'   => 2,
    'changed'   => REQUEST_TIME,
    'moderate'  => 0,
    'promote'   => 0,
    'revision'  => 1,
    'log'       => '',
    'status'    => 1,
    'sticky'    => 0,
    'type'      => 'page',
    'revisions' => NULL,
    'language'  => LANGUAGE_NONE,
    'uid'  => 0,
  );

  // Use the original node's created time for existing nodes.
  if (isset($settings['created']) && !isset($settings['date'])) {
    $settings['date'] = format_date($settings['created'], 'custom', 'Y-m-d H:i:s O');
  }

  // Merge body field value and format separately.
  $body = array(
    'value' => _monitoring_setup_random_name(32),
    'format' => filter_default_format(),
  );
  $settings['body'][$settings['language']][0] += $body;

  $node = (object) $settings;
  node_save($node);

  // Small hack to link revisions to our test user.
  db_update('node_revision')
    ->fields(array('uid' => $node->uid))
    ->condition('vid', $node->vid)
    ->execute();
  return $node;
}

/**
 * Generates random name.
 *
 * @param int $length
 *   Length of the generated string.
 *
 * @return string
 *   The generated string.
 */
function _monitoring_setup_random_name($length = 8) {
  $values = array_merge(range(65, 90), range(97, 122), range(48, 57));
  $max = count($values) - 1;
  $str = chr(mt_rand(97, 122));
  for ($i = 1; $i < $length; $i++) {
    $str .= chr($values[mt_rand(0, $max)]);
  }
  return $str;
}
