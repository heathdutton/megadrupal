<?php
/**
 * @file
 * Administrative interface..
 */

/**
 * Basic settings form.
 */
function indexisto_search_settings_form($form, &$form_state) {
  $form = array();

  $form['indexisto_search_index_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Your search index ID'),
    '#description' => t('You can obtain your index ID at adminpanel.indexisto.com'),
    '#default_value' => variable_get('indexisto_search_index_id', ''),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#weight' => 0,
  );

  // Secret key.
  $form['indexisto_search_secret_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API secret key'),
    '#default_value' => variable_get('indexisto_search_secret_api_key', ''),
    '#weight' => 1,
    '#required' => TRUE,
  );

  $form['indexisto_search_send_data'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable indexation'),
    '#description' => t('Turn indexation on\off.'),
    '#default_value' => variable_get('indexisto_search_send_data', 0),
    '#weight' => 2,
  );

  return system_settings_form($form);
}

/**
 * Content settings for indexation.
 */
function indexisto_search_content_settings($form, &$form_state) {
  $form = array();

  // We warn user, cuz change this settings can broke indexation.
  drupal_set_message(t('These settings are for advanced users. If you do not understand what they do or how to use them, please do not change anything, since it can hurt.'), 'warning', TRUE);

  // Get all content types.
  $content_types = node_type_get_types();

  $form['indexisto_search_path_aliases'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use path aliases'),
    '#description' => t("Enable this option to use path aliases for URLs."),
    '#default_value' => variable_get('indexisto_search_path_aliases', 1),
    '#weight' => 2,
  );

  // Content types.
  $form['content_types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content types'),
    '#weight' => 3,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  foreach ($content_types as $content_type => $content_type_info) {
    // Generate main fieldset for each content type.
    $form['content_types'][$content_type] = array(
      '#type' => 'fieldset',
      '#title' => $content_type_info->name . ' (' . $content_type . ')',
      '#weight' => 0,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    // Send data of current content type or not.
    // 0 - send enabled.
    // 1 - disabled.
    $form['content_types'][$content_type]['indexisto_search_' . $content_type . '_send_data'] = array(
      '#type' => 'checkbox',
      '#title' => t('Prevent this content type from being indexed.'),
      '#description' => t('This content type will not be indexed.'),
      '#default_value' => variable_get('indexisto_search_' . $content_type . '_send_data', 0),
      '#weight' => -1,
    );

    // Title field.
    $form['content_types'][$content_type]['indexisto_search_' . $content_type . '_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Document title'),
      '#description' => t('Provide document title, typically node:title.'),
      '#default_value' => variable_get('indexisto_search_' . $content_type . '_title', '[node:title]'),
      '#weight' => 0,
      '#token_insert' => TRUE,
    );

    // Content.
    $form['content_types'][$content_type]['indexisto_search_' . $content_type . '_body'] = array(
      '#type' => 'textfield',
      '#title' => t('Main document text'),
      '#description' => t('Provide main document text, typically node:body'),
      '#default_value' => variable_get('indexisto_search_' . $content_type . '_body', '[node:body]'),
      '#weight' => 1,
      '#token_insert' => TRUE,
    );

    // Summary text.
    $form['content_types'][$content_type]['indexisto_search_' . $content_type . '_sumtext'] = array(
      '#type' => 'textfield',
      '#title' => t('Summary document text'),
      '#description' => t('Summarize all the document text to be indexed (except title). For example node:body + node:tags + node:author'),
      '#default_value' => variable_get('indexisto_search_' . $content_type . '_sumtext', '[node:body] [node:field_tags]'),
      '#weight' => 2,
      '#token_insert' => TRUE,
    );

    // Image.
    $form['content_types'][$content_type]['indexisto_search_' . $content_type . '_image'] = array(
      '#type' => 'textfield',
      '#title' => t('Image'),
      '#description' => t('Choose image to be displayed in search results'),
      '#default_value' => variable_get('indexisto_search_' . $content_type . '_image', '[node:field_image]'),
      '#weight' => 3,
      '#token_insert' => TRUE,
    );

    // Tags.
    $form['content_types'][$content_type]['indexisto_search_' . $content_type . '_tags'] = array(
      '#type' => 'textfield',
      '#title' => t('Tags'),
      '#description' => t('Tags field of current content type.'),
      '#default_value' => variable_get('indexisto_search_' . $content_type . '_tags', '[node:field_tags]'),
      '#weight' => 4,
      '#token_insert' => TRUE,
    );

    // Add token list.
    $form['content_types'][$content_type]['token_help'] = array(
      '#title' => t('Replacement patterns'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 99,
    );

    $form['content_types'][$content_type]['token_help']['help'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array(
        'node',
      ),
    );
  }

  // Comments.
  $form['comments'] = array(
    '#type' => 'fieldset',
    '#title' => t('Comments'),
    '#weight' => 4,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Block comment from indexation.
  // 0 - allowed.
  // 1 - blocked.
  $form['comments']['indexisto_search_comments_send_data'] = array(
    '#type' => 'checkbox',
    '#title' => t('Prevent comments from being indexed.'),
    '#description' => t('Comments will not be indexed.'),
    '#default_value' => variable_get('indexisto_search_comments_send_data', 0),
    '#weight' => -1,
  );

  $form['comments']['indexisto_search_comments_body'] = array(
    '#type' => 'textfield',
    '#title' => t('Comment text'),
    '#description' => t('Actual text of the comment.'),
    '#default_value' => variable_get('indexisto_search_comments_body', '[comment:body]'),
    '#weight' => 1,
    '#token_insert' => TRUE,
  );

  // Add token list.
  $form['comments']['token_help'] = array(
    '#title' => t('Replacement patterns'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 99,
  );

  $form['comments']['token_help']['help'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array(
      'comment',
    ),
  );

  return system_settings_form($form);
}

/**
 * Settings for batch operations.
 */
function indexisto_search_batch_form($form, &$form_state) {
  $form = array();

  // Operation types.
  $operation_types = array(
    0 => t('Add\update index.'),
    1 => t('Remove from index.'),
  );

  $form['batch_operation_type'] = array(
    '#type' => 'radios',
    '#title' => t('Operation type'),
    '#default_value' => 0,
    '#options' => $operation_types,
    '#description' => t('Which operation will be performed.'),
  );

  $form['info'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#title' => t('Help'),
    '#description' => t('Choose content for the operation. Comments and main content types are indexed separately.'),
  );

  // Allowed types for operations.
  $allowed_types = array();

  // Get all content types.
  $content_types = node_type_get_types();

  // Add all found content types to array;
  foreach ($content_types as $content_type) {
    $allowed_types[] = $content_type->type;
  }

  $form['batch_operation_content_types'] = array(
    '#type' => 'checkboxes',
    '#options' => drupal_map_assoc($allowed_types),
    '#title' => t('Content types'),
  );

  $form['batch_operation_comments'] = array(
    '#type' => 'checkboxes',
    '#options' => drupal_map_assoc(array('comments')),
    '#title' => t('Comments'),
  );

  if (variable_get('indexisto_search_send_data', 0) && variable_get('indexisto_search_secret_api_key', 0)) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );
  }
  else {
    form_set_error('indexisto_search_batch_form', t('In order to use this form, you must enable sending of structured data option and specify the key API.'));
  }

  return $form;
}

/**
 * Submit of our batch form.
 */
function indexisto_search_batch_form_submit($form, &$form_state) {
  // Has selected content.
  $is_content = FALSE;
  foreach ($form_state['values']['batch_operation_content_types'] as $types) {
    if ($types != '0') {
      $is_content = TRUE;
    }
  }

  $is_comment = $form_state['values']['batch_operation_comments']['comments'] ? TRUE : FALSE;

  // If selected content and comments we return error.
  if ($is_comment && $is_content) {
    drupal_set_message('Please select either the content types or comments.', 'error');
  }
  elseif (!$is_comment && !$is_content) {
    drupal_set_message('Please select either the content types or comments', 'error');
  }
  // If comments, we must to adjust the data.
  elseif ($is_comment && !$is_content) {
    $form_state['values']['batch_operation_content_types'] = array('comment' => 'comment');
    indexisto_search_batch_start($form, $form_state);
  }
  else {
    indexisto_search_batch_start($form, $form_state);
  }

}

/**
 * First time activation form.
 */
function indexisto_search_settings_first_time($form, &$form_state) {
  $form = array();

  $form['register'] = array(
    '#type' => 'fieldset',
    '#title' => t('Register'),
    '#description' => t('In case you have no account at Indexisto.com fill the form below to automatically create account and search index for the correct module operation.'),
  );

  $form['register']['register_agree'] = array(
    '#type' => 'checkbox',
    '#title' => t('I want to register an account'),
    '#description' => t('Tick ​​this box to automatically create account with all the necessary settings.'),
  );

  $form['register']['register_email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-Mail'),
    '#description' => t('Specify valid E-Mail. Later you can use it as login at Indexisto.com.'),
    '#default_value' => variable_get('site_mail', ''),
  );

  $form['register']['register_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret key'),
    '#description' => t('Specify secret API key. Follow standard password security rules, use both digits and letters.'),
    '#default_value' => indexisto_search_random_key(15),
  );

  $form['auth'] = array(
    '#type' => 'fieldset',
    '#title' => t('Authorization'),
    '#description' => t('If you already have an account and index at Indexisto.com, fill in the appropriate fields.'),
  );

  $form['auth']['auth_agree'] = array(
    '#type' => 'checkbox',
    '#title' => t('I want to use existing settings.'),
    '#description' => t('Tick ​​this box, we will use the existing settings. You will be able to correct them in the future.'),
  );

  $form['auth']['index_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Index ID'),
    '#description' => t('Specify your existing index ID.'),
    '#default_value' => variable_get('indexisto_search_index_id', ''),
    '#size' => 60,
    '#maxlength' => 128,
  );

  $form['auth']['auth_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret key'),
    '#description' => t('Specify secret key for the index.'),
    '#default_value' => '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * First time settings form submit.
 */
function indexisto_search_settings_first_time_submit($form, &$form_state) {
  // User want to register.
  $is_register = $form_state['values']['register_agree'];

  // User want to authorize.
  $is_auth = $form_state['values']['auth_agree'];

  // User must select only one of it.
  if ($is_auth && $is_register) {
    drupal_set_message(t('Please choose one of the following options: login or register.'), 'error');
  }
  // User cant select nothing.
  elseif (!$is_auth && !$is_register) {
    drupal_set_message(t('Please choose one of the following options: login or register.'), 'error');
  }

  // If user want to register.
  if ($is_register && !$is_auth) {
    $email = $form_state['values']['register_email'];
    $secret = $form_state['values']['register_secret'];

    if (empty($email) || empty($secret)) {
      drupal_set_message(t('Please fill Email and Secret fields'), 'error');
    }
    else {
      $options = array(
        'method' => 'POST',
      );

      $host = INDEXISTO_INDEX_DOMAIN_NAME;
      $email = $form_state['values']['register_email'];
      $secret = $form_state['values']['register_secret'];

      $request_url = 'http://adminpanel.indexisto.com/adminpanel/apireg.rpc?host=' . $host . '&email=' . $email . '&secret=' . $secret;
      $result = drupal_http_request($request_url, $options);

      // If alright with request we finalize the data.
      if ($result->code == 200) {
        // Enable send data and specify default data.
        if (!empty($secret) && !empty($result->data)) {
          variable_set('indexisto_search_send_data', 1);
          variable_set('indexisto_search_secret_api_key', $secret);
          variable_set('indexisto_search_index_id', $result->data);
        }

        // Remove 'first time' installation mark.
        variable_set('indexisto_search_first_time', FALSE);
        // Clear cache for new menu items.
        menu_rebuild();
        // Congratulations.
        drupal_set_message(t('You have successfully registered at Indexisto.'), 'status');
      }
      else {
        drupal_set_message(t('Something went wrong with the registration of a new account. Perhaps you already have an account. Try to use another email'), 'error');
      }
    }
  }

  // If user already have an account.
  if ($is_auth && !$is_register) {
    $index_id = $form_state['values']['index_id'];
    $secret = $form_state['values']['auth_secret'];

    // We just need one index id.
    if (empty($index_id)) {
      drupal_set_message(t('Please specify index ID.'), 'error');
    }
    else {
      variable_set('indexisto_search_index_id', $index_id);

      // If secret specify too, we enable sending data.
      if (!empty($secret)) {
        variable_set('indexisto_search_send_data', 1);
        variable_set('indexisto_search_secret_api_key', $secret);
      }

      // Remove 'first time' mark.
      variable_set('indexisto_search_first_time', FALSE);
      // Clear cache for new menu items.
      menu_rebuild();
    }
  }
}


/**
 * Style settings form.
 */
function indexisto_search_style($form, &$form_state) {
  $form = array();

  $form['indexisto_search_placeholder'] = array(
    '#type' => 'textfield',
    '#title' => t('Placeholder text'),
    '#description' => t('The text that is displayed in the search box before entering search query.'),
    '#default_value' => variable_get('indexisto_search_placeholder', t('Start typing')),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['indexisto_search_input_style'] = array(
    '#type' => 'textfield',
    '#title' => t('Inline CSS styles for search input'),
    '#description' => t('Specify your own styles, if necessary.'),
    '#default_value' => variable_get('indexisto_search_input_style', INDEXISTO_DEFAULT_STYLES),
    '#size' => 60,
    '#maxlength' => 1000,
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
