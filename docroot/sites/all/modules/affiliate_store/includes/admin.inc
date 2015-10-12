<?php

/**
 * @file
 * Admin pages.
 */

/**
 * Menu callback; Return a page for affiliate store settings.
 */
function affiliate_store_settings_page($form, &$form_state) {
  // Show affiliate store setup error if any.
  if (empty($form_state['input'])) {
    affiliate_store_include('common');
    foreach (affiliate_store_setup_error() as $error) {
      drupal_set_message($error, 'error');
    }
  }

  $form['affiliate_store_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Store title'),
    '#description' => t(
      'The title will be used in block, menu, and ' .
      '<a href="@storefront">storefront</a>.',
      array('@storefront' => url('affiliate-store'))
    ),
    '#default_value' => affiliate_store_title(t('Store')),
    '#required' => TRUE,
  );
  $form['affiliate_store_domain'] = array(
    '#type' => 'item',
    '#title' => t('Store domain'),
    '#description' => t(
      'This is your website domain you should use to associate with your ' .
      'affiliate store account on the Hub.'
    ),
    '#markup' => check_url(affiliate_store_id()),
  );
  $form['affiliate_store_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Service API key'),
    '#description' => t(
      'Enter the service API key you obtained from !hub. You can sign up for ' .
      'one at !url if you are not already a member.',
      array(
        '!hub' => AFFILIATE_STORE_HUB,
        '!url' => l(
          AFFILIATE_STORE_HUB, AFFILIATE_STORE_HUB_URL,
          array('attributes' => array('target' => '_blank'))
        ),
      )
    ),
    '#default_value' => variable_get('affiliate_store_api_key', ''),
    '#required' => TRUE,
  );
  $form['affiliate_store_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Storefront message'),
    '#description' => t(
      'Message to display at the storefront, useful for showing disclaimer, ' .
      'recommendation, or instruction for your users.'
    ),
    '#default_value' => variable_get('affiliate_store_message', ''),
  );

  return system_settings_form($form);
}

/**
 * Menu callback; Return a page for affiliate store advanced settings.
 */
function affiliate_store_advanced_settings_page($form, &$form_state) {
  // Return bulk operations page if bulk operations button has been pressed.
  if (isset($form_state['values']['op'])
    && ($form_state['values']['op'] === t('Rename existing terms'))
  ) {
    return affiliate_store_term_bulk_rename_page($form, $form_state);
  }

  $form['cache'] = array(
    '#type' => 'fieldset',
    '#title' => t('Clear cached data'),
    '#description' => t(
      "Clear affiliate store caches. Only needed if your website has caching " .
      "turned on and you want to refresh stale pages in affiliate store " .
      "after making changes, e.g. in category mapping."
    ),
  );
  $form['cache']['clear'] = array(
    '#type' => 'submit',
    '#value' => t('Clear cached data'),
    '#submit' => array('affiliate_store_settings_cache_submit'),
  );

  $form['product'] = array(
    '#type' => 'fieldset',
    '#title' => t('Product'),
    '#description' => t('Configure product display options.'),
  );
  $form['product']['affiliate_store_product_call_to_action'] = array(
    '#type' => 'textfield',
    '#title' => t('Call to action text'),
    '#description' => t(
      'The text to use for persuading user to click on the product link.'
    ),
    '#default_value' => variable_get(
      'affiliate_store_product_call_to_action', t('Buy now')
    ),
    '#required' => TRUE,
  );
  $form['product']['affiliate_store_product_use_general'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use pseudo general categories'),
    '#description' => t(
      "Check to use term 'general' in place of empty categories, when they " .
      "are not assigned by merchant for a product."
    ),
    '#default_value' => variable_get(
      'affiliate_store_product_use_general', FALSE
    ),
  );

  $form['update'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update'),
    '#description' => t('Configure affiliate store update behaviour.'),
  );
  $form['update']['affiliate_store_auto_update'] = array(
    '#type' => 'checkbox',
    '#title' => t('Update affiliate store automatically'),
    '#description' => t('Uncheck to disable automatic update via cron.'),
    '#default_value' => variable_get('affiliate_store_auto_update', TRUE),
  );
  $form['update']['affiliate_store_retrieve_limit'] = array(
    '#type' => 'select',
    '#title' => t('Retrieval limit'),
    '#description' => t(
      'Maximum number of items to retrieve in one service call to the Hub. ' .
      'Larger value makes update faster but uses more bandwidth and memory. ' .
      'Lowering this value may help with update problem.'
    ),
    '#options' => drupal_map_assoc(
      array(
        10, 20, 30, 40, 50, 60, 70, 80, 90,
        100, 200, 300, 400, 500, 600, 700, 800, 900, 1000,
      )
    ),
    '#default_value' => variable_get('affiliate_store_retrieve_limit', 500),
  );
  $form['update']['affiliate_store_update_throttle'] = array(
    '#type' => 'select',
    '#title' => t('Update throttle'),
    '#description' => t(
      'Maximum number of operations to perform in one pass of update process ' .
      'before waiting for next turn. Larger value makes update faster but ' .
      'uses more system resources.'
    ),
    '#options' => drupal_map_assoc(
      array(
        10, 20, 30, 40, 50, 60, 70, 80, 90,
        100, 200, 300, 400, 500, 600, 700, 800, 900, 1000,
      )
    ),
    '#default_value' => variable_get('affiliate_store_update_throttle', 50),
  );

  $form['taxonomy'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxonomy'),
    '#description' => t(
      'Configure how taxonomy terms are imported into affiliate store.'
    ),
  );
  $form['taxonomy']['affiliate_store_term_case'] = array(
    '#type' => 'radios',
    '#title' => t('Term name case conversion'),
    '#description' => t(
      'Case conversion to use for category terms. Smartcase is like ' .
      'titlecase but with words designated as stop words below in lowercase.'
    ),
    '#options' => array(
      'none' => 'No conversion',
      'lower' => 'Lowercase: this is an example',
      'upper' => 'Uppercase: THIS IS AN EXAMPLE',
      'title' => 'Titlecase: This Is An Example',
      'smart' => 'Smartcase: This is an Example',
    ),
    '#default_value' => variable_get('affiliate_store_term_case', 'smart'),
    '#required' => TRUE,
  );
  $form['taxonomy']['affiliate_store_smartcase_stop_words'] = array(
    '#type' => 'textarea',
    '#title' => t('Smartcase stop words'),
    '#description' => t(
      'Words that should remain lowercase when not used as first word in a ' .
      'phrase, separated by comma. Do not use WYSIWYG editors on this field.'
    ),
    '#default_value' => variable_get(
      'affiliate_store_smartcase_stop_words',
      'a,an,and,as,at,by,for,is,in,into,of,off,on,onto,or,per,the,to,with'
    ),
  );
  $form['taxonomy']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Rename existing terms'),
    '#submit' => array(
      // Save settings before renaming terms.
      'affiliate_store_advanced_settings_page_submit',
      'system_settings_form_submit',
      'affiliate_store_settings_taxonomy_submit',
    ),
  );

  $form['#submit'] = array('affiliate_store_advanced_settings_page_submit');
  return system_settings_form($form);
}

/**
 * Submit callback for affiliate_store_advanced_settings_page().
 */
function affiliate_store_advanced_settings_page_submit($form, &$form_state) {
  // Trim spaces between stop words.
  $stopwords = &$form_state['values']['affiliate_store_smartcase_stop_words'];
  $stopwords = implode(',', array_map('trim', explode(',', $stopwords)));
}

/**
 * Submit callback for affiliate_store_advanced_settings_page() cache settings.
 */
function affiliate_store_settings_cache_submit($form, &$form_state) {
  affiliate_store_include('common');
  affiliate_store_clear_cache();
  drupal_set_message(t('Affiliate store caches have been cleared.'));
}

/**
 * Submit callback for affiliate_store_advanced_settings_page() taxonomy
 * settings.
 */
function affiliate_store_settings_taxonomy_submit($form, &$form_state) {
  // Rebuild form to go to 2nd step, e.g. to show confirmation form.
  $form_state['rebuild'] = TRUE;
  // This is needed because it had been unset after settings saved by
  // system_settings_form_submit().
  $form_state['values']['op'] = t('Rename existing terms');
}

/**
 * Menu callback; Return category mapping page.
 *
 * @param string $vocabulary
 *   Term vocabulary logical name.
 *
 * @see affiliate_store_vocabularies()
 */
function affiliate_store_category_map_page($form, &$form_state, $vocabulary) {
  // Return bulk operations page if bulk operations button has been pressed.
  if (isset($form_state['values']['op'])) {
    switch ($form_state['values']['op']) {
      case t('Delete'):
        return affiliate_store_term_synonym_bulk_delete_page(
          $form, $form_state,
          array_keys(array_filter($form_state['values']['tids']))
        );
      case t('Merge'):
        return affiliate_store_term_synonym_bulk_merge_page(
          $form, $form_state,
          array_keys(array_filter($form_state['values']['tids']))
        );
    }
  }

  $destination = drupal_get_destination();
  $vid = affiliate_store_get_vocabulary_id($vocabulary);
  $miss_tid = affiliate_store_get_term_id('', $vocabulary);
  $header = array(
    'term' => array(
      'data' => t('Term'),
      'type' => 'property',
      'specifier' => 'name',
      'sort' => 'asc',
    ),
    'synonym' => array(
      'data' => t('Synonym'),
      'type' => 'field',
      'specifier' => array(
        'field' => 'affiliate_store_synonym',
        'column' => 'value',
      ),
    ),
    'operations' => t('Operations'),
  );

  $query = new EntityFieldQuery();
  $query->pager(50);
  $query->tableSort($header);
  $query->entityCondition('entity_type', 'taxonomy_term');
  $query->propertyCondition('vid', $vid);
  $result = $query->execute();
  $terms = !empty($result['taxonomy_term']) ?
    taxonomy_term_load_multiple(array_keys($result['taxonomy_term'])) :
    array();

  $options = array();
  foreach ($terms as $tid => $term) {
    // Hide pseudo general categories.
    if ($tid == $miss_tid) {
      continue;
    }

    $options[$tid] = array(
      'term' => check_plain($term->name),
      'synonym' => check_plain(affiliate_store_get_term_synonym($term)),
      'operations' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => t('edit'),
          '#href' => "admin/structure/affiliate-store/map/term/$tid/edit",
          '#options' => array('query' => $destination),
        ),
      ),
    );
  }
  $form['tids'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No categories imported yet.'),
  );
  $form['pager'] = array('#theme' => 'pager');

  $form['terms'] = array(
    '#type' => 'value',
    '#value' => $terms,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['merge'] = array(
    '#type' => 'submit',
    '#value' => t('Merge'),
    '#validate' => array(
      'affiliate_store_category_map_bulk_operations_validate',
    ),
    '#submit' => array('affiliate_store_category_map_bulk_operations_submit'),
  );
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#validate' => array(
      'affiliate_store_category_map_bulk_operations_validate',
    ),
    '#submit' => array('affiliate_store_category_map_bulk_operations_submit'),
  );
  return $form;
}

/**
 * Validate callback for affiliate_store_category_map_page().
 */
function affiliate_store_category_map_bulk_operations_validate($form,
&$form_state) {
  if (!count(array_filter($form_state['values']['tids']))) {
    form_set_error('tids', t('No terms selected.'));
  }
}

/**
 * Submit callback for affiliate_store_category_map_page().
 */
function affiliate_store_category_map_bulk_operations_submit($form,
&$form_state) {
  // Rebuild form to go to 2nd step, e.g. to show confirmation form.
  $form_state['rebuild'] = TRUE;
}

/**
 * Return term synonym edit page.
 *
 * @param int $tid
 *   Term ID whose synonym to edit.
 */
function affiliate_store_category_map_edit_page($form, &$form_state, $tid) {
  $term = taxonomy_term_load($tid);
  $vocab = $term ? affiliate_store_get_vocabulary_name($term->vid) : '';
  // Make sure the term exists and is managed by affiliate store.
  $categories = array('category', 'subcategory', 'subsubcategory');
  if (in_array($vocab, $categories, TRUE)) {
    // Cancel link parameters. Use supplied destination if exists.
    $url = array('path' => "admin/structure/affiliate-store/map/$vocab");
    if (!empty($_GET['destination'])) {
      $url = drupal_parse_url(urldecode($_GET['destination'])) + $url;
    }

    $form['tid'] = array(
      '#type' => 'value',
      '#value' => $tid,
    );
    $form['term'] = array(
      '#type' => 'item',
      '#title' => t('Term'),
      '#markup' => check_plain($term->name),
    );
    $form['synonym'] = array(
      '#type' => 'textfield',
      '#title' => t('Synonym'),
      '#default_value' => affiliate_store_get_term_synonym($term),
    );

    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
    $form['actions']['cancel'] = array(
      '#markup' => l(t('Cancel'), $url['path'], $url),
    );
  }
  else {
    $form['error'] = array(
      '#markup' => t('Term not exists or not managed by affiliate store.'),
    );
  }
  return $form;
}

/**
 * Submit callback for affiliate_store_category_map_edit_page().
 */
function affiliate_store_category_map_edit_page_submit($form, &$form_state) {
  $term = taxonomy_term_load($form_state['values']['tid']);
  $synonym = $form_state['values']['synonym'];

  if ($synonym) {
    affiliate_store_include('common');
    $synonym = affiliate_store_convert_term_case($synonym);
    affiliate_store_save_data('taxonomy_term', $term, '_synonym', $synonym);
  }
  else {
    affiliate_store_delete_data('taxonomy_term', $term, '_synonym');
  }
  taxonomy_term_save($term);
}

/**
 * Return a confirmation page for bulk term renaming.
 */
function affiliate_store_term_bulk_rename_page($form, &$form_state) {
  $form['#submit'] = array('affiliate_store_term_bulk_rename_page_submit');
  return confirm_form(
    $form,
    t('Are you sure you want to rename existing terms?'),
    "admin/structure/affiliate-store",
    t('Any prior customization will be lost.'),
    t('Rename all'),
    t('Cancel')
  );
}

/**
 * Submit callback for affiliate_store_term_bulk_rename_page().
 */
function affiliate_store_term_bulk_rename_page_submit($form, &$form_state) {
  $path = drupal_get_path('module', 'affiliate_store');
  $batch = array(
    'operations' => array(
      array('affiliate_store_batch_rename_terms', array()),
    ),
    'finished' => 'affiliate_store_batch_rename_terms_finish',
    'title' => t('Rename terms'),
    'init_message' => t('Term renaming is starting.'),
    'progress_message' => t('Finished @current out of @total.'),
    'error_message' => t('Term renaming has encountered an error.'),
    'file' => "$path/includes/admin.batch.inc",
  );
  batch_set($batch);
}

/**
 * Return a confirmation page for bulk term synonym deletion.
 *
 * @param array $terms
 *   Term ID of synonyms to delete.
 */
function affiliate_store_term_synonym_bulk_delete_page($form, &$form_state,
$terms) {
  $items = array();
  $terms_info = array();
  foreach ($terms as $tid) {
    $terms_info[$tid] = $form_state['values']['terms'][$tid];
  }
  foreach ($terms_info as $info) {
    $synonym = affiliate_store_get_term_synonym($info);
    $items[] = t(
      'Term: %term, Synonym: %synonym',
      array('%term' => $info->name, '%synonym' => $synonym)
    );
  }

  $form['terms'] = array(
    '#type' => 'value',
    '#value' => $terms,
  );
  $form['term_list'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
  );
  $form['#submit'] = array(
    'affiliate_store_term_synonym_bulk_delete_page_submit',
  );
  $vocab = affiliate_store_get_vocabulary_name($terms_info[$terms[0]]->vid);
  return confirm_form(
    $form,
    t('Are you sure you want to delete these term synonyms?'),
    "admin/structure/affiliate-store/map/$vocab",
    t('This action cannot be undone.'),
    t('Delete all'),
    t('Cancel')
  );
}

/**
 * Submit callback for affiliate_store_term_synonym_bulk_delete_page().
 */
function affiliate_store_term_synonym_bulk_delete_page_submit($form,
&$form_state) {
  $terms = taxonomy_term_load_multiple($form_state['values']['terms']);

  $txn = db_transaction();
  try {
    foreach ($terms as $term) {
      affiliate_store_delete_data('taxonomy_term', $term, '_synonym');
      taxonomy_term_save($term);
    }
  }
  catch (Exception $e) {
    $txn->rollback();
    watchdog_exception('affiliate store', $e);
    throw $e;
  }

  drupal_set_message(
    format_plural(
      count($terms),
      'The term synonym has been deleted.',
      '@count term synonyms have been deleted.'
    )
  );
}

/**
 * Return a confirmation page for bulk term synonym merging.
 *
 * @param array $terms
 *   Term ID of synonyms to merge.
 */
function affiliate_store_term_synonym_bulk_merge_page($form, &$form_state,
$terms) {
  $default_synonym = '';
  $items = array();
  $terms_info = array();
  foreach ($terms as $tid) {
    $terms_info[$tid] = $form_state['values']['terms'][$tid];
  }
  foreach ($terms_info as $info) {
    $synonym = affiliate_store_get_term_synonym($info);
    // Use the 1st non-empty synonym as default.
    if (!$default_synonym) {
      $default_synonym = $synonym;
    }
    $items[] = t(
      'Term: %term, Synonym: %synonym',
      array('%term' => $info->name, '%synonym' => $synonym)
    );
  }

  $form['terms'] = array(
    '#type' => 'value',
    '#value' => $terms,
  );
  $form['term_list'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
  );
  $form['synonym'] = array(
    '#type' => 'textfield',
    '#title' => t('Synonym'),
    '#description' => t('Synonym for all the selected terms.'),
    '#default_value' => $default_synonym,
  );
  $form['#validate'] = array(
    'affiliate_store_term_synonym_bulk_merge_page_validate',
  );
  $form['#submit'] = array(
    'affiliate_store_term_synonym_bulk_merge_page_submit',
  );
  $vocab = affiliate_store_get_vocabulary_name($terms_info[$terms[0]]->vid);
  return confirm_form(
    $form,
    t("Merge categories"),
    "admin/structure/affiliate-store/map/$vocab",
    '',
    t('Save'),
    t('Cancel')
  );
}

/**
 * Validate callback for affiliate_store_term_synonym_bulk_merge_page().
 */
function affiliate_store_term_synonym_bulk_merge_page_validate($form,
&$form_state) {
  if (!$form_state['values']['synonym']) {
    form_set_error('synonym', t('You must specify a synonym.'));
  }
}

/**
 * Submit callback for affiliate_store_term_synonym_bulk_merge_page().
 */
function affiliate_store_term_synonym_bulk_merge_page_submit($form,
&$form_state) {
  $synonym = $form_state['values']['synonym'];
  // Only make changes if synonym not empty to prevent term synonyms from
  // being deleted.
  if (!$synonym) {
    return;
  }

  affiliate_store_include('common');
  $synonym = affiliate_store_convert_term_case($synonym);
  $terms = taxonomy_term_load_multiple($form_state['values']['terms']);

  $txn = db_transaction();
  try {
    foreach ($terms as $term) {
      affiliate_store_save_data('taxonomy_term', $term, '_synonym', $synonym);
      taxonomy_term_save($term);
    }
  }
  catch (Exception $e) {
    $txn->rollback();
    watchdog_exception('affiliate store', $e);
    throw $e;
  }

  drupal_set_message(
    format_plural(
      count($terms),
      'The term synonym has been merged.',
      '@count term synonyms have been merged.'
    )
  );
}

/**
 * Menu callback; Return affiliate store subscriptions page.
 */
function affiliate_store_subscription_page($form, &$form_state) {
  // Return bulk operations page if bulk operations button has been pressed.
  if (isset($form_state['values']['op'])
    && ($form_state['values']['op'] === t('Refetch'))
  ) {
    return affiliate_store_subscription_bulk_refetch_page(
      $form, $form_state,
      array_keys(array_filter($form_state['values']['ssnids']))
    );
  }

  $categories = array('category', 'subcategory', 'subsubcategory');
  $categories = affiliate_store_vocabularies($categories);
  foreach (array_keys($categories) as $vocab) {
    $categories[$vocab]['miss_tid'] = affiliate_store_get_term_id('', $vocab);
  }
  $header = array(
    array('data' => t('Network'), 'field' => 'network', 'sort' => 'asc'),
    array('data' => t('Merchant'), 'field' => 'merchant'),
    array('data' => t('Category'), 'field' => 'category'),
    array('data' => t('Subcategory'), 'field' => 'subcategory'),
    array('data' => t('Subsubcategory'), 'field' => 'subsubcategory'),
    array('data' => t('Product'), 'field' => 'product'),
  );

  $idx = 1;
  $on = array();
  $query = db_select('affiliate_store_subscription', 'ssn')
    ->extend('PagerDefault')
    ->limit(50)
    ->extend('TableSort')
    ->orderByHeader($header)
    ->fields('ssn', array('ssnid', 'ctid', 'sctid', 'ssctid'));
  foreach (affiliate_store_vocabularies() as $vocab => $info) {
    $as = $info['as'];
    $on[] = "p.{$as}tid = ssn.{$as}tid";
    $query->leftJoin('taxonomy_term_data', "t$idx", "t$idx.tid = ssn.{$as}tid");
    $query->addField("t$idx", 'name', $vocab);
    ++$idx;
  }
  $query->leftJoin('affiliate_store_product', 'p', implode(' AND ', $on));
  $query->addExpression('COUNT(DISTINCT p.nid)', 'product');
  $query->condition('ssn.refetch', 0);
  $query->groupBy('ssn.ssnid');

  $options = array();
  $subscriptions = array();
  foreach ($query->execute() as $ssn) {
    // Hide pseudo general categories.
    foreach ($categories as $vocab => $info) {
      if ($ssn->{$info['as'] . 'tid'} == $info['miss_tid']) {
        $ssn->$vocab = '';
      }
    }

    $subscriptions[$ssn->ssnid] = $ssn;
    $options[$ssn->ssnid] = array(
      check_plain($ssn->network),
      check_plain($ssn->merchant),
      check_plain($ssn->category),
      check_plain($ssn->subcategory),
      check_plain($ssn->subsubcategory),
      $ssn->product,
    );
  }
  $form['ssnids'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No subscriptions imported yet.'),
  );
  $form['pager'] = array('#theme' => 'pager');

  $form['subscriptions'] = array(
    '#type' => 'value',
    '#value' => $subscriptions,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Refetch'),
    '#validate' => array(
      'affiliate_store_subscription_bulk_operations_validate',
    ),
    '#submit' => array('affiliate_store_subscription_bulk_operations_submit'),
  );
  return $form;
}

/**
 * Validate callback for affiliate_store_subscription_page().
 */
function affiliate_store_subscription_bulk_operations_validate($form,
&$form_state) {
  if (!count(array_filter($form_state['values']['ssnids']))) {
    form_set_error('ssnids', t('No subscriptions selected.'));
  }
}

/**
 * Submit callback for affiliate_store_subscription_page().
 */
function affiliate_store_subscription_bulk_operations_submit($form,
&$form_state) {
  // Rebuild form to go to 2nd step, e.g. to show confirmation form.
  $form_state['rebuild'] = TRUE;
}

/**
 * Return a confirmation page for bulk subscriptions refetch.
 *
 * @param array $subscriptions
 *   Subscription IDs to refetch.
 */
function affiliate_store_subscription_bulk_refetch_page($form, &$form_state,
$subscriptions) {
  $form['subscriptions'] = array(
    '#type' => 'value',
    '#value' => $subscriptions,
  );
  $items = array();
  $subscriptions_info = array();
  foreach ($subscriptions as $ssnid) {
    $subscriptions_info[$ssnid] = (
      $form_state['values']['subscriptions'][$ssnid]
    );
  }
  foreach ($subscriptions_info as $info) {
    $items[] = t(
      'Network: %network, Merchant: %merchant, Category: %category, ' .
      'Subcategory: %subcategory, Subsubcategory: %subsubcategory',
      array(
        '%network' => $info->network,
        '%merchant' => $info->merchant,
        '%category' => $info->category,
        '%subcategory' => $info->subcategory,
        '%subsubcategory' => $info->subsubcategory,
      )
    );
  }
  $form['subscription_list'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
  );
  $form['#submit'] = array(
    'affiliate_store_subscription_bulk_refetch_page_submit',
  );
  return confirm_form(
    $form,
    t('Are you sure you want to refetch the products of these subscriptions?'),
    'admin/structure/affiliate-store/subscriptions',
    t('This action cannot be undone.'),
    t('Refetch all'),
    t('Cancel')
  );
}

/**
 * Submit callback for affiliate_store_subscription_bulk_refetch_page().
 */
function affiliate_store_subscription_bulk_refetch_page_submit($form,
&$form_state) {
  $subscriptions = $form_state['values']['subscriptions'];
  affiliate_store_include('subscription');
  affiliate_store_subscription_refetch($subscriptions);
  drupal_set_message(
    format_plural(
      count($subscriptions),
      'The subscription will be refetched at the next update.',
      '@count subscriptions will be refetched at the next update.'
    )
  );
}

/**
 * Menu callback; Return affiliate store update page.
 */
function affiliate_store_update_page($form, &$form_state) {
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  affiliate_store_include('common');
  if (affiliate_store_setup_error()) {
    $form['actions']['submit']['#disabled'] = TRUE;
    $message = t(
      'There is problem with your affiliate store setup. Please check your ' .
      '<a href="@settings">affiliate store settings</a>.',
      array('@settings' => url('admin/structure/affiliate-store'))
    );
    drupal_set_message($message, 'error');
  }

  return $form;
}

/**
 * Submit callback for affiliate_store_update_page().
 */
function affiliate_store_update_page_submit($form, &$form_state) {
  $options = array(
    'update_throttle' => variable_get('affiliate_store_update_throttle', 50),
    'sleep' => 1,
  );
  $path = drupal_get_path('module', 'affiliate_store');
  $batch = array(
    'operations' => array(
      array(
        'affiliate_store_batch_import_subscription',
        array(t('Importing pending data: @current of @total.'), $options),
      ),
      array('affiliate_store_batch_fetch_subscription', array($options)),
      array(
        'affiliate_store_batch_import_subscription',
        array(t('Importing new data: @current of @total.'), $options),
      ),
    ),
    'finished' => 'affiliate_store_batch_update_finish',
    'title' => t('Updating affiliate store'),
    'init_message' => t('Update process is starting.'),
    'progress_message' => t('Finished @current out of @total.'),
    'error_message' => t('Update has encountered an error.'),
    'file' => "$path/includes/admin.batch.inc",
  );
  batch_set($batch);
}
