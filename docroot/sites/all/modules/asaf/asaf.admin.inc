<?php

/**
 * @file
 * Administrative pages of asaf module
 */

function asaf_admin_settings_form() {
  $form = array();

  $form['asaf_forms'] = array(
    '#type' => 'textarea',
    '#title' => t('Forms'),
    '#description' => t('A list of form IDs. One form ID per line. Also, you can specify buttons which should be handled (for eaxmple: article_node_form@save+preview) or buttons which shouldn\'t be handled (article_node_form@-delete).'),
    '#default_value' => variable_get('asaf_forms', 'asaf_admin_settings_form'),
  );

  $form['asaf_show_form_ids'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show form ids'),
    '#description' => t('Will show a list of form IDs present on the particular page. <strong>Be careful, because all users (including anonymous) will see a debug messages when this option is enabled.</strong>'),
    '#default_value' => variable_get('asaf_show_form_ids', 0),
  );

  $form['experimental'] = array(
    '#type' => 'fieldset',
    '#title' => t('Experimental'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['experimental']['asaf_autoload_form_stuff'] = array(
    '#type' => 'checkbox',
    '#title' => t('Autoload forms stuff'),
    '#description' => t('It can help in some seldom cases when you try to ajaxify form and retrieve "Call to undefined function..." error.'),
    '#default_value' => variable_get('asaf_autoload_form_stuff', FALSE),
  );

  $form['experimental']['asaf_form_submit_by_enter'] = array(
    '#type' => 'checkbox',
    '#title' => t('Submit form by \'enter\' button'),
    '#default_value' => variable_get('asaf_form_submit_by_enter', FALSE),
  );

  return system_settings_form($form);
}