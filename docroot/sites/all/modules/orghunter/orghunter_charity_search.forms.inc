<?php

/**
 * Charity Search Form.
 */
function orghunter_charity_search_form($form, $form_state) {
  $values = array_merge(array(
    'search_term' => '',
    'ein' => '',
    'state' => '',
    'city' => '',
    'zip_code' => '',
    'category' => '',
    'eligible' => '',
  ), $_GET);

  $form['search_term'] = array(
    '#title' => t('Charity name, keyword or EIN'),
    '#type' => 'textfield',
    '#default_value' => $values['search_term'] ? $values['search_term'] : $values['ein'] ? $values['ein'] : '',
    '#attributes' => array(
      'title' => t('Enter part of the charity name or the charity EIN.'),
    )
  );

  $form['state'] = array(
    '#title' => t('State'),
    '#type' => 'select',
    '#default_value' => $values['state'],
    '#options' => array(
      '' => '- ' . t('All states') . ' -',
      'AL' => t('Alabama'),
      'AK' => t('Alaska'),
      'AZ' => t('Arizona'),
      'AR' => t('Arkansas'),
      'CA' => t('California'),
      'CO' => t('Colorado'),
      'CT' => t('Connecticut'),
      'DE' => t('Delaware'),
      'DC' => t('District of Columbia'),
      'FL' => t('Florida'),
      'GA' => t('Georgia'),
      'HI' => t('Hawaii'),
      'ID' => t('Idaho'),
      'IL' => t('Illinois'),
      'IN' => t('Indiana'),
      'IA' => t('Iowa'),
      'KS' => t('Kansas'),
      'KY' => t('Kentucky'),
      'LA' => t('Louisiana'),
      'ME' => t('Maine'),
      'MD' => t('Maryland'),
      'MA' => t('Massachusetts'),
      'MI' => t('Michigan'),
      'MN' => t('Minnesota'),
      'MS' => t('Mississippi'),
      'MO' => t('Missouri'),
      'MT' => t('Montana'),
      'NE' => t('Nebraska'),
      'NV' => t('Nevada'),
      'NH' => t('New Hampshire'),
      'NJ' => t('New Jersey'),
      'NM' => t('New Mexico'),
      'NY' => t('New York'),
      'NC' => t('North Carolina'),
      'ND' => t('North Dakota'),
      'OH' => t('Ohio'),
      'OK' => t('Oklahoma'),
      'OR' => t('Oregon'),
      'PA' => t('Pennsylvania'),
      'RI' => t('Rhode Island'),
      'SC' => t('South Carolina'),
      'SD' => t('South Dakota'),
      'TN' => t('Tennessee'),
      'TX' => t('Texas'),
      'UT' => t('Utah'),
      'VT' => t('Vermont'),
      'VA' => t('Virginia'),
      'WA' => t('Washington'),
      'WV' => t('West Virginia'),
      'WI' => t('Wisconsin'),
      'WY' => t('Wyoming'),
    ),
  );

  $form['city'] = array(
    '#title' => t('City'),
    '#type' => 'textfield',
    '#default_value' => $values['city'],
  );

  $form['zip_code'] = array(
    '#title' => t('Zip Code'),
    '#type' => 'textfield',
    '#default_value' => $values['zip_code'],
  );

  $form['category'] = array(
    '#title' => t('Category'),
    '#type' => 'select',
    '#default_value' => $values['category'],
    '#options' => array_merge(
      array('' => '- ' . t('All categories') . ' -'),
      orghunter_charity_search_categories()
    ),
  );

  $form['eligible'] = array(
    '#title' => t('Tax Deductible'),
    '#type' => 'checkbox',
    '#default_value' => $values['eligible'],
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  return $form;
}

/**
 * Charity Search Form submit.
 */
function orghunter_charity_search_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $values['category'] = $values['category'] == '?' ? '' : $values['category'];
  $query = array(
    'state'    => $values['state'],
    'city'     => $values['city'],
    'zip_code' => $values['zip_code'],
    'category' => $values['category'],
    'eligible' => $values['eligible'],
  );

  if (is_numeric($values['search_term']) && preg_match('/[0-9]{9}/', $values['search_term'])) {
    $query['ein'] = $values['search_term'];
  }
  else {
    $query['search_term'] = $values['search_term'];
  }

  drupal_goto('orghunter/charitysearch/results', array(
    'query' => $query,
  ));
}

/**
 * Charity Search Settings form.
 */
function orghunter_charity_search_settings_form($form, &$form_state) {
  $form['orghunter_charity_search_api_url'] = array(
    '#type' => 'textfield',
    '#title' => t('API URL'),
    '#description' => t('The OrgHunter API endpoint. Leave empty for the default URL.', array('%default' => ORGHUNTER_CHARITY_SEARCH_API_URL_DEFAULT)),
    '#default_value' => variable_get('orghunter_charity_search_api_url', ''),
    '#attributes' => array(
      'placeholder' => variable_get('orghunter_charity_search_api_url', '') ? '' : ORGHUNTER_CHARITY_SEARCH_API_URL_DEFAULT,
    )
  );

  $form['orghunter_charity_search_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#description' => t('The API Key provided by your OrgHunter account.'),
    '#default_value' => variable_get('orghunter_charity_search_api_key', ''),
  );

  $form['orghunter_charity_search_affiliate_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Affiliate ID'),
    '#description' => t('The affiliate ID provided by your Make My Donation account.'),
    '#default_value' => variable_get('orghunter_charity_search_affiliate_id', ''),
  );

  $form['orghunter_charity_search_rows'] = array(
    '#type' => 'textfield',
    '#title' => t('Results count'),
    '#description' => t('Number of results to display per page.'),
    '#default_value' => variable_get('orghunter_charity_search_rows', ORGHUNTER_CHARITY_SEARCH_API_ROWS_DEFAULT),
    '#size' => 2,
  );

  return system_settings_form($form);
}