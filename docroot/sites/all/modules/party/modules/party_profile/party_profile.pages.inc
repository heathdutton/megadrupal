<?php
/**
 * @file
 * Define pages needed for the party_profile module
 */

/**
 * Page callback for our data set-based party pieces.
 */
function party_profile_party_view_profile_set($party, $data_set_url_string) {
  $data_set = party_data_set_load($data_set_url_string);
  $data_set_name = $data_set['set_name'];
  $profile_type = $data_set['entity bundle'];

  // Load the profiles in this data set.
  $attached_entities = $party->getDataSetController($data_set_name)->getEntities();

  return 'This is currently broken but you should at least see the loaded data set.';

  // @todo: $sets was the array of old controllers, one per attached entity.

  // Output each profile provided the user has view access.
  foreach ($sets as $set) {
    $profile = $set->entity;
    if (profile2_access('view', $profile)) {
      $build['profile_' . $profile->pid] = array(
        '#type' => 'fieldset',
        '#title' => $profile->label,
      );
      $build['profile_' . $profile->pid]['view'] = $profile->view('party');
      $build['profile_' . $profile->pid]['actions'] = array(
        '#theme' => 'links',
        '#links' => $set->actions(),
        '#attributes' => array(
          'class' => array('crm-set-action-links'),
        ),
      );
    }
  }

  // Add a link to add further profiles to this set.
  $entity_info_profile = entity_get_info('profile2');
  $label = $entity_info_profile['bundles'][$profile_type]['label'];
  $build['profile-links'] = array(
    '#theme' => 'links',
    '#links' => array(
      'add' => array(
        'title' => "Add " . $label,
        'href'  => 'party/' . $party->pid . '/add/profile2_' . $profile_type,
      ),
    ),
    '#attributes' => array('class' => array('crm-profile-links')),
    '#weight' => 1,
  );

  return $build;
}
