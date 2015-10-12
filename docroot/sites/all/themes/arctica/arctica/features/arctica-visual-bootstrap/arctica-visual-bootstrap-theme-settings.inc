<?php

$form['arctica_settings']['aavb'] = array(
  '#title' => t('Arctica Visual Bootstrap'),
  '#type' => 'fieldset',
  '#collapsible' => TRUE,
  '#collapsed' => TRUE,
  '#weight' => 7,
);

$form['arctica_settings']['aavb']['design_packages'] = array(
  '#title' => t('Design packages'),
  '#type' => 'fieldset',
  '#description' => t('Not every theme requires a unique design for tabbed menus or printed pages. Enable these packages to get out-of-the box styling that your client will love.'),
);

$form['arctica_settings']['aavb']['design_packages']['aavb_print'] = array(
  '#type' => 'checkbox',
  '#title' => t('Print'),
  '#default_value' => theme_get_setting('aavb_print'),
  '#description' => t('Styling package for print.'),
);

$form['arctica_settings']['aavb']['design_packages']['aavb_forms'] = array(
  '#type' => 'checkbox',
  '#title' => t('Forms'),
  '#default_value' => theme_get_setting('aavb_forms'),
  '#description' => t('Styling package for form elements. Includes support for HTML5 form elements.'),
);

$form['arctica_settings']['aavb']['design_packages']['aavb_navigation'] = array(
  '#type' => 'checkbox',
  '#title' => t('Drupal navigation systems'),
  '#default_value' => theme_get_setting('aavb_navigation'),
  '#description' => t('Styling package for Drupal navigation menus. This includes primary tabs, secondary tabs, breadcrumbs and pagers.'),
);

$form['arctica_settings']['aavb']['design_packages']['aavb_messages'] = array(
  '#type' => 'checkbox',
  '#title' => t('System messages'),
  '#default_value' => theme_get_setting('aavb_messages'),
  '#description' => t('Styling package for Drupal system messages. Adds blue styling for "info" messages.'),
);