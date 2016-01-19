<?php

/**
 * @file
 * A collection f utility functions for the demo module. Not used in core.
 */
define('SMARTLING_DEMO_MODULE', 'smartling_demo_content');

define('TRAVEL_TAXONOMY_ADDED', t('Travel taxonomy has been added to the database.'));
define('DEMO_USERS_ADDED', t('Demo users have been added to the database.'));
define('DEMO_USERS_EXIST', t('Demo users already exist and have been added to the database.'));
define('NODES_COMMENTS_ADDED', t('Comments have been added to travel nodes.'));
define('NODES_COMMENT_DELETE', t('The comments have been deleted.'));
define('NO_NODES_DEMO_COMMENTS', t('No demo comments in the database.'));
define('DELETE_ADDITIONAL_ACCOUNT_FIELDS', t('The additional account fields have been deleted from the database'));
define('DELETE_DEMO_USERS', t('Demo users have been deleted'));
define('TRAVEL_VOCABULARY_DELETED', t('The travel taxonomy has been removed from the database.'));
define('TRAVEL_VOCABULARY_NOT_FOUND', t('The travel taxonomy cannot be found in taxonomy vocabularies.'));
define('TRAVEL_TAXONOMY_ADDED_TO_NODE', t('The travel taxonomy has been randomly added to travel nodes.'));
define('TAXONOMY_TRAVEL', 'travel');
define('DEMO_LANGUAGE_DEFAULT', language_default()->language);

/**
 * Implements hook_modules_enabled().
 */
function smartling_demo_content_modules_enabled(array $modules) {
  if (in_array(SMARTLING_DEMO_MODULE, $modules)) {
    drupal_cron_run();

    smartling_demo_content_create_comments_to_nodes();
    smartling_demo_content_create_additional_fields_for_account();
    smartling_demo_content_create_taxonomy();
    smartling_demo_content_add_taxonomy_to_travel_node();

    db_update('block')
      ->fields(array(
        'region' => 'sidebar_first',
        'status' => 1,
      ))
      ->condition('delta', 'language_content')
      ->condition('module', 'locale')
      ->execute();
  }
}

/**
 * Implements hook_modules_disabled().
 */
function smartling_demo_content_modules_disabled(array $modules) {
  if (in_array(SMARTLING_DEMO_MODULE, $modules)) {
    smartling_demo_content_delete_comments_to_nodes();
    smartling_demo_content_delete_additional_fields_for_account();
    smartling_demo_content_delete_taxonomy();
  }
}

/**
 * Create comments to nodes.
 */
function smartling_demo_content_create_comments_to_nodes() {
  // Make comment_body translatable.
  $field = field_info_field('comment_body');
  $field['translatable'] = 1;
  field_update_field($field);

  $nodes = node_load_multiple(array(), array(
    'type' => 'travel', //array('article', 'travel',),
  ));
  foreach ($nodes as $node) {
    $load_node = node_load($node->vid);
    if ($load_node->comment_count < 1) {
      $comment = (object) array(
        'nid' => $load_node->vid,
        'cid' => 0,
        'pid' => 0,
        'uid' => 1,
        'mail' => '',
        'is_anonymous' => 0,
        'homepage' => '',
        'status' => COMMENT_PUBLISHED,
        'subject' => smartling_demo_content_devel_create_greeking(mt_rand(1, 3), TRUE),
        'language' => DEMO_LANGUAGE_DEFAULT,
        'comment_body' => array(
          DEMO_LANGUAGE_DEFAULT => array(
            0 => array(
              'value' => smartling_demo_content_devel_create_greeking(mt_rand(2, 17), TRUE),
              'format' => 'filtered_html',
            ),
          ),
        ),
      );
      comment_submit($comment);
      comment_save($comment);
    }
  }

  smartling_demo_content_set_message_watchdog(NODES_COMMENTS_ADDED);
}

/**
 * Delete comments.
 */
function smartling_demo_content_delete_comments_to_nodes() {
  $nodes = node_load_multiple(array(), array(
    'type' => array(
      'article',
      'travel',
    ),
  ));
  $results = db_select('comment', 'c')
    ->fields('c', array('cid', 'uid'))
    ->execute()->fetchAll();
  $comment_id = array();
  foreach ($results as $result) {
    if (array_key_exists($result->uid, $nodes)) {
      $comment_id[] = $result->cid;
    }
  }

  if (count($comment_id) > 0) {
    comment_delete_multiple($comment_id);
    smartling_demo_content_set_message_watchdog(NODES_COMMENT_DELETE);
  }
  else {
    smartling_demo_content_set_message_watchdog(NO_NODES_DEMO_COMMENTS);
  }
}

/**
 * Create additional fields for account.
 *
 * @return null
 *   Return null.
 */
function smartling_demo_content_create_additional_fields_for_account() {
  $data_fields = array(
    array(
      'field' => array(
        'field_name' => 'field_nick_name_field',
        'type' => 'text',
      ),
      'instance' => array(
        'field_name' => 'field_nick_name_field',
        'entity_type' => 'user',
        'label' => 'Nick Name',
        'bundle' => 'user',
        'settings' => array(
          'user_register_form' => 1,
        ),
        'widget' => array(
          'type' => 'textfield',
          'weight' => '1',
        ),
      ),
    ),
    array(
      'field' => array(
        'field_name' => 'field_about_me_field',
        'type' => 'text',
      ),
      'instance' => array(
        'field_name' => 'field_about_me_field',
        'entity_type' => 'user',
        'label' => 'About Me',
        'bundle' => 'user',
        'settings' => array(
          'user_register_form' => 1,
        ),
        'widget' => array(
          'type' => 'textfield',
          'weight' => '1',
        ),
      ),
    ),
    array(
      'field' => array(
        'field_name' => 'field_horoscope_field',
        'type' => 'text',
      ),
      'instance' => array(
        'field_name' => 'field_horoscope_field',
        'entity_type' => 'user',
        'label' => 'Horoscope',
        'bundle' => 'user',
        'settings' => array(
          'user_register_form' => 1,
        ),
        'widget' => array(
          'type' => 'textfield',
          'weight' => '1',
        ),
      ),
    ),
  );

  $data_demo = array(
    'horoscope' => array(
      'table' => 'field_data_field_horoscope_field',
      'field' => 'field_horoscope_field_value',
    ),
    'about_me' => array(
      'table' => 'field_data_field_about_me_field',
      'field' => 'field_about_me_field_value',
    ),
    'nick_name' => array(
      'table' => 'field_data_field_nick_name_field',
      'field' => 'field_nick_name_field_value',
    ),
  );

  $data_user = array(
    array(
      'fields' => array(
        'name' => 'john_doe',
        'mail' => 'john_doe@example.com',
        'pass' => user_password(8),
        'status' => 1,
        'init' => 'email address',
        'roles' => array(
          DRUPAL_AUTHENTICATED_RID => 'authenticated user',
        ),
      ),
      'demo_data' => array(
        'horoscope' => 'Libra',
        'about_me' => 'This is description of John Doe male user',
        'nick_name' => 'John Doe',
      ),
    ),
    array(
      'fields' => array(
        'name' => 'jane_roe',
        'mail' => 'jane_roe@example.com',
        'pass' => user_password(8),
        'status' => 1,
        'init' => 'email address',
        'roles' => array(
          DRUPAL_AUTHENTICATED_RID => 'authenticated user',
        ),
      ),
      'demo_data' => array(
        'horoscope' => 'Aquarius',
        'about_me' => 'This is description of Jane Roe female user',
        'nick_name' => 'Jane Roe',
      ),
    ),
  );

  foreach ($data_fields as $value) {
    if (!field_info_field($value['field']['field_name'])) {
      field_create_field($value['field']);
      field_create_instance($value['instance']);
    }
  }

  foreach ($data_user as $value) {
    $load_user = user_load_by_name($value['fields']['name']);
    if (!is_object($load_user)) {
      $account = user_save('', $value['fields']);
      foreach ($data_demo as $key => $val) {
        db_insert($val['table'])
          ->fields(
            array(
              'entity_type' => 'user',
              'bundle' => 'user',
              'deleted' => 0,
              'entity_id' => $account->uid,
              'language' => 'und',
              'delta' => 0,
              $val['field'] => $value['demo_data'][$key],
            )
          )
          ->execute();
      }
    }
    else {
      smartling_demo_content_set_message_watchdog(DEMO_USERS_EXIST);
      return NULL;
    }
  }

  smartling_demo_content_set_message_watchdog(DEMO_USERS_ADDED);
}

/**
 * Delete additional fields for account.
 */
function smartling_demo_content_delete_additional_fields_for_account() {

  $data_fields = array(
    array(
      'field' => 'field_nick_name_field',
    ),
    array(
      'field' => 'field_about_me_field',
    ),
    array(
      'field' => 'field_horoscope_field',
    ),
  );

  $data_user = array(
    array(
      'name' => 'john_doe',
    ),
    array(
      'name' => 'jane_roe',
    ),
  );

  foreach ($data_fields as $value) {
    if (field_info_field($value['field'])) {
      field_delete_field($value['field']);
    }
  }

  foreach ($data_user as $val) {
    $user_load = user_load_by_name($val['name']);
    user_delete($user_load->uid);
  }

  smartling_demo_content_set_message_watchdog(DELETE_ADDITIONAL_ACCOUNT_FIELDS);
  smartling_demo_content_set_message_watchdog(DELETE_DEMO_USERS);
}

/**
 * Create taxonomy terms.
 */
function smartling_demo_content_create_taxonomy() {
  $terms_data = array('Turkey', 'Egypt', 'Dubai', 'Thailand', 'Spain');
  $get_taxonomy_vocabulary = taxonomy_vocabulary_machine_name_load(TAXONOMY_TRAVEL);

  if (!is_object($get_taxonomy_vocabulary)) {
    taxonomy_vocabulary_save((object) array(
      'name' => 'Travel',
      'machine_name' => TAXONOMY_TRAVEL,
    ));

    $get_taxonomy_vocabulary = taxonomy_vocabulary_machine_name_load(TAXONOMY_TRAVEL);
  }
  else {
    foreach (taxonomy_get_tree($get_taxonomy_vocabulary->vid) as $term) {
      taxonomy_term_delete($term->tid);
    }
  }

  foreach ($terms_data as $term_data) {
    taxonomy_term_save((object) array(
      'name' => $term_data,
      'vid' => $get_taxonomy_vocabulary->vid,
      'language' => DEMO_LANGUAGE_DEFAULT,
    ));
  }

  smartling_demo_content_set_message_watchdog(TRAVEL_TAXONOMY_ADDED);
}

/**
 * Delete taxonomy terms.
 */
function smartling_demo_content_delete_taxonomy() {
  $get_taxonomy_vocabulary = taxonomy_vocabulary_machine_name_load(TAXONOMY_TRAVEL);
  if (is_object($get_taxonomy_vocabulary)) {
    taxonomy_vocabulary_delete($get_taxonomy_vocabulary->vid);
    smartling_demo_content_set_message_watchdog(TRAVEL_VOCABULARY_DELETED);
  }
  else {
    smartling_demo_content_set_message_watchdog(TRAVEL_VOCABULARY_NOT_FOUND);
  }
}

/**
 * Add taxonomy term to travel node.
 */
function smartling_demo_content_add_taxonomy_to_travel_node() {
  $data_fields = array(
    array(
      'field' => array(
        'field_name' => 'field_taxonomy_field',
        'type' => 'taxonomy_term_reference',
      ),
      'instance' => array(
        'bundle' => TAXONOMY_TRAVEL,
        'default_value' => NULL,
        'deleted' => 0,
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'taxonomy',
            'settings' => array(),
            'type' => 'taxonomy_term_reference_link',
            'weight' => 3,
          ),
          'teaser' => array(
            'label' => 'above',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 0,
          ),
        ),
        'entity_type' => 'node',
        'field_name' => 'field_taxonomy_field',
        'label' => 'Travel Taxonomy',
        'required' => FALSE,
        'settings' => array(
          'allowed_values' => array(
            0 => array(
              'vocabulary' => array(
                0 => TAXONOMY_TRAVEL,
              ),
            ),
          ),
          'user_register_form' => FALSE,
        ),
        'widget' => array(
          'module' => 'options',
          'settings' => array(),
          'type' => 'options_select',
          'weight' => 4,
        ),
      ),
    ),
  );

  foreach ($data_fields as $value) {
    if (!field_info_field($value['field']['field_name'])) {
      field_create_field($value['field']);
      field_create_instance($value['instance']);
    }
  }
  $field = field_info_field('field_taxonomy_field');
  $field['settings']['allowed_values'][0]['vocabulary'] = TAXONOMY_TRAVEL;
  $field['cardinality'] = 2;
  field_update_field($field);

  $travel_nodes = node_load_multiple(array(), array('type' => TAXONOMY_TRAVEL));

  foreach ($travel_nodes as $node) {
    $load_node = node_load($node->nid, NULL, TRUE);
    foreach (smartling_demo_content_get_random_taxonomy_array() as $taxonomy) {
      $load_node->field_taxonomy_field[LANGUAGE_NONE][]['tid'] = $taxonomy->tid;
    }

    node_save($load_node);
  }

  smartling_demo_content_set_message_watchdog(TRAVEL_TAXONOMY_ADDED_TO_NODE);
}

/**
 * Get random taxonomy array.
 *
 * @return array
 *   Return random taxonomy array.
 */
function smartling_demo_content_get_random_taxonomy_array() {
  $get_taxonomy_vocabulary = taxonomy_vocabulary_machine_name_load(TAXONOMY_TRAVEL);
  $taxonomy_data = array();

  if (is_object($get_taxonomy_vocabulary)) {
    $taxonomy_terms = taxonomy_get_tree($get_taxonomy_vocabulary->vid);
    $rand_terms = (array) array_rand($taxonomy_terms, rand(1, 2));

    foreach ($rand_terms as $term) {
      $taxonomy_data[] = $taxonomy_terms[$term];
    }

  }

  return $taxonomy_data;
}

/**
 * Set drupal message and smartling log message.
 *
 * @param string $msg
 *   Message.
 */
function smartling_demo_content_set_message_watchdog($msg) {
  drupal_set_message(check_plain($msg));
  $log = smartling_log_get_handler();
  $log->info($msg);
}


/**
 * Copy devel_create_greeking().
 *
 * @param int $word_count
 *   Words count.
 * @param bool $title
 *   TRUE if is title. FALSE by default.
 *
 * @return string
 *   Return random string.
 */
function smartling_demo_content_devel_create_greeking($word_count, $title = FALSE) {
  $dictionary = array("abbas", "abdo", "abico", "abigo", "abluo", "accumsan",
    "acsi", "ad", "adipiscing", "aliquam", "aliquip", "amet", "antehabeo",
    "appellatio", "aptent", "at", "augue", "autem", "bene", "blandit",
    "brevitas", "caecus", "camur", "capto", "causa", "cogo", "comis",
    "commodo", "commoveo", "consectetuer", "consequat", "conventio", "cui",
    "damnum", "decet", "defui", "diam", "dignissim", "distineo", "dolor",
    "dolore", "dolus", "duis", "ea", "eligo", "elit", "enim", "erat",
    "eros", "esca", "esse", "et", "eu", "euismod", "eum", "ex", "exerci",
    "exputo", "facilisi", "facilisis", "fere", "feugiat", "gemino",
    "genitus", "gilvus", "gravis", "haero", "hendrerit", "hos", "huic",
    "humo", "iaceo", "ibidem", "ideo", "ille", "illum", "immitto",
    "importunus", "imputo", "in", "incassum", "inhibeo", "interdico",
    "iriure", "iusto", "iustum", "jugis", "jumentum", "jus", "laoreet",
    "lenis", "letalis", "lobortis", "loquor", "lucidus", "luctus", "ludus",
    "luptatum", "macto", "magna", "mauris", "melior", "metuo", "meus",
    "minim", "modo", "molior", "mos", "natu", "neo", "neque", "nibh",
    "nimis", "nisl", "nobis", "nostrud", "nulla", "nunc", "nutus", "obruo",
    "occuro", "odio", "olim", "oppeto", "os", "pagus", "pala", "paratus",
    "patria", "paulatim", "pecus", "persto", "pertineo", "plaga", "pneum",
    "populus", "praemitto", "praesent", "premo", "probo", "proprius",
    "quadrum", "quae", "qui", "quia", "quibus", "quidem", "quidne", "quis",
    "ratis", "refero", "refoveo", "roto", "rusticus", "saepius",
    "sagaciter", "saluto", "scisco", "secundum", "sed", "si", "similis",
    "singularis", "sino", "sit", "sudo", "suscipere", "suscipit", "tamen",
    "tation", "te", "tego", "tincidunt", "torqueo", "tum", "turpis",
    "typicus", "ulciscor", "ullamcorper", "usitas", "ut", "utinam",
    "utrum", "uxor", "valde", "valetudo", "validus", "vel", "velit",
    "veniam", "venio", "vereor", "vero", "verto", "vicis", "vindico",
    "virtus", "voco", "volutpat", "vulpes", "vulputate", "wisi", "ymo",
    "zelus");
  $dictionary_flipped = array_flip($dictionary);

  $greeking = '';

  if (!$title) {
    $words_remaining = $word_count;
    while ($words_remaining > 0) {
      $sentence_length = mt_rand(3, 10);
      $words = array_rand($dictionary_flipped, $sentence_length);
      $sentence = implode(' ', $words);
      $greeking .= ucfirst($sentence) . '. ';
      $words_remaining -= $sentence_length;
    }
  }
  else {
    // Use slightly different method for titles.
    $words = array_rand($dictionary_flipped, $word_count);
    $words = is_array($words) ? implode(' ', $words) : $words;
    $greeking = ucwords($words);
  }

  // Work around possible php garbage collection bug. Without an unset(), this
  // function gets very expensive over many calls (php 5.2.11).
  unset($dictionary, $dictionary_flipped);
  return trim($greeking);
}
