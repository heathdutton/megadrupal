<?php
/**
 * @file
 * Party info hook include.
 */

/**
 * Implements hook_party_data_set_info().
 */
function party_profile_party_data_set_info() {
  // Because this hook is invoked from hook_schema(), bad things happen if
  // we use profile2_get_types(). Hence just grab the type names directly
  // as that is all we need.
  // See http://drupal.org/node/1307506 for more details.
  $result = db_query("SELECT type, label FROM {profile_type}");
  $types = $result->fetchAllKeyed();

  $sets = array();
  foreach ($types as $type => $label) {
    $sets["profile2_" . $type] = array(
      'label' => $label,
      'entity type' => 'profile2',
      'entity bundle' => $type,
      'class' => 'PartyDefaultDataSet',
      // Provide details of admin interfaces
      'admin' => array(
        'create' => 'admin/structure/profiles/add',
        'import' => 'admin/structure/profiles/import',
        'edit' => 'admin/structure/profiles/manage/' . $type,
        'manage fields' => 'admin/structure/profiles/manage/' . $type . '/fields',
        'manage display' => 'admin/structure/profiles/manage/' . $type . '/display',
        'clone' => 'admin/structure/profiles/manage/' . $type . '/clone',
        'delete' => 'admin/structure/profiles/manage/' . $type . '/delete',
        'export' => 'admin/structure/profiles/manage/' . $type . '/export',
      ),
      // Provide a piece corresponding to each data set.
      'piece' => array(
        'path' => 'profile-' . str_replace('_', '-', $type),
        'maker' => 'view',
        // @todo: this needs to tell the access arguments for party_access()
        // about the argument for loading our attached entities.
        // In other words, some sort of %wildcard_to_arg() function maybe?
        'access arguments' => array("view any $type profile"),
      ),
      // Define actions for the data set.
      'actions' => array(
        'add' => array(
          // @TODO: this controller is not yet in use; the action menu item
          // is a dedicated form for now.
          'controller' => 'PartyDefaultDataSetUIAdd',
          'action label' => 'Add @data-set',
        ),
        /*
        // Does attach make sense for profiles?
        'attach' => array(
          'controller' => 'PartyDefaultDataSetUIAttach',
          'action label' => 'Attach existing @data-set',
        ),
        */
      ),
    );
  }
  return $sets;
}
