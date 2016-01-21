<?php
/**
 * @file
 * Forms for module 2gis.
 */

/**
 * Settings form for 2gis API.
 */
function dgis_settings_form($form, &$form_state) {
  $form['dgis_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#description' => t('Fill out the !form to get the key.', array(
      '!form' => l(
        t('form'),
        'http://partner.api.2gis.ru/',
        array('external' => TRUE, 'attributes' => array('target'=>'_blank'))
      )
    )),
    '#size' => 60,
    '#default_value' => variable_get('dgis_api_key', ''),
    '#required' => TRUE,
  );

  $form['dgis_api_version'] = array(
    '#type' => 'select',
    '#title' => t('API version'),
    '#options' => array('1.3' => '1.3'),
    '#default_value' => variable_get('dgis_api_version', '1.3'),
    '#required' => TRUE,
  );

  $form['dgis_lang'] = array(
    '#type' => 'select',
    '#title' => t('Language'),
    '#options' => array('ru' => t('RU'), 'en' => t('EN')),
    '#default_value' => variable_get('dgis_lang', 'ru'),
    '#required' => TRUE,
  );

  $cities_options = array();
  if (variable_get('dgis_api_key', FALSE)) {
    $cities = dgis_get_cities();
    foreach ($cities as $city) {
      $cities_options[$city['name']] = $city['name'] . ' (' . $city['code'] . ', ' . $city['timezone'] .
        ', objects count: ' . $city['geoscount'] . ', firms count: ' . $city['firmscount'] . ')';
    }
  }

  if (!empty($cities_options)) {
    reset($cities_options);
    if (!variable_get('dgis_city', FALSE)) {
      variable_set('dgis_city', key($cities_options));
    }
    $form['dgis_city'] = array(
      '#type' => 'select',
      '#title' => t('Location'),
      '#options' => $cities_options,
      '#default_value' => variable_get('dgis_city', key($cities_options)),
      '#required' => TRUE,
    );
  }

  $form['dgis_api_service_url_catalog'] = array(
    '#type' => 'textfield',
    '#title' => t('Catalog API url'),
    '#size' => 60,
    '#default_value' => variable_get('dgis_api_service_url_catalog', 'http://catalog.api.2gis.ru'),
    '#required' => TRUE,
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'dgis_config_form_submit';

  return $form;
}

/**
 * Submit for dgis_api_settings_form.
 */
function dgis_config_form_submit($form, $form_state) {
  if ($form_state['values']['dgis_lang'] != $form['dgis_lang']['#default_value']) {
    variable_del('dgis_city');
  }
}
