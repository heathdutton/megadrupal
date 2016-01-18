<?php
/**
 * @file
 * Admin forms.
 */

/**
 * Form constructor for general pardot settings.
 *
 * @ingroup forms
 */
function pardot_admin_form() {
  $form = array();

  $form['pardot_a_id'] = array(
    '#title' => t('Pardot account id'),
    '#type' => 'textfield',
    '#description' => t('The value show in the pardot demo script for piAId. eg. if the script has piAId = "1001"; this field should be 1001'),
    '#default_value' => variable_get('pardot_a_id', ''),
  );
  $form['pardot_c_id'] = array(
    '#title' => t('Default Pardot campaign id'),
    '#type' => 'textfield',
    '#description' => t('The value show in the pardot demo script for piCId. eg. if the script has piCId = "1001"; this field should be 1001'),
    '#default_value' => variable_get('pardot_c_id', ''),
  );
  $form['pardot']['pardot_validate_url'] = array(
    '#title' => t('Validate Pardot url'),
    '#type' => 'checkbox',
    '#description' => t('Validate Pardot form handler URL during webform creation.'),
    '#default_value' => variable_get('pardot_validate_url', 1),
  );
  // If curl isn't available want them that this functionality isn't available
  // and mark it as disabled.
  if (!function_exists('curl_getinfo')) {
    $form['pardot']['pardot_validate_url']['#description'] .= '<p class="warning">' .
      t('<a href="@curl">Curl</a> library required for this functionality not found.', array(
        '@curl' => 'http://php.net/manual/en/book.curl.php')) . '</p>';
    $form['pardot']['pardot_validate_url']['#default_value'] = FALSE;
    $form['pardot']['pardot_validate_url']['#disabled'] = TRUE;
  }

  // Visibilty setting, shameless stolen from googleanalytics module.
  $form['tracking_title'] = array(
    '#type' => 'item',
    '#title' => t('Tracking scope'),
  );
  $form['tracking'] = array(
    '#type' => 'vertical_tabs',
  );
  // Page specific visibility configurations.
  $php_access = user_access('use PHP for tracking visibility');
  $visibility = variable_get('pardot_visibility_pages', 0);
  $pages = variable_get('pardot_pages', PARDOT_PAGES);

  $form['tracking']['page_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pages'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  if ($visibility == 2 && !$php_access) {
    $form['tracking']['page_vis_settings'] = array();
    $form['tracking']['page_vis_settings']['visibility'] = array('#type' => 'value', '#value' => 2);
    $form['tracking']['page_vis_settings']['pages'] = array('#type' => 'value', '#value' => $pages);
  }
  else {
    $options = array(
      t('Every page except the listed pages'),
      t('The listed pages only'),
    );
    $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array(
      '%blog' => 'blog',
      '%blog-wildcard' => 'blog/*',
      '%front' => '<front>',
    ));

    if (module_exists('php') && $php_access) {
      $options[] = t('Pages on which this PHP code returns <code>TRUE</code> (experts only)');
      $title = t('Pages or PHP code');
      $description .= ' ' . t('If the PHP option is chosen, enter PHP code between %php. Note that executing incorrect PHP code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    else {
      $title = t('Pages');
    }
    $form['tracking']['page_vis_settings']['pardot_visibility_pages'] = array(
      '#type' => 'radios',
      '#title' => t('Add tracking to specific pages'),
      '#options' => $options,
      '#default_value' => $visibility,
    );
    $form['tracking']['page_vis_settings']['pardot_pages'] = array(
      '#type' => 'textarea',
      '#title' => $title,
      '#title_display' => 'invisible',
      '#default_value' => $pages,
      '#description' => $description,
      '#rows' => 10,
    );
  }

  // Render the role overview.
  $form['tracking']['role_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles'),
  );

  $form['tracking']['role_vis_settings']['pardot_visibility_roles'] = array(
    '#type' => 'radios',
    '#title' => t('Add tracking for specific roles'),
    '#options' => array(
      t('Add to the selected roles only'),
      t('Add to every role except the selected ones'),
    ),
    '#default_value' => variable_get('pardot_visibility_roles', 0),
  );

  $role_options = array_map('check_plain', user_roles());
  $form['tracking']['role_vis_settings']['pardot_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#default_value' => variable_get('pardot_roles', array()),
    '#options' => $role_options,
    '#description' => t('If none of the roles are selected, all users will be tracked. If a user has any of the roles checked, that user will be tracked (or excluded, depending on the setting above).'),
  );
  return system_settings_form($form);
}

/**
 * Form constructor for the pardot webform management form.
 *
 * @param node $webform_node
 *   The webform node pardot information is attached to.
 *
 * @see pardot_webform_components_form_validate()
 * @see pardot_webform_components_form_submit()
 * @see pardot_webform_components_form_delete_submit()
 *
 * @ingroup forms
 */
function pardot_webform_components_form($form, $form_state, $webform_node) {
  $form = array();

  $form['#tree'] = TRUE;
  $form['#node'] = $webform_node;
  $nid = $webform_node->nid;
  $record = pardot_webform_load($nid);

  $form['#record'] = $record;

  if (!isset($record->is_active)) {
    $record = new stdClass();
    $record->is_active = "";
    $record->url = "";
  }
  $form['details'] = array(
    '#type' => 'fieldset',
    '#title' => t('General'),
  );
  $form['details']['is_active'] = array(
    '#title' => 'Is active',
    '#type' => 'checkbox',
    '#default_value' => $record->is_active,
  );
  $form['details']['url'] = array(
    '#title' => 'Post url',
    '#type' => 'textfield',
    '#default_value' => $record->url,
    '#description' => t('Visit your "Form Handlers" page in Pardot. Click on a form link and then copy the "Endpoint URL" value here.<br /><br />!example', array(
      '!example' => theme('image', array('path' => drupal_get_path('module', 'pardot') . '/form-handler-url.jpg')),
    )),
  );

  $form['components'] = array(
    '#tree' => TRUE,
  );
  foreach ($webform_node->webform['components'] as $k => $component) {
    $form['components'][$k] = array(
      '#component' => $component,
      'key' => array(
        '#title' => 'Field name',
        '#type' => 'textfield',
        '#size' => 25,
        '#default_value' => isset($record->data[$component['form_key']]['key']) ? $record->data[$component['form_key']]['key'] : '',
      ),
    );
  }
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('pardot_webform_components_form_delete_submit'),
  );

  return $form;
}

/**
 * Theme callback for pardot_webform_components_form().
 */
function theme_pardot_webform_components_form($form) {
  $form = $form['form'];
  $rows = array();
  $output = '';

  $header = array(t('Name'), t('Type'), t('Pardot key'));

  foreach (element_children($form['components']) as $k) {
    $row = array();
    // Name.
    $row[] = $form['#node']->webform['components'][$k]['name'];
    // Type.
    $row[] = $form['#node']->webform['components'][$k]['type'];
    // Pardot key.
    unset($form['components'][$k]['key']['#title']);
    $row[] = drupal_render($form['components'][$k]['key']);
    $rows[] = $row;
  }

  $output .= drupal_render($form['details']);
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Form validation handler for pardot_webform_components_form().
 *
 * @see pardot_webform_components_form_submit()
 */
function pardot_webform_components_form_validate($form, $form_state) {
  // Check if we should validate the form handler.
  if (variable_get('pardot_validate_url', 1)) {
    if (function_exists('curl_getinfo')) {
      // Just check if the URL is active. No validation done if it is not.
      if ($form_state['values']['details']['is_active'] == 1) {

        // If so, check if it's a valid url.
        $check_url = curl_init($form_state['values']['details']['url']);
        // Use curl_setopt to get the response.
        curl_setopt($check_url, CURLOPT_RETURNTRANSFER, TRUE);
        curl_setopt($check_url, CURLOPT_HEADER, TRUE);

        // Fetch the url.
        $response = curl_exec($check_url);
        if (!$response) {
          drupal_set_message(t('Could not connect to host, no validation done for post URL'), 'status');
        }
        else {
          // Get the pardot cookie if it exists.
          preg_match('/^Set-Cookie:\s*([^;]*)/mi', $response, $cookie_result);
          parse_str($cookie_result[1], $cookie);

          // If the Pardot cookie exists.
          if (isset($cookie['pardot'])) {
            // Check for HTTP code, we need a 200.
            $http_code = curl_getinfo($check_url, CURLINFO_HTTP_CODE);
            if ($http_code != 200) {
              // Set form error.
              form_set_error('url', t('The Post url is not valid.'));
            }
          }
          // We are missing the Pardot cookie.
          else {
            form_set_error('url', t('This is not a pardot Post url.'));
          }
          // Always close an opened url.
          curl_close($check_url);
        }
      }
      else {
        // Print out message.
        drupal_set_message(t('No validation done for none active Post URL'), 'status');
      }
    }
  }
}

/**
 * Form submission handler for pardot_webform_components_form().
 *
 * Saves the pardot post url and webform information to the database.
 *
 * @see pardot_webform_components_form_validate()
 */
function pardot_webform_components_form_submit($form, $form_state) {
  $node = $form['#node'];

  $record = $form['#record'];
  if ($record) {
    $update = array('nid');
  }
  else {
    $record = new stdClass();
    $update = array();
  }

  $data = array();
  foreach (element_children($form['components']) as $k) {
    $component = $form['components'][$k]['#component'];
    $data[$component['form_key']]['key'] = $form_state['values']['components'][$k]['key'];
  }
  $record->nid = $node->nid;
  $record->url = $form_state['values']['details']['url'];
  $record->is_active = (bool) $form_state['values']['details']['is_active'];
  $record->data = $data;
  drupal_write_record('pardot_webform', $record, $update);
}

/**
 * Form submission handler for pardot_webform_components_form().
 *
 * Deletes the pardot post url from the database for the node.
 *
 * @see pardot_webform_components_form_validate()
 */
function pardot_webform_components_form_delete_submit($form, &$form_state) {
  $node = $form['#node'];
  db_delete('pardot_webform')
    ->condition('nid', $node->nid)
    ->execute();

  drupal_set_message(t('Pardot settings for this webform deleted.'), 'status');
}
