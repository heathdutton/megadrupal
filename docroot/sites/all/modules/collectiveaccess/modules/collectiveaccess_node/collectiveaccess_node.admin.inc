<?php
// $Id$

/**
 * @file
 * Renders administrative pages for collectiveaccess_node module
 */

function collectiveaccess_node_admin_settings() {
  $form = array();

  /* Content type settings */
  $form['collectiveaccess_node_contenttype_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content type settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $content_types = node_get_types('names');
  $form['collectiveaccess_node_contenttype_settings']['collectiveaccess_node_contenttype'] = array(
    '#type' => 'select',
    '#title' => t('Contenttype to import into'),
    '#description' => t('Specify the content type to import CollectiveAccess objects into'),
    '#required' => TRUE,
    '#multiple' => FALSE,
    '#options' => $content_types,
    '#default_value' => variable_get('collectiveaccess_node_contenttype', ''),
  );

  return system_settings_form($form);
}