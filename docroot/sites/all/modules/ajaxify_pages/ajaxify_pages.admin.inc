<?php
function ajaxify_pages_settings_form() {
  $form = array();

  $form['ajaxify_pages_links_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('Links selector'),
    '#default_value' => variable_get('ajaxify_pages_links_selector', '#main-menu a, a#logo, #site-name a'),
    '#description' => t('jQuery selector expression to select links to apply AJAXify Pages to.'),
  );
  $form['ajaxify_pages_content_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('Content selector'),
    '#default_value' => variable_get('ajaxify_pages_content_selector', '#page-wrapper'),
    '#description' => t('jQuery selector expression to select content to replace by AJAXify Pages.'),
  );
  
  return system_settings_form($form);
}
