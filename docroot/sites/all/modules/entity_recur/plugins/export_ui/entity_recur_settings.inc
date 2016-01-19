<?php

$plugin = array(
  'schema' => 'entity_recur_settings',
  'access' => 'administer site configuration',
  'allowed operations' => array(
    'clone' => FALSE,
  ),
  'menu' => array(
    'menu prefix' => 'admin/config/content',
    'menu item' => 'entity-recur',
    'menu title' => 'Entity Recur ',
    'menu description' => 'Entity Recur settings.',
    'items' => array(
      'add' => array(
        'access callback' => FALSE,
        'access arguments' => array(),
      ),
    ),
  ),
  'title singular' => t('Entity Recur setting'),
  'title singular proper' => t('Entity Recur setting'),
  'title plural' => t('Entity Recur settings'),
  'title plural proper' => t('Entity Recur settings'),
);

function entity_recur_settings_form(&$form, &$form_state) {
  $settings = $form_state['item'];
  $instances = field_info_instances($settings->entity_type, $settings->bundle);
  $entity_info = entity_get_info($settings->entity_type);
  $bundle_info = $entity_info['bundles'][$settings->bundle];
  $fields = array();
  foreach ($instances as $name => $field) {
    if ($field['widget']['module'] == 'date') {
      $fields[$name] = $field['label'] . ' (' . $name . ')';
    }
  }
  $form['entity_type'] = array(
    '#type' => 'value',
    '#value' => $settings->entity_type,
  );
  $form['bundle'] = array(
    '#type' => 'value',
    '#value' => $settings->bundle,
  );
  $form['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable recurring'),
    '#description' => t('If checked, users with the right permissiosn can create recurring @bundle copies', array('@bundle' => $bundle_info['label'])),
    '#default_value' => $settings->enabled,
  );
  $form['allow_past'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow dates in the past'),
    '#default_value' => $settings->allow_past,
    '#description' => t('If checked, recurrences with dates in the past will be allowed.'),
  );
  $form['max_duration'] = array(
    '#type' => 'select',
    '#title' => t('Max recurring duration'),
    '#options' => array(
      '0' => t('No max'),
      '1 week' => t('1 week'),
      '2 weeks' => t('2 weeks'),
      '1 month' => t('1 month'),
      '3 months' => t('3 months'),
      '6 months' => t('6 months'),
      '1 year' => t('1 year'),
    ),
    '#default_value' => $settings->max_duration,
    '#description' => t('Select a maximum time span that recurring will be allowed to continue to.'),
  );

  $form['date_field'] = array(
    '#type' => 'select',
    '#title' => t('Date field'),
    '#options' => $fields,
    '#default_value' => $settings->date_field && isset($fields[$settings->date_field]) ? $settings->date_field : NULL,
    '#description' => t('Select the date field that will be used to base the recurrences on.'),
    '#required' => TRUE,
  );
  $form['form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display recur options on entity add form'),
    '#default_value' => $settings->form,
    '#description' => t('If checked, recurring options will appear on the entity add form.'),
  );
  $form['batch_limit'] = array(
    '#type' => 'textfield',
    '#field_prefix' => t('Use a batch when') . '&nbsp;',
    '#field_suffix' => '&nbsp;' . t('or more entities will be generated.'),
    '#size' => 5,
    '#default_value' => $settings->batch_limit,
    '#required' => TRUE,
    '#element_validate' => array('element_validate_integer_positive'),
  );
  return $form;
}

function entity_recur_settings_form_submit($form, &$form_state) {
  $props = array_keys(get_object_vars($form_state['item']));
  foreach ($props as $prop) {
    if (isset($form_state['values'][$prop])) {
      $form_state['item']->{$prop} = $form_state['values'][$prop];
    }
  }
}
