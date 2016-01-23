<?php
/**
 * @file
 * Association support for profile2 module relationships
 */

/**
 * Implements hook_topic_map_export_association_info().
 */
function profile2_topic_map_export_association_info() {

  $return = array(
    array(
      'description' => t('Profile2 belongs to User'),
      'requirements' => array(
        'entities' => array('profile2', 'user'),
      ),
      'definitions' => array(
        'class' => array('profile-belongs-to-user' => t('Owned by')),
        'roles' => array(
          'profile-user' => t('Belonger'),
          'user-profile' => t('Owner'),
        ),
      ),
      'handler' => '_topic_map_export_process_profile_user_ownership',
      'handler_entity_type' => 'profile2',
    ),
  );

  return $return;

}


/**
 * Creates assocation for profile -> user ownership type relationship.
 *
 * @see _topic_map_export_sample_handler_process_taxonomy_term()
 * @see topic_map_export.api.php
 */
function _topic_map_export_process_profile_user_ownership($topic, $profile) {

  $members = array();
  $members[] = array('type' => 'profile2', 'id' => $profile->pid);
  $members[] = array('type' => 'user', 'id' => $profile->uid);

  return array($members);

}
