<?php
/**
 * Settings form
 * 
 * @param array $form
 * @param array $form_state
 * 
 * @return array
 */
function varnish_tag_invalidate_admin_settings_form($form, &$form_state) {
  $form = array();
  
  $form['varnish_tag_invalidate_header'] = array(
    '#type' => 'textfield',
    '#title' => t('Tag header'),
    '#default_value' => variable_get('varnish_tag_invalidate_header', 'X-Varnish-Tag'),
    '#description' => t('The header used to store the tag in Varnish.')    
  );
  
  $form['varnish_tag_invalidate_hosts'] = array(
    '#type' => 'textarea',
    '#title' => t('Site hosts'),
    '#default_value' => variable_get('varnish_tag_invalidate_hosts', ''),
    '#description' => t('Add hosts to be purged, including protocol and port, one per line. For example http://www.site.com, http://site.com:8080'),
  );

  return system_settings_form($form);
}

/**
 * Settings form for entities
 * 
 * @param array $form
 * @param array $form_state
 * @return array
 */
function varnish_tag_invalidate_entity_admin_settings_form($form, &$form_state) {
  $entity_types = array();
  
  foreach (entity_get_info() as $name => $info) {
    $entity_types[$name] = t($info['label']);
  }
  
  $form['varnish_tag_invalidate_entities_view'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Entity types'),
    '#default_value' => variable_get('varnish_tag_invalidate_entities_view', array()),
    '#options' => $entity_types,
  );
  
  return system_settings_form($form);
}

/**
 * Validation callback for varnish_tag_invalidate_entity_admin_settings_form.
 * 
 * Removes blank entries
 */
function varnish_tag_invalidate_entity_admin_settings_form_validate(&$form, &$form_state) {
  $form_state['values']['varnish_tag_invalidate_entities_view'] = array_filter($form_state['values']['varnish_tag_invalidate_entities_view']);
}
