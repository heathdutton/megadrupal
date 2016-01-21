<?php
/**
 * @file
 * Provides an HPCloud destination for the Backup and Migrate module.
 */

// Ugh. Dependency resolvers...
require_once drupal_get_path('module', 'backup_migrate') . '/includes/destinations.file.inc';

/**
 * Provides stream wrapper-based access to Backup and Migrate.
 *
 * This extends the existing file implementation, changing only the
 * fact that the "realpath" is a remote URL, and not a file system
 * path.
 */
class BackupMigrateHPCloud extends backup_migrate_destination_files {

  public function get_realpath() {
    return $this->get_location();
  }

  public function destinations() {
    $out = array();
    if (module_exists('backup_migrate_files')) {
      $opts = array(
        'destination_id' => 'hpcloud',
        'location' => 'hpcloud://',
        'name' => t('HPCloud (hpcloud://) files.'),
      );
      $out['hpcloud'] = backup_migrate_create_destination('filesource', $opts);
    }
    return $out;
  }

  public function edit_form() {

    $mapped = variable_get('hpcloud_enabled_schemes', NULL);

    if (empty($mapped)) {
      $params = array('!configure' => l('configure HPCloud wrappers', 'admin/config/services/hpcloud/wrappers'));
      drupal_set_message(t('No containers are mapped to HPCloud. You must !configure before backing up here.'), 'error');
    }

    $form = parent::edit_form();
    $settings = array(
      '%ex' => 'hpcloud://backups',
      '!setting' => l('Stream Wrapper Settings', 'admin/config/services/hpcloud/wrappers'),
    );
    $form['location']['#description'] = t('The HPCloud URL, for example %ex. To set the container, go to the !setting.', $settings);

    // No advanced form.
    unset($form['settings']['chmod']);
    unset($form['settings']['chgrp']);

    $params = array('!settings' => l('HPCloud authentication', 'admin/config/services/hpcloud'));
    $form['settings']['redirect'] = array(
      '#markup' => '<p>'
        . t('!settings must be configured. The current configuration is below.', $params)
        . '</p>'
        . $this->hpcloudInfoTable()
        . l('Test authentication', 'admin/config/services/hpcloud/test'),
        );

    // This is a dummy value to prevent the parent::edit_form_submit()
    // from choking.
    $form['settings']['chmod'] = array(
      '#type' => 'value',
      '#value' => 0,
    );
    return $form;
  }

  protected function hpcloudInfoTable() {
    $table = array(
      'header' => array(),
      'rows' => array(
        array('data' => array(t('Endpoint'), variable_get('hpcloud_account_endpoint','UNSPEFICIED'))),
        array('data' => array(t('Tenant ID'), variable_get('hpcloud_account_tenantid','UNSPECIFIED'))),
      ),
    );

    return theme('table', $table);
  }
}
