<?php
/**
 * @file
 * bassets_client_image_metadata.features.uuid_term.inc
 */

/**
 * Implements hook_uuid_features_default_terms().
 */
function bassets_client_image_metadata_uuid_features_default_terms() {
  $terms = array();

  $terms[] = array(
    'name' => 'Historienbild',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => '285e49e7-b519-4397-bf08-f2229bb7a55e',
    'vocabulary_machine_name' => 'image_motive_category',
  );
  $terms[] = array(
    'name' => 'Ereignisbild',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => '286aa1a7-d23a-43d4-bf88-fe3ffb703510',
    'vocabulary_machine_name' => 'image_motive_category',
  );
  $terms[] = array(
    'name' => 'Grafik',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => '378c3889-579a-4b78-a4d8-64790da991f5',
    'vocabulary_machine_name' => 'image_motive_type',
  );
  $terms[] = array(
    'name' => 'Alltagsbild',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => '4ceaa44c-c83b-424e-b44a-2cbdacb0229f',
    'vocabulary_machine_name' => 'image_motive_category',
  );
  $terms[] = array(
    'name' => 'Plakat',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => '4f1055b0-dba0-4c33-8dd9-4b79f49b381c',
    'vocabulary_machine_name' => 'image_motive_category',
  );
  $terms[] = array(
    'name' => 'Plastik',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => '4fb462f4-60ad-4392-86cf-4be393d25bfd',
    'vocabulary_machine_name' => 'image_motive_type',
  );
  $terms[] = array(
    'name' => 'Stadtbild',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => '5884efb9-348b-4840-a35e-8a661e5b0533',
    'vocabulary_machine_name' => 'image_motive_category',
  );
  $terms[] = array(
    'name' => 'Rekonstruktionszeichnung',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => '5d48d059-c9f5-4522-b1cb-22138acecece',
    'vocabulary_machine_name' => 'image_motive_category',
  );
  $terms[] = array(
    'name' => 'Landschaftsbild',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => '80cab72a-2438-4dfe-b4d2-e93d2a6b2848',
    'vocabulary_machine_name' => 'image_motive_category',
  );
  $terms[] = array(
    'name' => 'Karikatur',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => '9f71c0fc-1587-46f1-88a4-214b4b9080e3',
    'vocabulary_machine_name' => 'image_motive_category',
  );
  $terms[] = array(
    'name' => 'Malerei',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => 'bcfbfdfa-9dc2-45c8-85fb-c2c08a416412',
    'vocabulary_machine_name' => 'image_motive_type',
  );
  $terms[] = array(
    'name' => 'Personenbild',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => 'c7faea2a-20f4-4754-8797-d55d3971cf48',
    'vocabulary_machine_name' => 'image_motive_category',
  );
  $terms[] = array(
    'name' => 'Fotografie',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => 'cffcc8e8-40e2-40c8-a250-dcb16bdad011',
    'vocabulary_machine_name' => 'image_motive_type',
  );
  return $terms;
}
