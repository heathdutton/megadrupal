<?php
/**
 * @file
 * Page Callbacks for managing Social Media Applications.
 */

/**
 * List of Social Media Applications.
 */
function socialmedia_importer_ui_applications() {
  $query = 'SELECT id, app_name, app_id, app_secret, provider, is_authorized FROM {socialmedia_applications}';
  $results = db_query($query)->fetchAllAssoc('app_name');
  $header = array(
    'Application Name',
    'Application Id',
    'Application Secret',
    'Provider',
    array('data' => t('Operations'), 'colspan' => 3));
  $row = array();
  foreach ($results as $result) {
    $deauthorize_url = "admin/config/services/socialmedia_importer/application/" . $result->id . "/deauthorize";
    $authorize_url = "admin/config/services/socialmedia_importer/application/" . $result->id . "/authorize";
    $row[] = array(
      $result->app_name,
      $result->app_id,
      $result->app_secret,
      $result->provider,
      l(t('edit'), "admin/config/services/socialmedia_importer/application/" . $result->id . "/edit"),
      l(t('delete'), "admin/config/services/socialmedia_importer/application/" . $result->id . "/delete"),
      $result->is_authorized ? l(t('de-authorize'), $deauthorize_url) : l(t('authenticate'), $authorize_url),
    );
  }
  $output = theme('table', array('header' => $header, 'rows' => $row));
  return $output;
}

/**
 * Page callback: Displays add Application links for available providers.
 *
 * Redirects to application/add/[provider] if only one provider is available.
 */
function socialmedia_importer_ui_add_application_page() {
  $item = menu_get_item();
  if (!isset($item['mlid'])) {
    $item += db_query("SELECT mlid, menu_name FROM {menu_links} ml WHERE ml.router_path = :path AND module = 'system'", array(':path' => $item['path']))->fetchAssoc();
  }

  if (isset($cache[$item['mlid']])) {
    return $cache[$item['mlid']];
  }

  $content = array();
  $query = db_select('menu_links', 'ml', array('fetch' => PDO::FETCH_ASSOC));
  $query->join('menu_router', 'm', 'm.path = ml.router_path');
  $query
  ->fields('ml')
  // Weight should be taken from {menu_links}, not {menu_router}.
  ->fields('m', array_diff(drupal_schema_fields_sql('menu_router'), array('weight')))
  ->condition('ml.plid', $item['mlid'])
  ->condition('ml.menu_name', $item['menu_name'])
  ->condition('ml.hidden', 0);

  foreach ($query->execute() as $link) {
    _menu_link_translate($link);
    if ($link['access']) {
      // The link description, either derived from 'description' in
      // hook_menu() or customized via menu module is used as title attribute.
      if (!empty($link['localized_options']['attributes']['title'])) {
        $link['description'] = $link['localized_options']['attributes']['title'];
        unset($link['localized_options']['attributes']['title']);
      }
      // Prepare for sorting as in function _menu_tree_check_access().
      // The weight is offset so it is always positive, with a uniform 5-digits.
      $key = (50000 + $link['weight']) . ' ' . drupal_strtolower($link['title']) . ' ' . $link['mlid'];
      $content[$key] = $link;
    }
  }
  ksort($content);
  $cache[$item['mlid']] = $content;
  if (count($content) == 1) {
    $item = array_shift($content);
    drupal_goto($item['href']);
  }
  return theme('application_add_list', array('content' => $content));
}

/**
 * Page callback: Return a form to add/edit Social Media Application.
 * 
 * @param string $provider
 *   The Provider name.
 *   
 * @param int $id
 *   The Social Media Application id.
 */
function socialmedia_importer_ui_add_application_form($form, &$form_state, $provider, $id = NULL) {
  $form = array();
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $id,
  );
  if (!is_null($id)) {
    $application = socialmedia_importer_application_load($id);
  }
  if (is_null($provider) && is_object($application)) {
    $provider = $application->provider;
  }
  $form['provider'] = array(
    '#type' => 'value',
    '#value' => $provider,
  );
  $form['app_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Application Name'),
    '#description' => t('Enter the Name of your Application'),
    '#default_value' => isset($application->app_name) ? $application->app_name : '',
    '#required' => TRUE,
  );
  $form['app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Application Id'),
    '#default_value' => isset($application->app_id) ? $application->app_id : '',
    '#description' => t('An Application is required to access
        Social Media API. If you dont´t have an Application, you can
        create one in the corresponding Social Media Application page'),
    '#required' => TRUE,
  );
  $form['app_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Applicaion Secret'),
    '#default_value' => isset($application->app_secret) ? $application->app_secret : '',
    '#description' => t('An Application is required to access
        Social Media API. If you dont´t have an Application, you can
        create one in the corresponding Social Media Application page'),
    '#required' => TRUE,
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#suffix' => l(t('Cancel'), SOCIALMEDIA_IMPORTER_APP_LIST_PATH),
    '#weight' => 5,
  );
  if (!is_null($id)) {
    if (!empty($app->id)) {
      $form['id'] = array(
        '#type' => 'hidden',
        '#value' => $application->id,
      );
    }
  }
  return $form;
}

/**
 * Add/Edit Application form validate handler.
 */
function socialmedia_importer_ui_add_application_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $provider = $values['provider'];
  $app_id = $values['app_id'];
  $app_secret = $values['app_secret'];
  // Validate the Application.
  $application_provider_class = socialmedia_importer_get_application_provider_class($provider);
  if (!is_null($application_provider_class)) {
    $application = new $application_provider_class(NULL, $app_id, $app_secret);
    $is_valid = $application->validateApplication();
    if (!$is_valid) {
      form_error($form['app_id'],
          t('The Application Data are not valid. Please check your data from the Application console'));
      form_error($form['app_secret'],
          t('The Application Data are not valid. Please check your data from the Application console'));
      $_SESSION['messages']['error'] = array_unique($_SESSION['messages']['error']);
    }
  }
}

/**
 * Add/Edit Application Form Submit handler.
 */
function socialmedia_importer_ui_add_application_form_submit($form, &$form_state) {
  try {
    $fields = socialmedia_importer_application_save($form_state['values']);
    $is_authorized = 0;
    $_SESSION['smi_id'] = $fields['id'];
    if (!is_null($fields)) {
      if (array_key_exists('is_authorized', $fields)) {
        $is_authorized = $fields['is_authorized'];
      }
      if ($is_authorized) {
        // The Application is already authorized by the user.
        // Display confirm message.
        if (array_key_exists('is_updated', $fields)) {
          // The Application was updated.
          drupal_set_message(t('The Application %app has been updated.', array('%app' => $fields['app_name'])));
          $form_state['redirect'] = SOCIALMEDIA_IMPORTER_APP_LIST_PATH;
        }
        if (array_key_exists('is_new', $fields)) {
          // The Application is new created.
          watchdog('social media importer', 'Application %feed added.',
              array('%app' => $fields['app_name']),
              WATCHDOG_NOTICE, l(t('view'), 'admin/content/socialmedia_importer/app_list'));
          drupal_set_message(t('The Application %app has been added.', array('%app' => $fields['app_name'])));
          $form_state['redirect'] = SOCIALMEDIA_IMPORTER_APP_LIST_PATH;
        }
      }
      else {
        $application_provider = socialmedia_importer_get_application_provider_instance($fields['id']);
        if (!is_null($application_provider)) {
          $application_provider->authorizeApplication();
          $form_state['redirect'] = SOCIALMEDIA_IMPORTER_RESPONSE_HANDLER_PATH . $fields['id'];
        }
      }
    }
  }
  catch (Exception $e) {
    watchdog_exception('social media importer', $e);
    drupal_set_message(t('Error saving Social Media Application data'), 'error');
  }
}

/**
 * Page callback: Return a Form to delete a Social Media Application.
 * 
 * @param int $id
 *   The Social Media Application id.
 */
function socialmedia_importer_ui_delete_application_form($form, &$form_state, $id) {
  $id = (int) $id;
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $id,
  );
  $question = t('Are you sure you want to delete this Application?');
  $path = SOCIALMEDIA_IMPORTER_APP_LIST_PATH;
  $description = t("This Application will be deleted from the system and won't be available.");
  $yes = t('Delete');
  $no = t('Cancel');
  return confirm_form($form, check_plain($question), $path, check_plain($description), check_plain($yes), check_plain($no));
}

/**
 * Delete a Social Media Application form submit handler.
 */
function socialmedia_importer_ui_delete_application_form_submit($form, &$form_state) {
  $id = $form_state['values']['id'];
  if (!is_null($id)) {
    if (socialmedia_importer_application_delete($id)) {
      drupal_set_message(t("The Application is deleted successfully."));
    }
    else {
      drupal_set_message(t("Error occured while deleting the Application."), "error");
    }
  }
  else {
    drupal_set_message(t("Error occured: Can't find Application to be deleted."), "error");
  }
  $form_state['redirect'] = SOCIALMEDIA_IMPORTER_APP_LIST_PATH;
}

/**
 * Page callback: Return a Form to de-authorize a Social Media Application.
 *
 * @param int $id
 *   The Social Media Application id.
 */
function socialmedia_importer_ui_deauthorize_application_form($form, &$form_state, $id) {
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $id,
  );
  $question = t('Are you sure you want to revoke access token of this Application');
  $path = SOCIALMEDIA_IMPORTER_APP_LIST_PATH;
  $description = t("This Application can't be used for api call until authenticated again.");
  $yes = t('Deauthorize');
  $no = t('Cancel');
  return confirm_form($form, check_plain($question), $path, check_plain($description), check_plain($yes), check_plain($no));
}

/**
 * De-authorize a Social Media Application form submit handler.
 */
function socialmedia_importer_ui_deauthorize_application_form_submit($form, &$form_state) {
  $id = $form_state['values']['id'];
  if (!is_null($id)) {
    $application = socialmedia_importer_application_load($id);
    $is_deauthorized = socialmedia_importer_deauthorize_application($id);
    if ($is_deauthorized['success']) {
      drupal_set_message(t('The access to %application is revoked. If you want
          to make api call with this Application, you have to get a new access
          token.', array('%application' => $application->app_name)));
      drupal_goto(SOCIALMEDIA_IMPORTER_APP_LIST_PATH);
    }
    else {
      if (array_key_exists('error_message', $is_deauthorized)) {
        $error_message = $is_deauthorized['error_message'];
        drupal_set_message(t('%error_message', array('' => $error_message)), 'error');
        drupal_goto(SOCIALMEDIA_IMPORTER_APP_LIST_PATH);
      }
      else {
        drupal_set_message(t('The Application %application is not authorized.',
            array('%application' => $application->app_name)), 'warning');
        drupal_goto(SOCIALMEDIA_IMPORTER_APP_LIST_PATH);
      }
    }
  }
}

/**
 * Authorize a Social Media Application.
 * 
 * @param int $id
 *   The Application id.
 */
function socialmedia_importer_ui_authorize_application($id = NULL) {
  if ($id == NULL && isset($_SESSION['smi_id'])) {
    $id = $_SESSION['smi_id'];
  }
  elseif ($id) {
    $_SESSION['smi_id'] = $id;
  }
  $application_class = NULL;
  if ($id > 0) {
    $is_authorized = FALSE;
    $social_media_application = socialmedia_importer_get_application_provider_instance($id);
    if (!is_null($social_media_application)) {
      $social_media_application->authorizeApplication();
      $is_authorized = TRUE;
      unset($_SESSION['smi_id']);
    }
  }
}

/**
 * Returns HTML for a list of available provider for Application creation.
 *
 * @param array $variables
 *   An associative array containing:
 *   - content: An array of providers.
 */
function theme_application_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    foreach ($content as $item) {
      $items[] = array(
        'data' => '<span class="label">' . l($item['title'], $item['href'], $item['localized_options']) . '</span>' .
        '<div class="description">' . filter_xss_admin($item['description']) . '</div>',
      );
    }
    $list_data = array(
      'items' => $items,
      'title' => '',
      'type' => 'ul',
      'attributes' => array('class' => 'admin-list'));
    return theme_item_list($list_data);
  }
  else {
    $output = '<p>' . t('You need to enable a social media importer submodule.') . '</p>';
    return $output;
  }
}
