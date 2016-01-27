<?php

function phpbb_sso_admin() {
    $form = array();

    $form['phpbb'] = array(
        '#type' => 'fieldset',
        '#title' => 'phpBB Settings',
        '#collapsible' => true,
        '#collapsed' => false,
    );

    $form['phpbb']['phpbb_sso_phpbb_db'] = array(
        '#type' => 'textfield',
        '#title' => 'phpBB database',
        '#description' => 'The database name of your phpBB install. Leave blank to use the current Drupal database.',
        '#default_value' => variable_get('phpbb_sso_phpbb_db', ''),
    );

    $form['phpbb']['phpbb_sso_phpbb_table_prefix'] = array(
        '#type' => 'textfield',
        '#title' => 'phpBB table prefix',
        '#description' => 'The table prefix you chose when installing phpBB.'
                        . '<br/>Eg. Use <em>phpbb</em> if all your phpBB tables begin with <em>phpbb_</em>.',
        '#default_value' => variable_get('phpbb_sso_phpbb_table_prefix', ''),
    );

    $form['phpbb']['phpbb_sso_phpbb_path'] = array(
        '#type' => 'textfield',
        '#title' => 'phpBB path',
        '#description' => 'The directory path to your phpBB install, relative to your Drupal install.'
                        . '<br/>Eg. Use <em>phpbb3</em> if phpBB was installed into a directory named <em>phpbb3</em> and resides in your Drupal root directory.',
        '#default_value' => variable_get('phpbb_sso_phpbb_path', ''),
        '#required' => true,
    );

    $form['phpbb']['header'] = array(
        '#markup' => '<p><h3>Please note</h3>
<ol>
<li>If your Drupal website is located at <em>example.com</em>, phpBB must be located either at <em>example.com</em> or <em>subdomain.example.com</em>, but not <em>subdomain1.subdomain2.example.com</em>, or <em>other-website.com</em></li>
<li>Your Drupal website domain name <em>and</em> phpBB website domain name must contain a dot (.). For example, <em>localhost.localdomain</em> will work, but <em>localhost</em> would not work.</li>
</p>',
    );

    $form['debug'] = array(
        '#type' => 'fieldset',
        '#title' => 'Debugging',
        '#collapsible' => true,
        '#collapsed' => true,
    );

    $form['debug']['phpbb_sso_debug_enabled'] = array(
        '#type' => 'checkbox',
        '#title' => 'Enable debugging',
        '#description' => 'Enable or disable all debugging information.',
        '#default_value' => variable_get('phpbb_sso_debug_enabled', false),
    );

    $form['debug']['phpbb_sso_debug_screen'] = array(
        '#type' => 'checkbox',
        '#title' => 'Output: screen',
        '#description' => 'Print to screen.',
        '#default_value' => variable_get('phpbb_sso_debug_screen', false),
        '#states' => array(
          'invisible' => array(
            ':input[name="phpbb_sso_debug_enabled"]' => array('checked' => FALSE),
          ),
        ),
    );

    $form['debug']['phpbb_sso_debug_watchdog'] = array(
        '#type' => 'checkbox',
        '#title' => 'Output: watchdog',
        '#description' => 'Send to watchdog.',
        '#default_value' => variable_get('phpbb_sso_debug_watchdog', false),
        '#states' => array(
          'invisible' => array(
            ':input[name="phpbb_sso_debug_enabled"]' => array('checked' => FALSE),
          ),
        ),
    );

    $form['debug']['phpbb_sso_debug_devel'] = array(
        '#type' => 'checkbox',
        '#title' => 'Output: devel',
        '#description' => 'Use the devel module.',
        '#default_value' => variable_get('phpbb_sso_debug_devel', false),
        '#states' => array(
          'invisible' => array(
            ':input[name="phpbb_sso_debug_enabled"]' => array('checked' => FALSE),
          ),
        ),
    );

    return system_settings_form($form);
}

function phpbb_sso_admin_validate(&$form, &$form_state) {
    $db = $form_state['values']['phpbb_sso_phpbb_db'];
    if (!(phpbb_sso_admin_validate_db($db)))
        return;

    $table_prefix = $form_state['values']['phpbb_sso_phpbb_table_prefix'];
    if (!empty($table_prefix))
        $table_prefix .= '_';

    $known_table_name = 'users';
    if (!(phpbb_sso_admin_validate_table($db, $table_prefix . $known_table_name))) {
        return;
    }

    $path = $form_state['values']['phpbb_sso_phpbb_path'];
    if (!(phpbb_sso_admin_validate_path($path))) {
        return;
    }
}

function phpbb_sso_admin_validate_path($path) {
    if (!is_dir($path)) {
        form_set_error('phpbb_sso_phpbb_path', 'This path does not exist.');
        return false;
    }

    return true;
}

function phpbb_sso_admin_validate_table($db, $table) {
    $result = false;
    $conn = Database::getConnection();
    $settings = $conn->getConnectionOptions();
    if (!empty($db) && $settings['database'] != $db) {
        try {
            phpbb_sso_db_set_active($db);

            if (db_table_exists($table)) {
                $result = true;
            } else {
                form_set_error('phpbb_sso_phpbb_table_prefix', 'No phpBB tables could be found using this table prefix.');
            }
        }
        catch (Exception $e) {
            form_set_error('phpbb_sso_phpbb_db', 'Database error: ' . print_r($e->getMessage(), true));
        }
    } else {
        try {
            if (db_query("SELECT COUNT(*) FROM " . db_escape_table($table)))
                $result = true;
        } catch (Exception $e) {
            form_set_error('phpbb_sso_phpbb_table_prefix', 'No phpBB tables could be found using this table prefix.');
        }
    }

    db_set_active('default');
    return $result;
}

function phpbb_sso_admin_validate_db($db) {
    if (empty($db))
        return true;

    $result = false;
    try {
        phpbb_sso_db_set_active($db);
        Database::getConnection();
        $result = true;
    }
    catch (Exception $e) {
        if (preg_match('/Unknown database/i', $e->getMessage())) {
            form_set_error('phpbb_sso_phpbb_db', 'This database may not exist, or you do not have sufficient permissions to access the database.');
        } else {
            form_set_error('phpbb_sso_phpbb_db', 'Database error: %error.', array('%error' => print_r($e->getMessage(), true)));
        }
    }

    db_set_active('default');
    return $result;
}

function phpbb_sso_db_set_active($db) {
    $connection_info = Database::getConnectionInfo('default');
    $connection_info['default']['database'] = $db;
    Database::addConnectionInfo($db, 'default', $connection_info['default']);
    db_set_active($db);
}
