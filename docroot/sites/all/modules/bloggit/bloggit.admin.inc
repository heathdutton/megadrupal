<?php

/**
 * @file
 * Administration settings for Bloggit module.
 */

/**
 * Admin settings form.
 */
function bloggit_admin_settings_form($form_state) {
  $form = array();

  $form['bloggit_blog'] = array(
    '#tree' => TRUE,
  );

  $defaults = variable_get('bloggit_blog', array());

  $defaults += array(
    'path' => '',
    'categories_field' => 'field_categories',
    'tags_field' => 'field_tags',
    'author_name' => $GLOBALS['user']->name,
  );

  $form['bloggit_blog']['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Blog folder path'),
    '#description' => t('Path to the blog files. This should be inside the Drupal site root, if you want to have static files served from the uploaded blog entries.'),
    '#default_value' => $defaults['path'],
  );

  $taxonomy_fields = bloggit_admin_get_taxonomy_fields();

  $form['bloggit_blog']['categories_field'] = array(
    '#type' => 'select',
    '#title' => t('Field for storing categories'),
    '#description' => t('This field’s associated taxonomy vocabulary will be used for storing Bloggit entry categories. You will need to create a vocabulary, and a field instance for it on the Bloggit node type for it to appear here.'),
    '#options' => $taxonomy_fields['options'],
    '#default_value' => $defaults['categories_field'],
  );

  $form['bloggit_blog']['tags_field'] = array(
    '#type' => 'select',
    '#title' => t('Field for storing tags'),
    '#description' => t('Works just like the field for storing categories.'),
    '#options' => $taxonomy_fields['options'],
    '#default_value' => $defaults['tags_field'],
  );

  $form['bloggit_blog']['author_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Default author'),
    '#description' => t('The user that will be assigned as author on newly created bloggit entries.'),
    '#default_value' => $defaults['author_name'],
    '#autocomplete_path' => 'user/autocomplete',
  );

  $form['actions']['process_files'] = array(
    '#type' => 'submit',
    '#description' => t('Scan for blog file, and process them into nodes.'),
    '#submit' => array('bloggit_admin_process'),
    '#value' => t('Process blog files'),
    '#weight' => 80,
  );

  return system_settings_form($form);
}

/**
 * Validation for admin settings form.
 */
function bloggit_admin_settings_form_validate($form, &$form_state) {
  $taxonomy_fields = bloggit_admin_get_taxonomy_fields();

  // If a field was selected, store its vid in the configuration data.
  if ($form_state['values']['bloggit_blog']['categories_field']) {
    $form_state['values']['bloggit_blog']['categories_vid'] = $taxonomy_fields['vids'][$form_state['values']['bloggit_blog']['categories_field']];
  }

  if ($form_state['values']['bloggit_blog']['tags_field']) {
    $form_state['values']['bloggit_blog']['tags_vid'] = $taxonomy_fields['vids'][$form_state['values']['bloggit_blog']['tags_field']];
  }

  // Validate the entered user name and store the associated user id.
  $author = user_load_by_name($form_state['values']['bloggit_blog']['author_name']);
  if ($author) {
    $form_state['values']['bloggit_blog']['author_uid'] = $author->uid;
  }
  else {
    form_set_error('bloggit_blog][author_name', t('User %name could not be found.', array(
      '%name' => $form_state['values']['bloggit_blog']['author_name']
    )));
  }
}

/**
 * Admin action to process bloggit files.
 */
function bloggit_admin_process($form, &$form_state) {
  drupal_set_message(t('Started scanning for blog entries'));
  bloggit_process_files();
}

/**
 * Helper function to get taxonomy fields for our blogging type.
 */
function bloggit_admin_get_taxonomy_fields() {
  $fields = &drupal_static(__FUNCTION__);

  if (!isset($fields)) {
    // There may be a pretty API method to do this, but I haven’t found it 
    // yet. If you find it, please open a bug :)
    $queryset = db_query("
      SELECT field_name, fci.data AS instance_data, fc.data AS field_data
      FROM field_config_instance AS fci
      INNER JOIN field_config AS fc USING (field_name)
      WHERE fci.entity_type = 'node' AND fci.bundle = 'bloggit' AND fc.type = 'taxonomy_term_reference';
    ");

    $fields = array(
      'options' => array('' => t('None')),
      'vids' => array(),
    );

    foreach ($queryset as $row) {
      $instance_data = unserialize($row->instance_data);
      $field_data = unserialize($row->field_data);

      $fields['options'][$row->field_name] = $instance_data['label'];
      $fields['vids'][$row->field_name] = $field_data['settings']['allowed_values'][0]['vid'];
    }
  }

  return $fields;
}

