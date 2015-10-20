<?php

/**
 * @file
 * Contains administrative forms with Entity Abuse module configurations.
 */

/**
 * Form for managing module's configurations.
 */
function entity_abuse_settings_form($form) {

  $form['main'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
  );

  $form['main']['entity_abuse_own_content'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow user to abuse on his own content'),
    '#description' => t('Defines if user may complain on the own content'),
    '#default_value' => variable_get('entity_abuse_own_content', ENTITY_ABUSE_OWN_CONTENT_DISALLOWED),
  );

  $form['abuse_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Abuse form settings'),
    '#collapsible' => TRUE,
  );

  $form['abuse_form']['entity_abuse_reason_field'] = array(
    '#type' => 'select',
    '#title' => t('Reason field'),
    '#description' => t('Setting for adding reason textarea when user submits his complain.'),
    '#options' => array(
      ENTITY_ABUSE_REASON_FIELD_DISABLED => t('Disabled'),
      ENTITY_ABUSE_REASON_FIELD_OPTIONAL => t('Optional'),
      ENTITY_ABUSE_REASON_FIELD_REQUIRED => t('Required'),
    ),
    '#default_value' => variable_get('entity_abuse_reason_field', ENTITY_ABUSE_REASON_FIELD_OPTIONAL),
  );

  $form['abuse_link'] = array(
    '#type' => 'fieldset',
    '#title' => t('Abuse link settings'),
    '#collapsible' => TRUE,
  );

  $form['abuse_link']['entity_abuse_link_submit_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Submit abuse link title'),
    '#description' => t('Title of the "submit abuse" link which displayed on the content.'),
    '#default_value' => variable_get('entity_abuse_link_submit_title', ENTITY_ABUSE_LINK_SUBMIT_ABUSE_TITLE),
  );

  $form['abuse_link']['entity_abuse_link_cancel_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Cancel abuse link title'),
    '#description' => t('Title of the "cancel abuse" link which displayed on the content.'),
    '#default_value' => variable_get('entity_abuse_link_cancel_title', ENTITY_ABUSE_LINK_CANCEL_ABUSE_TITLE),
  );

  return system_settings_form($form);
}

/**
 * Alters the settings form of a node type.
 */
function entity_abuse_node_type_form_setting(&$form, $node_type) {

  $form['entity_abuse'] = array(
    '#type' => 'fieldset',
    '#title' => t('Entity Abuse'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'additional_settings',
    '#attributes' => array(
      'class' => array('entity-abuse-node-type-settings'),
    ),
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'entity_abuse') . '/js/entity_abuse.node_form.js')
    ),
  );

  $form['entity_abuse']['node'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node abuses'),
  );

  $form['entity_abuse']['node']['entity_abuse_enabled_node'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable abuses for this node type'),
    '#default_value' => variable_get('entity_abuse_enabled_node_' . $node_type, ENTITY_ABUSE_ENTITY_ENABLED_DEFAULT),
  );

  $field_display_tips = array(
    t('Do not display link - Abuse link will not added to the node, and you have to implement it by your own.'),
    t('Add a field - To the node will be added an extra field. You may configure its visibility and weight.'),
    t('Add a link - A simple link will be added to the list of other links in the node (comments, amount of views, etc).'),
  );

  $form['entity_abuse']['node']['entity_abuse_link_display_node'] = array(
    '#type' => 'select',
    '#title' => t('How to output an abuse link'),
    '#description' => theme('item_list', array('items' => $field_display_tips)),
    '#options' => array(
      ENTITY_ABUSE_LINK_DISPLAY_NONE => t('Do not display'),
      ENTITY_ABUSE_LINK_DISPLAY_AS_FIELD => t('Display as a field'),
      ENTITY_ABUSE_LINK_DISPLAY_AS_LINK => t('Display as a link'),
    ),
    '#default_value' => variable_get('entity_abuse_link_display_node_' . $node_type, ENTITY_ABUSE_LINK_DISPLAY_AS_FIELD),
    '#states' => array(
      'visible' => array(
        ':input[name="entity_abuse_enabled_node"]' => array('checked' => TRUE),
      ),
    ),
  );

  $entity_info = entity_get_info('node');
  $view_modes = array();
  foreach ($entity_info['view modes'] as $view_mode => $data) {
    $view_modes[$view_mode] = $data['label'];
  }

  $form['entity_abuse']['node']['entity_abuse_link_display_view_modes_node'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Show abuse link on next displays'),
    '#options' => $view_modes,
    '#states' => array(
      'visible' => array(
        ':input[name="entity_abuse_enabled_node"]' => array('checked' => TRUE),
        ':input[name="entity_abuse_link_display_node"]' => array('value' => ENTITY_ABUSE_LINK_DISPLAY_AS_LINK),
      ),
    ),
    '#default_value' => variable_get('entity_abuse_link_display_view_modes_node_' . $node_type, array('full')),
  );

  $form['entity_abuse']['comment'] = array(
    '#type' => 'fieldset',
    '#title' => t('Comment abuses'),
  );

  $form['entity_abuse']['comment']['entity_abuse_enabled_comment'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable abuses for comments in this node type'),
    '#default_value' => variable_get('entity_abuse_enabled_comment_' . $node_type, ENTITY_ABUSE_ENTITY_ENABLED_DEFAULT),
  );

  $form['entity_abuse']['comment']['entity_abuse_link_display_comment'] = array(
    '#type' => 'select',
    '#title' => t('How to output an abuse link'),
    '#description' => theme('item_list', array('items' => $field_display_tips)),
    '#options' => array(
      ENTITY_ABUSE_LINK_DISPLAY_NONE => t('Do not display'),
      ENTITY_ABUSE_LINK_DISPLAY_AS_FIELD => t('Display as a field'),
      ENTITY_ABUSE_LINK_DISPLAY_AS_LINK => t('Display as a link'),
    ),
    '#default_value' => variable_get('entity_abuse_link_display_comment_' . $node_type, ENTITY_ABUSE_LINK_DISPLAY_AS_FIELD),
    '#states' => array(
      'visible' => array(
        ':input[name="entity_abuse_enabled_comment"]' => array('checked' => TRUE),
      ),
    ),
  );
}

/**
 * Alters the settings form of a user account.
 */
function entity_abuse_user_form_settings(&$form) {

  $form['entity_abuse'] = array(
    '#type' => 'fieldset',
    '#title' => t('Entity abuse'),
    '#weight' => 0,
  );

  $form['entity_abuse']['entity_abuse_enabled_user'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable abuses for user account'),
    '#default_value' => variable_get('entity_abuse_enabled_user', ENTITY_ABUSE_ENTITY_ENABLED_DEFAULT),
  );

  $field_display_tips = array(
    t('Do not display link - Abuse link will not added to the user, and you have to implement it by your own.'),
    t('Add a field - To the user will be added an extra field. You may configure its visibility and weight.'),
  );

  $form['entity_abuse']['entity_abuse_link_display_user'] = array(
    '#type' => 'select',
    '#title' => t('How to output an abuse link'),
    '#description' => theme('item_list', array('items' => $field_display_tips)),
    '#options' => array(
      ENTITY_ABUSE_LINK_DISPLAY_NONE => t('Do not display'),
      ENTITY_ABUSE_LINK_DISPLAY_AS_FIELD => t('Display as a field'),
    ),
    '#default_value' => variable_get('entity_abuse_link_display_user', ENTITY_ABUSE_LINK_DISPLAY_AS_FIELD),
    '#states' => array(
      'visible' => array(
        ':input[name="entity_abuse_enabled_user"]' => array('checked' => TRUE),
      ),
    ),
  );

}

/**
 * Alters the settings form of a taxonomy vocabulary.
 */
function entity_abuse_taxonomy_vocabulary_form_settings(&$form, $machine_name) {

  $form['entity_abuse'] = array(
    '#type' => 'fieldset',
    '#title' => t('Entity abuse'),
    '#weight' => 0,
  );

  $form['entity_abuse']['entity_abuse_enabled_taxonomy_term_' . $machine_name] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable abuses for taxonomy terms'),
    '#default_value' => variable_get('entity_abuse_enabled_taxonomy_term_' . $machine_name, ENTITY_ABUSE_ENTITY_ENABLED_DEFAULT),
  );

  $field_display_tips = array(
    t('Do not display link - Abuse link will not added to the term, and you have to implement it by your own.'),
    t('Add a field - To the term will be added an extra field. You may configure its visibility and weight.'),
  );

  $form['entity_abuse']['entity_abuse_link_display_taxonomy_term_' . $machine_name] = array(
    '#type' => 'select',
    '#title' => t('How to output an abuse link'),
    '#description' => theme('item_list', array('items' => $field_display_tips)),
    '#options' => array(
      ENTITY_ABUSE_LINK_DISPLAY_NONE => t('Do not display'),
      ENTITY_ABUSE_LINK_DISPLAY_AS_FIELD => t('Display as a field'),
    ),
    '#default_value' => variable_get('entity_abuse_link_display_taxonomy_term_' . $machine_name, ENTITY_ABUSE_LINK_DISPLAY_AS_FIELD),
    '#states' => array(
      'visible' => array(
        ':input[name="entity_abuse_enabled_taxonomy_term_' . $machine_name . '"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['#submit'][] = 'entity_abuse_taxonomy_vocabulary_form_settings_submit';
}

/**
 * Saves the values after submit of vocabulary settings form.
 */
function entity_abuse_taxonomy_vocabulary_form_settings_submit($form, &$form_state) {

  // Get the bundle name of vocabulary.
  $bundle = $form_state['vocabulary']->machine_name;
  $values = $form_state['values'];

  // Save the submitted data.
  variable_set('entity_abuse_enabled_taxonomy_term_' . $bundle, $values['entity_abuse_enabled_taxonomy_term_' . $bundle]);
  variable_set('entity_abuse_link_display_taxonomy_term_' . $bundle, $values['entity_abuse_link_display_taxonomy_term_' . $bundle]);
}
