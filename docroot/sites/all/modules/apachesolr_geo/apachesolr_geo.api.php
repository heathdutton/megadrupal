<?php

/**
 * @file
 * API documentation for apachesolr_geo module.
 */

/**
 * By default, the form element for a geospatial search centerpoint is just a
 * text field, which expects a value of "{latitute},{longitude}". If you wish to
 * use a geocoder, you can alter the form element using this alter, then
 * implement hook_apachesolr_geo_save_query_settings_alter() to invoke your
 * geocoder and place the proper value into this field.
 * 
 * The $form_settings are provided so you can establish coherent default values.
 */
function hook_apachesolr_geo_center_form_element_alter(&$form_element, $form_settings = NULL) {}

/**
 * Used in conjunction with hook_apachesolr_geo_center_form_element_alter().
 * 
 * When implementing this hook, you should make sure that the value in the
 * $settings[$delta] is in the expected format of "{latitute},{longitude}". You can
 * then save the input into a separate index of the $settings array, and it will
 * be preserved.
 */
function hook_apachesolr_geo_save_query_settings_alter(&$settings, $delta){}
