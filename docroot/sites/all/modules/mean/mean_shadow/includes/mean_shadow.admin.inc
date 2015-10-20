<?php

function mean_shadow_settings_form() {
  $form['mean_shadow_entities'] = array(
    '#type' => 'fieldset',
    '#title' => t('Choose entities to shadow'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );

  $form['mean_shadow_entities']['mean_shadow_entities_user'] = array(
    '#type' => 'checkbox',
    '#title' => t('User'),
    '#default_value' => variable_get('mean_shadow_entities_user', 0),
  );

  // Node types
  $types = node_type_get_types();
  foreach ($types as $type) {
    $form['mean_shadow_entities']['mean_shadow_entities_' . $type->type] = array(
      '#type' => 'checkbox',
      '#title' => $type->name,
      '#default_value' => variable_get('mean_shadow_entities_' . $type->type, 0),
    );
  }

  return system_settings_form($form);
}
