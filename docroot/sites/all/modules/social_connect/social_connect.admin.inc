<?php
/**
 * @file
 * Administration interface Social connect module.
 */

/**
 * Form callback for settings form.
 */
function social_connect_settings_form($form, &$form_state) {
  $config = _social_connect_get_config();
  $form['hyves_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Hyves connect'),
    '#default_value' => isset($config['hyves'])?'1':'0',
    '#weight' => -50,
  );

  $form['facebook_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Facebook connect'),
    '#default_value' => isset($config['facebook'])?'1':'0',
    '#weight' => -30,
  );

  // Hyves settings.
  $form['hyves_settings'] = array(
    '#type' => 'container',
    '#weight' => -40,
    '#states' => array(
      'visible' => array(
        'input[name="hyves_enable"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['hyves_settings']['hyves_settings_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Hyves settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('Your Hyves aplication settings.<BR>You need to get application consumer key and secret from <a href="http://www.hyves.nl/developer/applications/">Hyves website</a>.'),
  );

  $form['hyves_settings']['hyves_settings_fieldset']['hyves_app_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Hyves consumer key'),
    '#default_value' => isset($config['hyves']['app_key'])?$config['hyves']['app_key']:'',
    '#required' => FALSE,
    '#states' => array(
      'required' => array(
        'input[name="hyves_enable"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['hyves_settings']['hyves_settings_fieldset']['hyves_app_key_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Hyves consumer key secret'),
    '#default_value' => isset($config['hyves']['app_key_secret'])?$config['hyves']['app_key_secret']:'',
    '#states' => array(
      'required' => array(
        'input[name="hyves_enable"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['hyves_settings']['hyves_settings_fieldset']['hyves_account_actions'] = array(
    '#type' => 'radios',
    '#title' => t('What should do when user login with Hyves first time and email exists?'),
    '#default_value' => isset($config['hyves']['account_actions'])?$config['hyves']['account_actions']:'merge',
    '#options' => array(
      'merge' => t('Merge with exists account'),
      'create' => t('Create new account'),
      'ask' => t('Ask user'),
    ),
  );

  $form['hyves_settings']['hyves_settings_fieldset']['hyves_profile_picture'] = array(
    '#type' => 'checkbox',
    '#title' => t('Replace user profile picture with Hyves profile picture'),
    '#default_value' => isset($config['hyves']['profile_picture'])?$config['hyves']['profile_picture']:'0',
  );

  $form['hyves_settings']['hyves_settings_fieldset']['hyves_profile_picture_override_contanier'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        'input[name="hyves_profile_picture"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['hyves_settings']['hyves_settings_fieldset']['hyves_profile_picture_override_contanier']['hyves_profile_picture_override'] = array(
    '#type' => 'checkbox',
    '#title' => t('Override profile picture'),
    '#description' => t('If checked - user profile picture will we overriden with Hyves profile picture.'),
    '#default_value' => isset($config['hyves']['profile_picture_override'])?$config['hyves']['profile_picture_override']:'0',
  );

  $form['hyves_settings']['hyves_settings_fieldset']['hyves_button_image_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use custom login button image'),
    '#default_value' => isset($config['hyves']['button_image'])?$config['hyves']['button_image']:'0',
  );

  $form['hyves_settings']['hyves_settings_fieldset']['hyves_button_image_upload'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        'input[name="hyves_button_image_enable"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['hyves_settings']['hyves_settings_fieldset']['hyves_button_image_upload']['hyves_upload'] = array(
    '#type' => 'managed_file',
    '#title' => t('Custom login button image'),
    '#default_value' => isset($config['hyves']['button_image'])?$config['hyves']['button_image']:'',
    '#upload_validators' => array('png', 'gif', 'jpg', 'jpeg'),
    '#upload_location' => 'public://social_connect_images/',
  );

  // Facebook settings.
  $form['facebook_settings'] = array(
    '#type' => 'container',
    '#weight' => -20,
    '#states' => array(
      'visible' => array(
        'input[name="facebook_enable"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['facebook_settings']['facebook_settings_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Facebook settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('Your Facebook application settings.<BR>You need to get application ID and secret from <a href="http://www.facebook.com/developers/apps.php">Facebook website</a>.'),
  );

  $form['facebook_settings']['facebook_settings_fieldset']['facebook_app_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook application ID'),
    '#default_value' => isset($config['facebook']['app_key'])?$config['facebook']['app_key']:'',
    '#required' => FALSE,
    '#states' => array(
      'required' => array(
        'input[name="facebook_enable"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['facebook_settings']['facebook_settings_fieldset']['facebook_app_key_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook application secret'),
    '#default_value' => isset($config['facebook']['app_key_secret'])?$config['facebook']['app_key_secret']:'',
    '#required' => FALSE,
    '#states' => array(
      'required' => array(
        'input[name="facebook_enable"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['facebook_settings']['facebook_settings_fieldset']['facebook_account_actions'] = array(
    '#type' => 'radios',
    '#title' => t('What should do when user login with Facebook first time and email exists?'),
    '#default_value' => isset($config['facebook']['account_actions'])?$config['facebook']['account_actions']:'merge',
    '#options' => array(
      'merge' => t('Merge with exists account'),
      'create' => t('Create new account'),
      'ask' => t('Ask user'),
    ),
  );

  $form['facebook_settings']['facebook_settings_fieldset']['facebook_profile_picture'] = array(
    '#type' => 'checkbox',
    '#title' => t('Replace user profile picture with Facebook profile picture'),
    '#default_value' => isset($config['facebook']['profile_picture'])?$config['facebook']['profile_picture']:'0',
  );

  $form['facebook_settings']['facebook_settings_fieldset']['facebook_profile_picture_override_contanier'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        'input[name="facebook_profile_picture"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['facebook_settings']['facebook_settings_fieldset']['facebook_profile_picture_override_contanier']['facebook_profile_picture_override'] = array(
    '#type' => 'checkbox',
    '#title' => t('Override profile picture'),
    '#description' => t('If checked - user profile picture will we overriden with Facebook profile picture.'),
    '#default_value' => isset($config['facebook']['profile_picture_override'])?$config['facebook']['profile_picture_override']:'0',
  );

  $form['facebook_settings']['facebook_settings_fieldset']['facebook_button_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom login button text'),
    '#default_value' => isset($config['facebook']['button_text'])?$config['facebook']['button_text']:'',
  );

  $form['facebook_settings']['facebook_settings_fieldset']['facebook_button_size'] = array(
    '#type' => 'select',
    '#title' => t('Login button size'),
    '#default_value' => isset($config['facebook']['button_size'])?$config['facebook']['button_size']:'medium',
    '#options' => array(
        'icon' => t('Favicon Only'),
        'small' => t('Small'),
        'medium' => t('Medium'),
        'large' => t('Large'),
        'xlarge' => t('Extra Large'),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Settings form submit function.
 */
function social_connect_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $social_enable = array('hyves' => FALSE, 'facebook' => FALSE);
  if ($values['hyves_enable'] == 1) {
    $social_enable['hyves'] = TRUE;
    $hyves_config = array();
    $hyves_config['app_key'] = $values['hyves_app_key'];
    $hyves_config['app_key_secret'] = $values['hyves_app_key_secret'];
    $facebook_config['account_actions'] = $values['hyves_account_actions'];
    $hyves_config['profile_picture'] = $values['hyves_profile_picture'];
    $hyves_config['profile_picture_override'] = $values['hyves_profile_picture_override'];
    if ($values['hyves_button_image_enable'] == 1) {
      $hyves_config['button_image_enable'] = TRUE;
      if ($values['hyves_upload'] != 0) {
        $hyves_config['button_image_file'] = $values['hyves_upload'];
      }
    }
    else {
      $hyves_config['button_image_enable'] = FALSE;
    }
    variable_set('social_hyves_config', $hyves_config);
  }
  if ($values['facebook_enable'] == 1) {
    $social_enable['facebook'] = TRUE;
    $facebook_config = array();
    $facebook_config['app_key'] = $values['facebook_app_key'];
    $facebook_config['app_key_secret'] = $values['facebook_app_key_secret'];
    $facebook_config['button_text'] = trim($values['facebook_button_text']);
    $facebook_config['button_size'] = $values['facebook_button_size'];
    $facebook_config['account_actions'] = $values['facebook_account_actions'];
    $facebook_config['profile_picture'] = $values['facebook_profile_picture'];
    $facebook_config['profile_picture_override'] = $values['facebook_profile_picture_override'];
    variable_set('social_facebook_config', $facebook_config);
  }
  variable_set('social_enable', $social_enable);
  drupal_set_message(t('Social connect settings has been successfully saved.'));
}

/**
 * Menu callback: Generate a form to configure Social connect field mappings.
 */
function social_connect_settings_mapping_form($form, &$form_state) {
  $maping = _social_connect_get_field_map();
  $fields = array();
  foreach (field_info_instances('user', 'user') as $field_name => $field) {
    $fields[$field_name] = $field['label'];
  }
  $form['#tree'] = TRUE;
  $form['#header'] = array('Source fieldname', 'User profile field', 'Override profile value?', 'Weight');
  $rows = array();

  foreach ($maping as $map) {

    $form[$map->fmid]['#map'] = $map;

    $form[$map->fmid]['name'] = array(
      '#type' => 'item',
      '#title' => check_plain($map->name),
      '#description' => t('Source: @source<BR />Constant: @constant', array('@source' => $map->source, '@constant' => $map->constant)),
    );
    $form[$map->fmid]['field_name'] =array(
      '#type' => 'item',
      '#title' => check_plain($fields[$map->field_name]),
      '#description' => t('Field machinename: @fieldname', array('@fieldname' => $map->field_name)),
    );
    $form[$map->fmid]['override'] = array('#markup' => ($map->override == 1)? t('Yes'): t('No'));

    $form[$map->fmid]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for field mapping'),
      '#title_display' => 'invisible',
      '#delta' => 10,
      '#default_value' => $map->weight,
    );

    $form[$map->fmid]['edit'] = array(
      '#type' => 'link',
      '#title' => t('edit'),
      '#href' => "admin/config/people/social_connect/mapping/edit/" . $map->fmid
    );

    $form[$map->fmid]['delete'] = array(
      '#type' => 'link',
      '#title' => t('delete'),
      '#href' => "admin/config/people/social_connect/mapping/delete/" . $map->fmid
    );
  }

  // Only make this form include a submit button and weight if more than one
  // mapping exists.
  if (count($maping) > 1) {
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));
  }

  return $form;
}

/**
 * Submit handler to Social connect field mapping.
 *
 * @see social_connect_settings_mapping_form()
 */
function social_connect_settings_mapping_form_submit($form, &$form_state) {
  foreach ($form_state['values'] as $fmid => $map) {
    if (is_numeric($fmid) && $form[$fmid]['#map']->weight != $form_state['values'][$fmid]['weight']) {
      $form[$fmid]['#map']->weight = $form_state['values'][$fmid]['weight'];
      db_update('social_connect_field_mapping')
            ->fields(array('weight' => $form_state['values'][$fmid]['weight']))
            ->condition('fmid', $fmid)
            ->execute();
    }
  }
  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Menu callback; deletes a Engage field from the Drupal to Engage mapping
 * configuration.
 */
function social_connect_settings_mapping_field_delete($form, &$form_state, $fmid = NULL) {
  $field_map = _social_connect_get_field_map($fmid);
  if (!$field_map) {
    drupal_not_found();
    drupal_exit();
  }
  $form['fmid'] = array('#type' => 'value', '#value' => $fmid);
  $form['name'] = array('#type' => 'value', '#value' => t('"(@source)@source_name => @field_name"', array('@source' => $field_map[$fmid]->source, '@source_name' => $field_map[$fmid]->name, '@field_name' => $field_map[$fmid]->field_name)));
  return confirm_form($form,
    t('Are you sure you want to delete the field mapping @name?', array('@name' => $form['name']['#value'])), 'admin/config/people/social_connect/mapping',
    t('This action cannot be undone.'),
    t('Delete'), t('Cancel'));
}

/**
 * Process an Engage field delete form submission.
 */
function social_connect_settings_mapping_field_delete_submit($form, &$form_state) {
  db_delete('social_connect_field_mapping')
    ->condition('fmid', $form_state['values']['fmid'])
    ->execute();

  unset($form_state['values']['fmid']);

  cache_clear_all();

  drupal_set_message(t('The field mapping @name has been deleted.', array('@name' => $form_state['values']['name'])));
  watchdog('social_connect', 'The field mapping @name deleted.', array('@name' => $form_state['values']['name']), WATCHDOG_NOTICE, l(t('view'), 'admin/config/people/social_connect/mapping'));

  $form_state['redirect'] = 'admin/config/people/social_connect/mapping';
  return;
}


/**
 * Menu callback: Generate a form to add/edit an Engage profile field.
 *
 * @ingroup forms
 * @see rpx_profile_field_form_validate()
 * @see rpx_profile_field_form_submit()
 */
function social_connect_settings_mapping_field_form($form, &$form_state, $arg = NULL) {

  $source_fields = _social_connect_get_source_fields();
  $fields = array();
  foreach (field_info_instances('user', 'user') as $field_name => $field) {
    $fields[$field_name] = $field['label'];
  }
  $source_field_list = array();
  foreach ($source_fields as $source_field) {
    $source_field_list[$source_field->source][$source_field->fid] = $source_field->name;
  }
  if (arg(5) == 'edit') {
    if (is_numeric($arg)) {
      $fmid = $arg;

      $edit = _social_connect_get_field_map($fmid);

      if (!$map = $edit[$fmid]) {
        drupal_not_found();
        return;
      }
      drupal_set_title(t('Edit "(@source)@name => @field_name" field map.', array('@source' => $map->source, '@name' => $map->name, '@field_name' => $map->field_name)), PASS_THROUGH);
      $form['fmid'] = array('#type' => 'value',
          '#value' => $fmid,
      );
    }
    else {
      drupal_not_found();
      return;
    }
  }
  else {
    drupal_set_title(t('Add new Social connect field map'), PASS_THROUGH);
    $map = new stdClass();
    $map->fid = '';
    $map->field_name = '';
    $map->override = 0;
    $map->weight = 0;
  }

  $form['fid'] = array(
    '#type' => 'select',
    '#title' => t('Source field'),
    '#default_value' => $map->fid,
    '#options' => $source_field_list,
  );
  $form['field_name'] = array(
    '#type' => 'select',
    '#title' => t('Profile field'),
    '#default_value' => $map->field_name,
    '#options' => $fields,
  );

  $form['override'] = array(
    '#type' => 'checkbox',
    '#title' => t('Override profile value?'),
    '#default_value' => $map->override,
  );

  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight for field mapping'),
    '#title_display' => 'invisible',
    '#delta' => 10,
    '#default_value' => $map->weight,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save field map'),
  );

  return $form;
}

/**
 * Process social_connect_settings_mapping_field_form submissions.
 */
function social_connect_settings_mapping_field_form_submit($form, &$form_state) {
  $values = array_intersect_key($form_state['values'], array_flip(array('fid', 'field_name', 'override', 'weight')));
  if (!isset($form_state['values']['fmid'])) {
    $fmid = db_insert('social_connect_field_mapping')
      ->fields($values)
      ->execute();
    drupal_set_message(t('The field map has been created.'));
    watchdog('social_connect', 'The field mapping @fmid created.', array('@fmid' => $fmid), WATCHDOG_NOTICE, l(t('view'), 'admin/config/people/social_connect/mapping'));
  }
  else {
    db_update('social_connect_field_mapping')
      ->fields($values)
      ->condition('fmid', $form_state['values']['fmid'])
      ->execute();
    drupal_set_message(t('The field has been updated.'));
  }
  cache_clear_all();

  $form_state['redirect'] = 'admin/config/people/social_connect/mapping';
  return;
}

function theme_social_connect_settings_mapping_form($variables) {
  $form = $variables['form'];

  $rows = array();

  foreach (element_children($form) as $key) {
    if (isset($form[$key]['name'])) {
      $map = &$form[$key];

      $row = array();
      $row[] = drupal_render($map['name']);
      $row[] = drupal_render($map['field_name']);
      $row[] = drupal_render($map['override']);
      if (isset($map['weight'])) {
        $map['weight']['#attributes']['class'] = array('mapping-weight');
        $row[] = drupal_render($map['weight']);
      }
      $row[] = drupal_render($map['edit']);
      $row[] = drupal_render($map['delete']);
      $rows[] = array('data' => $row, 'class' => array('draggable'));
    }
  }
  if (isset($form['actions'])) {
    drupal_add_tabledrag('mapping', 'order', 'sibling', 'mapping-weight');
  }
  $header = $form['#header'];
  $header[] = array('data' => t('Operations'), 'colspan' => '2');
  return theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No mapping available. <a href="@link">Add field map</a>.', array('@link' => url('admin/config/people/social_connect/mapping/add'))), 'attributes' => array('id' => 'mapping'))) . drupal_render_children($form);
}