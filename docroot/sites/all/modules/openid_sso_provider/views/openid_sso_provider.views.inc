<?php

/**
 * @file
 * Provide views data for OpenID SSO Provider module.
 */

/**
 * Implements hook_views_data().
 *
 * Provides Views integration for OpenID SSO Provider's relying parties.
 */
function openid_sso_provider_views_data() {

  $data = array();

  // ----------------------------------------------------------------------
  // openid_sso_provider_rps table

  $data['openid_sso_provider_rps']['table']['group']  = t('Relying party');
  $data['openid_sso_provider_rps']['table']['base'] = array(
    'field' => 'rpsid',
    'title' => t('OpenID SSO Relying Parties'),
    'help' => t('Relying Parties defined by the OpenID Single Sign-On Provider module.'),
  );
  $data['openid_sso_provider_rps']['table']['entity type'] = 'openid_sso_provider_rps';


  // Expose the relying party's label.
  $data['openid_sso_provider_rps']['label'] = array(
    'title' => t('Label'),
    'help' => t('The label of the relying party.'),
    'field' => array(
      'handler' => 'openid_sso_provider_handler_field_rps_label',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );


  // Expose the relying party's realm / url.
  $data['openid_sso_provider_rps']['realm'] = array(
    'title' => t('Realm'),
    'help' => t('The url of the relying party.'),
    'field' => array(
      'handler' => 'openid_sso_provider_handler_field_rps_realm',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );


  // Expose the relying party's trusted machine name.
  $data['openid_sso_provider_rps']['machine_name'] = array(
    'title' => t('Machine Name'),
    'help' => t('The machine-readable name of the relying party.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );


  // Expose the relying party's trusted boolean.
  $data['openid_sso_provider_rps']['trusted'] = array(
    'title' => t('Trusted'),
    'help' => t('Boolean indicating if a relying party is generally trustworthy.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'trusted-nottrusted' => array(t('Generally rusted'), t('Not generally trusted')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Generally trusted'),
      'type' => 'yes-no',
      'use equal' => TRUE, // Use status = 1 instead of status <> 0 in WHERE statment
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

    // Expose the relying party's trusted boolean.
  $data['openid_sso_provider_rps']['enabled'] = array(
    'title' => t('Enabled'),
    'help' => t('Boolean indicating if a relying party can be used.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'trusted-nottrusted' => array(t('Enabled'), t('Disabled')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Enabled'),
      'type' => 'yes-no',
      'use equal' => TRUE, // Use status = 1 instead of status <> 0 in WHERE statment
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );


  // ----------------------------------------------------------------------
  // openid_provider_relying_party table

  $data['openid_provider_relying_party']['table']['group']  = t('Visited relying parties');
  /*
    $data['openid_provider_relying_party']['table']['base'] = array(
    'field' => 'rpid',
    'title' => t('OpenID Provider: Relying Parties'),
    'help' => t('Relying Parties defined by the OpenID Single Sign-On Provider module.'),
  );
  */
  $data['openid_provider_relying_party']['table']['join'] = array(
     // Directly links to openid_sso_provider_rps table.
    'openid_sso_provider_rps' => array(
      'left_field' => 'realm',
      'field' => 'realm',
    ),
    // Directly links to users table.
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
      //'type' => 'INNER', @TODO: Make it a right join to include users in the listing that never logged in to a relying party site.
    ),
  );

  $data['openid_provider_relying_party']['uid'] = array(
    'title' => t('User'),
    'help' => t('The user ID of user once logged in from a specific relying party (may cause duplicates).'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'numeric' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
  );


  $data['openid_provider_relying_party']['realm'] = array(
    'title' => t("Realm URL"),
    'help' => t("User's used relying parties."),
    'field' => array(
      'handler' => 'views_handler_field',
      //'handler' => 'openid_sso_provider_handler_field_rps_realm',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'openid_sso_provider_handler_filter_rps_realm',
    ),
    'argument' => array(
      'handler' => 'openid_sso_provider_handler_argument_rps_realm',
    ),
  );

  return $data;
}


/**
 * Implements hook_views_data_alter().
 *
 * Applies changes to standard provided entity views integration.
 */
function openid_sso_provider_views_data_alter(&$data) {

  // Harmonize titles
  // $data['views_entity_openid_sso_provider_rps']['table'] = $data['openid_sso_provider_rps']['table'];

  // Disable provided rendered entity
  unset($data['views_entity_openid_sso_provider_rps']['rendered_entity']);

  // Change wording.
  $data['views_entity_openid_sso_provider_rps']['url']['title'] = t('Admin/edit link');
  $data['views_entity_openid_sso_provider_rps']['url']['help'] = t("The url of the relying party's edit link for administration.");

}
