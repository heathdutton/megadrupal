<?php
/**
 * @file
 * Entity metadata information integration.
 */

/**
 * Implements hook_entity_property_info_alter().
 */
function party_service_entity_property_info_alter(&$info) {
  $properties = &$info['party_service_key']['properties'];
  $properties['apikey']['label'] = t('API Key');
  $properties['from_domain']['label'] = t('Domain');
  $properties['from_domain']['description'] = t('The domain that this API key may be used from. Leave blank to allow all domains');
  $properties['service']['type'] = 'party_service';
  $properties['owner']['type'] = 'party';
  $properties['status']['type'] = 'boolean';
}
