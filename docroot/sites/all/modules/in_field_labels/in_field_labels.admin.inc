<?php

function in_field_labels_admin($form, &$form_state) {

  $form['in_field_labels_forms'] = array(
    '#type' => 'textarea',
    '#title' => 'Enabled forms',
    '#description' => t('Add one form id per line (not including # sign) or use * to trigger for all forms.'),
    '#default_value' => variable_get('in_field_labels_forms', IN_FIELD_LABELS_DEFAULT_FORMS),
  );

  return system_settings_form($form);
}
