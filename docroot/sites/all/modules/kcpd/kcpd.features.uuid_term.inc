<?php
/**
 * @file
 * kcpd.features.uuid_term.inc
 */

/**
 * Implements hook_uuid_features_default_terms().
 */
function kcpd_uuid_features_default_terms() {
  $terms = array();

  $terms[] = array(
    'name' => 'Romantic Comedy',
    'description' => '',
    'format' => 'plain_text',
    'weight' => 0,
    'uuid' => '1d408451-a20f-46b2-83f5-516101c19fed',
    'vocabulary_machine_name' => 'genre',
  );
  $terms[] = array(
    'name' => 'Horror',
    'description' => '',
    'format' => 'plain_text',
    'weight' => 0,
    'uuid' => '69c43042-362c-4d23-8df4-f6cd6ea0cb12',
    'vocabulary_machine_name' => 'genre',
  );
  $terms[] = array(
    'name' => 'Drama',
    'description' => '',
    'format' => 'plain_text',
    'weight' => 0,
    'uuid' => '77bd136c-9c5f-4f6e-9814-8466b12d8b19',
    'vocabulary_machine_name' => 'genre',
  );
  $terms[] = array(
    'name' => 'Action',
    'description' => '',
    'format' => 'plain_text',
    'weight' => 0,
    'uuid' => '98a5e643-e3ba-41cc-8c49-cc91ee8ab83b',
    'vocabulary_machine_name' => 'genre',
  );
  $terms[] = array(
    'name' => 'Comedy',
    'description' => '',
    'format' => 'plain_text',
    'weight' => 0,
    'uuid' => '9a495fda-6b22-413b-9d14-44e910ac64f7',
    'vocabulary_machine_name' => 'genre',
  );
  $terms[] = array(
    'name' => 'Musical',
    'description' => '',
    'format' => 'plain_text',
    'weight' => 0,
    'uuid' => 'a9b37dfc-c789-4903-8d2e-87db0cfdfe04',
    'vocabulary_machine_name' => 'genre',
  );
  $terms[] = array(
    'name' => 'Documentary',
    'description' => '',
    'format' => 'plain_text',
    'weight' => 0,
    'uuid' => 'e792cebf-f129-47fe-8a62-167bcbe7ec09',
    'vocabulary_machine_name' => 'genre',
  );
  return $terms;
}
