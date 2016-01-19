<?php

function freeipmi_admin() {
    return '';
}

function freeipmi_admin_chassis() {
    drupal_set_title('IPMI chassis management');

    $form = array();
    $form['hostname'] = array(
        '#type' => 'textfield',
        '#title' => 'Hostname / IP address',
        '#default_value' => variable_get('freeipmi_hostname', ''),
        '#required' => true,
    );
    $form['username'] = array(
        '#type' => 'textfield',
        '#title' => 'Username',
        '#default_value' => variable_get('freeipmi_username', ''),
        '#required' => true,
    );
    $form['password'] = array(
        '#type' => 'textfield',
        '#title' => 'Password',
        '#default_value' => variable_get('freeipmi_password', ''),
        '#required' => true,
    );
    $form['operation'] = array(
        '#type' => 'radios',
        '#title' => 'Operation',
        '#required' => true,
        '#options' => array(
            'get-chassis-capabilities' => 'Get chassis capabilities',
            'get-chassis-status' => 'Get chassis status',
            'get-system-restart-cause' => 'Get system restart cause',
            'get-power-on-hours-counter' => 'Get power on hours (POH) counter',
        ),
        '#default_value' => variable_get('freeipmi_last_chassis_operation', 'get-chassis-capabilities'),
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Submit',
    );

/*
      --chassis-control=CONTROL   Control the chassis.
      --chassis-identify=IDENTIFY Set chassis Identification.
*/

    return $form;
}

function freeipmi_admin_chassis_submit(&$form, &$form_state) {
    switch ($form_state['values']['operation']) {
        case 'get-chassis-capabilities':
        case 'get-chassis-status':
        case 'get-system-restart-cause':
        case 'get-power-on-hours-counter':
            $command = 'ipmi-chassis --' . escapeshellarg($form_state['values']['operation'])
                     . ' --hostname=' . escapeshellarg($form_state['values']['hostname'])
                     . ' --username=' . escapeshellarg($form_state['values']['username'])
                     . ' --password=' . escapeshellarg($form_state['values']['password']);
            break;

        default:
            drupal_set_message('Unknown operation.', 'error');
            return;
    }

    variable_set('freeipmi_last_chassis_operation', $form_state['values']['operation']);

    $command = escapeshellcmd($command);
    $output = shell_exec($command);
    drupal_set_message('<pre>' . $output . '</pre>');
}

function freeipmi_admin_power() {
    drupal_set_title('IPMI power management');

    $form = array();
    $form['hostname'] = array(
        '#type' => 'textfield',
        '#title' => 'Hostname / IP address',
        '#default_value' => variable_get('freeipmi_hostname', ''),
        '#required' => true,
    );
    $form['username'] = array(
        '#type' => 'textfield',
        '#title' => 'Username',
        '#default_value' => variable_get('freeipmi_username', ''),
        '#required' => true,
    );
    $form['password'] = array(
        '#type' => 'textfield',
        '#title' => 'Password',
        '#default_value' => variable_get('freeipmi_password', ''),
        '#required' => true,
    );
    $form['operation'] = array(
        '#type' => 'radios',
        '#title' => 'Operation',
        '#required' => true,
        '#options' => array(
            'stat' => 'Get power status of the target host',
            'on' => 'Power on the target host',
            'off' => 'Power off the target host',
            'cycle' => 'Power cycle the target host',
            'reset' => 'Reset the target host',
            'pulse' => 'Send power diagnostic interrupt to target host',
            'soft' => 'Initiate a soft-shutdown of the OS via ACPI',
            'on-if-off' => 'Issue a power on command instead of a power cycle or hard reset command if the remote machine\'s power is currently off',
            'wait-until-off' => 'Regularly query the remote BMC and return only after the machine has powered off',
            'wait-until-on' => 'Regularly query the remote BMC and return only after the machine has powered on',
        ),
        '#default_value' => variable_get('freeipmi_last_power_operation', 'stat'),
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Submit',
    );

    return $form;
}

function freeipmi_admin_power_submit(&$form, &$form_state) {
    switch ($form_state['values']['operation']) {
        case 'stat':
        case 'on':
        case 'off':
        case 'cycle':
        case 'reset':
        case 'pulse':
        case 'soft':
        case 'on-if-off':
        case 'wait-until-off':
        case 'wait-until-on':
            $command = 'ipmipower --' . escapeshellarg($form_state['values']['operation'])
                     . ' --hostname=' . escapeshellarg($form_state['values']['hostname'])
                     . ' --username=' . escapeshellarg($form_state['values']['username'])
                     . ' --password=' . escapeshellarg($form_state['values']['password']);
            break;

        default:
            drupal_set_message('Unknown operation.', 'error');
            return;
    }

    variable_set('freeipmi_last_power_operation', $form_state['values']['operation']);

    $command = escapeshellcmd($command);
    $output = shell_exec($command);
    drupal_set_message('<pre>' . $output . '</pre>');
}
