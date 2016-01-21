<?php
/**
 * @file
 * ns_promo.linkit_profiles.inc
 */

/**
 * Implements hook_default_linkit_profiles().
 */
function ns_promo_default_linkit_profiles() {
  $export = array();

  $linkit_profile = new stdClass();
  $linkit_profile->disabled = FALSE; /* Edit this to true to make a default linkit_profile disabled initially */
  $linkit_profile->api_version = 1;
  $linkit_profile->name = 'ns_promo_search';
  $linkit_profile->admin_title = 'Promo search';
  $linkit_profile->data = array(
    'plugins' => array(
      'entity:taxonomy_term' => array(
        'weight' => '0',
        'enabled' => 1,
      ),
      'entity:node' => array(
        'weight' => '0',
        'enabled' => 1,
      ),
      'entity:file' => array(
        'weight' => '0',
        'enabled' => 0,
      ),
      'entity:user' => array(
        'weight' => '0',
        'enabled' => 0,
      ),
    ),
    'entity:node' => array(
      'result_description' => '',
      'bundles' => array(
        'ns_article' => 0,
        'ns_promo' => 0,
      ),
      'group_by_bundle' => 0,
      'include_unpublished' => 0,
    ),
    'entity:file' => array(
      'result_description' => '',
      'bundles' => array(
        'application' => 0,
        'audio' => 0,
        'image' => 0,
        'text' => 0,
        'video' => 0,
        'default' => 0,
      ),
      'group_by_bundle' => 0,
      'show_scheme' => 0,
      'group_by_scheme' => 0,
      'image_extra_info' => array(
        'thumbnail' => 'thumbnail',
        'dimensions' => 'dimensions',
      ),
    ),
    'entity:taxonomy_term' => array(
      'result_description' => '',
    ),
    'entity:user' => array(
      'result_description' => '',
    ),
    'autocomplete' => array(
      'charLimit' => 3,
      'wait' => 350,
      'remoteTimeout' => 10000,
    ),
    'attributes' => array(
      'rel' => array(
        'weight' => '0',
        'enabled' => 0,
      ),
      'class' => array(
        'weight' => '0',
        'enabled' => 0,
      ),
      'accesskey' => array(
        'weight' => '0',
        'enabled' => 0,
      ),
      'id' => array(
        'weight' => '0',
        'enabled' => 0,
      ),
      'title' => array(
        'weight' => '0',
        'enabled' => 0,
      ),
    ),
  );
  $linkit_profile->role_rids = array(
    2 => 1,
  );
  $linkit_profile->weight = 0;
  $export['ns_promo_search'] = $linkit_profile;

  return $export;
}
