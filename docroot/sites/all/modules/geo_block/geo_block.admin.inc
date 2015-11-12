<?php

/**
 * @file
 * Administrative interface.
 */

/**
 * Module setting form.
 */
function geo_block_module_settings_form($form, &$form_state) {
  $form = array();

  $form['main_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Main settings'),
    '#weight' => 1,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['main_settings']['geo_block_geo_service'] = array(
    '#type' => 'select',
    '#title' => t('Geo-service'),
    '#description' => t('What geo-service will be used to determine the location of the visitor?'),
    '#options' => array(
      'ipgeobase' => 'ipgeobase.ru',
      'hostip' => 'hostip.info',
    ),
    '#default_value' => variable_get('geo_block_geo_service', GEO_BLOCK_DEFAULT_GEOSERVICE),
    '#required' => TRUE,
  );

  $form['city_select_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Сhange the city'),
    '#description' => t('This section displays various options to change the city for a user. It is necessary for cases when the city was not defined correctly.'),
    '#weight' => 2,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['city_select_settings']['top_toolbar'] = array(
    '#type' => 'fieldset',
    '#title' => t('Top toolbar'),
    '#description' => t('Toolbar on the top of the site.'),
    '#weight' => 2,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['city_select_settings']['top_toolbar']['geo_block_top_toolbar_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('On / Off'),
    '#default_value' => variable_get('geo_block_top_toolbar_enabled', 0),
  );
  $form['city_select_settings']['top_toolbar']['geo_block_top_toolbar_use_cookie'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use cookie'),
    '#description' => t("If checked, the cookie will be set after the close of the selection box and it won't display again."),
    '#default_value' => variable_get('geo_block_top_toolbar_use_cookie', 0),
  );

  $description = t("Insert the name with the ability to change the city in the right place on the page by inserting the HTML code.") . "<br />";
  $description .= "<strong>" . t("Code 1:") . "</strong>";
  $description .= t("&lt;gb:city-change&gt;&lt;/gb:city-change&gt; - use it only on template with &lt;!DOCTYPE html&gt;") . "<br />";
  $description .= "<strong>" . t("Code 2 (old method):") . "</strong>";
  $description .= t("&lt;span id=&quot;gb_placeholder&quot;&gt;&lt;/span&gt; or &lt;div id=&quot;gb_placeholder&quot;&gt;&lt;/div&gt;");

  $form['city_select_settings']['placeholder'] = array(
    '#type' => 'fieldset',
    '#title' => t('Placeholder'),
    '#description' => $description,
    '#weight' => 3,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['city_select_settings']['placeholder']['geo_block_placeholder_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('On / Off'),
    '#default_value' => variable_get('geo_block_placeholder_enabled', 0),
  );

  return system_settings_form($form);
}
