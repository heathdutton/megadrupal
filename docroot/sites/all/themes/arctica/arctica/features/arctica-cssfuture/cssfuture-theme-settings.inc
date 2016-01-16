<?php

$form['arctica_settings']['polyfills']['futurecss'] = array(
  '#title' => t('Arctica Future CSS'),
  '#type' => 'fieldset',
  '#collapsible' => TRUE,
  '#collapsed' => TRUE,
  '#weight' => 8,
);

$form['arctica_settings']['polyfills']['futurecss']['futurecss_enable'] = array(
  '#type'          => 'checkbox',
  '#title'         => t('Enable Arctica Future CSS'),
  '#default_value' => theme_get_setting('futurecss_enable'),
  '#prefix'        => '<p>Arctica Future CSS makes CSS3 styling available to old versions of internext explorer, giving you the awesome power of CSS3 in a time where internet explorer 7, 8 and 9 still matter. This is based on the CSS3PIE project and boxsizing.htc. </p>',
);

$form['arctica_settings']['polyfills']['futurecss']['futurecss_settings'] = array(
  '#title' => t('Future CSS Settings'),
  '#type' => 'fieldset',
  '#collapsible' => TRUE,
  '#collapsed' => FALSE,
);

$form['arctica_settings']['polyfills']['futurecss']['futurecss_settings']['process_selectors'] = array(
  '#type' => 'textarea',
  '#rows' => 8,
  '#title' => t('CSS selectors to process with CSS3PIE'),
  '#default_value' => theme_get_setting("process_selectors"),
  '#description' => t("Enter selectors that must get the PIE behavior. The selectors should be comma separated like in a normal stylesheet. Do not add a comma or curly bracket after the last selector."),
  '#required' => FALSE,
);