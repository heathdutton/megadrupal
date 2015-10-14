<?php

/**
 * @file
 * Configuration pages for CRM Core Profile.
 */

define('DEFAULT_WEIGHT_DELTA', 50);

/**
 * The main screen for CRM Core Profile.
 *
 * This function provides the main screen for managing, editing, cloning, etc.
 */
function crm_core_profile_main() {

  drupal_set_title(t('CRM Core Profile'));
  $output = '';
  $output .= '<h1>' . t('Available profiles') . '</h1>';

  $rows = array();
  foreach (crm_core_profile_load_all() as $profile) {

    // @todo How can we define that there is embedded usage?
    $displays = array();
    if ($profile->page) {
      $displays[] = t('Page');
    }
    if ($profile->block) {
      $displays[] = t('Block');
    }

    $path = '---';
    if ($profile->page) {
      $prepopulate = isset($profile->entities['crm_core_contact']['prepopulate']) ? $profile->entities['crm_core_contact']['prepopulate'] : FALSE;
      if ($prepopulate || $profile->page) {
        $path = l(check_plain($profile->page_path), $profile->page_path);
      }
      else {
        $path = l(check_plain('crm-core/profile/' . $profile->name), 'crm-core/profile/' . $profile->name);
      }
    }

    $status_toggle_path = 'admin/structure/crm-core/profile/manage/' . $profile->name . '/status';
    $status = l($profile->status ? t('Enabled') : t('Disabled'), $status_toggle_path);

    $operations = theme('links', array(
        'links' => array(
          'edit' => array(
            'title' => t('Edit'),
            'href' => 'admin/structure/crm-core/profile/manage/' . $profile->name . '/edit',
          ),
          'settings' => array(
            'title' => t('Settings'),
            'href' => 'admin/structure/crm-core/profile/manage/' . $profile->name . '/settings',
          ),
          'clone' => array(
            'title' => t('Clone'),
            'href' => 'admin/structure/crm-core/profile/manage/' . $profile->name . '/clone',
          ),
          'export' => array(
            'title' => t('Export'),
            'href' => 'admin/structure/crm-core/profile/manage/' . $profile->name . '/export',
          ),
        ),
        'attributes' => array('class' => array('links', 'inline')),
      )
    );

    $rows[] = array(
      'data' => array(
        l(check_plain($profile->label), 'admin/structure/crm-core/profile/manage/' . $profile->name . '/settings'),
        $profile->name,
        implode(', ', $displays),
        $path,
        $status,
        $operations,
      ),
    );
  }

  $header = array(
    t('Name'),
    t('Machine Name'),
    t('Display'),
    t('Path'),
    t('Status'),
    t('Operations'),
  );
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'crm-core-profiles-list'),
    'empty' => t('There are no profiles currently configured.'),
  ));

  return $output;
}

/**
 * Generates a new profile form or edit an existing form.
 */
function crm_core_profile_new_form($form, &$form_state, $profile, $clone = FALSE) {

  $form['#attached']['css'][] = drupal_get_path('module', 'crm_core_profile') . '/css/profile_edit.css';

  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Structure'), 'admin/structure'),
    l(t('CRM Core'), 'admin/structure/crm-core'),
    l(t('CRM Core Profile'), 'admin/structure/crm-core/profile'),
  );
  drupal_set_breadcrumb($breadcrumb);

  global $user;

  if (empty($profile->name)) {
    $form_state['is_new'] = TRUE;
  }

  // Updating profile fields from $form_state
  if (isset($form_state['values'])) {
    _crm_core_profile_update_profile($profile, $form_state['values']);
  }

  if (empty($form_state['is_new'])) {
    $form['#profile'] = $profile;
  }
  $weight = -DEFAULT_WEIGHT_DELTA;
  $form['#weight'] =& $weight;

  $form['is_new'] = array(
    '#type' => 'value',
    '#value' => 0,
  );

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $user->uid,
  );

  if ($clone) {
    // Do this if we are cloning a form.
    drupal_set_title(t('Save copy of @profile', array('@profile' => $profile->label)));
    $profile->clone = $clone;
    $profile->label = 'Copy of ' . $profile->label;
    $profile->name = 'copy_of_' . $profile->name;
    $form['is_new']['#value'] = 1;
    $form['created'] = array(
      '#type' => 'value',
      '#value' => time(),
    );
  }
  else {
    if (!empty($form_state['is_new'])) {
      // Do this if we are creating a new profile.
      drupal_set_title(t('New profile settings'));
      $form['is_new']['#value'] = 1;
      $form['created'] = array(
        '#type' => 'value',
        '#value' => time(),
      );
    }
    else {
      drupal_set_title(t('Profile settings for @profile', array('@profile' => $profile->label)));
    }
  }

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => 'Name',
    '#description' => 'Please enter the profile form name',
    '#maxlength' => 28,
    '#required' => TRUE,
    '#default_value' => empty($profile->label) ? '' : $profile->label,
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#maxlength' => 28,
    '#machine_name' => array(
      'exists' => 'crm_core_profile_new_form_machine_name_check',
      'source' => array('label'),
    ),
    '#default_value' => empty($profile->name) ? '' : $profile->name,
    '#disabled' => empty($profile->name) || $clone ? FALSE : TRUE,
  );

  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#description' => t('Check this box to enable the profile. Disabling a profile makes it unavailable for use.'),
    '#default_value' => empty($profile->status) ? FALSE : $profile->status,
  );

  $form['entities'] = array(
    '#type' => 'vertical_tabs',
    '#tree' => TRUE,
  );

  // Get a list of all entity handlers supported by CRM Core Profile.
  if (!isset($form_state['entity_handlers'])) {
    $form_state['entity_handlers'] = module_invoke_all('crm_core_profile_register_entity');
  }
  $entity_handlers = $form_state['entity_handlers'];

  // Display the entity handlers on the form.
  foreach ($entity_handlers as $entity_handler_name) {

    // Ensure that class really exists.
    if (class_exists($entity_handler_name)) {
      $entity_handler = new $entity_handler_name();
      $entity_info = $entity_handler->entityInfo();
      $entity_type = $entity_info['entity'];
      $entity_label = $entity_info['entity_label'];
      $entity_config = isset($profile->entities[$entity_type]) ? $profile->entities[$entity_type] : array();

      // Create the container for each entity type
      // supported by CRM Core Profile.
      $form['entities'][$entity_type] = array(
        '#type' => 'fieldset',
        '#title' => $entity_label,
      );
      $form['entities'][$entity_type]['label'] = array(
        '#type' => 'value',
        '#value' => $entity_label,
      );
      $form['entities'][$entity_type]['status'] = array(
        '#type' => 'checkbox',
        '#title' => t('Add a @entity', array('@entity' => $entity_label)),
        '#description' => t('Check this box to create an entity of this type when the profile form is submitted.'),
        '#default_value' => empty($entity_config['status']) ? FALSE : $entity_config['status'],
      );

      // Check to see if the entity actually uses bundles. this is set within
      // entityInfo using the add_bundles property. By default, we assume that
      // bundles are being added to each entity type. There are some entities
      // that will not need to allow users to select fields based on the bundle.
      if (!isset($entity_info['add_bundles']) || $entity_info['add_bundles'] == TRUE) {

        // This allows users to select a bundle and fields from the bundle.
        _crm_core_profile_bundle_selector($form, $entity_type, $entity_config, $entity_handler, $entity_label);
        _crm_core_profile_fields_selector($form, $entity_type, $entity_config, $entity_handler, $entity_label);
      }
      else {

        // If we are not selecting a bundle for this entity type, we don't need
        // a selector. The form will need to come from somewhere else and
        // implement it's own tools for adding fields to the profile.
        if (isset($entity_info['edit_callback'])) {
          // If there is an edit callback for this form item, use that and add
          // stuff to the form.
          $entity_handler->$entity_info['edit_callback']($form, $profile, $entity_handler, $entity_info, $entity_label);
        }
      }

      // Add tokens to the profile form.
      _crm_core_profile_add_tokens($form, $entity_type);
    }
  }

  // This is where we display the fields that have been added to a profile.
  $form['fields'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
    '#prefix' => '<div id="fields-container">',
    '#suffix' => '</div>',
  );

  // This is where we add fields to the profile if we are editing a profile,
  // there should already be entities with configuration information.
  if (!empty($profile->entities)) {

    // Loop through each entity.
    foreach ($profile->entities as $entity_type => $entity_config) {

      // Check to see if there is a handler for a given entity type.
      if (is_array($entity_config) && isset($entity_handlers[$entity_type])) {
        $entity_handler_name = $entity_handlers[$entity_type];

        // Make sure there is a class for the handler.
        if (class_exists($entity_handler_name)) {
          // Call the class and get the properties.
          $entity_handler = new $entity_handler_name();
          // Entity_properties is a list of meta-fields that cannot be retrieved
          // by simply getting fields for the entity.
          $entity_properties = $entity_handler->entityProperties();
          // Add fields to the fields container.
          if (isset($entity_config['fields_container'])) {
            // Loop through the fields that have already been added to the
            // profile.
            foreach (array_filter($entity_config['fields_container']['fields']) as $field_name) {
              // If we are dealing with a meta-field, it can be rendered
              // differently than a simple field. Entity handlers are
              // responsible for rendering them on their own.
              if (array_key_exists($field_name, $entity_properties)) {
                $entity_handler->renderPropertyConfig($form, $entity_type, $entity_config['label'], $entity_config['bundle'], $field_name);
              }
              else {
                // We are dealing with a standard field, use the default field
                // rendering items.
                $field_info = field_info_field($field_name);
                $field_handler = $field_info['type'] . 'ProfileFieldHandler';
                if (class_exists($field_handler)) {
                  $field_handler_obj = new $field_handler();
                  $field_handler_obj->renderFieldConfig($form, $entity_type, $entity_config['label'], $entity_config['bundle'], $field_name);
                }
                else {
                  $field_handler_obj = new DefaultProfileFieldHandler();
                  $field_handler_obj->renderFieldConfig($form, $entity_type, $entity_config['label'], $entity_config['bundle'], $field_name);
                }
              }
            }
          }
        }
      }
    }
  }

  $form['#attached']['js'] = array(
    drupal_get_path('module', 'crm_core_profile') . '/js/profile_edit.js',
  );

  $is_new = $form['is_new']['#value'];
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => $is_new ? t('Create') : t('Save'),
  );
  $form['actions']['configure'] = array(
    '#type' => 'submit',
    '#value' => $is_new ? t('Create and configure') : t('Save and configure'),
    '#weight' => 11,
    '#submit' => array('crm_core_profile_new_form_configure'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#weight' => 13,
    '#submit' => array('crm_core_profile_new_form_cancel'),
  );
  if (!$is_new) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 16,
      '#submit' => array('crm_core_profile_delete_submit'),
    );
  }

  return $form;
}

/**
 * Adds tokens to a form. This is called for every entity type.
 *
 * @todo: see if it makes sense to replace some of this with a token browser.
 *
 * @param array $form
 * @param string $entity_type : the name of the entity
 */
function _crm_core_profile_add_tokens(&$form, $entity_type) {

  // Display a list of tokens that can be used in the profile.
  if (module_exists('token')) {
    // We must load token values here to show them on the options form.
    drupal_add_js(drupal_get_path('module', 'token') . '/token.js');
    drupal_add_css(drupal_get_path('module', 'token') . '/token.css');
    drupal_add_library('token', 'treeTable');
  }

  $form['entities'][$entity_type]['token'] = array(
    '#type' => 'fieldset',
    '#title' => t('Token replacement'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="entities[' . $entity_type . '][status]"]' => array('checked' => TRUE),
      ),
    ),
  );

  if (module_exists('token')) {
    $form['entities'][$entity_type]['token']['tokens'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array(token_get_entity_mapping('entity', $entity_type)),
      '#dialog' => FALSE,
      // @todo: Maybe enable token browser to not have this load so long.
    );
  }
}

/**
 * Adds a fields container for any entity.
 *
 * The fields container is simply used to house fields
 * that can be added to a profile once a bundle is selected. This will be added
 * to entity handlers that do not support bundles as well.
 *
 * @param array $form
 *   Structured form array.
 * @param string $entity_type
 *   The entity type
 * @param bool $always_visible
 *   Tells CRM Core Profile whether or not to display fields by default.
 */
function _crm_core_profile_add_field_container(&$form, $entity_type, $always_visible = FALSE) {

  // Display a list of field associated with the selected bundle.
  $form['entities'][$entity_type]['fields_container'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="' . 'crm-core-profile-entity-' . strtr($entity_type, '_', '-') . '-fields-container' . '">',
    '#suffix' => '</div>',
  );

  if (!$always_visible) {
    $form['entities'][$entity_type]['fields_container']['#states'] = array(
      'visible' => array(
        ':input[name="entities[' . $entity_type . '][status]"]' => array('checked' => TRUE),
      ),
    );
  }
}

/**
 * Returns a set of fields for selecting bundles within a CRM Core Profile.
 *
 * This is the standard way of allowing users to choose an entity for a bundle.
 * It will create a form element that allows users to select a bundle, then
 * display the fields associated with it. Not every entity will need
 * bundles, especially ones that operate primarily using meta-fields.
 *
 * Developers can override this behavior using the handler->entityInfo() method,
 * by specifying several keys to the array. For example:
 *
 * - add_bundles    => FALSE : removes support for bundles
 * - edit_callback  => {some_callback} : specifies an alternate callback for
 * the edit screen.
 *
 * @param array $form
 *   Structured form array.
 * @param string $entity_type
 *   The entity type.
 * @param array $entity_config
 *   Information about the configuration of the entity.
 * @param array $entity_handler
 *   The name of the handler class used to process the entity.
 * @param array $entity_label
 *   The label used for the entity.
 */
function _crm_core_profile_bundle_selector(&$form, $entity_type, $entity_config, $entity_handler, $entity_label) {

  // Get a list of bundles that can be displayed.
  $entity_info = entity_get_info($entity_type);
  $bundle_options = array();
  foreach ($entity_info['bundles'] as $bundle => $bundle_info) {
    $bundle_options[$bundle] = $bundle_info['label'];
  }

  // Display a list of bundles that can be included in the profile.
  $form['entities'][$entity_type]['bundle'] = array(
    '#type' => 'select',
    '#title' => t('@entity Type', array('@entity' => $entity_label)),
    '#options' => $bundle_options,
    '#empty_option' => t('--Select--'),
    '#empty_value' => '',
    '#description' => t('Select type of the entity to create.'),
    '#states' => array(
      'visible' => array(
        ':input[name="entities[' . $entity_type . '][status]"]' => array('checked' => TRUE),
      ),
    ),
    '#ajax' => array(
      'callback' => '_crm_core_profile_entity_fields',
      'wrapper' => 'crm-core-profile-entity-' . strtr($entity_type, '_', '-') . '-fields-container',
    ),
    '#default_value' => empty($entity_config['bundle']) ? '' : $entity_config['bundle'],
  );

  // Add a field container for the selected entity.
  _crm_core_profile_add_field_container($form, $entity_type);
}

/**
 * Presents a list of fields for the given entity type to be added to a profile.
 *
 * By default, this function returns a list of form fields that can be added to
 * a profile once a bundle is selected. For entities that do not use bundles,
 * add a key to the $entity_type array to tell CRM Core Profile not to wait for
 * a bundle to be selected. For example:
 *
 * $entity_config['always_display'] = TRUE;
 *
 * @param array $form
 *   Structured form array.
 * @param string $entity_type
 *   The entity type.
 * @param array $entity_config
 *   Information about the configuration of the entity.
 * @param array $entity_handler
 *   The name of the handler class used to process the entity.
 * @param array $entity_label
 *   The label used for the entity.
 */
function _crm_core_profile_fields_selector(&$form, $entity_type, $entity_config, $entity_handler, $entity_label) {

  if (isset($entity_config['bundle']) || !empty($entity_config['always_display'])) {

    $entity_fields = $entity_handler->entityProperties();

    if (isset($entity_config['bundle'])) {

      foreach (field_info_instances($entity_type, $entity_config['bundle']) as $field) {
        // Allow entity handlers to override labels.
        if (isset($entity_fields[$field['field_name']])) {
          if (empty($entity_fields[$field['field_name']])) {
            // Remove properties with empty label.
            unset($entity_fields[$field['field_name']]);
          }
          continue;
        }
        $entity_fields[$field['field_name']] = check_plain($field['label']);
      }
    }

    unset($field);

    $form['entities'][$entity_type]['fields_container']['fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('@entity Fields', array('@entity' => $entity_label)),
      '#options' => $entity_fields,
      '#description' => t('Select fields of the entity to display.'),
      '#states' => array(
        'invisible' => array(
          ':input[name="entities[' . $entity_type . '][bundle]"]' => array('value' => ''),
        ),
      ),
      '#ajax' => array(
        'callback' => '_crm_core_profile_fields',
        'wrapper' => 'fields-container',
      ),
      '#default_value' => empty($entity_config['fields_container']['fields']) ? array() : $entity_config['fields_container']['fields'],
      '#attributes' => array('class' => array('crm-core-profile-entities-fields-container-fields')),
    );
  }
}

/**
 * New form helper.
 */
function _crm_core_profile_new_form_fields(&$form, $entity_label, $field_config, $sub_field = FALSE) {

  $field_separator = _crm_core_profile_field_separator($field_config);

  $form['fields'][$field_separator] = array();
  $row =& $form['fields'][$field_separator];

  $weight = isset($field_config['weight']) ? _crm_core_profile_weight_fix($field_config['weight']) : _crm_core_profile_default_weight();

  $row['#weight'] = $weight;

  if ($sub_field) {
    $row['#attributes']['class'] = array('sub-field');
  }

  $row['field_name'] = array(
    '#type' => 'value',
    '#value' => $field_config['field_name'],
  );
  $row['field_item'] = array(
    '#type' => 'value',
    '#value' => $field_config['field_item'],
  );
  $row['handler'] = array(
    '#type' => 'value',
    '#value' => $field_config['handler'],
  );
  $row['entity_type'] = array(
    '#type' => 'value',
    '#value' => $field_config['entity_type'],
  );
  $row['bundle'] = array(
    '#type' => 'value',
    '#value' => $field_config['bundle'],
  );

  $row['status'] = array(
    '#type' => 'checkbox',
    '#default_value' => isset($field_config['status']) ? $field_config['status'] : FALSE,
  );

  $row['title_wrapper'] = array(
    '#type' => 'container',
  );
  $row['title_wrapper']['title'] = array(
    '#markup' => t('@field_label (%field_entity)', array(
        '@field_label' => $field_config['field_label'],
        '%field_entity' => $entity_label,
      )
    ),
  );

  $row['hidden'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hidden'),
    '#title_display' => 'after',
    '#default_value' => isset($field_config['hidden']) ? $field_config['hidden'] : FALSE,
  );

  $field = field_info_field($field_config['field_name']);
  switch ($field['type']) {
    case 'list_integer':
    case 'list_float':
    case 'list_text':

      if (isset($field['settings']['allowed_values'])) {
        $options = $field['settings']['allowed_values'];
        array_unshift($options, t('--Please Select--'));
      }
      else {
        $options = array('' => t('Nothing to choose'));
      }

      $row['default'] = array(
        '#type' => 'select',
        '#title' => t('Default Value'),
        '#default_value' => isset($field_config['default']) ? $field_config['default'] : '',
        '#options' => $options,
      );
      break;

    default:
      $row['default'] = array(
        '#type' => 'textfield',
        '#title' => t('Default Value'),
        '#default_value' => isset($field_config['default']) ? $field_config['default'] : '',
        '#size' => 60,
        '#maxlength' => 200,
      );
  }

  $row['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight for @title', array('@title' => $field_config['field_label'])),
    '#title_display' => 'invisible',
    '#default_value' => $weight,
    '#delta' => DEFAULT_WEIGHT_DELTA,
  );
}

/**
 * Helper to fix display weight.
 */
function _crm_core_profile_weight_fix($weight) {
  if ($weight == 0) {
    // Table row positioned incorrectly if "#weight" is 0.
    $weight = 0.001;
  }

  return $weight;
}

/**
 * Ajax callback.
 */
function _crm_core_profile_entity_fields($form, $form_state) {
  $entity_type = $form_state['triggering_element']['#array_parents'][1];

  return $form['entities'][$entity_type]['fields_container'];
}

/**
 * Ajax callback.
 */
function _crm_core_profile_fields($form, $form_state) {
  return '<div id="fields-container">' . _theme_crm_core_profile_new_form_helper($form) . '</div>';
}

/**
 * Profile name validation callback.
 *
 * @param string $value
 *   Profile machine name.
 *
 * @return bool
 *   FALSE if unique.
 */
function crm_core_profile_new_form_machine_name_check($value) {

  $result = db_select('crm_core_profile')
    ->fields('crm_core_profile', array('name'))
    ->condition('name', $value)
    ->execute()
    ->fetchField();

  return $result;
}

/**
 * Validation callback.
 */
function crm_core_profile_new_form_validate($form, &$form_state) {
  // There is no need to validate form each time we adding the field.
  // Validating only on save events.
  if ($form_state['triggering_element']['#type'] == 'submit'
    && $form_state['triggering_element']['#value'] != t('Delete')
    && $form_state['triggering_element']['#value'] != t('Cancel')
  ) {
    $profile_config = $form_state['values'];
    $entity_handlers = $form_state['entity_handlers'];
    unset($profile_config['entities']['entities__active_tab']);
    foreach ($profile_config['entities'] as $entity_type => $entity_config) {
      if ($entity_config['status'] && empty($entity_config['bundle'])) {
        form_error($form['entities'][$entity_type]['bundle'], t('You must select type of %entity to create.',
          array(
            '%entity' => $entity_config['label'],
          )
        ));
      }
      // Allow entity handlers to validate this form.
      if ($entity_config['status'] && method_exists($entity_handlers[$entity_type], 'editFormValidate')) {
        $handler = new $entity_handlers[$entity_type]();
        $handler->editFormValidate($form, $form_state, $profile_config);
      }
    }
  }
}

/**
 * Submission callback.
 */
function crm_core_profile_new_form_submit($form, &$form_state) {

  $is_new = isset($form_state['is_new']);
  $profile = isset($form['#profile']) ? $form['#profile'] : new stdClass();
  $settings = $form_state['values'];

  if (empty($settings['fields'])) {
    $settings['fields'] = array();
  }

  _crm_core_profile_update_profile($profile, $settings);

  crm_core_profile_save($profile);

  if ($is_new) {
    $form_state['redirect'] = 'admin/structure/crm-core/profile/manage/' . $settings['name'] . '/edit';
  }
  drupal_set_message('Your profile has been successfully saved.');
}

/**
 * Submit handler for "Save and configure" button.
 */
function crm_core_profile_new_form_configure($form, &$form_state) {
  crm_core_profile_new_form_submit($form, $form_state);
  $settings = $form_state['values'];
  $form_state['redirect'] = 'admin/structure/crm-core/profile/manage/' . $settings['name'] . '/settings';
}

/**
 * Submit handler for "Cancel" button.
 */
function crm_core_profile_new_form_cancel() {
  $form_state['redirect'] = 'admin/structure/crm-core/profile';
}

/**
 * New profile theme implementation.
 */
function theme_crm_core_profile_new_form($variables) {
  $form = $variables['form'];

  if (isset($form['fields'])) {
    drupal_add_tabledrag('fields-table', 'order', 'sibling', 'field-weight');
    $form['fields']['content']['#markup'] = _theme_crm_core_profile_new_form_helper($form);
  }

  return drupal_render_children($form);
}

/**
 * Theme helper.
 */
function _theme_crm_core_profile_new_form_helper(&$form) {

  $rows = array();

  foreach (element_children($form['fields'], TRUE) as $id) {
    $field =& $form['fields'][$id];
    $field['weight']['#attributes']['class'][] = 'field-weight';
    $indentation_size = 0;
    if (isset($field['#attributes']['class'])) {
      $indentation_size = in_array('sub-field', $field['#attributes']['class']) ? 1 : 0;
    }

    $row = array(
      'data' => array(),
      'class' => array('draggable'),
    );

    $row['data'][] = theme('indentation', array('size' => $indentation_size)) . drupal_render($field['status']);
    $row['data'][] = drupal_render($field['title_wrapper']);
    if (isset($field['description_wrapper'])) {
      $row['data'][] = drupal_render($field['weight']);
      $row['data'][] = array(
        'data' => drupal_render($field['description_wrapper']),
        'colspan' => '2',
      );
    }
    else {
      $row['data'][] = drupal_render($field['weight']);
      $row['data'][] = drupal_render($field['hidden']);
      $row['data'][] = drupal_render($field['default']);
    }

    $rows[] = $row;
  }

  return theme('table', array(
    'rows' => $rows,
    'attributes' => array('id' => 'fields-table'),
  ));
}

/**
 * Presents the form for profile deletion.
 */
function crm_core_profile_delete_form($form, &$form_state, $crm_core_profile) {
  $form['name'] = array(
    '#type' => 'value',
    '#value' => $crm_core_profile->name,
  );

  $question = t('Are you sure you want to delete the profile @profile', array('@profile' => $crm_core_profile->name));
  $path = 'admin/structure/crm-core/profile';

  return confirm_form($form, $question, $path);
}

/**
 * Submit handler.
 */
function crm_core_profile_delete_form_submit($form, &$form_state) {
  $machine_name = $form_state['values']['name'];
  crm_core_profile_delete($machine_name);

  $form_state['redirect'] = 'admin/structure/crm-core/profile';
  drupal_set_message(t('The profile form has been successfully deleted'));
  menu_rebuild();
}

/**
 * Profile settings form.
 */
function crm_core_profile_settings_form($form, &$form_state, $profile) {

  drupal_set_title(t('Profile settings for @profile', array('@profile' => $profile->label)));
  $form_state['profile'] = $profile;

  $form['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display Settings'),
    '#collapsible' => TRUE,
  );

  $form['display']['page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create a page'),
    '#default_value' => $profile->page,
  );

  $form['display']['page_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Page Title'),
    '#default_value' => $profile->page_title,
    '#description' => 'Please provide a title for the page.',
    '#states' => array(
      'invisible' => array(
        ':input[name="page"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['display']['page_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Page Path'),
    '#default_value' => $profile->page_path,
    '#description' => 'Please provide a path to the page. Paths can include %user and %crm_core_contact as tokens. If you include a token, you must also specify prepopulation settings below.',
    '#states' => array(
      'invisible' => array(
        ':input[name="page"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['display']['block'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create a block'),
    '#default_value' => $profile->block,
  );

  $form['display']['block_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Block Title'),
    '#default_value' => $profile->block_title,
    '#states' => array(
      'invisible' => array(
        ':input[name="block"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['display']['page_redirect'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect Path'),
    '#default_value' => $profile->page_redirect,
    '#description' => 'Please provide a valid path where users will be taken after filling out the profile. Leave this field blank to return to the same page.',
  );

  $form['display']['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message to users'),
    '#cols' => 60,
    '#rows' => 3,
    '#default_value' => $profile->message,
    '#description' => t('Please enter a message to display to users each time this form is successfully submitted. Maximum length is 512 characters.'),
  );

  $form['roles_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Permissions'),
    '#collapsible' => TRUE,
  );

  $form['roles_wrapper']['description_wrapper'] = array(
    '#type' => 'container',
  );
  $form['roles_wrapper']['description_wrapper']['description'] = array(
    '#markup' => t('Please select the user roles that can access this profile. Any user without one of these roles will not be able to access the profile.'),
  );

  // Selects roles associated with the profile.
  $form['roles_wrapper']['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('User role access'),
    '#options' => drupal_map_assoc(user_roles()),
    '#default_value' => isset($profile->roles) ? drupal_map_assoc($profile->roles) : array(),
  );

  // Add permissions field.
  $perms = array(
    'none' => t('--- NONE ---'),
  );
  $module_info = system_get_info('module');

  foreach (module_implements('permission') as $module) {
    $permissions = module_invoke($module, 'permission');
    foreach ($permissions as $name => $perm) {
      $perms[$module_info[$module]['name']][$name] = strip_tags($perm['title']);
    }
  }

  ksort($perms);

  $form['roles_wrapper']['access_control_perm'] = array(
    '#type' => 'select',
    '#options' => $perms,
    '#title' => t('Permission level access'),
    '#default_value' => isset($profile->access_control_perm) ? $profile->access_control_perm : 'none',
    '#description' => t('If necessary, please select a specific permission for allowing access to this profile. Select <em>"none"</em> to ignore permission level access.'),
  );

  // Access control options.
  $ac_options = array(
    'default' => t('Allow all users with the selected roles to access this profile.'),
    'selected_user_or_contact_and_admins' => t('Allow the selected user / contact to access this profile, plus anyone with the selected permission.'),
    'selected_user_or_contact' => t('Allow only the selected user / contact to access this profile.'),
  );

  // Access control.
  $form['roles_wrapper']['access_control'] = array(
    '#type' => 'radios',
    '#title' => t('User level access'),
    '#default_value' => isset($profile->access_control) ? $profile->access_control : 'default',
    '#options' => $ac_options,
    '#description' => t('If this profile is being prepopulated by a user or contact id, this control allows you to limit access to the currently logged in user. You can also allow access to users with a specific permission.'),
  );

  // Entity handlers are used to add fields for settings to the form based on
  // the entities that are selected.
  $entity_handlers = module_invoke_all('crm_core_profile_register_entity');
  foreach ($entity_handlers as $entity_type => $entity_handler_name) {
    if (isset($profile->entities[$entity_type]) && !empty($profile->entities[$entity_type]['status'])) {
      $entity_handler = new $entity_handler_name();
      $entity_handler->configEntityForm($profile, $form);
    }
  }

  // Submit buttons.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['actions']['finish'] = array(
    '#type' => 'submit',
    '#value' => t('Save and finish'),
    '#weight' => 11,
    '#submit' => array('crm_core_profile_settings_form_update'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#weight' => 13,
    '#submit' => array('crm_core_profile_new_form_cancel'),
  );
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#weight' => 16,
    '#submit' => array('crm_core_profile_delete_submit'),
  );

  return $form;
}

/**
 * Settings form validator.
 *
 * This handler validates submissions from the settings form. The validation
 * functions are executed dynamically based on what entities are being handled
 * in the profile.
 */
function crm_core_profile_settings_form_validate($form, &$form_state) {

  $profile = $form_state['profile'];
  $values = $form_state['values'];

  // Handling page creation. checking to see if configured paths are available.
  if ($values['page'] && !empty($values['page_path'])) {
    // Checking if this path is already taken.
    if ($values['page_path'] != $profile->page_path) {
      $menu_item = menu_get_item($values['page_path']);
      if ($menu_item && $menu_item['path'] == $values['page_path']) {
        form_set_error('page_path', t('A page with that path already exists. Please select another path for this page.'));
      }
    }
  }

  // Validate fields created by entity handlers.
  $entity_handlers = module_invoke_all('crm_core_profile_register_entity');
  foreach ($entity_handlers as $entity_handler_name) {
    $entity_handler = new $entity_handler_name();
    $entity_handler->configEntityFormValidate($form, $form_state);
  }
}

/**
 * Settings form submit handler.
 *
 * This handler saves some basic settings for the profile on it's own,
 * and executes the
 */
function crm_core_profile_settings_form_submit($form, &$form_state) {

  $profile = $form_state['profile'];
  form_state_values_clean($form_state);
  $settings = $form_state['values'];

  // Make sure we are only saving page paths when we are creating a page for
  // the profile.
  if (!$settings['page']) {
    unset($settings['page_path']);
    unset($settings['page_title']);
  }
  // Remove previously registered block.
  if (empty($settings['block'])) {
    db_delete('block')
      ->condition('module', 'crm_core_profile')
      ->condition('delta', 'ccp_' . $profile->name)
      ->execute();
  }
  else {
    cache_clear_all(NULL, 'cache_block');
  }

  $entity_handlers = module_invoke_all('crm_core_profile_register_entity');

  foreach ($entity_handlers as $entity_handler_name) {
    $entity_handler = new $entity_handler_name();
    $entity_info = $entity_handler->entityInfo();
    $entity_type = $entity_info['entity'];
    if (isset($settings[$entity_type])) {
      foreach ($settings[$entity_type] as $key => $value) {
        $profile->entities[$entity_type][$key] = $value;
      }
      unset($settings[$entity_type]);
    }
  }

  // Update individual settings in profile.
  foreach ($settings as $key => $value) {
    $profile->{$key} = $value;
  }

  // Filter out disabled roles.
  $profile->roles = array_filter($settings['roles']);

  // Save the profile. This will do a menu_rebuild.
  crm_core_profile_save($profile);

  drupal_set_message('Your profile settings have been saved');
}

/**
 * Submit handler for "Save and finish" button.
 */
function crm_core_profile_settings_form_update($form, &$form_state) {
  crm_core_profile_settings_form_submit($form, $form_state);
  $form_state['redirect'] = 'admin/structure/crm-core/profile';
}

/**
 * Submit handler for "Delete" button.
 */
function crm_core_profile_delete_submit($form, &$form_state) {
  $profile = isset($form['#profile']) ? $form['#profile'] : $form_state['profile'];
  $form_state['redirect'] = 'admin/structure/crm-core/profile/manage/' . $profile->name . '/delete';
}

/**
 * Profile export form.
 */
function crm_core_profile_export_form($form, &$form_state, $profile) {

  drupal_set_title(t('Export @profile', array('@profile' => $profile->label)));
  module_load_include('inc', 'ctools', 'includes/export');

  $form['profile'] = array(
    '#type' => 'textarea',
    '#rows' => 50,
    '#default_value' => ctools_export_crud_export('crm_core_profile', $profile),
  );

  return $form;
}

/**
 * Profile import form.
 */
function crm_core_profile_import_form($form, &$form_state) {

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Profile name'),
    '#description' => t('Please enter the profile form name.'),
    '#maxlength' => 28,
    '#default_value' => empty($form_state['values']) ? '' : $form_state['values']['label'],
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#maxlength' => 28,
    '#title' => t('Profile machine readable name'),
    '#default_value' => empty($form_state['values']) ? '' : $form_state['values']['name'],
    '#description' => t('Enter the name to use for this profile if it is different from the source profile. Leave blank to use the name of the source profile.'),
  );

  $form['name_override'] = array(
    '#type' => 'checkbox',
    '#title' => t('Replace an existing profile if one exists with the same name'),
    '#default_value' => empty($form_state['values']) ? FALSE : $form_state['values']['name_override'],
  );

  $form['profile'] = array(
    '#type' => 'textarea',
    '#title' => t('Paste CRM Core Profile code here'),
    '#default_value' => empty($form_state['values']) ? '' : $form_state['values']['profile'],
    '#states' => array(
      'required' => array(
        ':input[name="profile"]' => array('empty' => TRUE),
      ),
    ),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['import'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#weight' => 15,
    '#submit' => array('crm_core_profile_new_form_cancel'),
  );

  return $form;
}

/**
 * Profile import form validate handler.
 */
function crm_core_profile_import_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  if (!empty($values['profile'])) {
    // Declaring $crm_core_profile just to fool syntax highlighter.
    $crm_core_profile = array();
    ob_start();
    eval($values['profile']);
    ob_end_clean();

    if (!is_object($crm_core_profile)) {
      form_set_error('profile', t('Unable to interpret profile code.'));
    }

    // Profile name must be alphanumeric or underscores, no other punctuation.
    if (!empty($values['name']) && preg_match('/[^a-zA-Z0-9_]/', $values['name'])) {
      form_set_error('name', t('Profile name must be alphanumeric or underscores only.'));
    }

    $machine_name_check = FALSE;
    // User didn't provided new machine name, checking from code.
    if (empty($values['name']) && crm_core_profile_new_form_machine_name_check($crm_core_profile->name)) {
      // Profile already exists.
      $machine_name_check = TRUE;
    }
    elseif (crm_core_profile_new_form_machine_name_check($values['name'])) {
      $machine_name_check = TRUE;
    }
    if (!$values['name_override'] && $machine_name_check) {
      form_set_error('name', t('A profile by that name already exists, please choose a different name.'));
    }
    $form_state['profile'] = $crm_core_profile;
  }
  else {
    form_set_error('profile', t('You must put exported CRM Core Profile code into that field.'));
  }
}

/**
 * Profile import form submit handler.
 */
function crm_core_profile_import_form_submit($form, &$form_state) {
  $profile = $form_state['profile'];
  if (!empty($form_state['values']['label'])) {
    $profile->label = $form_state['values']['label'];
  }
  if (!empty($form_state['values']['name'])) {
    $profile->name = $form_state['values']['name'];
  }
  crm_core_profile_save($profile);
  drupal_set_message(t('CRM Core Profile %profile was successfully imported.', array('%profile' => $profile->label)));
  $edit_path = 'admin/structure/crm-core/profile/manage/' . $profile->name . '/edit';
  $form_state['redirect'] = $edit_path;
}

