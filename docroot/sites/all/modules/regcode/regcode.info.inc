<?php

/**
 * @file
 * Entity property info integration for the regcode module.
 */

/**
 * Implements hook_entity_property_info().
 */
function regcode_entity_property_info() {
  $info = array();

  $info['regcode']['properties']['rid'] = array(
    'label' => t("Code ID"),
    'type' => 'integer',
    'description' => t("The unique ID of the registration code."),
    'schema field' => 'rid',
  );
  $info['regcode']['properties']['created'] = array(
    'label' => t("Date created"),
    'type' => 'date',
    'description' => t("The date the registration code was created."),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer registration codes',
    'schema field' => 'created',
  );
  $info['regcode']['properties']['code'] = array(
    'label' => t("The registration code"),
    'type' => 'string',
    'description' => t("The registration code string."),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer registration codes',
    'schema field' => 'code',
  );
  $info['regcode']['properties']['tags'] = array(
    'label' => t("Tags"),
    'type' => 'list<taxonomy_term>',
    'description' => t("The associated taxonomy terms."),
    'getter callback' => 'regcode_get_term_ids',
  );

  return $info;
}
