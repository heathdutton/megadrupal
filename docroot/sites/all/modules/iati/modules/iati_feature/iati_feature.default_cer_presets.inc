<?php
/**
 * @file
 * iati_feature.default_cer_presets.inc
 */

/**
 * Implements hook_default_cer().
 */
function iati_feature_default_cer() {
  $export = array();

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->entity_types_content_fields = 'iati_budget*iati_budget*field_iati_activity_ref*node*iati_activity*field_iati_activity_budget';
  $cnr_obj->enabled = 1;
  $export['iati_budget*iati_budget*field_iati_activity_ref*node*iati_activity*field_iati_activity_budget'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->entity_types_content_fields = 'iati_budget*iati_budget*field_iati_org_ref*node*iati_organisation*field_recipient_country_budget';
  $cnr_obj->enabled = 1;
  $export['iati_budget*iati_budget*field_iati_org_ref*node*iati_organisation*field_recipient_country_budget'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->entity_types_content_fields = 'iati_budget*iati_budget*field_iati_org_ref*node*iati_organisation*field_recipient_org_budget';
  $cnr_obj->enabled = 1;
  $export['iati_budget*iati_budget*field_iati_org_ref*node*iati_organisation*field_recipient_org_budget'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->entity_types_content_fields = 'iati_budget*iati_budget*field_iati_org_ref*node*iati_organisation*field_total_budget';
  $cnr_obj->enabled = 1;
  $export['iati_budget*iati_budget*field_iati_org_ref*node*iati_organisation*field_total_budget'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->entity_types_content_fields = 'iati_location*iati_location*field_iati_activity_ref*node*iati_activity*field_iati_location';
  $cnr_obj->enabled = 1;
  $export['iati_location*iati_location*field_iati_activity_ref*node*iati_activity*field_iati_location'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->entity_types_content_fields = 'node*iati_activity*field_iati_activity_budget*iati_budget*iati_budget*field_iati_activity_ref';
  $cnr_obj->enabled = 1;
  $export['node*iati_activity*field_iati_activity_budget*iati_budget*iati_budget*field_iati_activity_ref'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->entity_types_content_fields = 'node*iati_activity*field_iati_location*iati_location*iati_location*field_iati_activity_ref';
  $cnr_obj->enabled = 1;
  $export['node*iati_activity*field_iati_location*iati_location*iati_location*field_iati_activity_ref'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->entity_types_content_fields = 'node*iati_organisation*field_recipient_country_budget*iati_budget*iati_budget*field_iati_org_ref';
  $cnr_obj->enabled = 1;
  $export['node*iati_organisation*field_recipient_country_budget*iati_budget*iati_budget*field_iati_org_ref'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->entity_types_content_fields = 'node*iati_organisation*field_recipient_org_budget*iati_budget*iati_budget*field_iati_org_ref';
  $cnr_obj->enabled = 1;
  $export['node*iati_organisation*field_recipient_org_budget*iati_budget*iati_budget*field_iati_org_ref'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->entity_types_content_fields = 'node*iati_organisation*field_total_budget*iati_budget*iati_budget*field_iati_org_ref';
  $cnr_obj->enabled = 1;
  $export['node*iati_organisation*field_total_budget*iati_budget*iati_budget*field_iati_org_ref'] = $cnr_obj;

  return $export;
}
