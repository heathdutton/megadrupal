<?php

/**
 * @file
 * Administration page.
 */

/**
 * Visibility configuration form.
 */
function vertical_tabs_config_admin_settings() {

  $roles = user_roles();
  $ct_list = node_type_get_types();
  $vertical_tabs = vertical_tabs_config_vertical_tab_list();

  $conf = vertical_tabs_config_get_config();

  $form['desc'] = array(
    '#type' => 'item',
    '#markup' => t('For each content type, select which vertical tabs need to be hidden depending on roles.'),
  );

  $form['vt'] = array(
    '#type' => 'vertical_tabs',
    '#title' => 'Configuration',
  );

  foreach ($ct_list as $ct_machine_name => $obj) {

    $form['vt']['hide_' . $ct_machine_name] = array(
      '#type' => 'fieldset',
      '#title' => $obj->name,
      '#weight' => 5,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#group' => 'content_types_config',
    );

    $form['vt']['hide_' . $ct_machine_name]['config'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#collapsed' => TRUE,
    );

    $form['vt']['hide_' . $ct_machine_name]['config']['desc'] = array(
      '#type' => 'item',
      '#markup' => t("Select all vertical tabs that will be hidden for @ct. If you don't select any role, vertical tabs will be hidden to all roles.", array('@ct' => $obj->name)),
    );

    $form['vt']['hide_' . $ct_machine_name]['config']['roles'] = array(
      '#type' => 'fieldset',
      '#title' => t('Hide only by role'),
      '#weight' => 5,
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#group' => 'content_types_config_roles',
    );

    $form['vt']['hide_' . $ct_machine_name]['config']['tabs'] = array(
      '#type' => 'fieldset',
      '#title' => t('Hidded vertical tabs'),
      '#weight' => 6,
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#group' => 'content_types_config_tabs',
    );

    foreach ($roles as $rid => $value) {

      $def = 0;
      if (isset($conf[$ct_machine_name]['roles']) && is_array($conf[$ct_machine_name]['roles'])) {
        if (in_array($rid, $conf[$ct_machine_name]['roles'])) {
          $def = 1;
        }
      }

      $form['vt']['hide_' . $ct_machine_name]['config']['roles']['role_' . $ct_machine_name . '_' . $rid] = array(
        '#type' => 'checkbox',
        '#title' => t('Apply config for') . ' ' . $value,
        '#default_value' => $def,
        '#group' => 'vertical_tabs_roles',
      );
    }

    foreach ($vertical_tabs as $vt_machine_name => $vt_human_name) {

      $def = isset($conf[$ct_machine_name][$vt_machine_name]) ? $conf[$ct_machine_name][$vt_machine_name] : 0;

      $form['vt']['hide_' . $ct_machine_name]['config']['tabs']['hide_' . $ct_machine_name . '_' . $vt_machine_name] = array(
        '#type' => 'checkbox',
        '#title' => t('Hide') . ' ' . $vt_human_name,
        '#default_value' => $def,
        '#group' => 'vertical_tabs_hide',
      );
    }
  }

  $form['#submit'][] = 'vertical_tabs_config_admin_settings_submit';
  return system_settings_form($form, TRUE);
}


/**
 * Configuration form submit.
 */
function vertical_tabs_config_admin_settings_submit($form, $form_state) {

  $config = array();

  $roles = user_roles();
  $ct_list = node_type_get_types();
  $vertical_tabs = vertical_tabs_config_vertical_tab_list();

  foreach ($ct_list as $ct_machine_name => $obj) {

    $selected_roles = array();
    foreach ($roles as $rid => $value) {
      if ($form_state['values']['role_' . $ct_machine_name . '_' . $rid] == 1) {
        $selected_roles[] = $rid;
      }
    }

    foreach ($vertical_tabs as $vt_machine_name => $vt_human_name) {

      $data = array(
        'vertical_tab' => $vt_machine_name,
        'content_type' => $ct_machine_name,
        'hidden' => $form_state['values']['hide_' . $ct_machine_name . '_' . $vt_machine_name],
        'roles' => json_encode($selected_roles),
      );
      $config[] = $data;
    }
  }

  vertical_tabs_config_save_config($config);
}

/**
 * Order configuration form.
 */
function vertical_tabs_config_admin_settings_order() {

  $order = 0;
  $vertical_tabs = vertical_tabs_config_vertical_tab_list();

  $form['desc'] = array(
    '#type' => 'item',
    '#markup' => t('Set the desired weight for each vertical tab'),
  );

  foreach ($vertical_tabs as $vt_machine_name => $vt_human_name) {
    $form['vertical_tabs_config_' . $vt_machine_name] = array(
      '#type' => 'textfield',
      '#title' => $vt_human_name . ' ' . t('weight'),
      '#default_value' => variable_get('vertical_tabs_config_' . $vt_machine_name, $order),
    );
    $order++;
  }

  return system_settings_form($form, TRUE);
}
