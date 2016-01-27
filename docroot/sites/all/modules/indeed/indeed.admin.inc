<?php

/**
* @file
* Administration page callbacks for the Indeed module.
*/

/**
* Form builder. Configure Indeed search.
*/
function indeed_admin_settings() {
  
  $form['indeed_pubid'] = array(
    '#type' => 'textfield',
    '#title' => t('Indeed publisher id'),
    '#default_value' => variable_get('indeed_pubid', '8947677247274827'),
    '#required' => TRUE,
  );
  
  $form['indeed_q'] = array(
    '#type' => 'textfield',
    '#title' => t('Query'),
    '#default_value' => variable_get('indeed_q', 'drupal'),
    '#required' => TRUE,
  );
  
  $form['indeed_l'] = array(
    '#type' => 'textfield',
    '#title' => t('Location'),
    '#default_value' => variable_get('indeed_l'),
  );
  
  $form['indeed_radius'] = array(
    '#type' => 'textfield',
    '#title' => t('Radius'),
    '#default_value' => variable_get('indeed_radius', '25'),
    '#rules' => array('rule' => 'numeric', 'error' => 'Must be a number'),
    '#required' => TRUE,
  );
  $form['indeed_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Limit'),
    '#default_value' => variable_get('indeed_limit', '10'),
    '#required' => TRUE,
  );
  
  $form['indeed_sort'] = array(
    '#type' => 'select',
    '#title' => 'Sort by',
    '#options' => array('relevance' => t('Relevance'),
                        'date' => t('Date'),
                  ),
    '#default_value' => variable_get('indeed_sort'),
  );
  
  $form['indeed_country'] = array(
    '#type' => 'select',
    '#title' => 'Country',
    '#options' => array('us' => t('United States'),
                        'ar' => t('Argentina'),
                        'au' => t('Australia'),
                        'at' => t('Austria'),
                        'bh' => t('Bahrain'),
                        'be' => t('Belgium'),
                        'br' => t('Brazil'),
                        'ca' => t('Canada'),
                        'cl' => t('Chile'),
                        'cn' => t('China'),
                        'co' => t('Colombia'),
                        'cz' => t('Czech Republic'),
                        'dk' => t('Denmark'),
                        'fi' => t('Finland'),
                        'fr' => t('France'),
                        'de' => t('Germany'),
                        'gr' => t('Greece'),
                        'hk' => t('Hong Kong'),
                        'hu' => t('Hungary'),
                        'in' => t('India'),
                        'id' => t('Indonesia'),
                        'ie' => t('Ireland'),
                        'il' => t('Israel'),
                        'it' => t('Italy'),
                        'jp' => t('Japan'),
                        'kr' => t('Korea'),
                        'kw' => t('Kuwait'),
                        'lu' => t('Luxembourg'),
                        'my' => t('Malaysia'),
                        'mx' => t('Mexico'),
                        'nl' => t('Netherlands'),
                        'nz' => t('New Zealand'),
                        'no' => t('Norway'),
                        'om' => t('Oman'),
                        'pk' => t('Pakistan'),
                        'pe' => t('Peru'),
                        'ph' => t('Philippines'),
                        'pl' => t('Poland'),
                        'pt' => t('Portugal'),
                        'qa' => t('Qatar'),
                        'ro' => t('Romania'),
                        'ru' => t('Russia'),
                        'sa' => t('Saudi Arabia'),
                        'sg' => t('Singapore'),
                        'za' => t('South Africa'),
                        'es' => t('Spain'),
                        'se' => t('Sweden'),
                        'ch' => t('Switzerland'),
                        'tw' => t('Taiwan'),
                        'tr' => t('Turkey'),
                        'ae' => t('United Arab Emirates'),
                        'gb' => t('United Kingdom'),
                        've' => t('Venezuela'),
                  ),
    '#default_value' => variable_get('indeed_country'),
  );

  $form['#submit'][] = 'indeed_admin_settings_submit';

  return system_settings_form($form);
}

/**
* Validate Indeed settings submission.
*/
function indeed_admin_settings_validate($form, &$form_state) {
  $radius = $form_state['values']['indeed_radius'];
  if (!is_numeric($radius) || $radius == 0) {
    form_set_error('indeed_radius', t('Radius: Please enter number 1 or greater.'));
  }
  
  $limit = $form_state['values']['indeed_limit'];
  if (!is_numeric($limit) || $limit <= 1) {
    form_set_error('indeed_limit', t('Limit: Please enter number 2 or greater.'));
  }
}

function indeed_admin_settings_submit() {
  return;
}
