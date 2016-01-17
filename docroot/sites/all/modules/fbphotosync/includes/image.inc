<?php
/**
 * @file
 * Image module integration.
 */

/**
 * Implements hook_form_field_ui_field_edit_form_alter().
 */
function fbphotosync_form_field_ui_field_edit_form_alter(&$form, $form_state) {
  if ($form['#field']['module'] == 'image') {
    $facebook = fbphotosync_facebook_sdk();
    $settings = isset($form['#instance']['settings']['fbphotosync']) ? $form['#instance']['settings']['fbphotosync'] : array();

    // Settings form.
    $form['instance']['settings']['fbphotosync'] = array(
      '#type' => 'container',
      '#weight' => 20,
      '#tree' => TRUE,
    );
    $form['instance']['settings']['fbphotosync']['status'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use Facebook Photo Sync'),
      '#default_value' => isset($settings['status']) ? $settings['status'] : FALSE,
    );

    // Settings fieldset.
    $form['instance']['settings']['fbphotosync']['settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Facebook Photo Sync settings'),
      '#states' => array(
        'visible' => array(
          ':input[name="instance[settings][fbphotosync][status]"]' => array('checked' => TRUE),
        ),
      ),
    );

    // Account settings.
    $form['instance']['settings']['fbphotosync']['settings']['account'] = array(
      '#type' => 'fieldset',
      '#title' => t('Account settings'),
    );
    $form['instance']['settings']['fbphotosync']['settings']['account']['id'] = array(
      '#type' => 'select',
      '#title' => t('Account'),
      '#options' => fbphotosync_account_ids($facebook),
      '#default_value' => isset($settings['settings']['account']['id']) ? $settings['settings']['account']['id'] : key(fbphotosync_account_ids($facebook)),
      '#ajax' => array(
        'callback' => 'fbphotosync_image_ajax',
        'wrapper' => 'edit-instance-settings-fbphotosync',
      ),
      '#required' => TRUE,
    );

    // Album settings.
    $form['instance']['settings']['fbphotosync']['settings']['album'] = array(
      '#type' => 'fieldset',
      '#title' => t('Album settings'),
    );
    $form['instance']['settings']['fbphotosync']['settings']['album']['type'] = array(
      '#type' => 'select',
      '#title' => t('Type'),
      '#options' => array(
        'existing' => t('Existing album'),
        'dynamic' => t('Dynamically generated album'),
      ),
      '#default_value' => isset($settings['settings']['album']['type']) ? $settings['settings']['album']['type'] : 0,
      '#required' => TRUE,
    );
    list($account_type, $account_id) = explode(':', isset($form_state['input']['instance']['settings']['fbphotosync']) ? $form_state['input']['instance']['settings']['fbphotosync']['settings']['account']['id'] : $form['instance']['settings']['fbphotosync']['settings']['account']['id']['#default_value']);
    $form['instance']['settings']['fbphotosync']['settings']['album']['existing'] = array(
      '#type' => 'select',
      '#title' => t('Album'),
      '#options' => fbphotosync_album_ids($facebook, $account_id),
      '#default_value' => isset($settings['settings']['album']['existing']) ? $settings['settings']['album']['existing'] : 0,
      '#states' => array(
        'visible' => array(
          ':input[name="instance[settings][fbphotosync][settings][album][type]"]' => array('value' => 'existing'),
        ),
      ),
    );
    $form['instance']['settings']['fbphotosync']['settings']['album']['dynamic'] = array();
    $form['instance']['settings']['fbphotosync']['settings']['album']['dynamic']['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#default_value' => isset($settings['settings']['album']['dynamic']['name']) ? $settings['settings']['album']['dynamic']['name'] : '',
      '#element_validate' => array('_fbphotosync_imagefield_element_validate'),
      '#states' => array(
        'visible' => array(
          ':input[name="instance[settings][fbphotosync][settings][album][type]"]' => array('value' => 'dynamic'),
        ),
      ),
    );
    $form['instance']['settings']['fbphotosync']['settings']['album']['dynamic']['message'] = array(
      '#type' => 'textarea',
      '#title' => t('Description'),
      '#default_value' => isset($settings['settings']['album']['dynamic']['message']) ? $settings['settings']['album']['dynamic']['message'] : '',
      '#states' => array(
        'visible' => array(
          ':input[name="instance[settings][fbphotosync][settings][album][type]"]' => array('value' => 'dynamic'),
        ),
      ),
    );

    // Image settings.
    $form['instance']['settings']['fbphotosync']['settings']['image'] = array(
      '#type' => 'fieldset',
      '#title' => t('Image settings'),
    );
    $form['instance']['settings']['fbphotosync']['settings']['image']['cardinality'] = array(
      '#type' => 'select',
      '#title' => t('Number of values'),
      '#options' => array(0 => t('Unlimited')) + drupal_map_assoc(range(1, 10)),
      '#default_value' => isset($settings['settings']['image']['cardinality']) ? $settings['settings']['image']['cardinality'] : 0,
      '#required' => TRUE,
      '#description' => t('Maximum number of values of images to be uploaded to Facebook.'),
    );
    $form['instance']['settings']['fbphotosync']['settings']['image']['message'] = array(
      '#type' => 'textarea',
      '#title' => t('Description'),
      '#default_value' => isset($settings['settings']['image']['message']) ? $settings['settings']['image']['message'] : '',
    );

    // Replacement patterns.
    $entity_info = entity_get_info($form['#instance']['entity_type']);
    $form['instance']['settings']['fbphotosync']['settings']['tokens'] = array(
      '#type' => 'fieldset',
      '#title' => t('Replacement patterns'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => theme('token_tree', array('token_types' => array($entity_info['token type'], 'file'))),
    );
  }
}

/**
 *
 */
function fbphotosync_image_ajax($form, &$form_state) {
  return $form['instance']['settings']['fbphotosync'];
}

/**
 * Element validation callback for Dynamic album fields.
 */
function _fbphotosync_imagefield_element_validate($element, &$form_state) {
  if ($form_state['values']['instance']['settings']['fbphotosync']['status'] == TRUE && $form_state['values']['instance']['settings']['fbphotosync']['settings']['album']['type'] == 'dynamic' && empty($element['#value'])) {
    form_error($element, t('@field field is required.', array('@field' => $element['#title'])));
  }
}

/**
 * Implements hook_entity_insert().
 */
function fbphotosync_entity_insert($entity, $type) {
  $settings = variable_get('fbphotosync_settings', array());
  $entity_info = entity_get_info($type);
  $bundle_name = !empty($entity_info['entity keys']['bundle']) ? $entity->{$entity_info['entity keys']['bundle']} : $type;
  foreach (field_info_instances($type, $bundle_name) as $instance) {
    if ($instance['widget']['module'] == 'image' && isset($instance['settings']['fbphotosync']) && $instance['settings']['fbphotosync']['status'] == TRUE && isset($entity->{$instance['field_name']})) {
      if ($facebook = fbphotosync_facebook_sdk()) {
        if (fbphotosync_facebook_sdk_is_authenticated($facebook, $settings['access_token'])) {
          $field = field_info_field($instance['field_name']);

          // Set file upload support.
          $facebook->setFileUploadSupport(TRUE);

          // Extract account type and ID from instance settings.
          list($account_type, $account_id) = explode(':', $instance['settings']['fbphotosync']['settings']['account']['id']);

          // Process field files.
          foreach (element_children($entity->{$instance['field_name']}) as $langcode) {
            foreach (element_children($entity->{$instance['field_name']}[$langcode]) as $delta) {
              $item = $entity->{$instance['field_name']}[$langcode][$delta];
              if ($instance['settings']['fbphotosync']['settings']['image']['cardinality'] == 0 || $instance['settings']['fbphotosync']['settings']['image']['cardinality'] > $delta) {
                // Prepare field.
                if (function_exists($function = "{$instance['widget']['module']}_field_load")) {
                  $items = array(array(&$item));
                  $function($type, array($entity), $field, array($instance), $langcode, $items, FIELD_LOAD_CURRENT);
                  $item = $items[0][0];
                }

                // Gather token data information.
                $token_data = array(
                  $entity_info['token type'] => $entity,
                  'file' => (object) $item,
                );

                // Account type specific logic.
                switch ($account_type) {
                  case 'application':
                  case 'page':
                    $response = $facebook->api("/{$account_id}?fields=access_token");
                    $access_token = $response['access_token'];
                }

                // Album type specific logic.
                switch ($instance['settings']['fbphotosync']['settings']['album']['type']) {
                  case 'dynamic':
                    // Process album name and description.
                    $args = array(
                      'name' => token_replace($instance['settings']['fbphotosync']['settings']['album']['dynamic']['name'], $token_data, array('clear' => TRUE)),
                      'message' => token_replace($instance['settings']['fbphotosync']['settings']['album']['dynamic']['message'], $token_data, array('clear' => TRUE)),
                    );

                    // Check if album already exists.
                    $albums = $facebook->api("/{$account_id}/albums");
                    $album_exists = FALSE;
                    foreach ($albums['data'] as $album) {
                      if ($album['name'] == $args['name']) {
                        $album_exists = TRUE;
                        break;
                      }
                    }

                    // Create new album.
                    if (!$album_exists) {
                      // Add access_token if present.
                      if (isset($access_token)) {
                        $args['access_token'] = $access_token;
                      }

                      $album = $facebook->api("/{$account_id}/albums", 'post', $args);
                    }
                    break;

                  case 'existing':
                    $album = array('id' => $instance['settings']['fbphotosync']['settings']['album']['existing']);
                }

                // Facebook expects the realpath to the file prepended with a '@'.
                $args = array(
                  'image' => '@' . drupal_realpath($item['uri']),
                );
                // Add access_token if present.
                if (isset($access_token)) {
                  $args['access_token'] = $access_token;
                }

                // Process image message/description.
                $args['message'] = token_replace($instance['settings']['fbphotosync']['settings']['image']['message'], $token_data, array('clear' => TRUE));

                // Upload the image file.
                // @TODO - Store the Facebook Photo ID aginst the File ID.
                $data = $facebook->api("/{$album['id']}/photos", 'post', $args);
              }
            }
          }
        }
      }
    }
  }
}
