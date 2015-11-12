<?php
    /**
   * @file yelp_api.admin.inc
   * Admin functionality.
   */
   
  /**
   * Form constructor for yelp api settings form.
   *
   * @see yelp_api_admin_settings_submit()
   * @ingroup forms
   */
  function yelp_api_admin_settings($form, &$form_state) {
    $form['yelp-keys'] = array(
      '#type'    => 'fieldset',
      '#title'  => t('Yelp API keys'),
      '#description'  => t('To use Yelps services you will need to provide your API keys. You can obtain API keys') . ' ' .
                         l(t('here'), 'http://www.yelp.com/developers', array('attribute' => array('target' => '_blank'))),
    );
    // retrieve the stored key values.
    $keys  =  _yelp_api_get_keys();
    foreach ($keys as $key => $value) {
      $form['yelp-keys']['yelp_api_' . $key] = array(
        '#type'            => 'textfield',
        '#title'          => str_replace('_', ' ', $key),
        '#default_value'  => $value,
      );
    }
    
    $form['yelp-settings'] = array(
      '#type'          => 'fieldset',
      '#title'        => t('Yelp API Settings'),
      '#description'  => t('Provide default settings for the Yelp API'),
    );
    
    $form['yelp-settings']['yelp_api_default_cc'] = array(
      '#type'          => 'select',
      '#title'        => t('Default Country'),
      '#description'  => t('Select the default country to return results from'),
      '#options'      => _yelp_api_country_codes(),
      '#default_value' => variable_get('yelp_api_default_cc', 'US'),
    );
    
    $form['yelp-settings']['yelp_api_default_lang'] = array(
      '#type'          => 'select',
      '#title'        => t('Default Language'),
      '#description'  => t('Select the default language to return results in'),
      '#options'      => _yelp_api_lang_codes(),
      '#default_value' => variable_get('yelp_api_default_lang', 'en'),
    );
    
    $form['yelp-settings']['yelp_api_default_lang_filter'] = array(
      '#type'          => 'checkbox',
      '#title'        => t('Restrict results to only the choosen language'),
      '#default_value' => variable_get('yelp_api_default_lang_filter', 0),
    );
    
    // allow for setting addressfield options, if the module is installed.
    if (module_exists('addressfield')) {
      $form['yelp-settings']['yelp_api_use_addressfield'] = array(
        '#type'        => 'checkbox',
        '#title'      => t('Use the addressfield module for location fields'),
        '#default_value' => variable_get('yelp_api_use_addressfield', 0),
      );
      // if the addressfield module is not patched, then let user know they cannot use it.
      if (yelp_api_patched_addressfield()) {
        $form['yelp-settings']['yelp_api_use_addressfield']['#description'] = t('If checked, where ever a yelp api form is used (in yelp blocks for example) the addressfield module will be used to collect address information');
      }
      else{
        $form['yelp-settings']['yelp_api_use_addressfield']['#default_value'] = 0;
        $form['yelp-settings']['yelp_api_use_addressfield']['#disabled']    = TRUE;
        $form['yelp-settings']['yelp_api_use_addressfield']['#description'] =  t('The addressfield is installed, however it cannot be used without the');
        $form['yelp-settings']['yelp_api_use_addressfield']['#description'] .= ' ' . l(t('following patch'), 'http://drupal.org/node/970048#comment-5712492', array('absolute' => TRUE, 'attributes' => array('target' => '_blank')));
      }  
    }

    $form['yelp-cache'] = array(
      '#type'          => 'fieldset',
      '#title'        => t('Yelp API Cache settings'),
    );
  
    $form['yelp-cache']['yelp_api_default_cache_results'] = array(
      '#type'          => 'select',
      '#title'        => t('Cache Query Results'),
      '#description'  => t('Select how many hours to cache results per API request. Caching will will help reduce the occurance of maxing out rate limits.'),
      '#options'      => range(0, 24),
      '#default_value' => 1,
    );
    $form['yelp-cache']['clear-cache'] = array(
        '#type'        => 'button',
        '#value'      => t('Clear Yelp API cache'),
        '#name'        => 'clear-cache',
    );
    
    
    $form = system_settings_form($form);
    $form['#submit'][] = 'yelp_api_admin_settings_submit';
    return $form;
  }
  
  /**
   * Form submission handler for yelp_api_admin_settings()
   */
  function yelp_api_admin_settings_submit($form, &$form_state) {
    if ( isset($form_state['input']['clear-cache']) ) {
      cache_clear_all('yelp-api-search', 'cache', TRUE);
      drupal_set_message(t('Yelp API cache has been cleared'));
    }
  }