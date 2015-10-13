<?php
/**
 * Implementation of Admin Settings form for OTRS Connection Configuration 
 */
function otrs_config_form($form, &$form_state) {
  global $databases;
  
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Define connection information for OTRS server'),
  );

  $form['connectionsoap'] = array(
    '#type' => 'fieldset',
    '#title' => t('OTRS SOAP Connection'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['connectionsoap']['otrssoapusername'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#required' => TRUE,
    '#default_value' => variable_get('otrssoapusername',''),
    '#description' => "Please enter username of OTRS soap user.",
  );
  $form['connectionsoap']['otrssoappassword'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#required' => TRUE,
    '#default_value' => variable_get('otrssoappassword',''),
    '#description' => "Please enter password of OTRS soap user.",
  );
  $form['connectionsoap']['otrssoapurl'] = array(
    '#type' => 'textfield',
    '#title' => "Connection URL",
    '#required' => TRUE,
    '#default_value' => variable_get('otrssoapurl',''),
    '#description' => 'Format is "http://xxxxxxxxxx/otrs/rpc.pl"',
  ); 

  // Following code is for establishing OTRS MySQL Database connection, 
  // This is for future use where SOAP calls are inefficient and data is recieved through MySQL queries.

  /*
  $form['connectionmysql'] = array(
    '#type' => 'fieldset',
    '#title' => t('OTRS MySQL Connection'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $connected = "No Connection established";
  foreach($databases as $database => $dbconnection) {
    if($database == variable_get('otrsdb','otrs')) {
      db_set_active($database);
      try {
        db_query('select 1 from {customer_user}');
        $connected = "Connection successful";
      }
      catch(Exception $e)
      {
        $connected .= " ".$e->getMessage();
      }
      db_set_active();
    }
  }

  $form['connectionmysql']['otrsdb'] = array(
    '#type' => 'textfield',
    '#title' => t('MySQL Connection'),
    '#default_value' => variable_get('otrsdb','otrs'),
    '#description' => "Auto detect OTRS MySQL connection details from settings.php, $connected",
  );
  */
  return system_settings_form($form);
}

