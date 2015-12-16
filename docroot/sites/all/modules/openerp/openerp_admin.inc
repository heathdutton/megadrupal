<?php

// $Id: openerp_admin.inc,v 1.1 2010/11/19 02:05:48 boogiebug Exp $

/**
 * @file
 *  Administration form for OpenERP Proxy API module
 *
 * Copyright (c) 2010 Pinacono Co., Ltd.
 */

/**
 * settings form
 */
function openerp_settings_form() {
  global $user;

  $proxy = OpenERPProxy::getProxy();

  // gathering information
 
  $ver = $proxy->request('db', 'server_version',array());
  $dbs = _openerp_get_dbs();



  if ( $proxy->login() == FALSE ) {
   // drupal_set_message(OpenERPProxy::message(), 'error');
   	
    $models = array();
   
  }
  else {
  
    $models = _openerp_get_models();
    
  }
 

  $url = variable_get('openerp.url', NULL);
  $db  = variable_get('openerp.database', NULL);

  $uid = variable_get('openerp.user', NULL);
  $pwd = OpenERPProxy::decrypt(variable_get('openerp.password', NULL));
  

  // set the form
  $form = array(
    'openerp' => array(
      '#type'        => 'fieldset',
      '#title'       => 'Server Configuration',
      '#collapsible' => TRUE,
      '#collapsed'   => ! empty($dbs),
      '#description' => t(
        '<h3>Current Server Information:</h3>'.
        '<p>'.
        '<div>Server version: <em>!version</em></div>'.
        '</p>',
        array(
          '!version'  => $ver,
        )),

      'page_size' => array(
        '#type'  => 'textfield',
        '#title' => t('Default Page Size'),
        '#size'  => 5,
        '#maxlength' => 5,
        '#default_value' => variable_get('openerp.default_page_size', 40),
        '#description'   => t('Specify the maximum row of data per page to be displayed in a list (tree) view, or "0" to use configuration from database.'),
      ),

      'theme' => array(
        '#type'  => 'textfield',
        '#title' => t('jQuery UI Theme for Widget'),
        '#size'  => 64,
        '#maxlength' => 256,
        '#default_value' => variable_get('openerp.theme', ''),
        '#description'   => t('Name of jQuery UI theme to apply to the UI widgets. Leave blank for none.'),
      ),

      'url' => array(
        '#type'  => 'textfield',
        '#title' => t('OpenERP XML-RPC Server URL'),
        '#size'  => 64,
        '#maxlength'     => 256,
        '#default_value' => variable_get('openerp.url', 'http://erp.server.com:8069/xmlrpc'),
        '#description'   => t('Specify the OpenERP XML-RPC Server URL. (e.g., http://erp.server.com:8069/xmlrpc'),
      ),

      'submit' => array(
        '#type'     => 'submit',
        '#value'    => 'Save Server Settings',
        '#validate' => array('openerp_server_validate'),
        '#submit'   => array('openerp_server_submit'),
      ),
    ),

    'user' => array(
      '#type'        => 'fieldset',
      '#title'       => 'Default User Configuration',
      '#collapsible' => TRUE,
      '#collapsed'   => empty($dbs) || (isset($db) && isset($uid) && isset($pwd)),
      '#description' => t('Default user to be used as an active user when the current Drupal user do not have an OpenERP account.'),

      'erp-dbname' => array(
        '#type'  => 'select',
        '#title' => t('OpenERP Database'),
        '#default_value' => variable_get('openerp.database', ''),
        '#options'       => (array) $dbs,
        '#description'   => t('Specify the OpenERP database name.'),
      ),

      'erp-user' => array(
        '#type'  => 'textfield',
        '#title' => t('OpenERP User'),
        '#size'  => 30,
        '#maxlength'     => 256,
        '#default_value' => variable_get('openerp.user', ''),
        '#description'   => t('Specify the OpenERP user name.'),
      ),

      'erp-passwd' => array(
        '#type'  => 'password',
        '#title' => t('OpenERP Password'),
        '#size'  => 15,
        '#description' => t('Specify the password for the OpenERP user. Leave both fields blank to keep the old password.'),
      ),

      'erp-passwd-confirm' => array(
        '#type'  => 'password',
        '#title' => t('Confirm Password'),
        '#size'  => 15,
        '#description' => t('Please re-enter the password to confirm.'),
      ),

      'submit' => array(
        '#type'     => 'submit',
        '#value'    => 'Save User Settings',
        '#validate' => array('openerp_user_validate'),
        '#submit'   => array('openerp_user_submit'),
      ),
    ),

    'workbench' => array(
      '#type'        => 'fieldset',
      '#title'       => t('Database Workbench'),
      '#weight'      => 99, // put it at the end of form
      '#collapsible' => TRUE,
      '#collapsed'   => empty($dbs) || empty($_SESSION['openerp.workbench.result']),
      '#description' => t('Provide quick access to the OpenERP server data.'),

      'model' => array(
        '#type'  => 'select',
        '#title' => t('Model Name'),
        '#options'     => (array) $models,
        '#description' => t('Select the OpenERP model name to retrieve the data'),
      ),

      'fields' => array(
        '#type'  => 'textfield',
        '#title' => t('Fields'),
        '#size'  => 64,
        '#maxlength' => 256,
        '#default_value' => '',
        '#description' => t('List of field(s) to retrieve, seperated by comma (,) or leave blank to get all fields.'),
      ),

      'filter' => array(
        '#type'  => 'textfield',
        '#title' => t('Filter Options'),
        '#size'  => 64,
        '#maxlength' => 256,
        '#default_value' => '',
        '#description' => t('Additional filter options in PHP code format, e.g., <em>array(\'field1\',\'=\',\'value1\'), array(\'field2\',\'=\',\'value2\'), ...</em>, etc.'),
      ),

      'offset' => array(
        '#type'  => 'textfield',
        '#title' => t('Offset'),
        '#size'  => 10,
        '#maxlength'     => 8,
        '#default_value' => 0,
        '#description'   => t('Specifiy the data offset'),
      ),

      'limit' => array(
        '#type'  => 'textfield',
        '#title' => t('Limit'),
        '#size'  => 10,
        '#maxlength'     => 8,
        '#default_value' => 10,
        '#description'   => t('Specifiy the data limit'),
      ),

      'submit' => array(
        '#type'     => 'submit',
        '#value'    => 'Retrieve Data',
        '#validate' => array('openerp_workbench_validate'),
        '#submit'   => array('openerp_workbench_submit'),
      ),
    ),
  );

  if ( ! empty($_SESSION['openerp.workbench.result']) ) {
    $form['workbench']['result'] = array(
      '#type'        => 'fieldset',
      '#title'       => 'Database Workbench result',
      '#collapsible' => TRUE,
      '#collapsed'   => FALSE,
      '#description' => t('Result from the Database Workbench Query.'),

      'data' => array(
        '#prefix' => '<div id="openerp-workbench-result-data">',
        '#markup'  => $_SESSION['openerp.workbench.result'],
        '#suffix' => '</div>',
      ),
    );
  }

  $_SESSION['openerp.workbench.result'] = NULL;
  return $form;
}

/**
 * Validate server configuration
 */
function openerp_server_validate($form, &$form_data) {

  if ( parse_url($form_data['values']['url']) === FALSE ) {
    form_set_error('url', 'A valid URL is required.');
  }

  if ( ! is_numeric($form_data['values']['page_size']) || ((int) $form_data['values']['page_size'] != $form_data['values']['page_size']) ) {
    form_set_error('page_size', 'A valid integer is required.');
  }
  elseif ( ((int) $form_data['values']['page_size']) < 0 ) {
    form_set_error('page_size', 'Page size must be greater than or equal to 0');
  }

  if ( ! empty($form_data['values']['theme']) && ! is_dir(JQUERY_UI_PATH . "/themes/{$form_data['values']['theme']}") ) {
    form_set_error("Theme {$form_data['values']['theme']} is not existing");
  }
}

/**
 * Validate user configuration
 */
function openerp_user_validate($form, &$form_data) {

  $dbs = _openerp_get_dbs();

  if ( ! array_key_exists($form_data['values']['erp-dbname'], $dbs) ) {
    form_set_error('erp-dbname', 'Database not existing');
  }

  if ( empty($form_data['values']['erp-user']) ) {
    form_set_error('erp-user', 'User login id is required');
  }

  $passwd = OpenERPProxy::decrypt(variable_get('openerp.password', NULL));

  if ( ! empty($form_data['values']['erp-passwd']) ) {
    if ( $form_data['values']['erp-passwd'] != $form_data['values']['erp-passwd-confirm'] ) {
      form_set_error('erp-password', 'Password confirmation is not match the password field');
      return;
    }

    $password = $form_data['values']['erp-passwd'];
  }

  $proxy = OpenERPProxy::getProxy();


  if ( $proxy->request('common', 'login', array($form_data['values']['erp-dbname'], $form_data['values']['erp-user'], $password)) === FALSE ) {
    form_set_error('erp-user', 'OpenERP cannot authenticate user');
  }
}

/**
 * Validate workbench parameters
 */
function openerp_workbench_validate($form, &$form_data) {
  $models = _openerp_get_models();


  if ( ! array_key_exists($form_data['values']['model'], $models) ) {
    form_set_error('model', "Model {$form_data['values']['model']} is not existing");
  }

  if ( ! is_numeric($form_data['values']['offset']) ) {
    form_set_error('offset', "Offset must be a valid integer");
  }

  if ( ! is_numeric($form_data['values']['limit']) || ($form_data['values']['limit'] < 0) ) {
    form_set_error('limit', "Limit must be an integer and must be larger than 0");
  }

}

/**
 * Save server configurations
 */
function openerp_server_submit($form, &$form_data) {
  
  variable_set('openerp.url', $form_data['values']['url']);
  variable_set('openerp.default_page_size', $form_data['values']['page_size']);
  variable_set('openerp.theme', $form_data['values']['theme']);
  drupal_set_message(t('Server configurations are successfully saved.'));
}

/**
 * Save OpenERP user configurations
 */
function openerp_user_submit($form, &$form_data) {

  if ( ! empty($form_data['values']['erp-passwd']) ) {
    variable_set('openerp.password', OpenERPProxy::encrypt($form_data['values']['erp-passwd']));
  }
  variable_set('openerp.user',     $form_data['values']['erp-user']);
  variable_set('openerp.database', $form_data['values']['erp-dbname']);
  drupal_set_message(t('OpenERP user configurations are successfully saved.'));
}

/**
 * workbench handle
 */
function openerp_workbench_submit($form, &$form_data) {
	
  $filter = array();
  $fields = array();
 
  $proxy  = OpenERPProxy::getProxy();


  if ( ! empty($form_data['values']['filter']) ) {
    $filter = eval("return array({$form_data['values']['filter']});");
  }

  if ( ! empty($form_data['values']['fields']) ) {
    $fields = explode(',', $form_data['values']['fields']);
  }

  try {
    if ( ($r = $proxy->search($form_data['values']['model'], $filter, (int) $form_data['values']['offset'], (int) $form_data['values']['limit'])) === FALSE ) {
       
    	throw new Exception(join("\n", OpenERPProxy::message()));
      
    }

    if ( ($r = $proxy->read($form_data['values']['model'], $r, $fields)) === FALSE ) {
    	
      throw new Exception(join("\n", OpenERPProxy::message()));
    }

    if ( count($r) <= 0 ) {
    	
      throw new Exception("There is not data for the model <em>{$form_data['values']['model']}</em>");
    }
  } catch ( Exception $e ) {
  	
    drupal_set_message($e->getMessage(), 'error');
    return;
  }

  // dump data in a table
  $msg  = "<h3>{$form_data['values']['model']}</h3>".
          "<table>";

  // header
  $msg .= '<tr>';
  foreach ( $r[0] as $name => $val ) {
    $msg .= "<th>$name</th>";
  }
  $msg .= '</tr>';

  // data
  foreach ( $r as $data ) {
    $msg .= '<tr>';
    foreach ( $data as $name => $val ) {
      $msg .= "<td>";
      $msg .= htmlentities( is_array($val) ? implode(', ', $val) :  $val, ENT_QUOTES );
      $msg .= "</td>";
    }
    $msg .= '</tr>';
  }

  $msg .= '</table>';
  $_SESSION['openerp.workbench.result'] = $msg;
}


