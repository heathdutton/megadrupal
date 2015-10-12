<?php
/**
 * @file
 * Administration pages callbacks for the Yandex.Webmaster module.
 */

/**
 * Display configuration page for the Yandex.Webmaster module.
 */
function yandex_webmaster_admin_settings($form, &$form_state) {
  drupal_add_css(drupal_get_path('module', 'yandex_webmaster') . '/css/yandex_webmaster.css');

  $form = array();
  $form['branding'] = yandex_webmaster_branding_item(TRUE);

  // Parse host list.
  $hostlist = yandex_webmaster_get_hosts();
  // Check for current host.
  $current_host = yandex_webmaster_idna_convert_operation($_SERVER['HTTP_HOST'], 'decode');
  $current_host_id = yandex_webmaster_get_host_id();

  // Select language and domain name.
  $form['yandex_webmaster_domain'] = array(
    '#type' => 'select',
    '#title' => t('Select Yandex.Webmaster domain name'),
    '#options' => array(
      'com' => t('Default') . ' (webmaster.yandex.com)',
      'ru' => t('Russian') . ' (webmaster.yandex.ru)',
      'ua' => t('Ukrainian') . ' (webmaster.yandex.ua)',
    ),
    '#default_value' => variable_get('yandex_webmaster_domain', 'com'),
  );
  // Empty host list.
  if (!count($hostlist)) {
    return system_settings_form($form);
  }

  // Check for failed verification and sites in subfolders.
  if ($current_host_id != -1 && $hostlist[$current_host_id]['verification'] == 'VERIFICATION_FAILED') {
    $uin = yandex_webmaster_get_host_uin($current_host_id);
    // Get uin for current host.
    if (!$uin) {
      drupal_set_message(t('An error occurred while getting current host UIN from Yandex.Webmaster service.'), 'error');
    }
    else {
      if (base_path() != '/') {
        $form['manual_verification_message'] = array(
          '#type' => 'item',
          '#markup' => '<div class="messages warning">'
            . t('Current site (@site) can\'t be automatic verified. You will need to create empty text file "@filename" in site root directory.', array(
              '@site' => $current_host,
              '@filename' => 'yandex_' . $uin . '.txt'
            ))
            . '</div>',
        );
      }
      elseif (variable_get('yandex_webmaster_verification_type', '') == 'META_TAG') {
        $form['manual_verification_message'] = array(
          '#type' => 'item',
          '#markup' => '<div class="messages warning">'
            . t('Unfortunately, Yandex.Webmaster service may not process meta-tag verification for the first time. To run check procedure once again, !link.', array(
              '!link' => l(t('click here'), 'http://webmaster.yandex.com/site/verification.xml?wizard=verification&wizard-hostid=' . $current_host_id . '&host=' . $current_host_id, array(
                'attributes' => array(
                  'target' => '_blank',
                  'title' => t('Open in new window'),
                ),
              ))
            ))
            . '</div>',
        );
      }
    }
  }

  // Display hosts list.
  $basic_host = variable_get('yandex_webmaster_basic_host_id', 0);
  $header = array(
    'hostname' => t('Host name'),
    'tcy' => t('TCY'),
    'url_count' => t('URL count'),
    'index_count' => t('Index count'),
    'last_access' => t('Last access'),
    'actions' => t('Actions'),
  );
  $options = array();

  // Fill rows.
  foreach ($hostlist as $host_id => $host) {
    $actions = '';
    if ($host['url-count'] > 0) {
      $actions .= l(t('Open statistics'), 'admin/reports/yandex_webmaster/' . $host_id, array(
        'attributes' => array(
          /* 'class' => 'yandex_webmaster_chart', */
          'class' => 'yandex_webmaster_actions',
          'title' => t('Open statistics'),
        ),
      ));
    }
    if ($host_id == $current_host_id) {
      $actions .= l(t('Remove site'), 'admin/config/yandex_webmaster/remove', array(
        'attributes' => array(
          /* 'class' => 'yandex_webmaster_remove', */
          'class' => 'yandex_webmaster_actions',
          'title' => t('Remove site'),
        ),
      ));
    }

    // Get right hostname.
    $hostname = $host['name'];
    $url = yandex_webmaster_idna_convert_operation('http://' . $host['name'], 'encode');
    // Display host state as icon.
    $state = $host['verification'];
    $virused = $host['virused'] == 'false' ? '' : ' VIRUSED';

    $status = yandex_webmaster_state_description($state, 'verification') . ' ' . yandex_webmaster_state_description($host['crawling'], 'index');
    $options[$host_id] = array(
      '#attributes' => array(
        'class' => array('yandex_webmaster_host_row' . $virused),
      ),
      'hostname' => '<div class="yandex_webmaster_host_state ' . $state . $virused . '" title="' . $status . '">'
        . '<div class="yandex_webmaster_hostname">' . $hostname
        . l($hostname, $url, array(
          'attributes' => array(
            'class' => 'yandex_webmaster_external_link',
            'target' => '_blank',
            'title' => t('Open in new window'),
          ),
        ))
        . '<br />'
        . '<span>'
        . $status
        . '</span> '
        . '</div>',
      'tcy' => $host['tcy'],
      'url_count' => $host['url-count'] > 0 ? number_format($host['url-count'], 0, '', ' ') : $host['url-count'],
      'index_count' => $host['index-count'] > 0 ? number_format($host['index-count'], 0, '', ' ') : $host['index-count'],
      'last_access' => '<div class="yandex_webmaster_last_access">'
        . ($host['last-access'] > 0 ? t('@time ago', array('@time' => format_interval(time() - $host['last-access']))) . '<br /><span>' . format_date($host['last-access']) . '</span>' : $host['last-access'])
        . '</div>',
      'actions' => $actions,
    );
  }
  // Add current host to list, if it has not been added.
  if ($current_host_id == -1) {
    $options[$current_host] = array(
      'hostname' => '<div class="yandex_webmaster_host_state WAITING" title="' . t('Not in your host list. Need to be added.') . '">'
        . '<div class="yandex_webmaster_hostname">' . $current_host . '<br />'
        . '<span>' . t('Not in your host list. Need to be added.') . '</span> '
        . '</div>',
      'tcy' => array(),
      'url_count' => array(),
      'index_count' => array(),
      'last_access' => array(),
      'actions' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => t('Add site'),
          '#href' => 'admin/config/services/yandex_webmaster/add',
          '#attributes' => array(
            'class' => array('yandex_webmaster_add_site'),
          ),
        ),
      ),
    );
  }
  // Display host list.
  $form['yandex_webmaster_basic_host_id'] = array(
    '#title' => 'Host list',
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#default_value' => $basic_host,
    '#multiple' => FALSE,
    '#empty' => t('Not one host was previously added.'),
  );

  return system_settings_form($form);
}

/**
 * Yandex.Webmaster playground.
 */
function yandex_webmaster_admin_playground($form, &$form_state) {
  $form['request'] = array(
    '#type' => 'textfield',
    '#title' => t('Request URL'),
    '#field_prefix' => yandex_webmaster_domain_name() . 'api/v2/',
  );
  $form['result'] = array(
    '#type' => 'textarea',
    '#title' => t('Result'),
    '#rows' => 30,
  );
  if (isset($form_state['result'])) {
    $form['result']['#value'] = print_r($form_state['result'], 1);
  }
  else {
    $form['result']['#value'] = print_r(yandex_webmaster_http_request(yandex_webmaster_domain_name() . 'api/v2/'), 1);
  }
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send'),
  );

  return $form;
}

/**
 * Submit handler for playground form.
 */
function yandex_webmaster_admin_playground_submit($form, &$form_state) {
  $form_state['result'] = yandex_webmaster_http_request(yandex_webmaster_domain_name() . 'api/v2/' . $form_state['values']['request']);
  $form_state['rebuild'] = TRUE;
}

/**
 * Display page for adding current site to Yandex.Webmaster service.
 */
function yandex_webmaster_add_site($form, &$form_state) {
  // Verification type.
  $form['yandex_webmaster_use_metatag'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use meta-tag on the front page for verification'),
    '#disabled' => yandex_webmaster_get_host_id() != -1,
    '#default_value' => FALSE,
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add site'),
  );

  return $form;
}

/**
 * Submit handler for yandex_webmaster_add_site form.
 */
function yandex_webmaster_add_site_submit($form, &$form_state) {
  $host = yandex_webmaster_idna_convert_operation($_SERVER['HTTP_HOST'], 'encode');

  // Send POST request.
  $options = array(
    'method' => 'POST',
    'headers' => array('Authorization' => 'OAuth ' . yandex_services_auth_info()),
    'data' => '<host><name>' . $host . '</name></host>',
  );
  $result = drupal_http_request(yandex_webmaster_hosts_list_resource(), $options);
  // Host was successfully added to Yandex.Webmaster.
  if ($result->code == 201) {
    drupal_set_message(t('Site was successfully added to Yandex.Webmaster.'));
  }
  // If an error has occurred.
  else {
    preg_match('/<error>(.*?)<\/error>/', $result->data, $matches);
    $error = $matches[1];
    drupal_set_message(t('An error occurred while adding new site to Yandex.Webmaster. @error', array('@error' => $error)), 'error');
  }

  drupal_goto('admin/config/services/yandex_webmaster');
}

/**
 * Remove current site from Yandex.Webmaster service.
 */
function yandex_webmaster_remove_site($form, &$form_state) {
  $form['text'] = array(
    '#type' => 'item',
    '#markup' => t('Are you sure want to remove current site from Yandex.Webmaster service?'),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Remove'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );
  return $form;
}

/**
 * Submit procedure for removing current site from Yandex.Webmaster service.
 */
function yandex_webmaster_remove_site_submit($form, &$form_state) {
  if ($form_state['values']['op'] != t('Remove')) {
    drupal_goto('admin/config/services/yandex_webmaster');
  }

  $hostlist = yandex_webmaster_get_hosts();
  // Empty host list.
  if (!count($hostlist)) {
    drupal_set_message(t('An error occurred while sending delete request to Yandex.Webmaster service.'), 'error');
    drupal_goto('admin/config/services/yandex_webmaster');
  }
  $current_host_id = yandex_webmaster_get_host_id();
  // Send DELETE-request.
  $options = array(
    'method' => 'DELETE',
    'headers' => array('Authorization' => 'OAuth ' . yandex_services_auth_info()),
  );
  $result = drupal_http_request(yandex_webmaster_domain_name() . 'api/v2/hosts/' . $current_host_id, $options);
  if ($result->code == 204) {
    drupal_set_message(t('Site was sucessfully removed from Yandex.Webmaster service.'));
  }
  else {
    drupal_set_message(t('An error occurred while sending delete request to Yandex.Webmaster service.'), 'error');
  }
  variable_set('yandex_webmaster_basic_host_id', 0);

  drupal_goto('admin/config/services/yandex_webmaster');
  return array();
}
