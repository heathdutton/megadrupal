<?php

/**
 * @file
 * Admin UI page that allows site administrators to manage
 * Content-Type mapping between Drupal and Alfresco.
 */

/**
 * Form allows mapping of different types between Drupal and Alfresco.
 */
function oscaddie_alfresco_model__admin__mapping() {
  module_load_include('inc', 'oscaddie_alfresco', 'includes/oscaddie_alfresco.admin');

  drupal_add_css(drupal_get_path('module', 'oscaddie_alfresco') . '/css/stylesheets/oscaddie_alfresco-admin.css');
  drupal_add_css(drupal_get_path('module', 'oscaddie_alfresco_model') . '/css/oscaddie_alfresco-model-admin.css');
  drupal_add_js(drupal_get_path('module', 'oscaddie_alfresco_model') . '/js/oscaddie_alfresco-mapping.js', array('scope' => 'footer'));
  $sections = oscaddie_alfresco__admin__sections_available();

  $form = drupal_get_form('oscaddie_alfresco_model__admin__mapping_types');

  $sidebar = theme('oscaddie_alfresco_admin_sidebar', array('sections' => $sections));
  $content = theme('oscaddie_alfresco_admin_content', array('sidebar' => $sidebar, 'content' => render($form)));
  return theme('oscaddie_alfresco_admin', array('content' => $content));
}

/**
 * Form that allows registering content-types to Alfresco.
 */
function oscaddie_alfresco_model__admin__mapping_types() {
  $form['#prefix'] = '<div id="mapping" class="clearfix">';
  $form['#suffix'] = '</div>';

  // Get all available content-types in Drupal.
  foreach (node_type_get_types() as $type => $info) {
    $options[$type] = $info->name;
  }

  $form['drupal'] = array(
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
    '#tree' => TRUE,
    '#prefix' => '<div id="drupal"><h3 class="icon drupal">' . t("Drupal Content Type") . '</h3>',
    '#suffix' => '</div>',
  );

  if (!empty($options)) {
    $form['drupal']['type'] = array(
      '#type' => 'radios',
      '#title' => '',
      '#options' => $options,
    );
  }
  else {
    $form['drupal']['type'] = array(
      '#type' => 'item',
      '#description' => t("You do not have any content type currently in Drupal."),
    );
  }

  // Get all available types on Alfresco.
  $form['alfresco'] = array(
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
    '#tree' => TRUE,
    '#prefix' => '<div id="alfresco"><h3 class="icon alfresco">' . t("Alfresco Content Model") . '</h3>',
    '#suffix' => '</div>',
  );

  if ($alfresco = oscaddie_alfresco_model__admin__alfresco_types_get()) {
    // dpm($alfresco, 'TYPES');

    foreach ($alfresco as $username => $types) {
      $form['alfresco'][$username] = array(
        '#type' => 'fieldset',
        '#title' => $types['site_name'],
        '#collapsed' => FALSE,
        '#collapsible' => TRUE,
        '#attributes' => array('class' => array('alfresco-types')),
      );

      $options = array();

      foreach ($types['types'] as $data) {
        $options[$data['name']] = $data['title'];
      }

      $form['alfresco'][$username]['options'] = array(
        '#type' => 'radios',
        '#options' => $options,
      );
    }
  }
  else {
    $form['alfresco']['model'] = array(
      '#type' => 'item',
      '#description' => t("You do not have any content models currently in Alfresco."),
    );
  }

  // Mapping fields.
  $form['mapping'] = array(
    '#tree' => TRUE,
    '#prefix' => '<div class="clear"></div><div id="mapping-fieldset"><h3>' . t("Mapping") . '</h3>',
    '#suffix' => '<div class="clear"></div></div>',
  );

  $form['mapping']['drupal_select'] = array(
    '#type' => 'select',
    '#title' => t('Drupal'),
    '#options' => array('' => t("Create New")),
  );

  $form['mapping']['drupal_label'] = array(
    '#type' => 'item',
    '#title' => t('Drupal'),
    '#value' => '<span></span>',
    '#prefix' => '<div id="edit-mapping-drupal-label-wrapper">',
    '#suffix' => '</div>',
  );

  $form['mapping']['sync'] = array(
    '#type' => 'select',
    '#title' => t('Sync Direction'),
    '#default_value' => OSCADDIE_ALFRESCO_MODEL_SYNC_FULL,
    '#options' => array(
      OSCADDIE_ALFRESCO_MODEL_SYNC_READ  => t('<---'),
      OSCADDIE_ALFRESCO_MODEL_SYNC_WRITE => t('--->'),
      OSCADDIE_ALFRESCO_MODEL_SYNC_FULL  => t('<--->'),
    ),
  );

  $form['mapping']['alfresco_select'] = array(
    '#type' => 'select',
    '#title' => t('Alfresco'),
    '#options' => array('' => t("Create New")),
  );

  $form['mapping']['alfresco_label'] = array(
    '#type' => 'item',
    '#title' => t('Alfresco'),
    '#value' => '<span></span>',
    '#prefix' => '<div id="edit-mapping-alfresco-label-wrapper">',
    '#suffix' => '</div>',
  );

  $form['buttons'] = array(
    '#prefix' => '<div class="clear"></div><div class="buttons">',
    '#suffix' => '</div>',
  );

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#attributes' => array('class' => array('button', 'ok')),
  );

  $form['buttons']['cancel'] = array(
    '#type' => 'item',
    '#value' => l(t("Cancel"), 'admin/oscaddie_alfresco', array('attributes' => array('class' => array('button', 'cancel')))),
  );

  return $form;
}

/**
 * Drupal form validation handler.
 */
function oscaddie_alfresco_model__admin__mapping_types_validate($form, &$form_state) {
  // TODO: FOR DRUPAL: Check if content-type is already mapped to another type during type creation
  // TODO: FOR ALFRESCO: Check if content-type already exists and mapped to a Drupal type.
}

/**
 * Submit handler for oscaddie_alfresco_model__admin__mapping_types() form.
 */
function oscaddie_alfresco_model__admin__mapping_types_submit($form, &$form_state) {
  $values = $form_state['values'];

  // Gather all content-types that are being send to Alfresco.
  if (!empty($values['drupal']['type'])) {
    $type = $values['drupal']['type'];
    $sync = $values['mapping']['sync'];
    oscaddie_alfresco_model__admin__mapping_types_drupal($type, $sync);
  }

  // Gather all content-types that are received from Alfresco.
  if (!empty($values['alfresco'])) {
    foreach ($values['alfresco'] as $site => $options) {
      if (!empty($options['options'])) {
        $types[$site] = $options['options'];
      }
    }

    if (!empty($types)) {
      // TODO: Derive the sync mode base on the access level.
      $sync = $values['mapping']['sync'];
      oscaddie_alfresco_model__admin__mapping_types_alfresco($types, $sync);
    }
  }

  $form_state['redirect'] = 'admin/oscaddie_alfresco/mapping';
}

/**
 * Handler to registering a Drupal content-type to Alfresco.
 */
function oscaddie_alfresco_model__admin__mapping_types_drupal($types, $sync = OSCADDIE_ALFRESCO_MODEL_SYNC_FULL) {
  $definitions = array();

  // Extract drupal content-types schema.
  $definitions[] = oscaddie_alfresco_model__drupal_type_definition($types, $sync);

  // Package the type definition and send to alfresco.
  if (!empty($definitions)) {
    $types_info = node_type_get_types();

    $package = oscaddie_alfresco_model__drupal_type_definition_package($definitions);
    $response = oscaddie_alfresco__alfresco_api__types_register($package);

    if (!empty($response[$types])) {
      list($type_a, $property) = explode(':', $response[$types]);
      oscaddie_alfresco_model__map_set($types, $type_a, $sync);

      drupal_set_message(t("<em>@type</em> successfully created on Alfresco.", array('@type' => $types_info[$types]->name)));
    }
    elseif ($response === FALSE) {
      drupal_set_message(t("Unable to create <em>@type</em> on Alfresco. Content Type already exists or invalid.", array('@type' => $types_info[$types]->name)));
    }
    elseif ($response === OSCADDIE_ALFRESCO_CODE_ALFRESCO_NOT_RESPONDING) {
      drupal_set_message(t("Unable to connect to Alfresco. Please contact your network administrator."), 'error', FALSE);
    }
  }
}

/**
 * Handler to create a Drupal content-type from a given Alfresco type.
 */
function oscaddie_alfresco_model__admin__mapping_types_alfresco($types, $sync = OSCADDIE_ALFRESCO_MODEL_SYNC_FULL) {
  $alfresco_settings = variable_get('oscaddie_alfresco_settings_alfresco', array());

  if (!($alfresco_types = oscaddie_alfresco__alfresco_api__types_get())) {
    drupal_set_message(t("Unable to contact Alfresco Server. Please contact your site administrator."), 'error', FALSE);
    watchdog('oscaddie_alfresco_model', 'Unable to contact Alfresco Server.', array(), WATCHDOG_ERROR);
    return FALSE;
  }

  $unpackaged = oscaddie_alfresco_model__alfresco_api__types_definition_unpackage($alfresco_types);

  // Look for the correct Type within the Username in Alfresco.
  foreach ($types as $username => $selected_type) {
    // Find the correct type to create base on username and selected type.
    if (!empty($unpackaged[$username][$selected_type])) {
      $unpackaged_type = $unpackaged[$username][$selected_type];

      $machine_name = $unpackaged_type['type'];
      $title = $unpackaged_type['title'];
      $description = "";
      $has_body = $unpackaged_type['has_body'];
      $fields = $unpackaged_type['fields'];
      $node_type = array_keys(node_type_get_names());

      // Check if the current site owner owns the type.
      if ($alfresco_settings['username'] == $unpackaged_type['site_owner']) {
        // Current site owner owns this type.
        // Check if the type already exists -- DO NOTHING if exists.
        if (!array_key_exists($machine_name, $node_type)) {
          // Type does not exists so we create new.
          $description = t("Backup of a type from Alfresco.");
          try {
            $node_info = oscaddie_alfresco_model__drupal_type_create($machine_name, $title, $description, $has_body, $fields);
          }
          catch (Exception $e) {
            throw new Exception('Failed to create Drupal type in Alfresco', 0, $e);
          }
        }
      }
      else {
        // Current site owner do not own this type.
        if (empty($node_type)) {
          // Type does not exists so we create new.
          $title = sprintf("%s - %s", $title, $unpackaged_type['site_name']);
          $description = t(
            "Copy of a type from Alfresco created in <em>@site_name</em>.",
            array('@site_name' => $unpackaged_type['site_name'])
          );
          $node_info = oscaddie_alfresco_model__drupal_type_create($machine_name, $title, $description, $has_body, $fields);
        }
        else {
          // Check further that this type was already created and mapped.
          // This prevent user trying to create and map twice.
          // If this case exists we DO NOTHING.
          try {
            $map_status = oscaddie_alfresco_model__map_status($unpackaged_type['site_owner'] . $unpackaged_type['type'], 'alfresco');
            if ($map_status) {
              if ($map_status->status == OSCADDIE_ALFRESCO_MODEL_MAP_DISABLED) {
                // User have the type of the same name but was not mapped from Alfresco.
                $machine_name = sprintf("%s_%s", $machine_name, $unpackaged_type['site_owner']);
                $title = sprintf("%s - %s", $title, $unpackaged_type['site_name']);
                $description = t("Copy of a type from Alfresco created in <em>@site_name</em>.", array('@site_name' => $unpackaged_type['site_name']));

                $node_info = oscaddie_alfresco_model__drupal_type_create($unpackaged_type['type'], $unpackaged_type['title'], $description, $unpackaged_type['has_body'], $unpackaged_type['fields']);
              }
            }
            else {
              // Content doesn't exist on site.
              $machine_name = sprintf('%s_%s', $machine_name, $unpackaged_type['site_owner']);
              $title = sprintf('%s - %s', $title, $unpackaged_type['site_name']);
              $description = t('Copy of a type from Alfresco created in <em>@site_name</em>.', array('@site_name' => $unpackaged_type['site_name']));

              $node_info = oscaddie_alfresco_model__drupal_type_create($machine_name, $title, $description, $unpackaged_type['has_body'], $unpackaged_type['fields']);
            }
          }
          catch (Exception $e) {
            throw new Exception('Failed to map model status', 0, $e);
          }
        }
      }

      if (!empty($node_info)) {
        // Send mapping to Alfresco if node created correctly.
        if ($alfresco_settings['username'] != $unpackaged_type['site_owner']) {
          $fields_map = array();

          $standard = array(
            'language',
            'promote',
            'status',
            'sticky',
            'title',
          );

          foreach ($standard as $property) {
            $fields_map[] = array(
              'field_name_drupal'   => $property,
              'field_name_alfresco' => $property,
            );
          }

          if ($has_body) {
            $fields_map[] = array(
              'field_name_drupal'   => 'teaser',
              'field_name_alfresco' => 'teaser',
            );
          }

          foreach ($unpackaged_type['fields'] as $property) {
            if (array_key_exists($property['field_name'], $node_info->fields)) {
              $fields_map[] = array(
                'field_name_drupal'   => $property['field_name'],
                'field_name_alfresco' => $property['field_name'],
              );
            }
          }

          $map[] = array(
            'type_drupal' => $node_info->type,
            'type_alfresco' => $unpackaged_type['type'],
            'creator' => $unpackaged_type['site_owner'],
            'fields' => $fields_map,
          );
          try {
            $map_package = oscaddie_alfresco__alfresco_api__map_definition_package($map);
            // Send mapping to Alfresco.
            oscaddie_alfresco__alfresco_api__types_map($map_package);
          }
          catch (Exception $e) {
            throw new Exception('Failed to map definition', 0, $e);
          }
        }

        // Set mapping.
        try {
          $type_a = $unpackaged_type['site_owner'] . $unpackaged_type['type'];
          oscaddie_alfresco_model__map_set($node_info->type, $type_a, $sync);
        }
        catch (Exception $e) {
          throw new Exception('Failed to map set', 0, $e);
        }
      }
    }
  }
}

/**
 * Get all Alfresco Content Types from server and extract necessary data.
 *
 * @return array
 *   An array of all types in Alfresco.
 */
function oscaddie_alfresco_model__admin__alfresco_types_get() {
  $list = array();

  if (!($types = oscaddie_alfresco__alfresco_api__types_get())) {
    return FALSE;
  }

  foreach ($types as $idx => $type) {
    if (!isset($list[$type->creater])) {
      $list[$type->creater] = array(
        'site_name' => $type->siteName,
        'access'    => $type->access,
      );
    }

    $list[$type->creater]['types'][$type->name] = array(
      'name' => $type->name,
      'title' => $type->title,
    );
  }

  return $list;
}
