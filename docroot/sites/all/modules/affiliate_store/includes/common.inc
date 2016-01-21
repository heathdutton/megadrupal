<?php

/**
 * @file
 * Common and helper functions.
 */

/**
 * Add a vocabulary term reference field to affiliate store product node type.
 *
 * @param string $name
 *   Vocabulary logical name.
 *
 * @return boolean
 *   TRUE if success, FALSE otherwise.
 *
 * @see affiliate_store_vocabularies()
 */
function affiliate_store_add_vocabulary_term_reference_field($name) {
  if (!affiliate_store_in_vocabularies($name)) {
    return FALSE;
  }
  $vocab = taxonomy_vocabulary_load(affiliate_store_get_vocabulary_id($name));
  if (!$vocab) {
    return FALSE;
  }

  $field_name = "affiliate_store_$name";
  $entity_type = 'node';
  $bundle = 'affiliate_store_product';

  // Create term reference field.
  if (!field_info_field($field_name)) {
    $field = array(
      'field_name' => $field_name,
      'type' => 'taxonomy_term_reference',
      'entity_types' => array($entity_type),
      'settings' => array(
        'allowed_values' => array(
          array(
            'vocabulary' => $vocab->machine_name,
            'parent' => 0,
          ),
        ),
      ),
    );
    field_create_field($field);
  }

  // Create term reference instance.
  if (!field_info_instance($entity_type, $field_name, $bundle)) {
    $instances = array(
      'network' => array(
        'required' => TRUE,
        'display' => array(
          'default' => array('label' => 'hidden', 'type' => 'hidden'),
          'teaser' => array('label' => 'hidden', 'type' => 'hidden'),
        ),
      ),
      'merchant' => array(
        'required' => TRUE,
        'display' => array(
          'default' => array('label' => 'hidden', 'type' => 'hidden'),
          'teaser' => array('label' => 'hidden', 'type' => 'hidden'),
        ),
      ),
      'category' => array('required' => FALSE),
      'subcategory' => array('required' => FALSE),
      'subsubcategory' => array('required' => FALSE),
    );
    $instance_base = array(
      'field_name' => $field_name,
      'entity_type' => $entity_type,
      'bundle' => $bundle,
      'label' => $vocab->name,
      'display' => array(
        'default' => array('label' => 'hidden'),
        'teaser' => array('label' => 'hidden'),
      ),
    );
    field_create_instance($instances[$name] + $instance_base);
  }
  return TRUE;
}

/**
 * Add a synonym field to terms of a vocabulary.
 *
 * @param string $name
 *   Vocabulary logical name.
 *
 * @return boolean
 *   TRUE if success, FALSE otherwise.
 *
 * @see affiliate_store_vocabularies()
 */
function affiliate_store_add_vocabulary_term_synonym_field($name) {
  // Network and merchant will not be allowed to have synonym.
  $vocabs = array('category', 'subcategory', 'subsubcategory');
  if (!affiliate_store_in_vocabularies($name, $vocabs)) {
    return FALSE;
  }
  $vocab = taxonomy_vocabulary_load(affiliate_store_get_vocabulary_id($name));
  if (!$vocab) {
    return FALSE;
  }

  $field_name = 'affiliate_store_synonym';
  $entity_type = 'taxonomy_term';
  $bundle = $vocab->machine_name;

  // Create synonym field.
  if (!field_info_field($field_name)) {
    $field = array(
      'field_name' => $field_name,
      'type' => 'text',
      'entity_types' => array($entity_type),
    );
    field_create_field($field);
  }

  // Create synonym instance.
  if (!field_info_instance($entity_type, $field_name, $bundle)) {
    $instance = array(
      'field_name' => $field_name,
      'entity_type' => $entity_type,
      'bundle' => $bundle,
      'label' => t('Synonym'),
      'display' => array(
        'default' => array('label' => 'hidden', 'type' => 'hidden'),
      ),
    );
    field_create_instance($instance);
  }
  return TRUE;
}

/**
 * Clear all affiliate store caches.
 */
function affiliate_store_clear_cache() {
  cache_clear_all('affiliate_store', 'cache_block', TRUE);
  $url = url('affiliate-store', array('absolute' => TRUE));
  cache_clear_all($url, 'cache_page', TRUE);
}

/**
 * Convert a text using a specific string case.
 *
 * @param string $text
 *   The text that will be converted.
 * @param string $case
 *   (optional) One of the following string conversion case:
 *   - none
 *   - lower
 *   - upper
 *   - title
 *   - smart
 *   Default to 'smart'.
 *
 * @return string
 *   The converted text.
 */
function affiliate_store_convert_case($text, $case = 'smart') {
  $map = array(
    'none' => array(),
    'lower' => array('drupal_strtolower'),
    'upper' => array('drupal_strtoupper'),
    // Titlecase and smartcase requires the term comes in lowercase first.
    'title' => array('drupal_strtolower', 'affiliate_store_ucwords'),
    'smart' => array('drupal_strtolower', 'affiliate_store_smartcase'),
  );
  if (!empty($map[$case])) {
    foreach ($map[$case] as $conversion) {
      $text = $conversion($text);
    }
  }
  return $text;
}

/**
 * Helper function for converting term name string case.
 *
 * @param string $text
 *   The term name that will be converted.
 * @param string $case
 *   (optional) Conversion case to use. Default to NULL meaning use conversion
 *   case configured at settings page.
 *
 * @return string
 *   The converted term name.
 */
function affiliate_store_convert_term_case($text, $case = NULL) {
  if ($case === NULL) {
    $case = variable_get('affiliate_store_term_case', 'smart');
  }
  return affiliate_store_convert_case($text, $case);
}

/**
 * Cron job; Update affiliate store.
 */
function affiliate_store_cron_update() {
  // Run as user 1 so node creation has user 1 as author.
  affiliate_store_impersonate_user(1);

  affiliate_store_include('batch');
  $max_time = ini_get('max_execution_time');
  // If there is no execution time limit, use 5 minutes.
  $max_time = $max_time ? ($max_time * 0.5) : (300 * 0.5);
  do {
    $context = array(
      'operation' => isset($context['operation']) ? $context['operation'] : 0,
      'max_operation' => variable_get('affiliate_store_update_throttle', 50),
    );
    $result = affiliate_store_batch_dequeue('%', $context, 0.5, 'page');
    if ($result === TRUE) {
      // Log to watchdog if there is result on executed batch.
      $errors = affiliate_store_batch_results($context['results'], 'errors');
      if ($errors) {
        watchdog(
          'affiliate store',
          'Error occurred when updating affiliate store. !result',
          array('!result' => theme('item_list', array('items' => $errors))),
          WATCHDOG_ERROR
        );
      }
      $messages = affiliate_store_batch_results($context['results']);
      if ($messages) {
        watchdog(
          'affiliate store',
          'Affiliate store updated. !result',
          array('!result' => theme('item_list', array('items' => $messages)))
        );
      }

      if ($context['operation'] >= $context['max_operation']) {
        break;
      }
    }
    // If batch queue emptied, queue another round of update batches.
    elseif ($result === NULL) {
      // Skip if scheduled time not reach yet. Update will be twice a day.
      // Skip if there is still inactive batch waiting to run.
      $next_update = variable_get('affiliate_store_last_update', 0) + 43200;
      if ((REQUEST_TIME < $next_update)
        || affiliate_store_batch_count('%', TRUE)
      ) {
        break;
      }

      affiliate_store_include('subscription');
      affiliate_store_subscription_sync($errors);
      if (!$errors) {
        variable_set('affiliate_store_last_update', REQUEST_TIME);
      }
    }
    else {
      // Dequeue error, most probably manual update is running. Retry later.
      watchdog(
        'affiliate store',
        'Cannot update affiliate store, probably because manual update is ' .
        'also running.',
        NULL, WATCHDOG_WARNING
      );
      break;
    }
  } while ((timer_read('page') / 1000) < $max_time);

  affiliate_store_revert_user();
}

/**
 * Decode a gzip compressed string.
 *
 * @param string $data
 *   String to decode.
 *
 * @return string
 *   Decoded string. Return NULL if decoding failed.
 */
function affiliate_store_gzdecode($data) {
  $file = tempnam(realpath(file_directory_temp()), 'file');
  $fp = fopen($file, 'wb');
  if (!$fp) {
    return NULL;
  }

  fwrite($fp, $data);
  fclose($fp);
  ob_start();
  readgzfile($file);
  $data = ob_get_clean();
  unlink($file);
  return $data;
}

/**
 * Generate a keyed hash value using the HMAC method.
 *
 * @param string $algo
 *   Name of hashing algorithm.
 * @param string $data
 *   Message to be hashed.
 * @param string $key
 *   Shared secret key used for generating the HMAC variant of the message
 *   digest.
 * @param boolean $raw_output
 *   (optional) When set to TRUE, outputs raw binary data. FALSE outputs
 *   lowercase hexits. Default to FALSE.
 *
 * @return mixed
 *   A string containing the calculated message digest as lowercase hexits
 *   unless raw_output is set to true in which case the raw binary
 *   representation of the message digest is returned. Return NULL if hashing
 *   failed.
 */
function affiliate_store_hash_hmac($algo, $data, $key, $raw_output = FALSE) {
  // PHP >= 5.1.2.
  if (function_exists('hash_hmac')) {
    return hash_hmac($algo, $data, $key, $raw_output);
  }
  // Fall back to using phpseclib library.
  if (module_exists('libraries')) {
    $path = libraries_get_path('phpseclib') . '/Crypt/Hash.php';
    if (include_once DRUPAL_ROOT . '/' . $path) {
      $hash = new Crypt_Hash($algo);
      $hash->setKey($key);
      $output = $hash->hash($data);
      return $raw_output ? $output : bin2hex($output);
    }
  }
  return NULL;
}

/**
 * Decode JSON encoded string into PHP associative array.
 *
 * @param string $json
 *   JSON encoded string.
 * @param boolean &$error
 *   Return TRUE if decoding failed, FALSE otherwise.
 *
 * @return mixed
 *   PHP variable. Return NULL if decoding failed, but check $error to be sure
 *   if error really occurred as JSON can also returns NULL.
 */
function affiliate_store_json_decode($json, &$error) {
  $error = FALSE;
  // PHP >= 5.2.0.
  if (function_exists('json_decode')) {
    return json_decode($json, TRUE);
  }
  // Fall back to using Services_JSON PEAR library.
  if (module_exists('libraries')) {
    $path = libraries_get_path('Services_JSON') . '/JSON.php';
    if (include_once DRUPAL_ROOT . '/' . $path) {
      $services_json = new Services_JSON(SERVICES_JSON_LOOSE_TYPE);
      return $services_json->decode($json);
    }
  }
  $error = TRUE;
  return NULL;
}

/**
 * Insert or update product node.
 *
 * Product node attributes are assembled from three sources that are ordered in
 * the following precedence order: Hub > existing node > default values.
 *
 * The product attributes are first set using that of Hub, then any missing
 * attribute is filled in by existing node attributes in case of update, and
 * finally by default values. Default values are those defined by module and
 * content type workflow settings.
 *
 * @param mixed &$product
 *   An associative array representing product with the following keys:
 *   - network: Network name.
 *   - merchant: Merchant name.
 *   - category: (optional) Product category name.
 *   - subcategory: (optional) Product subcategory name.
 *   - subsubcategory: (optional) Product subsubcategory name.
 *   - nwtid: (optional) Network term ID. If specified, network name is not
 *     used for term ID look up.
 *   - mtid: (optional) Merchant term ID. If specified, merchant name is not
 *     used for term ID look up.
 *   - ctid: (optional) Product category term ID. If specified, product
 *     category name is not used for term ID look up.
 *   - sctid: (optional) Product subcategory term ID. If specified, product
 *     subcategory name is not used for term ID look up.
 *   - ssctid: (optional) Product subsubcategory term ID. If specified, product
 *     subsubcategory name is not used for term ID look up.
 *   - pid: Product ID.
 *   - title: Product title.
 *   - price: (optional) Product price.
 *   - currency: (optional) Price currency.
 *   - url: Product URL.
 *   - thumb_url: (optional) Product thumbnail URL.
 *   - image_url: (optional) Product image URL.
 *   - description: (optional) Product short description.
 *   - status: Product status integer.
 *   Return the saved product node object.
 *
 * @return mixed
 *   One of SAVED_NEW, SAVED_UPDATED, or SAVED_DELETED. NULL if no operation.
 */
function affiliate_store_save_product(&$product) {
  $op = SAVED_NEW;
  $query = db_select('affiliate_store_product', 'p')
    ->distinct()
    ->fields('p', array('nid'))
    ->condition('pid', $product['pid']);

  // Populate product term IDs. Missing vocabulary term names will be replaced
  // using values specified in settings page. Terms that are not yet exist will
  // be created.
  foreach (affiliate_store_vocabularies() as $vocab => $info) {
    $astid = $info['as'] . 'tid';
    $field = "affiliate_store_$vocab";
    if (empty($product[$astid])) {
      if (empty($product[$vocab])) {
        // Get name of vocabulary missing term.
        $product[$vocab] = affiliate_store_get_term_name(0, $vocab);
      }
      $product[$astid] = affiliate_store_get_term_id($product[$vocab], $vocab);
    }
    // Set product taxonomy terms.
    $product[$field][LANGUAGE_NONE][0]['tid'] = $product[$astid];
    $query->condition($astid, $product[$astid]);
  }

  // Check if the product already exists in database.
  $nid = $query->execute()->fetchField();
  if ($nid) {
    // If the product already exists and its status is to be unpublished, just
    // delete the node.
    if (!$product['status']) {
      node_delete($nid);
      return SAVED_DELETED;
    }

    // Reuse existing node attributes since they might have been customized by
    // user.
    $product += (array) node_load($nid);
    $op = SAVED_UPDATED;
  }
  elseif (!$product['status']) {
    // Ignore if the product doesn't exists and its status is to be unpublished.
    return NULL;
  }

  // Set product default attributes values.
  $product += array(
    'uid' => 1,
    'type' => 'affiliate_store_product',
    'description' => '',
    'language' => LANGUAGE_NONE,
    'log' => t('Imported from Hub.'),
  );
  // Set product attributes using content type default workflow settings.
  $workflow = variable_get("node_options_$product[type]", array('status'));
  $product += array_fill_keys($workflow, 1);
  $product['body'][LANGUAGE_NONE][0] = array(
    'value' => $product['description'],
    'format' => _affiliate_store_get_filtered_html_format(),
  );

  $product = (object) $product;
  node_save($product);
  return $op;
}

/**
 * Save a module managed term.
 *
 * @param string $name
 *   Term permanent name.
 * @param string $vocabulary
 *   Vocabulary logical name of term.
 *
 * @return int
 *   Term ID if successful, 0 otherwise.
 *
 * @see affiliate_store_vocabularies()
 */
function affiliate_store_save_term($name, $vocabulary) {
  $vocabs = affiliate_store_vocabularies();
  if (!isset($vocabs[$vocabulary])) {
    return 0;
  }

  $term = (object) array(
    'vid' => affiliate_store_get_vocabulary_id($vocabulary),
    'name' => (
      // Network or merchant name is saved as is.
      in_array($vocabulary, array('network', 'merchant'), TRUE) ?
      $name : affiliate_store_convert_term_case($name)
    ),
  );

  $txn = db_transaction();
  try {
    taxonomy_term_save($term);
    $tid = (int) $term->tid;

    // If the term is a missing term default value, then assign a synonym to it
    // with a nicer name without the brackets.
    if ($name === affiliate_store_get_term_name(0, $vocabulary)) {
      $synonym = preg_replace('/^<(.*)>$/', '$1', $name);
      $synonym = affiliate_store_convert_term_case($synonym);
      affiliate_store_save_data('taxonomy_term', $term, '_synonym', $synonym);
      taxonomy_term_save($term);
    }

    // Store term permanent name in lowercase.
    db_insert("affiliate_store_$vocabulary")
      ->fields(array(
        $vocabs[$vocabulary]['as'] . 'tid' => $tid,
        'title' => drupal_strtolower($name),
      ))
      ->execute();
  }
  catch (Exception $e) {
    $txn->rollback();
    watchdog_exception('affiliate store', $e);
    throw $e;
  }

  return $tid;
}

/**
 * Save or update a module managed vocabulary.
 *
 * Existing vocabulary will be updated instead if there is one matching the
 * default machine name defined by module.
 *
 * @param string $name
 *   Vocabulary logical name.
 * @param int $vid
 *   (optional) Specify a valid vocabulary ID to update instead of searching
 *   for one matching the default machine name. Default to 0 which create
 *   a new vocabulary if no existing one is found.
 *
 * @return int
 *   Vocabulary ID if successful, 0 otherwise.
 *
 * @see affiliate_store_vocabularies()
 */
function affiliate_store_save_vocabulary($name, $vid = 0) {
  // Required settings.
  $vocab_base = array(
    'hierarchy' => 0,
    'module' => 'affiliate_store',
  );
  // Overridable settings.
  $vocabs = array(
    'network' => array(
      'name' => t('Affiliate store networks'),
      'machine_name' => 'affiliate_store_networks',
      'description' => t('Names of affiliate store networks.'),
      'weight' => -10,
    ),
    'merchant' => array(
      'name' => t('Affiliate store merchants'),
      'machine_name' => 'affiliate_store_merchants',
      'description' => t('Names of affiliate store merchants.'),
      'weight' => -9,
    ),
    'category' => array(
      'name' => t('Affiliate store categories'),
      'machine_name' => 'affiliate_store_categories',
      'description' => t('Names of affiliate store product categories.'),
      'weight' => -8,
    ),
    'subcategory' => array(
      'name' => t('Affiliate store subcategories'),
      'machine_name' => 'affiliate_store_subcategories',
      'description' => t('Names of affiliate store product subcategories.'),
      'weight' => -7,
    ),
    'subsubcategory' => array(
      'name' => t('Affiliate store subsubcategories'),
      'machine_name' => 'affiliate_store_subsubcategories',
      'description' => t('Names of affiliate store product subsubcategories.'),
      'weight' => -6,
    ),
  );
  if (!isset($vocabs[$name])) {
    return 0;
  }

  $vocab_ori = $vid ? taxonomy_vocabulary_load($vid) : NULL;
  $vocab_ori = $vocab_ori ?
    $vocab_ori :
    // Check if there is existing vocabulary with the same machine name.
    taxonomy_vocabulary_machine_name_load($vocabs[$name]['machine_name']);
  $vocab_ori = $vocab_ori ? (array) $vocab_ori : array();

  $vocab = $vocab_base + $vocab_ori + $vocabs[$name];
  $vocab = (object) $vocab;

  // NOTE: A core bug causing exception when there is database DDL statement
  // after calling db_transaction().
  // See issue http://drupal.org/node/1007830
  // $txn = db_transaction();
  // try {
    taxonomy_vocabulary_save($vocab);
    $vid = (int) $vocab->vid;
    variable_set("affiliate_store_vid_$name", $vid);

    // Add term synonym and term reference fields.
    affiliate_store_add_vocabulary_term_synonym_field($name);
    affiliate_store_add_vocabulary_term_reference_field($name);
  // }
  // catch (Exception $e) {
  //   $txn->rollback();
  //   watchdog_exception('affiliate store', $e);
  //   throw $e;
  // }

  return $vid;
}

/**
 * Check for affiliate store setup error.
 *
 * @return array
 *   An associative array of error messages keyed by setting names that have
 *   problem. If the array is empty, it means the setup is OK.
 */
function affiliate_store_setup_error() {
  $messages = array();
  if (!variable_get('affiliate_store_api_key', '')) {
    $messages['affiliate_store_api_key'] = t(
      'You have not enter your service API key.'
    );
  }
  if (affiliate_store_json_decode('{"test": ["test"]}', $error) === NULL) {
    $messages['affiliate_store_json_decode'] = t(
      'No support for JSON decoding found. See module README.txt for ' .
      'installation instruction.'
    );
  }
  if (affiliate_store_hash_hmac('sha256', 'test', 'test') === NULL) {
    $messages['affiliate_store_hash_hmac'] = t(
      'No support for HMAC generation found. See module README.txt for ' .
      'installation instruction.'
    );
  }
  return $messages;
}

/**
 * Titlecase words in a string with stop words exception.
 *
 * @param string $text
 *   The text that will be converted.
 * @param mixed $stopwords
 *   (optional) An array of stop words or a string of stop words separated by
 *   comma to be remained in lowercase. Default to NULL meaning use stop words
 *   as configured at settings page.
 *
 * @return string
 *   Input $text in titlecase with stop words in lowercase.
 */
function affiliate_store_smartcase($text, $stopwords = NULL) {
  if ($stopwords === NULL) {
    $stopwords = variable_get(
      'affiliate_store_smartcase_stop_words',
      'a,an,and,as,at,by,for,is,in,into,of,off,on,onto,or,per,the,to,with'
    );
  }

  $text = affiliate_store_ucwords($text);
  if ($stopwords) {
    $stopwords = is_string($stopwords) ? explode(',', $stopwords) : $stopwords;
    $stopwords = array_map('trim', $stopwords);
    $delimiters = array_fill(0, count($stopwords), '/');
    $stopwords = array_map('preg_quote', $stopwords, $delimiters);
    $stopwords = implode('|', array_map('drupal_ucfirst', $stopwords));
    $text = preg_replace_callback(
      "/(?<=(?<!:|'|ʼ|'s|ʼs)[" . PREG_CLASS_UNICODE_WORD_BOUNDARY . "])" .
      "($stopwords)" .
      "(?=$|[" . PREG_CLASS_UNICODE_WORD_BOUNDARY . "])/u",
      '_affiliate_store_smartcase_callback',
      $text
    );
  }
  return $text;
}

/**
 * Capitalizes the first letter of each word in a UTF-8 string.
 *
 * @param string $text
 *   The text that will be converted.
 *
 * @return string
 *   The input $text with each word capitalized.
 */
function affiliate_store_ucwords($text) {
  return preg_replace_callback(
    '/(^|[' . PREG_CLASS_UNICODE_WORD_BOUNDARY . '])' .
    '([^' . PREG_CLASS_UNICODE_WORD_BOUNDARY . '])/u',
    '_affiliate_store_ucwords_callback',
    $text
  );
}

/**
 * Impersonate another user.
 *
 * @param mixed $new_user
 *   User ID or user object to impersonate.
 *
 * @return object
 *   Current user.
 *
 * @see affiliate_store_revert_user()
 */
function affiliate_store_impersonate_user($new_user) {
  global $user;
  static $user_original = NULL;
  static $old_state = TRUE;

  if ($new_user) {
    // Prevent session saving.
    $user_original = $user;
    $old_state = drupal_save_session();
    drupal_save_session(FALSE);
    $user = is_numeric($new_user) ? user_load($new_user) : $new_user;
  }
  elseif ($user_original) {
    // Restore original user and session saving state.
    $user = $user_original;
    drupal_save_session($old_state);
  }
  return $user;
}

/**
 * Revert to previous user after impersonating.
 *
 * @return object
 *   Current user.
 *
 * @see affiliate_store_impersonate_user()
 */
function affiliate_store_revert_user() {
  return affiliate_store_impersonate_user(NULL);
}

/**
 * Get filter format suitable for filtering HTML text.
 *
 * @return string
 *   Filter format ID.
 */
function _affiliate_store_get_filtered_html_format() {
  // Test for default filtered HTML format.
  if (filter_format_load('filtered_html')) {
    return 'filtered_html';
  }
  // Also test for filtered HTML format from Drupal 6 in case this Drupal
  // install had went through core update.
  elseif (filter_format_load('1')) {
    return '1';
  }
  // Use fallback format as last measure.
  return filter_fallback_format();
}

/**
 * Lowercase stop words in affiliate_store_smartcase().
 *
 * @param array $matches
 *   Matched stop words. Elements 0 and 1 must be the full match and the stop
 *   word.
 *
 * @return string
 *   Stop word in lowercase.
 */
function _affiliate_store_smartcase_callback($matches) {
  return drupal_strtolower($matches[1]);
}

/**
 * Helper callback that is used to capitalize the first character of a word.
 *
 * @param array $matches
 *   Matched characters. Elements 0, 1, and 2 must be the full match, the word
 *   boundary, and the first character of the word.
 *
 * @return string
 *   Matched word boundary and uppercase of the first character.
 */
function _affiliate_store_ucwords_callback($matches) {
  // Make exceptions. Prevent apostrophe from being regarded as boundary by
  // PREG_CLASS_UNICODE_WORD_BOUNDARY, so 's would not becomes 'S.
  if (in_array($matches[1], array("'", "ʼ"), TRUE)) {
    return $matches[1] . $matches[2];
  }
  return $matches[1] . drupal_strtoupper($matches[2]);
}
