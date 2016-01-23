<?php

/**
 * @file
 * Describe hooks provided by the drupal_sync module.
 */

function hook_drupal_sync_prepare_types() {

}

function hook_drupal_sync_default_fields_info($fields) {

}

function hook_drupal_sync_types_build_form_alter($form, $types, $defaults = array()) {

}

function hook_drupal_sync_entity_fields_values_alter(&$values, $entity_remote, $entity_type) {

}

function hook_drupal_sync_error_notification() {

}

function hook_drupal_sync_entity_bundle_properties() {

}

function hook_drupal_sync_entity_field_types_info() {

}

function hook_drupal_sync_field_to_send_alter() {

}

function hook_drupal_sync_field_values_to_send_alter() {

}

function hook_drupal_sync_get_sync_types() {

}

function hook_drupal_sync_get_entity_type_id_alter($type_id, $entity_type, $entity_type_obj) {

}

function hook_drupal_sync_settings_submit_form_alter(&$data, $form_state) {

}

function hook_drupal_sync_entity_prepare_to_queue($entities_settings, $type, $entity, $op) {

}

function hook_drupal_sync_processings_incoming_operation_before($op_result, $entity, $context) {

}

function hook_drupal_sync_processings_incoming_operation_after($op_result, $entity, $context) {

}

function hook_drupal_sync_entity_add_field($field_key, $values, $local_field_type, $entity_local, $entity_remote, $entity_type) {
  
}