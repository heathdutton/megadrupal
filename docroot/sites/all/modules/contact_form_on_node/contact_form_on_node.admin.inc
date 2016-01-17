<?php

/**
 * @file
 * Functions that are only called on the admin pages.
 */

/**
 * Module settings form.
 */
function contact_form_on_node_settings() {
  $form['contact_form_on_node_info'] = array(
    '#value' => t('<p>The contact_form_on_node module allows you to display a contact form right under a node</p>'),
  );

  // General settings.
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['general']['contact_form_on_node_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Contact form to use'),
    '#description' => t("Select the contact form to use - by default, the user's contact form is used."),
    '#default_value' => variable_get('contact_form_on_node_mode', CONTACT_FORM_ON_NODE_MODE_USER),
    '#options' => array(
      CONTACT_FORM_ON_NODE_MODE_USER => t("User's contact form"),
      CONTACT_FORM_ON_NODE_MODE_SYSTEM => t("System-wide contact form"),
    ),
  );
  $form['general']['contact_form_on_node_show_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add a link to contact the author in full view'),
    '#default_value' => variable_get('contact_form_on_node_show_link', FALSE),
    '#description' => t('Check this box if you want to add a link to the content to contact the author.'),
  );
  $form['general']['contact_form_on_node_show_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add a form to contact the author in full view'),
    '#default_value' => variable_get('contact_form_on_node_show_form', TRUE),
    '#description' => t('Check this box if you want to add a form to the content to contact the author.'),
  );
  $form['general']['contact_form_on_node_show_link_in_teaser'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add a link to contact the author in teaser view'),
    '#default_value' => variable_get('contact_form_on_node_show_link_in_teaser', TRUE),
    '#description' => t('Check this box if you want to add a link to the teaser of the content to contact the author.'),
  );
  $form['general']['contact_form_on_node_show_form_in_teaser'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add a form to contact the author in teaser view'),
    '#default_value' => variable_get('contact_form_on_node_show_form_in_teaser', FALSE),
    '#description' => t('Check this box if you want to add a form to the teaser of the content to contact the author.'),
  );

  $form['general']['contact_form_on_node_subject_is_title'] = array(
    '#type' => 'checkbox',
    '#title' => t("Default the message's subject to be the content's title"),
    '#default_value' => variable_get('contact_form_on_node_subject_is_title', FALSE),
    '#description' => t('Checking this box will place the title of the content the form is on in the subject field of the form itself, following the text "Re: ".'),
  );

  // Types settings.
  $form['types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content types'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $select = array();
  $types = node_type_get_types();
  foreach ($types as $key => $type) {
    $names[$key] = $type->name;
    $select[$key] = $names[$key];
  }
  $form['types']['contact_form_on_node_types'] = array(
    '#title' => t('Included content types'),
    '#type' => 'checkboxes',
    '#default_value' => variable_get('contact_form_on_node_types', array()),
    '#options' => $select,
    '#description' => t('Select the content types for which you want a contact form and/or link added.'),
    '#prefix' => '<div class="contact_form_on_node_checkboxes">',
    '#suffix' => '</div>',
  );

  return system_settings_form($form);
}
