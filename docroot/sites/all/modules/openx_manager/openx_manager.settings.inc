<?php
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file
 * Module settings.
 */

/**
 * Module settings.
 *
 * @ingroup form
 */
function _openx_manager_settings() {
  $image_path = file_create_path(file_directory_path() .'/openx_manager_banners');
  if (!file_check_directory($image_path, FILE_CREATE_DIRECTORY)) {
    drupal_set_message(t('It was not possible to create the %dir directory. Check the permissons.', array(
      '%dir' => $image_path)));
  }

  $form['openx_manager_server'] = array(
    '#default_value'  => variable_get('openx_manager_server', ''),
    '#title'          => t('Server'),
    '#type'           => 'textfield',
  );
  $form['openx_manager_username'] = array(
    '#default_value'  => variable_get('openx_manager_username', ''),
    '#title'          => t('Username'),
    '#type'           => 'textfield',
  );
  $form['openx_manager_password'] = array(
    '#title'          => variable_get('openx_manager_password', '') ? t('Change password') : t('Password'),
    '#type'           => 'password',
  );
  $form['openx_manager_user_integration'] = array(
    '#default_value'  => variable_get('openx_manager_user_integration', FALSE),
    '#description'    => t('If you want that users might be integrated with a OpenX user'),
    '#title'          => t('User integration'),
    '#type'           => 'checkbox',
  );

  // Campaign settings
  $form['campaign'] = array(
    '#title'          => t('Campaign'),
    '#type'           => 'fieldset',
  );
  $options = array(
    'impressions' => t('Impressions'),
    'clicks'      => t('Clicks'),
    'conversions' => t('Conversions'),
  );
  $form['campaign']['openx_manager_campaign_inventory'] = array(
    '#default_value'  => variable_get('openx_manager_campaign_inventory', drupal_map_assoc(array_keys($options))),
    '#description'    => t('What types of limitations the campaigns might use?'),
    '#options'        => $options,
    '#title'          => t('Inventory'),
    '#type'           => 'checkboxes',
  );

  // Banner settings
  $form['banner'] = array(
    '#title'          => t('Banner'),
    '#type'           => 'fieldset',
  );
  $options = array(
    'sql'   => t('OpenX server banner (stored as a SQL data)'),
    'web'   => t('OpenX server banner (stored as a file)'),
    'url2'  => t('Local banner'),
    'url'   => t('External banner'),
    'html'  => t('HTML banner'),
    'txt'   => t('Text ad'),
  );
  $form['banner']['openx_manager_banner_types'] = array(
    '#default_value'  => variable_get('openx_manager_banner_types', drupal_map_assoc(array_keys($options))),
    '#description'    => t('What banner types you allow on your server?'),
    '#options'        => $options,
    '#title'          => t('Types'),
    '#type'           => 'checkboxes',
  );
  $form['banner']['openx_manager_banner_target'] = array(
    '#default_value'  => variable_get('openx_manager_banner_target', ''),
    '#description'    => t('All banners should the same window target? Leave it blank so they might choose their own.'),
    '#title'          => t('Banner window target'),
    '#type'           => 'textfield',
  );

  $form['#validate'][] = '_openx_manager_settings_validate';
  return system_settings_form($form);
}

/**
 * Validate the settings data.
 *
 * @ingroup form
 */
function _openx_manager_settings_validate(&$form, &$form_state) {
  if (!empty($form_state['values']['openx_manager_server'])) {
    if (!valid_url($form_state['values']['openx_manager_server'], TRUE)) {
      form_set_error('openx_manager_server', 'Invalid server URL');
    }

    // Filter the server URL
    if (drupal_substr($form_state['values']['openx_manager_server'], -1, 1) == '/') {
      $url = drupal_substr($form_state['values']['openx_manager_server'], 0, -1);
      form_set_value($form['openx_manager_server'], $url, $form_state);
    }

    // Check if all data are right by calling the API
    if (!empty($form_state['values']['openx_manager_username'])
        and !empty($form_state['values']['openx_manager_password'])) {
      if (!_openx_manager_session(FALSE, $form_state['values']['openx_manager_server'],
          $form_state['values']['openx_manager_username'],
          $form_state['values']['openx_manager_password'])) {
        form_set_error('openx_manager_server', t('Some of these values are wrong.'));
      }
    }
  }
}

/**
 * Import data from the OpenX server
 */
function _openx_manager_import() {
  $form['ntype'] = array(
    '#options'  => array(
      'advertiser'  => t('Advertiser'),
      'banner'      => t('Banner'),
      'campaign'    => t('Campaign'),
      'manager'     => t('Manager'),
      'publisher'   => t('Publisher'),
//       'user'        => t('User'),
      'zone'        => t('Zone'),
    ),
    '#required' => TRUE,
    '#title'    => t('Import'),
    '#type'     => 'radios',
  );
  $form['submit'] = array(
    '#value'    => t('Import'),
    '#type'     => 'submit',
  );

  return $form;
}

/**
 * Import data from the OpenX server
 */
function _openx_manager_import_submit(&$form, &$form_state) {
  $ntype = $ntype2 = $form_state['values']['ntype'];
  $sql_id     = 'o'. drupal_substr($ntype, 0, 1) .'id';
  $sql_table  = 'openx_manager_'. $ntype;
  switch ($ntype) {
    case 'advertiser':
      $api_op     = 'getAdvertiserListByAgencyId';
      $sql_id_parrent    = 'omid';
      $sql_table_parrent = 'openx_manager_manager';
      $fields     = array(
        'title'                 => 'advertiserName',
        'info_contact'          => 'contactName',
        'info_email'            => 'emailAddress',
        'oaid'                  => 'advertiserId',
      );
      break;
    case 'banner':
      $api_op     = 'getBannerListByCampaignId';
      $sql_id_parrent    = 'ocid';
      $sql_table_parrent = 'openx_manager_campaign';
      $fields     = array(
        'title'                 => 'bannerName',
        'obid'                  => 'bannerId',
        'ocid'                  => 'campaignId',
        'btype'                 => 'storageType',
        'data_image'            => 'aImage',
        'data_url'              => 'imageURL',
        'data_text'             => 'htmlTemplate',
        'destination_url'       => 'url',
        'size_height'           => 'height',
        'size_width'            => 'width',
      );
      break;
    case 'campaign':
      $api_op     = 'getCampaignListByAdvertiserId';
      $sql_id_parrent    = 'oaid';
      $sql_table_parrent = 'openx_manager_advertiser';
      $fields     = array(
        'title'                 => 'campaignName',
        'ocid'                  => 'campaignId',
        'oaid'                  => 'advertiserId',
        'contract_activation'   => 'startDate',
        'contract_expiration'   => 'endDate',
        'inventory_impressions' => 'impressions',
        'inventory_clicks'      => 'clicks',
        'contract_expiration'   => 'priority',
        'distribution_weight'   => 'weight',
      );
      break;
    case 'manager':
      $api_op     = 'getAgencyList';
      $ntype2     = 'agency';
      $fields     = array(
        'title'                 => 'agencyName',
        'omid'                  => 'agencyId',
        'accountId'             => 'accountId',
        'contact'               => 'contactName',
        'email'                 => 'emailAddress',
      );
      break;
    case 'publisher':
      $api_op     = 'getPublisherListByAgencyId';
      $sql_id_parrent    = 'omid';
      $sql_table_parrent = 'openx_manager_manager';
      $fields     = array(
        'title'                 => 'publisherName',
        'contact'               => 'contactName',
        'email'                 => 'emailAddress',
        'opid'                  => 'publisherId',
      );
      break;
    case 'user':
      $api_op     = 'getUserListByAccountId';
      $sql_id_parrent    = 'oacid';
      $sql_table_parrent = 'openx_manager_manager';
      $fields     = array(
        'title'                 => 'userName',
        'contact'               => 'contactName',
        'email'                 => 'emailAddress',
        'ouid'                  => 'userId',
      );
      break;
    case 'zone':
      $api_op     = 'getZoneListByPublisherId';
      $sql_id_parrent    = 'opid';
      $sql_table_parrent = 'openx_manager_publisher';
      $fields     = array(
        'title'                 => 'zoneName',
        'ozid'                  => 'zoneId',
        'opid'                  => 'publisherId',
        'size_height'           => 'height',
        'size_width'            => 'width',
        'zone_type'             => 'type',
      );
      break;
  }

  // Get the existing nodes
  $existing_oids = array();
  if ($sql_existing_oids = db_query("SELECT %s AS id FROM {%s}", $sql_id, $sql_table)) {
    while ($id = db_fetch_object($sql_existing_oids)) {
      $existing_oids[$id->id] = $id->id;
    }
  }
  unset($sql_existing_oids);

  // Check if it requires a upper ID
  if (isset($sql_id_parrent)) {
    $oids = db_query("SELECT %s AS id FROM {%s}", $sql_id_parrent, $sql_table_parrent);
    while ($id = db_fetch_object($oids)) {
      if ($nodes = _openx_manager_api($api_op, (int) $id->id)) {
        _openx_manager_import_submit_save($nodes, $existing_oids, $fields, $ntype, $ntype2);
      }
    }
  }
  else {
    if ($nodes = _openx_manager_api($api_op)) {
      _openx_manager_import_submit_save($nodes, $existing_oids, $fields, $ntype, $ntype2);
    }
  }
}

/**
 * Helper function that really imports the data into a new node
 */
function _openx_manager_import_submit_save(&$nodes, &$existing_oids, &$fields, &$ntype, &$ntype2) {
  global $user;
dpm($nodes);
  foreach ($nodes as $node) {
    if (empty($existing_oids[$node[$ntype2 .'Id']])) {
      $new_node = array(
        'type'    => 'openx_'. $ntype,
        'uid'     => $user->uid,
        'name'    => $user->name,
      );
      foreach ($fields as $index => $value) {
        $new_node[$index] = $node[$value];
      }

      // Banner image must be saved
      if (!empty($new_node['data_image'])) {
        $filename = file_save_data($new_node['data_image']['content'],
          file_directory_path() . $new_node['data_image']['filename']);
        $new_node['data_image'] = db_result(db_query("SELECT fid FROM {file}
          WHERE filename = '%s'", $filename));
      }

      // Submit the value, like submiting the form, then save it
      $new_node = node_submit($new_node);
      node_save($new_node);

      // Since creating nodes like this doesnt generate any
      // message, create our own
      drupal_set_message(t('Created a new %type: !title', array(
        '!title'  => l($new_node->title, 'node/'. $new_node->nid),
        '%type'   => $new_node->type,
      )));
    }
  }
}
