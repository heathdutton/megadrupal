<?php
  /**
   * @file yelp_api.form.inc
   *  Contains functions relating to forms.
   */

  /**
   * Builds form fields for a yelp object
   *
   * @param array $form
   *  A drupal form array
   * @param array &$form_state
   *  The state of the form, passed by reference.
   * @param object $yelp_object
   *  Optional existing yelp object to prepopulate fields with
   *
   * @return array
   *  Modified Drupal form array, containg yelp api fields.
   */
  function yelp_api_form_fields($form , &$form_state , $yelp_object=FALSE) {
    module_load_include('inc' , 'yelp_api' , 'yelp_api.pages');
    $form['yelp-settings'] = array(
      '#type'    => 'fieldset' , 
      '#title'  => 'Yelp Settings' , 
    );
    
    // yelp location field(s)
    $form['yelp-settings']['location'] = array(
      '#type'          => 'fieldset' , 
      '#title'        => t('Location') , 
      '#description'  => t('Provide address that should be used by this block')  , 
    );
    
    //use addressfield if able
    if (yelp_api_use_addressfield() && (!$yelp_object || (strpos($yelp_object->yelp_settings->location , '|')>0))) {
      $form['yelp-settings']['location']['addressfield'] = array(
        '#type'            => 'addressfield' , 
        '#title'          => 'Address' , 
        '#context'        => array('countries' => array('US' , 'CA' , 'GB')) , 
        '#description'    => t('Location to search from. Must be a valid address  , or city  , or zipcode  , or state. (Example: 41 Winter St  , Boston MA 02115) . ') , 
        '#default_value'  => '' , 
        '#element_validate' => array('yelp_api_form_field_validate') , 
      );
    }
    else{
      $form['yelp-settings']['location']['address'] = array(
        '#type'            => 'textfield' , 
        '#title'          => 'Address' , 
        '#description'    => t('Location to search from. Must be a valid address  , or city  , or zipcode  , or state. (Example: 41 Winter St  , Boston MA 02115) . ') , 
        '#default_value'  => '' , 
        '#element_validate' => array('yelp_api_form_field_validate') , 
      );
    }
    
    //yelp radius
    $form['yelp-settings']['radius'] = array(
      '#type'              => 'textfield' , 
      '#title'            => t('Search Radius') , 
      '#description'      => t('The radius in miles to search (max 50) (default 10)') , 
      '#default_value'    => 10 , 
      '#element_validate' => array('yelp_api_form_field_validate') , 
    );
      
    //yelp max results
    $form['yelp-settings']['max_results'] = array(
      '#type'            => 'textfield' , 
      '#title'          => t('Max Results') , 
      '#description'    => t('The maximum number of results to return (max 20) (default 4)') , 
      '#default_value'  => 4 , 
      '#element_validate' => array('yelp_api_form_field_validate') , 
    );
    
    //yelp country code
    $form['yelp-settings']['cc'] = array(
      '#title'          => t('Country') , 
      '#description'    => t('Country to search for buisnesses.(US , CA or UK)') , 
      '#element_validate' => array('yelp_api_form_field_validate') , 
    );
    
    //allow token values
    if (module_exists('token')) {
      $form['yelp-settings']['cc']['#type'] = 'textfield';
    }
    else{
      $form['yelp-settings']['cc']['#type'] = 'select';
      module_load_include('inc' , 'yelp_api' , 'yelp_api.pages');
      $form['yelp-settings']['cc']['#options'] = _yelp_api_country_codes();
    }
    $form['yelp-settings']['cc']['#default_value'] = variable_get('yelp_api_default_cc' , 'US');
        
    //yelp language
    $form['yelp-settings']['lang'] = array(
      '#title'          => t('Language') , 
      '#description'    => t('Language to return. (639-1 2 char lang code)') , 
      '#element_validate' => array('yelp_api_form_field_validate') , 
    );
    if (module_exists('token')) {
      $form['yelp-settings']['lang']['#type'] = 'textfield';
    }
    else{
      $form['yelp-settings']['lang']['#type'] = 'select';
      module_load_include('inc' , 'yelp_api' , 'yelp_api.pages');
      $form['yelp-settings']['lang']['#options'] = _yelp_api_lang_codes();
    }
    $form['yelp-settings']['lang']['#default_value'] = variable_get('yelp_api_default_lang' , 'en');
  
    // yelp categories field
    module_load_include('inc' , 'yelp_api' , 'yelp_api.categories');
    $selected_categories = ($yelp_object && isset($yelp_object->yelp_settings->categories)) ? $yelp_object->yelp_settings->categories:FALSE;
    yelp_api_build_form_categories($form , $selected_categories);
    if (isset($form['yelp-categories'])){
			$form['yelp-settings']['yelp-categories'] = $form['yelp-categories'];
			unset($form['yelp-categories']);
			$form['yelp-settings']['yelp-categories']['#description'] = t('Select which categories from yelp to query');
			$form['yelp-settings']['yelp-categories']['#element_validate'] = array('yelp_api_form_field_validate');
    }
    //add token description to specific fields
    if (module_exists('token')) {
      $token_desc                                                      = t('This field supports tokens.');
      if (isset($form['yelp-settings']['location']['address'])) {
        $form['yelp-settings']['location']['address']['#description'] .= $token_desc;
      }  
      $form['yelp-settings']['radius']['#description']               .= $token_desc;
      $form['yelp-settings']['max_results']['#description']          .= $token_desc;
      $form['yelp-settings']['cc']['#description']                   .= $token_desc;
      $form['yelp-settings']['lang']['#description']                 .= $token_desc;
      $form['tokens'] = array(
        '#theme'  => 'token_tree' , 
        '#token_types'  => array('node' , 'user' , 'term') , 
      );
    }
    
    if (isset($form['yelp-settings']['location']['addressfield'])) {
      //remove 'cc'  , will use the addressfield version
      unset($form['yelp-settings']['cc']);
      $form['yelp-settings']['location']['addressfield']['#default_value']= array('country' => variable_get('yelp_api_default_cc' , 'US'));
    }
    //passed a yelp_object? add it's values to the form
    if (is_object($yelp_object)) {
      $form['yelp-id'] = array(
        '#type'    => 'hidden' , 
        '#value'  => $yelp_object->yelp_id , 
      );
      if (isset($form['yelp-settings']['location']['addressfield']) && (strpos($yelp_object->yelp_settings->location , '|')>0)) {
        //set address
        $location = explode('|' , $yelp_object->yelp_settings->location);
        $form['yelp-settings']['location']['addressfield']['#default_value'] = array(
          'thoroughfare'        => $location[0] , 
          'premise'              => $location[1] , 
          'locality'            => $location[2] , 
          'administrative_area'  => $location[3] , 
          'postal_code'          => $location[4] , 
          'country'              => $yelp_object->yelp_settings->cc , 
        );
      }
      else{
        $form['yelp-settings']['location']['address']['#value'] = trim(str_replace('|' , ' ' , $yelp_object->yelp_settings->location));
        $form['yelp-settings']['cc']['#value']                  = $yelp_object->yelp_settings->cc;
      }
      $form['yelp-settings']['radius']['#value']              = $yelp_object->yelp_settings->radius;
      $form['yelp-settings']['max_results']['#value']          = $yelp_object->yelp_settings->max_results;
      $form['yelp-settings']['lang']['#value']                = $yelp_object->yelp_settings->lang;
    }
  
    //add custom validate function
    $form['#validate'][] = 'yelp_api_form_fields_validate';
    //add custom submit function
    //$form['submit'][]    = 'yelp_api_form_fields_submit';
    return $form;
  }
  
  /**
   * Validation function for individual yelp_api form fields. Invoked by submitting a form with fields from
   * yelp_api_form_fields(). Display error message if values passed are incorrect.
   *
   * @param array $element
   *  A form field array
   *
   * @param array &$form_state
   *  The form state passed by reference.
   *
   * @return nothing
   *  This function returns nothing.
   *
   * @see yelp_api_form_fields()
   */
  function yelp_api_form_field_validate($element  , &$form_state) {
    $field   = isset($element['#name']) ? $element['#name']:str_replace('edit-' , '' , $element['#id']);
    //dont know why $element['#value'] is wrong  , need to use value from form_state['input'];
    $value  = isset($form_state['input'][$field]) ? $form_state['input'][$field]:FALSE;
    $error  = FALSE;
    switch ($field) {
      case 'radius':
        //provided a token value?
        if (module_exists('token') && preg_match('/^\[([^\]]+)\]$/' , $value)) {
          //assume token provided (meets structure)
        //else  , must be an intval
        }
        elseif ((intval($value)!= $value)||($value>50)) {
          $error = t('Must provide an integer value between 1-50');
        }
        break;  //provided a token value?
      case 'max_results':
        if (module_exists('token') && preg_match('/^\[([^\]]+)\]$/' , $value)) {
          //assume token provided (meets structure)
        //else  , must be an intval
        }
        elseif ((intval($value)!= $value)||($value>20)) {
          $error = t('Must provide an integer value between 1-20');
        }
        break;
      case 'address':
        //must provide a value
        if (drupal_strlen(trim($value))<1) {
          $error = t('Must provide an address');
        }
        break;
      case 'addressfield':
        // must at least provide a postal_code
        // addressfield should take care of the value validation
        if (drupal_strlen(trim($value['address']['postal_code']))<1) {
          $error = t('Must provide at least a postal_code');
        }
        //make sure country is selected
        if (drupal_strlen(trim($value['address']['country']))<1) {
          $error = 'Must select a country . ';
        //make sure it is a valid country
        }
        elseif (!in_array($value['address']['country'] , array('US' , 'CA' , 'UK' , 'GB'))) {
          $error = t('Must select a valid country (United States  , Canada or United Kingdom)');
        }
        break;
      case 'cc':
        if (module_exists('token') && preg_match('/^\[([^\]]+)\]$/' , $value)) {
          //assume a token was provided.
        }
        elseif (!in_array($value , array('US' , 'CA' , 'UK' , 'GB'))) {
          $error = t('Must provide a valid country code (2 char code)');
        }
        break;
      case 'lang':
        module_load_include('inc' , 'yelp_api' , 'yelp_api.pages');
        $langs = _yelp_api_lang_codes();
        if (module_exists('token') && preg_match('/^\[([^\]]+)\]$/' , $value)) {
          //assume a token was provided.
        }
        elseif (!isset($langs[$value])) {
          $error = t('Must provide a valid language code');
        }
        break;
      case 'yelp-categories':
        // make sure category was choosen
        module_load_include('inc' , 'yelp_api' , 'yelp_api.categories');
        $value = array('values' => $form_state['input']);
        $selected_categories = yelp_api_selected_form_categories($value);
        if ($selected_categories && (sizeof($selected_categories)>0)) {
          //categories were selected
        }
        else{
          $error = t('Must select at least one category');
        }
    }
    
    if ($error) {
      form_set_error($field , $error);
    }
  }
  
  /**
   * Validates all form fields invoked from yelp_api_form_fields(). Displays form errors if there are any.
   *
   * @param array $form
   *  a Drupal form array
   * @param array &$form_state
   *  the state of the form, passed by reference.
   *
   * @return nothing
   *  this function returns nothing.
   *
   * @see yelp_api_form_fields()
   */
  function yelp_api_form_fields_validate($form , &$form_state) {
    $values  = $form_state['values'];
    //make sure an address was provided.
    if (yelp_api_use_addressfield()) {
        // must at least provide a postal_code
        // addressfield should take care of the value validation
        if (drupal_strlen(trim($values['addressfield']['address']['postal_code']))<1) {
          form_set_error('addressfield' , 'Must provide at least a postal_code');
        }
        //make sure country is selected
        if (drupal_strlen(trim($values['addressfield']['address']['country']))<1) {
          form_set_error('addressfield' , 'Must select a country . ');
        //make sure it is a valid country
        }
        elseif (!in_array($values['addressfield']['address']['country'] , array('US' , 'CA' , 'UK' , 'GB'))) {
          form_set_error('addressfield' , 'Must select a valid country (United States  , Canada or United Kingdom)');
        }
      //set the CC value
      $values['cc'] = $values['addressfield']['address']['country'];
      //check address field values
    }
    elseif (drupal_strlen(trim($values['address']))<1) {
      form_set_error('address' , t('Must provide an address.'));
    }
    
    // make sure radius is a int (if not token)
    $radius  = $values['radius'];
    if (module_exists('token') && preg_match('/^\[([^\]]+)\]$/' , $radius)) {
      //assume a token was provided.
    }
    elseif (intval($radius)!=$radius) {
      form_set_error('radius'  , t('Must provide a integer between 1-50'));
    }
    
    //max sure max is an int (if not token)
    $max = $values['max_results'];
    if (module_exists('token') && preg_match('/^\[([^\]]+)\]$/' , $max)) {
      //assume a token value was provided
    }
    elseif ((intval($max)!=$max)||($max>20)||($max<1)) {
      form_set_error('max_results' , t('Must provide a integer between 1-20'));
    }
    
    // make sure valid country code was provided
    $cc = $values['cc'];
    if (module_exists('token') && preg_match('/^\[([^\]]+)\]$/' , $radius)) {
        //assume a token was provided.
    }
    elseif (!in_array($cc , array('US' , 'CA' , 'UK' , 'GB'))) {
      form_set_error('cc' , t('Must provide a valid country code (2 char code)'));
    }
    
    // make sure valid lang was provided
    $lang = $values['lang'];
    module_load_include('inc' , 'yelp_api' , 'yelp_api.pages');
    $langs = _yelp_api_lang_codes();
    if (module_exists('token') && preg_match('/^\[([^\]]+)\]$/' , $lang)) {
      //assume a token was provided.
    }
    elseif (!isset($langs[$lang])) {
      form_set_error('lang' , t('Must provide a valid language code'));
    }
    
    // make sure category was choosen
    module_load_include('inc' , 'yelp_api' , 'yelp_api.categories');
    $selected_categories = yelp_api_selected_form_categories($form_state);
    if ($selected_categories && (sizeof($selected_categories)>0)) {
      //categories were selected
    }
    else{
      form_set_error('categories' , t('Must select at least one category'));
    }
  }
  
  /**
   * Submission function for yelp_api form fields. Saves the submitted Yelp API search data.
   * Invoked by yelp_api_form_fields()
   *
   * @param array $form_state
   *  The state of the submitted form, contains submitted values
   *
   * @return int
   *  Returns the yelp_id of the saved yelp object, or boolean FALSE.
   *
   * @see yelp_api_form_fields()
   */
  function yelp_api_form_fields_submit($form_state) {
    $data = array(
      'radius'      => $form_state['values']['radius'] , 
      'max_results'  => $form_state['values']['max_results'] , 
      'lang'        => $form_state['values']['lang'] , 
      'categories'  => serialize(yelp_api_selected_form_categories($form_state)) , 
    );
    //get location, use address field values?
    if (yelp_api_use_addressfield() && isset($form_state['values']['addressfield'])) {
      $location = array(
        $form_state['values']['addressfield']['address']['thoroughfare'] , 
        $form_state['values']['addressfield']['address']['premise'] , 
        $form_state['values']['addressfield']['address']['locality'] , 
        $form_state['values']['addressfield']['address']['administrative_area'] , 
        $form_state['values']['addressfield']['address']['postal_code'] , 
      );
      $data['cc']        = $form_state['values']['addressfield']['address']['country'];
      $data['location']  = implode('|' , $location);
    }
    else{
      $data['cc']       = $form_state['values']['cc'];
      $data['location'] = $form_state['values']['address'];
    }  
    
    if (isset($form_state['values']['yelp-id'])) {
      $data['yelp-id'] = $form_state['values']['yelp-id'];
    }
    //save the submitted data
    return yelp_api_save_data($data);
  }