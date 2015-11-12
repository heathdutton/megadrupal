<?php
/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */


/**
 * @defgroup socialmedia_importer Social Media Importer module integrations.
 *
 * Module integrations with the Social Media Importer module.
 */


/**
 * @defgroup socialmedia_importer_hooks Social Media Importer's hooks
 * @{
 * Hooks that can be implemented by other modules in order to extend the 
 * Social Media Importer module.
 */

/**
 * Provide Informations about the Social Media Application.
 * 
 * @return array
 *   An associative array keyed by the provider machine name:
 *     - class: the class that handle the Application.
 *     - name: the name of the provider.
 *     - description: a help description.
 */
function hook_socialmedia_importer_application_info() {
  $info = array(
    'facebook' => array(
      'class' => 'SocialMediaImporterFacebookApplication ',
      'name' => 'Facebook',
      'description' => t("A Facebook Application is required to access 
        Facebook's API. Use your Application name, id and secret to add a 
        Facebook Application. If you do not have a Facebook Application, 
        you can create one at: !url.",
        array('!url' => l(t('https://developers.facebook.com/apps'), 'https://developers.facebook.com/apps'))),
    ),
  );
  return $info;
}

/**
 * Save (create new or update) a Social Media Application.
 *
 * @param array $application
 *   An associative array of Application details keyed by
 *   the Application fields names. If id is provided, the Application will
 *   be updated.
 *
 * @return array
 *   Return an array of the saved fields keyed by the Application fields
 *   names or FALSE if the record insert or update failed.
 */
function socialmedia_importer_application_save($application) {

}

/**
 * Get access token for an Application in order to make api calls.
 * 
 * @param int $id
 *   The Social Media Application id.
 */
function socialmedia_importer_authorize_application($id) {

}

/**
 * Revoke access for an Application.
 * 
 * @param int $id
 *   The Social Media Application id.
 */
function socialmedia_importer_deauthorize_application($id) {
  return array('success' => FALSE);
}

/**
 * Return the data of a Social Media Application.
 * 
 * @param int $id
 *   The Social Media Application id.
 */
function socialmedia_importer_get_application_data($id) {

}

/**
 * Respond to creation of a new Social Media Application.
 *
 * This hook is invoked from socialmedia_importer_application_save().
 * 
 * @param array $fields
 *   The Application fields to insert.
 */
function hook_socialmedia_importer_application_insert($fields) {
  db_insert('mytable')
  ->fields($fields)
  ->execute();
}

/**
 * Respond to updates to a node.
 *
 * This hook is invoked from socialmedia_importer_application_save().
 * 
 * @param array $fields
 *   The Application fields to update.
 */
function hook_socialmedia_importer_application_update($fields) {
  db_update('mytable')
  ->fields($fields)
  ->condition('id', $fields['id'])
  ->execute();
}

/**
 * Respond to Social Media Application deletion.
 *
 * This hook is invoked from socialmedia_importer_application_delete()
 *  before the Application is removed from the
 * aocialmedia_applications table in the database.
 *
 * @param int $id
 *   The id of the Application that is being deleted.
 */
function hook_socialmedia_importer_application_delete($id) {
  db_delete('mytable')
  ->condition('id', $id)
  ->execute();
}

/**
 * Get the url for social media data import.
 *
 * @param int $id
 *   The Social Media Application id.
 * 
 * @return string
 *   The url for data import.
 */
function socialmedia_importer_get_data_import_url($id) {
  return $GLOBALS['base_url'] . 'socialmedia_importer/import_data/' . $id;
}

/**
 * @}
 */
