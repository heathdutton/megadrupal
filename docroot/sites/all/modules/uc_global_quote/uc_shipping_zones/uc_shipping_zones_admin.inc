<?php

/**
 * @file
 * Administration pages for uc_shipping_zones
 */

/**
 * Theme zones admin form
 */
function theme_uc_shipping_zones_admin($form) {
  $output = '';
  $header = array(t('Delete'), t('Name'), t('Countries'), t('Regions'), '');
  $result = db_query("SELECT * FROM {uc_shipping_zones} ORDER BY name")->fetchAll();

  $rows = array();
  foreach ($result as $r) {
    $row = array();
    $zid = $r->zid;
    $row[] = drupal_render($form['form']['zones'][$zid]['delete']);
    $row[] = $r->name;
    $row[] = str_replace('|', ' ', $r->countries);
    if ($r->regions && count($r->regions)) {
      $row[] = str_replace('|', ' ', $r->regions);
    }
    else {
      $row[] = 'All';
    }
    $row[] = l('edit', 'admin/store/settings/quotes/methods/zones/' . $zid);
    $rows[] = $row;
  }

  if (count($rows) == 0) {
    $rows[] = array(array(
        'data' => t('No zones found'),
        'colspan' => 4,
      ));
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows));

  $output .= drupal_render_children($form['form']);
  return $output;
}

/*
 * uc_shipping_zones admin form
 */
function uc_shipping_zones_admin($form, $form_state = array(), $id = 0) {


  $form = array();
  $form['zones'] = array(
    '#tree' => TRUE,
  );

  $field = 'Add a new zone';
  $edit = new stdClass();
  $edit->zid = NULL;
  $edit->name = NULL;
  $edit->id = NULL;
  $edit->country = NULL;
  $edit->regions = NULL;
  $edit->countries = array();

  $zquery = db_query("SELECT * FROM {uc_shipping_zones}")->fetchAll();
  foreach ($zquery as $zone) {
    $zid = $zone->zid;
    $form['zones'][$zid]['delete'] = array(
      '#type' => 'checkbox',
      '#default_value' => 0,
    );
    // Edit zone
    if (is_numeric($id) && $id == $zid) {
      $field = "Edit Zone";
      $edit = $zone;
      $edit->countries = explode('|', $zone->countries);
      $edit->regions   = explode('|', $zone->regions);
    }
  }

  $countries = array();
  foreach (uc_get_country_data() as $c) {
    $countries[$c['country_name'] . ',' . $c['country_id']] = $c['country_name'];
  }

  if (isset($form_state['storage']['zone']['country']) ) {
    $edit->countries = $form_state['storage']['zone']['country'];
    unset($form_state['storage']['zone']['country']);
  }

  $form['zone'] = array(
    '#type' => 'fieldset',
    '#title' => t($field),
    '#description' => t('Shipping zone options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<div id="shipping-zone-wrapper">', // This is our wrapper div.
    '#suffix' => '</div>',
    '#tree' => TRUE, // Don't forget to set #tree!
  );

  $form['zone']['zid'] = array(
    '#type' => 'hidden',
    '#value' => $edit->zid,
  );

  $form['zone']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Zone name'),
    '#default_value' => '',
    '#default_value' => $edit->name,
    '#size' => 40,
    '#description' => t('Name for the zone'),
  );

  $form['zone']['country'] = array(
    '#type' => 'select',
    '#title' => t('Countries'),
    '#options' => $countries,
    '#default_value' => $edit->countries,
    '#multiple' => TRUE,
    '#size' => 10,
    '#description' => t('Select all the countries of the zone.') . l('Add countries', 'admin/store/settings/countries/edit'),
    '#ajax' => array(
      'callback' => 'uc_shipping_zones_regions_ajax',
      'wrapper' => 'dropdown-region-replace',
      'method' => 'replace',
    ),
  );
  // Regions
  if ($form_state && array_key_exists('values', $form_state) && $form_state['values']['zone'] && $form_state['values']['zone']['country']) {
    $edit->countries = $form_state['values']['zone']['country'];
  }

  $form['zone']['region'] = array(
    '#type' => 'select',
    '#title' => t('Region'),
    '#options' => uc_shipping_zones_get_regions($edit->countries),
    '#default_value' => $edit->regions,
    '#multiple' => TRUE,
    '#description' => t('Select regions.'),
    '#prefix' => '<div id="dropdown-region-replace">',
    '#suffix' => '</div>',
    '#size' => 8,
  );
  if ($edit->zid) {
    $form['zone']['cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
    );
  }
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit Changes'),
  );
  $form['#redirect'] = 'admin/store/settings/quotes/methods/zones';

  return $form;
}

function uc_shipping_zones_regions_ajax($form, $form_state) {
  return $form['zone']['region'];
}

function uc_shipping_zones_admin_validate($form, &$form_state) {

  // Cancel edit
  if ($form_state['input']['op'] == t('Cancel')) {
    $form_state['redirect'] = 'admin/store/settings/quotes/methods/zones';
    return;
  }

  $cerror = NULL;
  $rerror = NULL;
  $values = $form_state['values'];

  if (!isset($values['zones']) || !is_array($values['zones']) || !array_search( array('delete' => 1), $values['zones'])) {
    if (!$values['zone']['name']) {
      form_set_error('name',   t('Enter a name for the zone'));
    }
    if (!is_array($values['zone']['country']) || count($values['zone']['country']) < 1) {
      form_set_error('country',   t('You must select a country for your zone'));
    }
    else {
      // Check overlapping zones.
      $zones = uc_shipping_zones_get();
      $cerror = array();
      $rerror = array();
        
      if (isset($values['zone']['region'][''])) $values['zone']['region'] = NULL;
    
      foreach ($zones as $zone) {
         
         // Not the same outdated zone
         if (!$values['zone']['zid'] || $values['zone']['zid'] != $zone->zid) {
                 
             $check_regions = FALSE;
             
             // Check for overlapping countries
             foreach($values['zone']['country'] as $vc) {
                 // Country defined in another zone
                 if (in_array($vc, explode('|', $zone->countries))){
                     // all regions
                     if ($values['zone']['region'] == NULL || $zone->regions == NULL) {
                         $cerror[] = $vc;
                     }
                     // Defined regions
                     else {
                         $check_regions = TRUE;
                     }
                 }
             }
             // Check for region overlapping
             if ($check_regions) {
                foreach($values['zone']['region'] as $vr){
                    if (in_array($vr, explode('|', $zone->regions))) {
                        $rerror[] = $vr;
                    }
                }
             }
         } 
      }

      if (count($cerror)) {
        form_set_error('country',   t('The following countries are selected in other zones: %list', array('%list' => implode(', ', $cerror))));
      }
      if (count($rerror)) {
        form_set_error('region',   t('The following regions are selected in other zones: %list', array('%list' => implode(', ', $rerror))));
      }
    }
  }
}
/**
 * Returns a region array ready for a select box
 *
 * @param $selection array of country name,ids strings
 *
 * @return an array ready to feed a select box options
 *
 */
function uc_shipping_zones_get_regions($selection) {

  if (!count($selection)) {
    return array('' => 'All');
  }

  $countries = array();

  foreach ($selection as $country) {
    $cid = explode(',', $country);
    $countries[] = $cid[1];
  }

  $result = db_select('uc_zones', 'z')
            ->condition('zone_country_id', $countries, 'IN')
            ->fields('z', array('zone_id', 'zone_name'))
            ->orderBy('zone_name')
            ->execute();

  $options[''] = t('All');
  foreach ($result as $zone) {
    $options[$zone->zone_name . ',' . $zone->zone_id] = $zone->zone_name;
  }
  return $options;
}

/**
 * Admin form submit
 */
function uc_shipping_zones_admin_submit($form, &$form_state) {
  if ($form_state['input']['op'] == t('Cancel')) {
    return;
  }

  // check for and handle any deletions
  if (isset($form_state['values']['zones']) && is_array($form_state['values']['zones'])) {
    foreach ($form_state['values']['zones'] as $zid => $value) {
      if ($value['delete']) {
        db_delete("uc_shipping_zones")->condition('zid', $zid)->execute();
      }
    }
  }

  if ($form_state['values']['zone']['name'] && is_array($form_state['values']['zone']['country']) && count($form_state['values']['zone']['country'])) {
    $countries = implode('|', $form_state['values']['zone']['country']);
    $regions = implode('|', $form_state['values']['zone']['region']);

    $fields = array(
      'name' => check_plain($form_state['values']['zone']['name']),
      'countries' => $countries,
      'regions' => $regions,
    );

    if ($form_state['values']['zone']['zid']) {
      db_update('uc_shipping_zones')->fields($fields)->condition('zid', $form_state['values']['zone']['zid'], '=')->execute();
      $form_state['redirect'] = 'admin/store/settings/quotes/methods/zones';
    }
    else {
      db_insert('uc_shipping_zones')->fields($fields)->execute();
    }
  }

}
