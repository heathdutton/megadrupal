<?php
/**
 * @file
 * Admin Settings for Infusionsoft.
 */

/**
 * Admin Settings Form Callback
 */
function infusionsoft_admin_settings() {
  $form = array();

  $form['tracking'] = array(
    '#type' => 'fieldset',
    '#title' => t('Infusionsoft Tracking'),
    '#collapsible' => TRUE,
  );

  $tracking_example_url = drupal_get_path('module', 'infusionsoft') . '/images/infusionsoft-tracking-code.png';
  $tracking_image = theme('image', array(
    'attributes' => array('style' => 'border: 1px solid #666; max-width: 100%;'),
    'alt' => 'Tracking Code example.',
    'path' => $tracking_example_url
  ));

  $form['tracking']['infusionsoft_tracking_screenshot'] = array(
    '#markup' => $tracking_image,
  );

  $form['tracking']['infusionsoft_tracking'] = array(
    '#type' => 'textarea',
    '#rows' => 2,
    '#default_value' => variable_get('infusionsoft_tracking', ''),
    '#description' => '',
    '#title' => 'Infusionsoft Tracking URL',
    '#required' => TRUE,
    '#description' => '<a href="http://ug.infusionsoft.com/article/AA-01051/0/Embed-the-Infusionsoft-tracking-code-into-a-web-page.html" target="_blank">View instructions</a>
      on how to find the web tracking code for your application. Note: we only
      need the src url (@see above screenshot).',
  );

  $form['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('Infusionsoft API Settings'),
    '#collapsible' => TRUE,
  );

  $form['api']['infusionsoft_api_instructions'] = array(
    '#markup' => '<p>Note: these settings are required to integrate Drupal user
      accounts with Infusionsoft.</p>'
  );

  $app_name_image_example_url = drupal_get_path('module', 'infusionsoft') . '/images/infusionsoft-app-name.png';
  $app_name_image = theme('image', array(
    'attributes' => array('style' => 'border: 1px solid #666; max-width: 100%;'),
    'alt' => 'App Name example.',
    'path' => $app_name_image_example_url
  ));

  $form['api']['infusionsoft_app_name_screenshot'] = array(
    '#markup' => $app_name_image,
  );

  $form['api']['infusionsoft_app_name'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('infusionsoft_app_name', ''),
    '#description' => '',
    '#title' => 'Infusionsoft Application Name (domain prefix)',
    '#required' => TRUE,
    '#description' => 'You will need to find the name for your application from
      The Infusionsoft administrative dashboard (see screenshot above for where
      to find this).',
  );

  $api_key_image_example_url = drupal_get_path('module', 'infusionsoft') . '/images/infusionsoft-api-key.png';
  $api_key_image = theme('image', array(
    'attributes' => array('style' => 'border: 1px solid #666; max-width: 100%;'),
    'alt' => 'API Key example.',
    'path' => $api_key_image_example_url
  ));

  $form['api']['infusionsoft_api_key_screenshot'] = array(
    '#markup' => $api_key_image,
  );

  $form['api']['infusionsoft_api_key'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('infusionsoft_api_key', ''),
    '#description' => '',
    '#title' => 'Infusionsoft API Key',
    '#required' => TRUE,
    '#description' => 'To find/set the API key you will need to navigate to your
      application\'s settings page. The value you need is called the "Encrypted
      Key" (see above screenshot).'
  );

  $form['debug'] = array(
    '#type' => 'fieldset',
    '#title' => t('Debugging Options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['debug']['infusionsoft_debug_requests'] = array(
    '#type' => 'checkbox',
	'#title' => 'Show all XML-RPC requests in log',
	'#default_value' => variable_get('infusionsoft_debug_requests', 0),
	'#return_value' => 1,
  );

  return system_settings_form($form);
}

/**
 * User Contact Form
 */
function infusionsoft_user_contact_form($form, &$form_state, $account) {
  if (!infusionsoft_check_settings()) {
    return FALSE;
  }

  $form = array(
    '#prefix' => '<div id="infusionsoft-user-widget">',
    '#suffix' => '</div>',
  );

  $infusionsoft_id = infusionsoft_contact_id($account, FALSE);

  if (!$infusionsoft_id) {
    $form['quick_add'] = array(
      '#type' => 'fieldset',
      '#title' => 'Add User to Infusionsoft',
    );
    $form['quick_add']['add'] = array(
      '#type' => 'submit',
      '#value' => 'Add to Infusionsoft',
      '#submit' => array('infusionsoft_user_form_submit'),
      '#name' => 'add_user',
      '#ajax' => array(
        'wrapper' => 'infusionsoft-user-widget',
        'callback' => 'infusionsoft_user_form_js',
      ),
    );
  }
  else {
    $available_groups = infusionsoft_groups_options();
    $groups = infusionsoft_group_contact_options($infusionsoft_id);

    $form['quick_add'] = array(
      '#type' => 'fieldset',
      '#title' => 'Remove User from Infusionsoft',
    );
    $form['quick_add']['user_id'] = array(
      '#markup' => t('User is infusionsoft Contact# !contact_num', array('!contact_num' => $infusionsoft_id)),
      '#prefix' => '<div>',
      '#suffix' => '</div>',
    );

    $live_link = 'https://' . variable_get('infusionsoft_app_name', '') . '.infusionsoft.com/Contact/manageContact.jsp';

    $form['quick_add']['user_link'] = array(
      '#markup' => l(
        t('View User in Infusionsoft'), $live_link, array(
        'query' => array('view' => 'edit', 'ID' => $infusionsoft_id),
        'attributes' => array('target' => '_blank'),
        )
      ),
      '#prefix' => '<div>',
      '#suffix' => '</div>',
    );

    if ($groups) {
      $form['groups'] = array(
        '#type' => 'fieldset',
        '#title' => 'Current Infusionsoft Groups (Tags)',
      );

      $group_rows = array();
      foreach ($groups as $group) {

        unset($available_groups[$group['GroupId']]);

        $form['groups'][$group['GroupId']] = array(
          '#type' => 'submit',
          '#value' => t('Remove from Group'),
          '#submit' => array('infusionsoft_user_form_submit'),
          '#name' => 'remove_group_' . $group['GroupId'],
        );

        $group_rows[$group['GroupId']] = array(
          $group['ContactGroup'],
          format_date(strtotime($group['DateCreated']->iso8601)),
          drupal_render($form['groups'][$group['GroupId']]),
        );
      }
      $form['groups']['group_table'] = array(
        '#theme' => 'table',
        '#header' => array(t('Group (Tag)'), t('Date Added'), t('Remove')),
        '#rows' => $group_rows,
      );
    }

    if ($available_groups) {
      $form['available_groups'] = array(
        '#type' => 'fieldset',
        '#title' => 'Available Infusionsoft Groups (Tags)',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      $group_rows = array();

      foreach ($available_groups as $id => $group) {
        $form['available_groups'][$id] = array(
          '#type' => 'submit',
          '#value' => t('Add to Group'),
          '#submit' => array('infusionsoft_user_form_submit'),
          '#name' => 'add_group_' . $id,
        );

        $group_rows[$id] = array(
          $group . ' (ID: ' . $id . ')',
          drupal_render($form['available_groups'][$id]),
        );
      }
      $form['available_groups']['group_table'] = array(
        '#theme' => 'table',
        '#header' => array(t('Group (Tag)'), t('Add')),
        '#rows' => $group_rows,
      );
    }
  }

  $form_state['user'] = $account;

  return $form;
}

/**
 * User Form JS
 */
function infusionsoft_user_form_js($form, $form_state) {
  return $form;
}

/**
 * User Form Submit Handler
 */
function infusionsoft_user_form_submit($form, &$form_state) {

  $action = $form_state['triggering_element']['#name'];

  if ($action === 'add_user') {
    $infusionsoft_id = infusionsoft_user_to_contact($form_state['user']);
  }
  else {
    $infusionsoft_id = infusionsoft_contact_id($form_state['user']);
  }

  if (strpos($action, "remove_group_") !== FALSE) {
    $group_id = str_replace("remove_group_", "", $action);
    $action = "remove_group";
  }

  if (strpos($action, "add_group_") !== FALSE) {
    $group_id = str_replace("add_group_", "", $action);
    $action = "add_group";
  }

  switch ($action) {
    case 'remove_group':
      if (infusionsoft_user_op("remove", "group", $infusionsoft_id, $group_id)) {
        drupal_set_message(t("User Removed from Group"));
      }
      break;
    case 'add_group':
      if (infusionsoft_user_op("add", "group", $infusionsoft_id, $group_id)) {
        drupal_set_message(t("User Added to Group"));
      }
      break;
  }

  $form_state['rebuild'] = TRUE;
}
