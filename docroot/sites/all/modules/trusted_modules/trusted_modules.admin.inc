<?php

function trusted_modules_admin() {
    module_load_include('inc', 'trusted_modules', 'trusted_modules.verify');
    module_load_include('php', 'trusted_modules', 'lib/vendor/autoload');
    drupal_add_css(drupal_get_path('module', 'trusted_modules') . '/css/trusted_modules.admin.css');
    drupal_add_js(drupal_get_path('module', 'trusted_modules') . '/lib/jquery.tablesorter.min.js');
    drupal_add_js(drupal_get_path('module', 'trusted_modules') . '/js/trusted_modules.admin.js');

    $modules = trusted_modules_get_status();
    $rows = array();

    foreach ($modules as $name => $status) {
        $row = array();
        $row['name'] = '<span class="name">' . $name . '</span>';

        if ($status['trusted']) {
            $row['trusted'] = '<span class="trusted success">' . t('Yes') . '</span>';
            $row['error'] = '';
        } else {
            $row['trusted'] = '<span class="trusted failure">' . t('No') . '</span>';
            $row['error'] = '<span class="error">' . nl2br($status['error']) . '</span>';
        }

        $rows[$name] = $row;
    }

    $form['status'] = array(
        '#type' => 'tableselect',
        '#header' => array('name' => t('Name'), 'trusted' => t('Trusted'), 'error' => t('Status')),
        '#options' => $rows,
        '#empty' => t('No enabled modules.'),
        '#attributes' => array('class' => array('tablesorter')),
    );

    return $form;
}

function trusted_modules_admin_settings() {
    $form = array();
    $form['trusted_modules_disable_on_install'] = array(
        '#title' => t('Disable untrusted modules on install'),
        '#description' => t('Disable an untrusted module if it is enabled.'),
        '#type' => 'select',
        '#options' => array('nothing' => 'Do nothing', 'report' => 'Report only', 'disable' => 'Disable automatically and report'),
        '#default_value' => variable_get('trusted_modules_disable_on_install'),
    );
    $form['trusted_modules_watchdog_level'] = array(
        '#title' => t('Watchdog level'),
        '#description' => t('Set the level of alerts.'),
        '#type' => 'select',
        '#options' => watchdog_severity_levels(),
        '#default_value' => variable_get('trusted_modules_watchdog_level'),
        '#states' => array(
          'invisible' => array(
            'select[name="trusted_modules_disable_on_install"]' => array('value' => 'nothing'),
          ),
        ),

    );

    return system_settings_form($form);
}

function trusted_modules_admin_certificates() {
    $form = array();
    $form['drupal'] = array(
        '#type' => 'fieldset',
        '#title' => 'Drupal Certificates',
    );

    $drupal_cert = variable_get('trusted_modules_certificate_drupal_org', '');
    if (!empty($drupal_cert)) {
        $form['drupal']['drupal_cert'] = array(
            '#type' => 'item',
            '#title' => 'Drupal.org Certificate',
            '#markup' => '<p><b>Certificate Status:</b> Trusted</p><pre>' . check_plain($drupal_cert) . '</pre>',
        );
    }

    $form['drupal']['download_drupal_cert'] = array(
        '#type' => 'submit',
        '#value' => t('Download Drupal.org certificate'),
    );

    $form['modules'] = array(
        '#type' => 'fieldset',
        '#title' => 'Module Certificates',
    );

    module_load_include('inc', 'trusted_modules', 'trusted_modules.verify');
    $modules = system_list('module_enabled');
    foreach ($modules as $module) {
        $file = dirname(realpath($module->filename)) . '/.module.cert.pem';
        if (file_exists($file)) {
            $verify_module = new VerifyModule(dirname(realpath($module->filename)));
            try {
                $verify_module->verify_module_cert();
                $status = 'Trusted';
            } catch (Exception $e) {
                $status = '<span class="error">' . $e->getMessage() . '</span>';
            }

            $form['modules'][$module->name] = array(
                '#type' => 'item',
                '#title' => $module->info['name'],
                '#markup' => '<p><b>Certificate Status:</b> ' . $status . '</p><pre>' . check_plain(trim(file_get_contents($file))) . '</pre>',
            );
        }
    }

    return $form;
}

function trusted_modules_admin_certificates_submit(&$form, &$form_state) {
    if ($form_state['clicked_button']['#parents'][0] == 'download_drupal_cert') {
        $certificate = trusted_modules_fetch_drupal_certificate();
        if (!empty($certificate)) {
            variable_set('trusted_modules_certificate_drupal_org', check_plain($certificate));
            drupal_set_message('Downloaded Drupal.org certificate successfully.');
        }
    }
}
