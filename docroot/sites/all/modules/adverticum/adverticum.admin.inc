<?php

/**
 * @file
 * Administration form.
 */

/**
 * Module configuration form.
 */
function adverticum_admin_settings($form, &$form_state) {
  $form['adverticum_tag_global'] = array(
    '#type' => 'radios',
    '#title' => t('Global tag setting'),
    '#description' => t('If set, the tag setting for a zone will be ignored and all zones will be displayed using this tag.'),
    '#attributes' => array('class' => array('tag-global')),
    '#default_value' => variable_get('adverticum_tag_global', 'none'),
    '#options' => array(
      'none' => t('None'),
      'goa3' => t('Goa3'),
      'js' => t('JavaScript')
    ),
    '#weight' => -1,
  );

  $form['blocks'] = array(
    '#title' => t('Adverticum blocks'),
    '#type' => 'fieldset',
    '#description' => t('The zone ID can be found in the Adverticum administration interface. 
      The name can be any alpha-numeric string, this will be used for the block name.
      Note that some type of banner appereances (Layer) are supported by the Goa3 tag only.'),
    '#prefix' => '<div id="zones-wrapper">',
    '#suffix' => '</div>'
  );

  $form['blocks']['adverticum_zones'] = array(
    '#tree' => TRUE,
    '#theme' => 'adverticum_admin_settings_zones',
    '#element_validate' => array('adverticum_admin_settings_zones_validate'),
  );

  if (empty($form_state['zones'])) {
    $zones = variable_get('adverticum_zones', array());
    if (empty($zones)) {
      $form_state['zones'] = array_fill(0, 3, array('id' => '', 'name' => '', 'tag' => '', 'active' => ''));
    }
    else {
      $form_state['zones'] = $zones;
    }
  }

  foreach ($form_state['zones'] as $id => $zone) {
    $form['blocks']['adverticum_zones'][$id]['id'] = array(
      '#type' => 'textfield',
      '#default_value' => isset($form_state['zones'][$id]['id']) ? $form_state['zones'][$id]['id'] : '',
      '#size' => 7,
      '#attributes' => array('class' => array('zone-id')),
    );
    $form['blocks']['adverticum_zones'][$id]['name'] = array(
      '#type' => 'textfield',
      '#default_value' => isset($form_state['zones'][$id]['name']) ? $form_state['zones'][$id]['name'] : '',
      '#size' => 64,
      '#attributes' => array('class' => array('zone-name')),
    );
    $form['blocks']['adverticum_zones'][$id]['tag'] = array(
      '#type' => 'radios',
      '#default_value' => isset($form_state['zones'][$id]['tag']) ? $form_state['zones'][$id]['tag'] : '',
      '#options' => array(
        'goa3' => t('Goa3'),
        'js' => t('JavaScript'),
      ),
    );
    $form['blocks']['adverticum_zones'][$id]['active'] = array(
      '#type' => 'checkbox',
      '#default_value' => isset($form_state['zones'][$id]['active']) ? $form_state['zones'][$id]['active'] : '',
    );
    $form['blocks']['adverticum_zones'][$id]['delete'] = array(
      '#type' => 'submit',
      '#value' => t('delete'),
      '#submit' => array('adverticum_admin_settings_del_zone'),
      '#name' => 'adverticum-zones-delete-' . $id,
      '#key' => $id,
      '#disabled' => (count($form_state['zones']) == 1),
    );
    if (!empty($zone['id'])) {
      $form['blocks']['adverticum_zones'][$id]['configure'] = array(
        '#type' => 'submit',
        '#value' => t('configure block'),
        '#submit' => array('adverticum_admin_settings_config_block'),
        '#name' => 'adverticum-zones-configure-' . $id,
        '#key' => $id,
      );
    }
  }

  $form['blocks']['add_zone'] = array(
    '#type' => 'submit',
    '#value' => t('Add row'),
    '#submit' => array('adverticum_admin_settings_add_zone'),
    '#ajax' => array(
      'callback' => 'adverticum_admin_settings_add_zone_callback',
      'wrapper' => 'zones-wrapper'
    ),
  );

  $form['#submit'] = array('adverticum_admin_settings_submit');
  return system_settings_form($form);
}

/**
 * Handles form submit.
 */
function adverticum_admin_settings_submit(&$form, &$form_state) {
  unset($form_state['storage']);
}

/**
 * Handles form submit with the 'Add row' button.
 */
function adverticum_admin_settings_add_zone($form, &$form_state) {
  $form_state['zones'][] = array('id' => '', 'name' => '', 'tag' => '', 'active' => '');
  $form_state['rebuild'] = TRUE;
}

/**
 * Callback function for the 'Add row' button.
 */
function adverticum_admin_settings_add_zone_callback($form, &$form_state) {
  return $form['blocks'];
}

/**
 * Handles form submit with the 'Delete' button.
 */
function adverticum_admin_settings_del_zone($form, &$form_state) {
  unset($form_state['zones'][$form_state['clicked_button']['#key']]);
  if (count($form_state['zones']) < 3) {
    $form_state['zones'][] = array('id' => '', 'name' => '', 'tag' => '', 'active' => '');
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Handles form submit with the 'Configure block' button.
 */
function adverticum_admin_settings_config_block($form, &$form_state) {
  drupal_goto('admin/structure/block/manage/adverticum/'. $form_state['clicked_button']['#key']);
}

/**
 * Handles form validation.
 */
function adverticum_admin_settings_zones_validate(&$form, &$form_state) {
  $errors = FALSE;

  if (isset($form_state['clicked_button']['#submit'])) {
    return;
  }

  $zones = $form_state['values']['adverticum_zones'];
  foreach ($zones as $index => $zone) {
    if (empty($zone['id']) && empty($zone['name'])) {
      unset($form_state['zones'][$index], $form_state['values']['adverticum_zones'][$index]);
      $form_state['rebuild'] = TRUE;
    }
    elseif (empty($zone['id'])) {
      $errors = TRUE;
      form_error($form[$index]['id'], t('Zone ID field is required.'));
    }
    elseif (empty($zone['tag'])) {
      $errors = TRUE;
      form_error($form[$index]['tag'], t('You need to choose a tag type for the zone.'));
    }
  }

  if (!$errors) {
    form_set_value($form, $zones, $form_state);
  }
}
