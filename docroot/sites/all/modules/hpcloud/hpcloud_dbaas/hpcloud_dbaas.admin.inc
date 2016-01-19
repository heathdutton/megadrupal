<?php
/**
 * @file
 * Administration for Drupal DBaaS.
 */

function hpcloud_dbaas_admin_settings() {
  $form = array();
  return system_settings_form($form);
}

function hpcloud_dbaas_admin_instance_list_form($form, &$form_state) {
  $form['create'] = array(
    '#type' => 'fieldset',
    '#title' => t('Create a New Database'),
    // '#description' => t('short description'),
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
    //'#group' => 'group_name', //for vertical tab grouping.
  );

  $form['create']['dbname'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('The name of the new database instance.'),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
    //'#default_value' => 'some string',
  );
  $form['create']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create'),
  );

  $form['listing'] = array(
    '#markup' => theme_table(hpcloud_dbaas_admin_instance_list()),
  );
  return $form;
}
function hpcloud_dbaas_admin_instance_list_form_submit($form, &$form_state) {

  if (empty($form['values']['dbname'])) {
    drupal_set_message('Could not find DB name.', 'error');
    return;
  }

  $id = _hpcloud_login();
  //$tname = variable_get('hpcloud_account_tenantname');
  //$dbaas = \HPCloud\Services\DBaaS::newFromServiceCatalog($id->serviceCatalog, $id->token(), $tname);
  $dbaas = \HPCloud\Services\DBaaS::newFromIdentity($id);

  $details = $dbaas->instance()->create($form['values']['dbname']);

  $op = array(
    '%dsn' => $details->dsn(),
    '%user' => $details->username(),
    '%pass' => $details->password(),
  );
  $msg = t('Your new database is at %dsn with username %user and password %pass', $op);
  drupal_set_message($msg);
}

function hpcloud_dbaas_admin_instance_list() {
  $id = _hpcloud_login(TRUE);
  //$tname = variable_get('hpcloud_account_tenantname');
  //$dbaas = \HPCloud\Services\DBaaS::newFromServiceCatalog($id['catalog'], $id['token'], $tname);
  $dbaas = \HPCloud\Services\DBaaS::newFromIdentity($id);

  $list = $dbaas->instance()->listInstances();

  $table = array(
    '#theme' => 'table',
    'header' => array('name', 'id', 'status', 'host'),
    'caption' => NULL,
    'rows' => array(),
    'colgroups' => array(),
    'sticky' => FALSE,
    'empty' => t('There are no database instances for this account.'),
    'attributes' => array(),
  );

  foreach ($list as $inst) {
    $delete = 'admin/config/services/hpcloud-dbaas/instance/delete/' . $inst->id();
    $table['rows'][] = array( 'data' => array(
      array('data' => check_plain($inst->name())),
      array('data' => check_plain($inst->id())),
      array('data' => check_plain($inst->status())),
      array('data' => check_plain($inst->hostname())),
      array('data' => l($delete, 'Delete')),
    ));
  }

  return $table;
}

function hpcloud_dbaas_admin_snapshot_list_form($form, &$form_state) {
  $form['create'] = array(
    '#type' => 'fieldset',
    '#title' => t('Create a New Database Snapshot'),
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
  );
  $form['create']['snapshot'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('The name of the new database snapshot.'),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
    //'#default_value' => 'some string',
  );
  $form['create']['instance'] = array(
    '#type' => 'select',
    '#title' => t('Database'),
    '#description' => t('The database instance to snapshot.'),
    '#required' => TRUE,
    '#options' => array('FOO'),
    //'#default_value' => 'some string',
  );
  $form['create']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create'),
  );

  $form['listing'] = array(
    '#markup' => theme_table(hpcloud_dbaas_admin_snapshot_list()),
  );
  return $form;
}
function hpcloud_dbaas_admin_snapshot_list() {
  $table = array(
    '#theme' => 'table',
    'header' => array('name', 'id', 'date', 'instance id'),
    'caption' => NULL,
    'rows' => array(),
    'colgroups' => array(),
    'sticky' => FALSE,
    'empty' => t('There are no database snapshots for this account.'),
    'attributes' => array(),
  );
  return $table;
}
