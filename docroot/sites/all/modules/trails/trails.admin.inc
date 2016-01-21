<?php
/**
 * @file
 * Administration functionality.
 */

/**
 * Settings form for Trails.
 */
function trails_admin_settings() {
  $form = array();

  $form['breadcrumb'] = array(
    '#type' => 'fieldset',
    '#title' => t('Breadcrumb settings'),
    '#collapsible' => TRUE,
  );
  $form['breadcrumb']['trails_breadcrumb'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use trail for breadcrumb'),
    '#default_value' => variable_get('trails_breadcrumb', TRUE),
    '#description' => t('Selecting this option causes Trails to set the currently applied trail for the page breadcrumb when preprocessing a page for display (i.e. after a page has been built) if the trail is not empty. This should be disabled if, for example, using Rules to configure breadcrumbs with Trails.'),
  );
  $form['breadcrumb']['trails_breadcrumb_home'] = array(
    '#type' => 'checkbox',
    '#title' => t('Prepend "Home" to the breadcrumb'),
    '#default_value' => variable_get('trails_breadcrumb_home', TRUE),
    '#description' => t('Insert a link to "Home" at the front of the breadcrumb if it is to be set. It is usually okay to leave this option enabled.'),
    '#states' => array(
      'visible' => array(
        ':input[name="trails_breadcrumb"]' => array('checked' => TRUE),
      ),
    ),
  );

  return system_settings_form($form);
}
