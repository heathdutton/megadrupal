<?php
function apachesolr_media_index_action_form($form, $form_state) {
  if ($environment = apachesolr_environment_load(variable_get('apachesolr_media_default_environment'))) {
  dpm(entity_get_info());
  $form['config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuration'),
    '#collapsible' => TRUE,
  );

  $form['config']['bundles'] = array(
    '#type' => 'markup',
    '#markup' => t('Select the entity types and bundles that should be indexed.'),
  );
  }
  return $form;
}

