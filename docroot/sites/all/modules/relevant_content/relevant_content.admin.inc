<?php

/**
 * @file relevant_content.admin.inc
 * This include contains all the admin-only functionality
 */

/**
 * Settings page form
 *
 * This function provides the module settings page form which is used for enabling content type blocks.
 */
function relevant_content_admin() {
  $content_types = node_type_get_names();
  $vocabularies = taxonomy_get_vocabularies();

  $rows = array();
  foreach (relevant_content_get_settings() as $block) {
    array_walk($block['types'],  '_relevant_content_array_map_key_to_values', $content_types);
    array_walk($block['vocabs'], '_relevant_content_array_map_key_to_values', $vocabularies);

    // Set the operations based on status and storage
    // TODO - is there a neater way to code this?
    $ops = array();
    switch ($block['status']) {
      // Default / Disabled
      default :
      case RELEVANT_CONTENT_STATUS_DISABLED :
        // Show Enable op
        $ops[] = l(t('Enable'), RELEVANT_CONTENT_ADMIN_BASE_PATH . '/' . $block['id'] . '/enable');

        if ($block['storage'] == RELEVANT_CONTENT_STORAGE_OVERRIDDEN) {
          $ops[] = l(t('Revert'), RELEVANT_CONTENT_ADMIN_BASE_PATH . '/' . $block['id'] . '/revert');
        }
        if ($block['storage'] != RELEVANT_CONTENT_STORAGE_DEFAULT) {
          $ops[] = l(t('Edit'), RELEVANT_CONTENT_ADMIN_BASE_PATH . '/' . $block['id'] . '/edit');
        }
        break;

      case RELEVANT_CONTENT_STATUS_ENABLED :
        // If enabled, show Edit + either Delete, Disable or Revert -depending on storage
        $ops[] = l(t('Edit'), RELEVANT_CONTENT_ADMIN_BASE_PATH . '/' . $block['id'] . '/edit');

        switch ($block['storage']) {
          // NORMAL == Database - therefore delete
          case RELEVANT_CONTENT_STORAGE_NORMAL :
            $ops[] = l(t('Delete'), RELEVANT_CONTENT_ADMIN_BASE_PATH . '/' . $block['id'] . '/delete');
            break;

          // DEFAULT == In Coder - therefore disable
          case RELEVANT_CONTENT_STORAGE_DEFAULT :
            $ops[] = l(t('Disable'), RELEVANT_CONTENT_ADMIN_BASE_PATH . '/' . $block['id'] . '/disable');
            break;

          // OVERRIDDEN == In Code AND in DB - therefore, revert
          case RELEVANT_CONTENT_STORAGE_OVERRIDDEN :
            $ops[] = l(t('Revert'), RELEVANT_CONTENT_ADMIN_BASE_PATH . '/' . $block['id'] . '/revert');
            break;
        }
        break;
    }

    $rows[$block['id']] = array(
      'block' => $block,
      'ops' => $ops,
    );
  }

  return theme('relevant_content_admin_overview', array('rows' => $rows));
}


/**
 * Private function used as a callback for array_map which sets the item to the value of $value using $key as either an array offset or an object property.
 *
 * @param $item
 *   This is a variable reference to the item being mapped to. Chaning this value will change the value in the array being mapped using array_map
 * @param $key
 *   They key of $item in the array being mapped
 * @param $values
 *   A user defined array passed in. In this case, it us used for reference purposes
 */
function _relevant_content_array_map_key_to_values(&$item, $key, $values) {
  if (isset($values[$key])) {
    if (is_object($values[$key])) {
      $item = $values[$key]->name;
    }
    else {
      $item = $values[$key];
    }
  }
  else {
    $item = t('Error: Item missing.');
  }
}



/**
 * Form API based function which generates the add AND edit forms. The form is an 'edit' form if a block id is passed in for editing.
 *
 * @param $block_id
 *   mixed - The Block ID of the block being editted. Will effectively be the array key in the $settings array.
 */
function relevant_content_admin_block_form($form, &$form_state, $block = NULL) {
  // If $block is NULL, define an empty array instead of FALSE
  if (!$block) {
    $block = array();
  }

  // Sanitize the block to defaults
  $block = _relevant_content_santize_block($block);

  $form = array();

  $form['settings'] = array(
    '#tree' => TRUE,
  );

  if ($block['id']) {
    $form['settings']['id'] = array(
      '#type' => 'value',
      '#value' => $block['id'],
    );
    $form['settings']['id_field'] = array(
      '#type' => 'item',
      '#title' => t('Block <em>ID</em>'),
      '#description' => t('The <em>ID</em> is used to uniquely identify a block.'),
      '#markup' => check_plain($block['id']),
    );
  }
  else {
    $form['settings']['id'] = array(
      '#type' => 'textfield',
      '#title' => t('Block <em>ID</em>'),
      '#description' => t('The <em>ID</em> is used to uniquely identify a block. Please use lowercase characters, numbers and underscores only'),
      '#required' => TRUE,
      '#maxlength' => 255,
      '#size' => 12,
    );
  }

  $form['settings']['types'] = array(
    '#type' => 'checkboxes',
    '#title' =>  t('Enabled Content Types'),
    '#description' => t('Check the content types you would like to search for.'),
    '#options' => node_type_get_names(),
    '#default_value' => $block['types'],
  );


  $vocabs = array();
  foreach (taxonomy_get_vocabularies() as $vid => $voc) {
    $vocabs[$vid] = $voc->name;
  }

  $form['settings']['vocabs'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enabled Vocabularies'),
    '#description' => t('Check the vocabularies you would like to search for'),
    '#options' => $vocabs,
    '#default_value' => $block['vocabs'],
  );

  $form['settings']['max_items'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Items'),
    '#description' => t('What is the maximum number of results would you like returned?'),
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => TRUE,
    '#default_value' => $block['max_items'],
  );

  $form['settings']['header'] = array(
    '#type' => 'text_format',
    '#title' => t('Header Text'),
    '#description' => t('Optionally provide some text to appear above the listing.'),
    '#rows' => 3,
    '#default_value' => $block['header_text'],
    '#parents' => array('settings', 'header'),
    '#format' => $block['header_format'],
  );

  if (module_exists('token')) {
    $form['settings']['token_pattern'] = array(
      '#type' => 'fieldset',
      '#title' => t('Token Pattern'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $form['settings']['token_pattern']['token_settings'] = array(
      '#type' => 'textfield',
      '#title' => t('Token pattern'),
      '#description' => t('Optionally define a token pattern here to override the default output. Leave blank to use the default hyperlink to nodes style. Allowed HTML tags: @tags.', array('@tags' => _relevant_content_filter_xss_display_allowed_tags())),
      '#default_value' => $block['token_settings'],
      '#element_validate' => array('token_element_validate'),
      '#token_types' => array('node'),
      '#parents' => array('settings', 'token_settings'),
    );
    $form['settings']['token_pattern']['token_help'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('node'),
    );
  }

  $form['settings']['absolute_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Absolute Links'),
    '#description' => t('If enabled, the generated URL will be absolute rather than relative.<br />NOTE: This has no effect on token patterns, only the default link style.'),
    '#default_value' => $block['absolute_links'],
  );

  $form['settings']['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Status'),
    '#description' => t('If enabled, the block will appear in the lists. If disabled, the settings will be stored but the block will not appear in the block listings or anywhere on the site.'),
    '#default_value' => $block['status'],
  );

  $form['settings']['op'] = array(
    '#type' => 'value',
    '#value' => $block['id'] ? 'edit' : 'add',
  );

  $form['settings']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Settings'),
  );

  return $form;
}


/**
 * Validate function for the above form
 */
function relevant_content_admin_block_form_validate($form, &$form_state) {
  if ($form_state['values']['settings']['op'] == 'add') {
    if (preg_match('#[^a-z0-9_]#', $form_state['values']['settings']['id'])) {
      form_set_error('settings][id', t('Invalid character detected. Please only use lowercase characters, numbers and underscores.'));
      return;
    }

    if (relevant_content_get_settings($form_state['values']['settings']['id'])) {
      form_set_error('settings][id', t('This <em>Block ID</em> has already been used, please try another.'));
    }
  }

  if (!is_numeric($form_state['values']['settings']['max_items']) || $form_state['values']['settings']['max_items'] <= 0) {
    form_set_error('settings][limit', t('The limit must be a positive numeric value, eg 5.'));
  }

  $a = array_filter($form_state['values']['settings']['types']);
  if (empty($a)) {
    form_set_error('settings][types', t('You must select at least one content type to display.'));
  }

  $a = array_filter($form_state['values']['settings']['vocabs']);
  if (empty($a)) {
    form_set_error('settings][vocabs', t('You must select at least one vocabulary to limit your term searching to.'));
  }
}


/**
 *  Submit handler for the block add/edit form.
 */
function relevant_content_admin_block_form_submit($form, &$form_state) {
  // Get some sanitized settings from the form state
  $block = _relevant_content_santize_block($form_state['values']['settings']);

  // Save this block
  relevant_content_save_block($block);

  // Redirect the form back to the content overview page
  $form_state['redirect'] = RELEVANT_CONTENT_ADMIN_BASE_PATH;
}


/**
 * Operator Page Callback - this dispatches to the appropriate confirm form
 */
function relevant_content_admin_block_operator_callback($block, $op) {
  return drupal_get_form($op['callback'], $block);
}


/**
 * Form API based function for geneating the confirmation page for deleting a block from the settings.
 */
function relevant_content_admin_delete_confirm($form, &$form_state, $block) {
  $form['#rc_block'] = $block;

  return confirm_form(
    $form,
    t('Are you sure you want to delete block "%delta"?', array('%delta' => $block['id'])),
    RELEVANT_CONTENT_ADMIN_BASE_PATH,
    t('Note: This action cannt be undone')
  );
}


/**
 *  Delete confirmation form submission handler. Usets the block settings from the settings array based on the hidden delta field passed in from the previous funciton.
 */
function relevant_content_admin_delete_confirm_submit($form, &$form_state) {
  // Delete the block
  relevant_content_delete_block($form['#rc_block']['id']);

  // Set th redirect
  $form_state['redirect'] = RELEVANT_CONTENT_ADMIN_BASE_PATH;
}


/**
 * Form API based function for the confirmation page for reverting a block from the settings.
 */
function relevant_content_admin_revert_confirm($form, &$form_state, $block) {
  $form['#rc_block'] = $block;

  return confirm_form(
    $form,
    t('Are you sure you want to revert block "%block"?', array('%block' => $block['id'])),
    RELEVANT_CONTENT_ADMIN_BASE_PATH,
    t('Note: This action cannot be undone. Any changes made to this block will be lost and the block will revert to the settings provided by the its module.')
  );
}


/**
 *  Revert confirmation form submission handler.
 */
function relevant_content_admin_revert_confirm_submit($form, &$form_state) {
  // A revert is basically a delete...
  relevant_content_delete_block($form['#rc_block']['id']);

  $form_state['redirect'] = RELEVANT_CONTENT_ADMIN_BASE_PATH;
}


/**
 * Form API based function for the confirmation page for disableing a block from the settings.
 */
function relevant_content_admin_disable_confirm($form, &$form_state, $block) {
  $form['#rc_block'] = $block;

  return confirm_form(
    $form,
    t('Are you sure you want to disable block "%block"?', array('%block' => $block['id'])),
    RELEVANT_CONTENT_ADMIN_BASE_PATH,
    t('Note: Disabled blocks are not deleted, however they do not appear on the block listing page.')
  );
}


/**
 *  Revert confirmation form submission handler.
 */
function relevant_content_admin_disable_confirm_submit($form, &$form_state) {
  // A disable is basically a delete...
  relevant_content_set_block_status($form['#rc_block']['id'], 0);

  $form_state['redirect'] = RELEVANT_CONTENT_ADMIN_BASE_PATH;
}


/**
 * Form API based function for the confirmation page for disableing a block from the settings.
 */
function relevant_content_admin_enable_confirm($form, &$form_state, $block) {
  $form['#rc_block'] = $block;

  return confirm_form(
    $form,
    t('Are you sure you want to enable block "%block"?', array('%block' => $block['id'])),
    RELEVANT_CONTENT_ADMIN_BASE_PATH,
    t('Note: Once enabled, this block will appear on the block listing page. You will still need to assign it to a region.')
  );
}


/**
 *  Revert confirmation form submission handler.
 */
function relevant_content_admin_enable_confirm_submit($form, &$form_state) {
  // A disable is basically a delete...
  relevant_content_set_block_status($form['#rc_block']['id'], 1);

  $form_state['redirect'] = RELEVANT_CONTENT_ADMIN_BASE_PATH;
}

