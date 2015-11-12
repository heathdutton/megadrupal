<?php
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file
 * Manage all OpenX Zones.
 */

define('OX_ZONE_BANNER',        0);
define('OX_ZONE_INTERSTITIAL',  1);
define('OX_ZONE_POPUP',         2);
define('OX_ZONE_TEXT',          3);
define('OX_ZONE_EMAIL',         4);

/**
 * Implementation of hook_access().
 *
 * For Zones
 */
function _openx_node_zone_access($op, $node, $account) {
  if ($op == 'view') {
    return user_access('view openx zones', $account);
  }
  elseif ($op == 'create') {
    return user_access('create openx zones', $account);
  }
  elseif ($op == 'update' or $op == 'delete') {
    if (user_access('edit own openx zones', $account) and ($account->uid == $node->uid)) {
      return TRUE;
    }
  }
}

/**
 * Implementation of hook_delete().
 *
 * For Zones
 */
function _openx_node_zone_delete(&$node) {
  _openx_manager_api('deleteZone', (int) $node->ozid);
  db_query('DELETE FROM {openx_manager_zone} WHERE nid = %d', $node->nid);
}

/**
 * Implementation of hook_form().
 *
 * For Zones
 */
function _openx_node_zone_form(&$node, $form_state) {
  $form['title'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Zone'),
    '#required'       => TRUE,
    '#default_value'  => $node->title,
    '#weight'         => -5,
  );
  $form['ozid'] = array(
    '#value'          => $node->ozid,
    '#type'           => 'value',
  );
  if (empty($node->opid)) {
    $opid = $_GET['opid'];
    $publisher = node_load($_GET['nid']);
  }
  else {
    $opid = $node->opid;
    $publisher = node_load(db_result(db_query('SELECT nid FROM {openx_manager_publisher}
      WHERE opid = %d', $opid)));
  }
  $form['opid'] = array(
    '#value'          => $opid,
    '#type'           => 'value',
  );
  $form['publisher'] = array(
    '#value'          => l($publisher->title, 'node/'. $publisher->nid),
    '#title'          => t('Publisher'),
    '#type'           => 'item',
  );
  $form['zone_type'] = array(
    '#default_value'  => empty($node->zone_type) ? OX_ZONE_BANNER : $node->zone_type,
    '#options'        => array(
      OX_ZONE_BANNER        => t('Banner, Button or Rectangle'),
      OX_ZONE_INTERSTITIAL  => t('Interstitial or Floating DHTML'),
      OX_ZONE_POPUP         => t('Popup'),
      OX_ZONE_TEXT          => t('Text'),
      OX_ZONE_EMAIL         => t('Email or Newsletter'),
    ),
    '#type'           => 'select',
  );
  $options = array(
    '468x60'  => t('IAB Full Banner (468 x 60)'),
    '120x600' => t('IAB Skyscraper (120 x 600)'),
    '728x90'  => t('IAB Leaderboard (728 x 90)'),
    '120x90'  => t('IAB Button 1 (120 x 90)'),
    '120x60'  => t('IAB Button 2 (120 x 60)'),
    '234x60'  => t('IAB Half Banner (234 x 60)'),
    '728x90'  => t('IAB Leader Board (728 x 90) *'),
    '88x31'   => t('IAB Micro Bar (88 x 31)'),
    '125x125' => t('IAB Square Button (125 x 125)'),
    '120x240' => t('IAB Vertical Banner (120 x 240)'),
    '180x150' => t('IAB Rectangle (180 x 150) *'),
    '300x250' => t('IAB Medium Rectangle (300 x 250) *'),
    '336x280' => t('IAB Large Rectangle (336 x 280)'),
    '240x400' => t('IAB Vertical Rectangle (240 x 400)'),
    '250x250' => t('IAB Square Pop-up (250 x 250)'),
    '160x600' => t('IAB Wide Skyscraper (160 x 600) *'),
    ''        => t('Custom'),
  );
  $form['size_preset'] = array(
    '#default_value'  => empty($options[$node->size_width .'x'. $node->size_height]) ?
      '' : $node->size_width .'x'. $node->size_height ,
    '#description'    => t('Banner size, in pixels.'),
    '#options'        => $options,
    '#title'          => t('Preset Size'),
    '#type'           => 'select',
  );
  $form['size_width'] = array(
    '#default_value'  => $node->size_width,
    '#size'           => 5,
    '#title'          => t('Width'),
    '#type'           => 'textfield',
  );
  $form['size_height'] = array(
    '#default_value'  => $node->size_height,
    '#size'           => 5,
    '#title'          => t('Height'),
    '#type'           => 'textfield',
  );
  $form['#submit'][] = '_openx_node_zone_submit';
  return $form;
}

/**
 * Implementation of hook_insert().
 *
 * For Zones
 */
function _openx_node_zone_insert(&$node) {
  drupal_write_record('openx_manager_zone', $node);
}

/**
 * List all banners that might get linked to this zone
 *
 * @ingroup form
 */
function _openx_node_zone_link_banner($forn_state, $zone, $type) {
  $nid_sql = db_query("SELECT banner.nid FROM {openx_manager_banner} banner
    INNER JOIN {openx_manager_link} link ON link.obid = banner.obid
    WHERE link.ozid = %d", $zone->ozid);
  while ($nid = db_result($nid_sql)) {
    $enabled[$nid] = TRUE;
  }

  $nid_sql = pager_query("SELECT nid FROM {openx_manager_banner}
    WHERE size_height = %d AND size_width = %d", 10, 0, NULL,
    $zone->size_height, $zone->size_width);
  $form['pager_top'] = array(
    '#value' => theme('pager', NULL, 10, 0),
  );
  $form['link'] = array(
    '#tree'           => TRUE,
  );
  $form['link_default'] = array(
    '#tree'           => TRUE,
  );
  while ($nid = db_result($nid_sql)) {
    $node = node_load($nid);
    $form['link'][$node->obid] = array(
      '#default_value'  => $enabled[$nid],
      '#type'           => 'checkbox',
      '#title'          => l($node->title, 'node/'. $node->nid),
    );
    $form['link_default'][$node->obid] = array(
      '#value'          => $enabled[$nid],
      '#type'           => 'value',
    );
  }
  $form['pager_bottom'] = array(
    '#value' => theme('pager', NULL, 10, 0),
  );

  $form['ozid'] = array(
    '#type'           => 'value',
    '#value'          => $zone->ozid,
  );
  if (!empty($form['link'])) {
    $form['submit'] = array(
      '#type'           => 'submit',
      '#value'          => t('Link'),
    );
  }
  return $form;
}

/**
 * List all banners that might get linked to this zone
 *
 * @ingroup form
 */
function _openx_node_zone_link_banner_submit(&$form, &$forn_state) {
  foreach ($forn_state['values']['link'] as $obid => $link) {
    if ($link and empty($forn_state['values']['link_default'][$obid])
        and _openx_manager_api('linkBanner', (int) $forn_state['values']['ozid'], (int) $obid)) {
      $link_data = array(
        'ozid'  => $forn_state['values']['ozid'],
        'obid'  => $obid,
      );
      drupal_write_record('openx_manager_link', $link_data);
    }
    elseif (!$link and !empty($forn_state['values']['link_default'][$obid])
        and _openx_manager_api('unlinkBanner', (int) $forn_state['values']['ozid'], (int) $obid)) {
      db_query("DELETE FROM {openx_manager_link} WHERE ozid = %d AND obid = %d",
      $forn_state['values']['ozid'] , $obid);
    }
  }
}

/**
 * List all banners that might get linked to this zone
 *
 * @param $forn_state
 *   Array. Sent to all forms.
 * @param $object_id
 *   Number. Zone, campaign or banner ID, depending on the the $type.
 * @param $type
 *   String. What object is the the base: 'zone', 'banner', 'campaign'.
 * @ingroup form
 */
function _openx_node_zone_link_campaign($forn_state, $zone, $type) {
  $nid_sql = db_query("SELECT campaign.nid FROM {openx_manager_campaign} campaign
    INNER JOIN {openx_manager_link} link ON link.ocid = campaign.ocid
    WHERE link.ozid = %d", $zone->ozid);
  while ($nid = db_result($nid_sql)) {
    $enabled[$nid] = TRUE;
  }

  $nid_sql = pager_query("SELECT nid FROM {openx_manager_campaign}", 10);
  $form['pager_top'] = array(
    '#value' => theme('pager', NULL, 10, 0),
  );
  $form['link'] = array(
    '#tree'           => TRUE,
  );
  $form['link_default'] = array(
    '#tree'           => TRUE,
  );
  while ($nid = db_result($nid_sql)) {
    $node = node_load($nid);
    $form['link'][$node->ocid] = array(
      '#default_value'  => $enabled[$nid],
      '#type'           => 'checkbox',
      '#title'          => $node->title,
    );
    $form['link_default'][$node->ocid] = array(
      '#value'          => $enabled[$nid],
      '#type'           => 'value',
    );
  }
  $form['pager_bottom'] = array(
    '#value' => theme('pager', NULL, 10, 0),
  );

  $form['ozid'] = array(
    '#type'           => 'value',
    '#value'          => $zone->ozid,
  );
  if (!empty($form['link'])) {
    $form['submit'] = array(
      '#type'           => 'submit',
      '#value'          => t('Link'),
    );
  }
  return $form;
}

/**
 * List all banners that might get linked to this zone
 *
 * @ingroup form
 */
function _openx_node_zone_link_campaign_submit(&$form, &$forn_state) {
  foreach ($forn_state['values']['link'] as $ocid => $link) {
    if ($link and empty($forn_state['values']['link_default'][$ocid])
        and _openx_manager_api('linkCampaign', (int) $forn_state['values']['ozid'], (int) $ocid)) {
      $link_data = array(
        'ozid'  => $forn_state['values']['ozid'],
        'ocid'  => $ocid,
      );
      drupal_write_record('openx_manager_link', $link_data);
    }
    elseif (!$link and !empty($forn_state['values']['link_default'][$ocid])
        and _openx_manager_api('unlinkCampaign', (int) $forn_state['values']['ozid'], (int) $ocid)) {
      db_query("DELETE FROM {openx_manager_link} WHERE ozid = %d AND ocid = %d",
      $forn_state['values']['ozid'] , $obid);
    }
  }
}

/**
 * Implementation of hook_load().
 *
 * For Zones
 */
function _openx_node_zone_load(&$node) {
  $sql = db_query('SELECT * FROM {openx_manager_zone} WHERE nid = %d', $node->nid);
  while ($field = db_fetch_array($sql)) {
    foreach ($field as $key => $value) {
      $node->$key = $value;
    }
  }
}

/**
 * Unify the hook_insert and hook_update common parts.
 */
function _openx_node_zone_submit(&$form, &$form_state) {
  if ($form_state['values']['op'] != t('Preview')) {
    // Increase the code readability
    $node = &$form_state['values'];

    $data = array(
      'zoneName'    => $node['title'],
      'publisherId' => (int) $node['opid'],
      'type'        => (int) $node['zone_type'],
    );
    if (!empty($node['size_width']) and !empty($node['size_height'])) {
      $data['width']    = (int) $node['size_width'];
      $data['height']   = (int) $node['size_height'];
    }

    // Update or insert
    if (empty($node['nid'])) {
      $node['ozid'] = _openx_manager_api('addZone', $data);
    }
    else {
      $data['zoneId'] = (int) $node['ozid'];
      _openx_manager_api('modifyZone', $data);
    }
  }
}

/**
 * Implementation of hook_update().
 *
 * For Zones
 */
function _openx_node_zone_update(&$node) {
  drupal_write_record('openx_manager_zone', $node, 'nid');
}

/**
 * Implementation of hook_validate().
 *
 * For Zones
 */
function _openx_node_zone_validate(&$node) {
  if (!variable_get('openx_manager_server', '')) {
    form_set_error('title', t('OpenX integration not configured'));
  }

  // Check if the integer values are really integers
  $integers_values = array(
    'size_width',
    'size_height',
  );
  foreach ($integers_values as $value) {
    if (!empty($node->{$value}) and !is_numeric($node->{$value})) {
      form_set_error($value, t('It must be numeric'));
    }
  }
}

/**
 * Since hook_validate cannot change the value of node's fields,
 * a nodeapi implementation should be created too.
 *
 * @ingroup form
 */
function _openx_node_zone_validate2(&$form, &$form_state) {
  if (!empty($form_state['values']['size_preset'])) {
    $size = explode('x', $form_state['values']['size_preset']);
    form_set_value($form['size_width'], $size[0], $form_state);
    form_set_value($form['size_height'], $size[1], $form_state);
  }
}

/**
 * Implementation of hook_load().
 *
 * For Zones
 */
function _openx_node_zone_view(&$node, $teaser = FALSE, $page = FALSE) {
  $node->content['openx_zone'] = array(
    '#value'  => theme('openx_zone', $node),
    '#weight' => 1,
  );

  return $node;
}

/**
 * Display the Zone fields
 *
 * @ingroup themable
 */
function theme_openx_zone(&$node) {
  $publisher = node_load(db_result(db_query('SELECT nid FROM {openx_manager_publisher}
    WHERE opid = %d', $node->opid)));
  $fields['publisher'] = array(
    '#value'          => l($publisher->title, 'node/'. $publisher->nid),
    '#title'          => t('Publisher'),
    '#type'           => 'item',
  );
  $options = array(
    OX_ZONE_BANNER        => t('Banner, Button or Rectangle'),
    OX_ZONE_INTERSTITIAL  => t('Interstitial or Floating DHTML'),
    OX_ZONE_POPUP         => t('Popup'),
    OX_ZONE_TEXT          => t('Text'),
    OX_ZONE_EMAIL         => t('Email or Newsletter'),
  );
  $fields['zone_type'] = array(
    '#value'          => $options[$node->zone_type],
    '#title'          => t('Type'),
    '#type'           => 'item',
  );
  $options = array(
    '468x60'  => t('IAB Full Banner (468 x 60)'),
    '120x600' => t('IAB Skyscraper (120 x 600)'),
    '728x90'  => t('IAB Leaderboard (728 x 90)'),
    '120x90'  => t('IAB Button 1 (120 x 90)'),
    '120x60'  => t('IAB Button 2 (120 x 60)'),
    '234x60'  => t('IAB Half Banner (234 x 60)'),
    '728x90'  => t('IAB Leader Board (728 x 90)'),
    '88x31'   => t('IAB Micro Bar (88 x 31)'),
    '125x125' => t('IAB Square Button (125 x 125)'),
    '120x240' => t('IAB Vertical Banner (120 x 240)'),
    '180x150' => t('IAB Rectangle (180 x 150)'),
    '300x250' => t('IAB Medium Rectangle (300 x 250)'),
    '336x280' => t('IAB Large Rectangle (336 x 280)'),
    '240x400' => t('IAB Vertical Rectangle (240 x 400)'),
    '250x250' => t('IAB Square Pop-up (250 x 250)'),
    '160x600' => t('IAB Wide Skyscraper (160 x 600)'),
    ''        => t('Custom (!width x !height)', array(
      '!width'  => $node->size_width,
      '!height' => $node->size_height
    )),
  );
  $fields['size'] = array(
    '#value'          => empty($options[$node->size_width .'x'. $node->size_height]) ?
      $options[''] : $options[$node->size_width .'x'. $node->size_height],
    '#title'          => t('Size'),
    '#type'           => 'item',
  );

  // Print all field in the right order
  $output = '';
  foreach ($fields as $field) {
    $output .= drupal_render($field);
  }
  return $output;
}

/**
 * List all banners that might get linked to this zone
 *
 * @ingroup themable
 */
function theme__openx_node_zone_link_banner(&$form) {
  return drupal_render($form);
}
