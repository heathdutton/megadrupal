<?php
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file
 * Manage all OpenX Campaigns.
 */

// OpenX constants (on constants.php)
define('MAX_FINANCE_CPM',    1);
define('MAX_FINANCE_CPC',    2);
define('MAX_FINANCE_CPA',    3);
define('MAX_FINANCE_MT',     4); // Monthly Tennancy

/**
 * Implementation of hook_access().
 *
 * For Campaigns
 */
function _openx_node_campaign_access($op, $node, $account) {
  if ($op == 'view') {
    return user_access('view openx campaigns', $account);
  }
  elseif ($op == 'create') {
    return user_access('create openx campaigns', $account);
  }
  elseif ($op == 'update' or $op == 'delete') {
    if (user_access('edit own openx campaigns', $account) and ($account->uid == $node->uid)) {
      return TRUE;
    }
  }
}

/**
 * Implementation of hook_delete().
 *
 * For Campaigns
 */
function _openx_node_campaign_delete(&$node) {
  _openx_manager_api('deleteCampaign', (int) $node->ocid);
  db_query('DELETE FROM {openx_manager_campaign} WHERE nid = %d', $node->nid);
}

/**
 * Implementation of hook_form().
 *
 * For Campaigns
 */
function _openx_node_campaign_form(&$node, $form_state) {
  $form['title'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Campaign'),
    '#required'       => TRUE,
    '#default_value'  => $node->title,
    '#weight'         => -5,
  );
  // Internal values
  $form['ocid'] = array(
    '#value'          => $node->ocid,
    '#type'           => 'value',
  );
  if (empty($node->oaid)) {
    $oaid = $_GET['oaid'];
    $advertiser = node_load($_GET['nid']);
  }
  else {
    $oaid = $node->oaid;
    $advertiser = node_load(db_result(db_query('SELECT nid FROM {openx_manager_advertiser}
      WHERE oaid = %d', $oaid)));
  }
  $form['oaid'] = array(
    '#value'          => $oaid,
    '#type'           => 'value',
  );
  $form['advertiser'] = array(
    '#value'          => l($advertiser->title, 'node/'. $advertiser->nid),
    '#title'          => t('Advertiser'),
    '#type'           => 'item',
  );

  // Inventory
  $inventory = variable_get('openx_manager_campaign_inventory', array('impressions' => 'impressions'));
  $form['inventory'] = array(
    '#title'          => t('Inventory'),
    '#type'           => 'fieldset',
  );
  if ($inventory['impressions']) {
    $form['inventory']['inventory_impressions'] = array(
      '#default_value'  => empty($node->inventory_impressions) ? '' : $node->inventory_impressions,
      '#description'    => t('Limit the campaign by number of impressions. Leave the values blank for unlimited.'),
      '#title'          => t('Impressions'),
      '#type'           => 'textfield',
      '#size'           => 10
    );
  }
  if ($inventory['clicks']) {
    $form['inventory']['inventory_clicks'] = array(
      '#default_value'  => empty($node->inventory_clicks) ? '' : $node->inventory_clicks,
      '#description'    => t('Limit the campaign by number of clicks. Leave the values blank for unlimited.'),
      '#title'          => t('Clicks'),
      '#type'           => 'textfield',
      '#size'           => 10
    );
  }
  if ($inventory['conversions']) {
    $form['inventory']['inventory_conversions'] = array(
      '#default_value'  => empty($node->inventory_conversions) ? '' : $node->inventory_conversions,
      '#description'    => t('Limit the campaign by number of conversions. Leave the values blank for unlimited.'),
      '#title'          => t('Conversions'),
      '#type'           => 'textfield',
      '#size'           => 10
    );
  }
  $form['contract'] = array(
    '#title'          => t('Contract'),
    '#type'           => 'fieldset',
  );
  $contract_activation = $node->contract_activation;
  if (is_numeric($node->contract_activation)) {
    $node->contract_activation = array(
      'day'   => date('j', $node->contract_activation),
      'month' => date('n', $node->contract_activation),
      'year'  => date('Y', $node->contract_activation),
    );
  }
  $form['contract']['contract_activation'] = array(
    '#default_value'  => $node->contract_activation,
    '#title'          => t('Activation date'),
    '#type'           => 'date',
  );
  $form['contract']['contract_activation_unlimited'] = array(
    '#default_value'  => !$contract_activation,
    '#title'          => t('Activate this campaign immediately'),
    '#type'           => 'checkbox',
  );
  $contract_expiration = $node->contract_expiration;
  if (is_numeric($node->contract_expiration)) {
    $node->contract_expiration = array(
      'day'   => date('j', $node->contract_expiration),
      'month' => date('n', $node->contract_expiration),
      'year'  => date('Y', $node->contract_expiration),
    );
  }
  $form['contract']['contract_expiration'] = array(
    '#default_value'  => $node->contract_expiration,
    '#title'          => t('Expiration date'),
    '#type'           => 'date',
  );
  $form['contract']['contract_expiration_unlimited'] = array(
    '#default_value'  => !$contract_expiration,
    '#title'          => t("Don't expire this campaign on a specific date"),
    '#type'           => 'checkbox',
  );
  $form['contract']['contract_revenue'] = array(
    '#default_value'  => $node->contract_revenue,
    '#title'          => t('Revenue Information'),
    '#type'           => 'textfield',
    '#size'           => 10
  );
  $form['contract']['contract_revenue_unit'] = array(
    '#default_value'  => empty($node->contract_revenue_unit) ? MAX_FINANCE_CPM : $node->contract_revenue_unit,
    '#options'        => array(
      MAX_FINANCE_CPM   => t('CPM'),
      MAX_FINANCE_CPC   => t('CPC'),
      MAX_FINANCE_CPA   => t('CPA'),
      MAX_FINANCE_MT    => t('Monthly Tenancy'),
    ),
    '#type'           => 'select',
  );
  if (user_access('create openx campaigns - priority')) {
    $form['priority'] = array(
      '#title'          => t('Priority'),
      '#type'           => 'fieldset',
    );
    $options = array();
    $options[-1] = t('Exclusive (Overrides other linked campaigns)');
    for ($index = 10; $index > 0; $index--) {
      $options[$index] = t('High (Paid campaigns level !level)', array('!level' => $index));
    }
    $options[0] = t('Low (House and unpaid campaigns)');
    $form['priority']['priority_level_level'] = array(
      '#default_value'  => $node->priority_level_level,
      '#options'        => $options,
      '#title'          => t('Priority Level'),
      '#type'           => 'select',
    );
    $form['priority']['distribution'] = array(
      '#title'          => t('Distribution'),
      '#type'           => 'fieldset',
    );
    $form['priority']['distribution']['priority_distribution_level'] = array(
      '#default_value'  => $node->priority_distribution_level,
      '#options'        => array(
        t('Automatic - Distribute the remaining inventory evenly over the remaining number of days'),
        t('Manual'),
        t('None - Set the campaign weight to'),
      ),
      '#type'           => 'radios',
    );
    $form['priority']['misc'] = array(
      '#title'          => t('Miscellaneous'),
      '#type'           => 'fieldset',
    );
    $form['priority']['misc']['priority_misc_hide'] = array(
      '#default_value'  => $node->priority_misc_hide,
      '#title'          => t('Hide the advertiser and websites of this campaign'),
      '#type'           => 'checkbox',
    );
    $form['priority']['misc']['priority_misc_positioning'] = array(
      '#default_value'  => $node->priority_misc_positioning,
      '#title'          => t('Companion positioning'),
      '#type'           => 'checkbox',
    );
  }
  else {
    $fields = array('priority_level_level', 'priority_distribution_level',
      'priority_misc_hide', 'priority_misc_positioning');
    foreach ($fields as $index) {
      $form[$index] = array(
        '#value' => $node->{$index}
      );
    }
  }
  $form['delivery'] = array(
    '#title'          => t('Delivery capping per visitor'),
    '#type'           => 'fieldset',
  );
  $form['delivery']['delivery_limit_total'] = array(
    '#default_value'  => $node->delivery_limit_total,
    '#title'          => t('Total limit'),
    '#type'           => 'textfield',
    '#size'           => 10
  );
  $form['delivery']['delivery_limit_session'] = array(
    '#default_value'  => $node->delivery_limit_session,
    '#title'          => t('Per session'),
    '#type'           => 'textfield',
    '#size'           => 10
  );
  $form['delivery']['delivery_reset_view'] = array(
    '#default_value'  => $node->delivery_reset_view,
    '#title'          => t('Reset view counters after'),
    '#type'           => 'date',
  );

  $form['#submit'][] = '_openx_node_campaign_submit';
  return $form;
}

/**
 * Implementation of hook_insert().
 *
 * For Campaigns
 */
function _openx_node_campaign_insert(&$node) {
  drupal_write_record('openx_manager_campaign', $node);
}

/**
 * Implementation of hook_load().
 *
 * For Campaigns
 */
function _openx_node_campaign_load(&$node) {
  $sql = db_query('SELECT * FROM {openx_manager_campaign} WHERE nid = %d', $node->nid);
  while ($field = db_fetch_array($sql)) {
    foreach ($field as $key => $value) {
      $node->$key = $value;
    }
  }
}

/**
 * Unify the hook_insert and hook_update common parts.
 */
function _openx_node_campaign_submit(&$form, &$form_state) {
  if ($form_state['values']['op'] != t('Preview')) {
    $node = &$form_state['values'];


    // Required and optional values on OpenX server
    $data = array(
      'campaignName'  => $node['title'],
      'advertiserId'  => (int) $node['oaid'],
    );

    // Deal XML RPC dates
    include_once './includes/xmlrpc.inc';
    if (empty($node['contract_activation_unlimited'])) {
      $node['contract_activation'] = mktime(0, 0, 0, $node['contract_activation']['month'],
        $node['contract_activation']['day'], $node['contract_activation']['year']);
      $data['startDate'] = xmlrpc_date($node['contract_activation']);
    }
    else {
      $node['contract_activation'] = 0;
      $data['startDate'] = xmlrpc_date('00000000T00:00:00');
    }
    if (empty($node['contract_expiration_unlimited'])) {
      $node['contract_expiration'] = mktime(0, 0, 0, $node['contract_expiration']['month'],
        $node['contract_expiration']['day'], $node['contract_expiration']['year']);
      $data['endDate'] = xmlrpc_date($node['contract_expiration']);
    }
    else {
      $node['contract_expiration'] = 0;
      $data['endDate'] = xmlrpc_date('00000000T00:00:00');
    }

    $data['impressions']  = (int) $node['inventory_impressions'];
    $data['clicks']       = (int) $node['inventory_clicks'];
    $data['priority']     = (int) $node['contract_expiration'];
    $data['weight']       = (int) $node['distribution_weight'];

    // Update or insert
    if (empty($node['nid'])) {
      $node['ocid'] = _openx_manager_api('addCampaign', $data);
    }
    else {
      $data['campaignId'] = (int) $node['ocid'];
      _openx_manager_api('modifyCampaign', $data);
    }
  }
}

/**
 * Implementation of hook_update().
 *
 * For Campaigns
 */
function _openx_node_campaign_update(&$node) {
  drupal_write_record('openx_manager_campaign', $node, 'nid');
}

/**
 * Implementation of hook_form().
 *
 * For Campaigns
 */
function _openx_node_campaign_validate(&$node) {
  if (!variable_get('openx_manager_server', '')) {
    form_set_error('title', t('OpenX integration not configured'));
  }

  // Check if the integer values are really integers
  $integers_values = array(
    'inventory_impressions',
    'inventory_clicks',
    'inventory_conversions',
    'contract_revenue',
    'priority_level_level',
    'priority_distribution_level',
    'priority_misc_hide',
    'priority_misc_positioning',
    'delivery_limit_total',
    'delivery_limit_session',
  );
  foreach ($integers_values as $value) {
    if (!empty($node->{$value}) and !is_numeric($node->{$value})) {
      form_set_error($value, t('It must be numeric'));
    }
  }
}

/**
 * Implementation of hook_load().
 *
 * For Campaigns
 */
function _openx_node_campaign_view(&$node, $teaser = FALSE, $page = FALSE) {
  $node->advertiser = node_load(db_result(db_query('SELECT nid FROM {openx_manager_advertiser}
    WHERE oaid = %d', $node->oaid)));
  $node->content['openx_campaign'] = array(
    '#value'  => theme('openx_campaign', $node),
    '#weight' => 1,
  );

  return $node;
}

/**
 * Display the Campaign fields
 */
function theme_openx_campaign(&$node) {
  $fields['advertiser'] = array(
    '#value'          => l($node->advertiser->title, 'node/'. $node->advertiser->nid),
    '#title'          => t('Advertiser'),
    '#type'           => 'item',
  );
  $fields['inventory'] = array(
    '#title'          => t('Inventory'),
    '#type'           => 'fieldset',
    '#weight'         => 1,
  );
  $inventory = variable_get('openx_manager_campaign_inventory', array('impressions' => 'impressions'));
  if ($inventory['impressions']) {
    $fields['inventory']['inventory_impressions'] = array(
      '#value'          => empty($node->inventory_impressions) ? t('unlimited') : $node->inventory_impressions,
      '#title'          => t('Impressions'),
      '#type'           => 'item',
    );
  }
  if ($inventory['clicks']) {
    $fields['inventory']['inventory_clicks'] = array(
      '#value'          => empty($node->inventory_clicks) ? t('unlimited') : $node->inventory_clicks,
      '#title'          => t('Clicks'),
      '#type'           => 'item',
    );
  }
  if ($inventory['conversions']) {
    $fields['inventory']['inventory_conversions'] = array(
      '#value'          => empty($node->inventory_conversions) ? t('unlimited') : $node->inventory_conversions,
      '#title'          => t('Conversions'),
      '#type'           => 'item',
    );
  }
  $fields['contract'] = array(
    '#title'          => t('Contract'),
    '#type'           => 'fieldset',
    '#weight'         => 2,
  );
  if ($node->contract_activation) {
    $date = format_date($node->contract_activation, 'small');
  }
  else {
    $date = t('Activate this campaign immediately');
  }
  $fields['contract']['contract_activation'] = array(
    '#value'          => $date,
    '#title'          => t('Activation date'),
    '#type'           => 'item',
  );
  if ($node->contract_activation) {
    $date = format_date($node->contract_expiration, 'small');
  }
  else {
    $date = t("Don't expire this campaign on a specific date");
  }
  $fields['contract']['contract_expiration'] = array(
    '#value'          => $date,
    '#title'          => t('Expiration date'),
    '#type'           => 'item',
  );
  $options = array(
    MAX_FINANCE_CPM   => t('CPM'),
    MAX_FINANCE_CPC   => t('CPC'),
    MAX_FINANCE_CPA   => t('CPA'),
    MAX_FINANCE_MT    => t('Monthly Tenancy'),
  );
  $fields['contract']['contract_revenue'] = array(
    '#value'          => check_plain($node->contract_revenue) .' '. $options[$node->contract_revenue_unit],
    '#title'          => t('Revenue Information'),
    '#type'           => 'item',
  );
  $fields['priority'] = array(
    '#title'          => t('Priority'),
    '#type'           => 'fieldset',
    '#weight'         => 3,
  );
  if (user_access('create openx campaigns - priority')) {
    $options = array();
    $options[-1] = t('Exclusive (Overrides other linked campaigns)');
    for ($index = 10; $index > 0; $index--) {
      $options[$index] = t('High (Paid campaigns level !level)', array('!level' => $index));
    }
    $options[0] = t('Low (House and unpaid campaigns)');
    $fields['priority']['priority_level_level'] = array(
      '#value'          => $options[$node->priority_level_level],
      '#title'          => t('Priority'),
      '#type'           => 'item',
    );
    $options = array(
      t('Automatic - Distribute the remaining inventory evenly over the remaining number of days'),
      t('Manual'),
      t('None - Set the campaign weight to'),
    );
    $fields['priority']['priority_distribution_level'] = array(
      '#value'          => $options[$node->priority_distribution_level],
      '#title'          => t('Distribution'),
      '#type'           => 'item',
    );
    $options = array(t('No'), t('Yes'));
    $fields['priority']['priority_misc_hide'] = array(
      '#value'          => $options[$node->priority_misc_hide],
      '#title'          => t('Hide the advertiser and websites of this campaign'),
      '#type'           => 'item',
    );
    $fields['priority']['priority_misc_positioning'] = array(
      '#value'          => $options[$node->priority_misc_positioning],
      '#title'          => t('Companion positioning'),
      '#type'           => 'item',
    );
  }
  $fields['delivery'] = array(
    '#title'          => t('Delivery capping per visitor'),
    '#type'           => 'fieldset',
    '#weight'         => 4,
  );
  $fields['delivery']['delivery_limit_total'] = array(
    '#value'          => empty($node->delivery_limit_total) ? t('unlimited') : $node->delivery_limit_total,
    '#title'          => t('Total limit'),
    '#type'           => 'item',
  );
  $fields['delivery']['delivery_limit_session'] = array(
    '#value'          => empty($node->delivery_limit_session) ? t('unlimited') : $node->delivery_limit_session,
    '#title'          => t('Per session'),
    '#type'           => 'item',
  );
  $fields['delivery']['delivery_reset_view'] = array(
    '#value'          => empty($node->delivery_reset_view) ? t('never') : $node->delivery_reset_view,
    '#title'          => t('Reset view counters after'),
    '#type'           => 'item',
  );

  // Banners list
  $fields['banners'] = array(
    '#collapsible'    => TRUE,
    '#title'          => t('Banners'),
    '#type'           => 'fieldset',
    '#weight'         => 10,
  );
  $view = views_get_view('openx_manager_banners');
  $fields['banners'][] = array(
    '#value'          => $view->execute_display(NULL, array($node->ocid)),
    '#type'           => 'item',
  );

  // Print all field in the right order
  $output = '';
  foreach ($fields as $field) {
    $output .= drupal_render($field);
  }
  return $output;
}
