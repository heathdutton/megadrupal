<?php

/**
 * Builds a new Apester settings builder
 * @return ApesterSettingsBuilder
 */
function apester_get_settings_builder() {
    return new ApesterSettingsBuilder(new ApesterFormBuilder());
}

/**
 * Apester settings form builder
 */
function apester_settings_admin() {
    $settings_builder = apester_get_settings_builder();

    return $settings_builder->build();
}

/**
 * Implements form validate hook
 */
function apester_settings_admin_validate($form, &$form_state) {
    if (apester_validate_token($form_state['values']['auth_token'])) {
        return;
    }

    form_set_error('auth_token', t('Authentication token is invalid'));
}

/**
 * Implements form submit hook post validation
 */
function apester_settings_admin_submit($form, &$form_state) {
    variable_set(APESTER_AUTH_VAR, $form_state['values']['auth_token']);

    drupal_set_message('Apester settings were saved successfully');
}