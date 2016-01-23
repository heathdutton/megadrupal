<?php
/**
 * @file
 * This file contains the crud operations for comments.
 */

/**
 * Function to create a comment.
 *
 * @param array $data
 *   The data from which the comment will be created.
 *
 * @return array
 *   Returns OpenLucius services message which contains the id, a message and
 *   the path to the item.
 * @throws ServicesException
 *   If the call is not accepted this method will throw a ServicesException 406.
 */
function _openlucius_services_create_comment(array $data) {
  global $user;

  // Fetch creation requirements for this type.
  $requirements = _openlucius_services_entity_requirements('comment');

  // Check if the requirements are set.
  foreach ($requirements as $requirement) {
    if (empty($data[$requirement])) {
      openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_MISSING_PARAMETER, array("@parameter" => $requirement));
    }
  }

  // Exclude show_clients if the user is a client.
  // That would really mess things up.
  if (openlucius_core_user_is_client() && isset($data['show_clients'])) {
    unset($data['show_clients']);
  }

  // Verify that the id is of the correct type.
  if (!is_numeric($data['id'])) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_WRONG_PARAMETER_TYPE, array(
      '@input'       => gettype($data['id']),
      '@requirement' => 'integer',
      '@for'         => 'id',
    ));
  }

  // Load the node.
  $node = node_load($data['id']);

  // Check if we have a node to work with.
  if (empty($node)) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_NODE_NOT_FOUND, array("@nid" => $data['id']));
  }

  // Get the group id.
  $wrapper    = entity_metadata_wrapper('node', $node);
  $group_node = $wrapper->field_shared_group_reference->value();
  $group_id    = $group_node->nid;

  // Check if the user has access to the group.
  if (!openlucius_core_user_in_group($group_id, $user->uid) && $user->uid != 1) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_GROUP_ACCESS_DENIED);
  }

  // Build the comment type string.
  $comment_type = 'comment_node_' . $node->type;

  // Create the basic comment entity.
  $comment = entity_create('comment', array(
    'nid'       => $data['id'],
    'uid'       => $user->uid,
    'node_type' => $comment_type,
  ));

  // Unset the id as it is already used.
  unset($data['id']);

  // Fetch the wrapper for easy access.
  $comment_wrapper = entity_metadata_wrapper('comment', $comment);

  // Loop through data and set the values.
  foreach ($data as $field => $value) {

    // Fetch the drupal field.
    $drupal_field = openlucius_services_field_mapping('comment', $field);

    // Update the field.
    _openlucius_services_comment_set_field($comment_wrapper, $drupal_field, $data);
  }

  // Save the comment.
  $comment_wrapper->save();

  // Fetch the cid.
  $cid = $comment_wrapper->getIdentifier();

  // Check if we have files.
  if (!empty($_FILES)) {

    // Fetch the drupal field.
    $drupal_field = openlucius_services_field_mapping('comment', 'files');

    // Try to attach the files.
    _openlucius_services_comment_attach_files($cid, $drupal_field, $comment_type);
  }

  return openlucius_services_message($cid, t('Your comment has been posted successfully.'), url('comment/' . $cid, array(
    'absolute' => TRUE,
    'fragment' => 'comment-' . $cid
  )));
}

/**
 * Function to update a comment.
 *
 * @param int $cid
 *   The comment id to be updated.
 * @param array $data
 *   The data required for updating a comment.
 *
 * @return bool
 *   Either TRUE when successful or FALSE when the update fails.
 * @throws ServicesException
 *   If the call is not accepted this method will throw a ServicesException 406.
 */
function _openlucius_services_update_comment($cid, array $data) {
  global $user;

  // Verify that the submitted $cid is indeed an integer.
  // TODO this shouldn't be necessary, int is set in hook_services_resources.
  if (!is_numeric($cid)) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_WRONG_PARAMETER_TYPE, array(
      '@input'       => gettype($cid),
      '@requirement' => 'integer',
      '@for'         => 'id',
    ));
  }

  // Load the comment.
  $comment = comment_load($cid);

  // Verify that the comment exists.
  if (empty($comment)) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_COMMENT_NOT_FOUND, array("@cid" => $cid));
  }

  // Verify that this user may edit the comment.
  if ($comment->uid !== $user->uid && $user->uid != 1) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_COMMENT_EDIT_ACCESS_DENIED);
  }

  // Fetch the wrapper for easy access.
  $comment_wrapper = entity_metadata_wrapper('comment', $comment);

  // Loop through data and set the values.
  foreach ($data as $field => $value) {

    // Fetch the drupal field.
    $drupal_field = openlucius_services_field_mapping('comment', $field);

    // Update the field.
    _openlucius_services_comment_set_field($comment_wrapper, $drupal_field, $data);
  }

  // Save the comment.
  $comment_wrapper->save();

  return openlucius_services_message($cid, t('Your comment has been updated successfully.'), url('comment/' . $cid, array(
    'absolute' => TRUE,
    'fragment' => 'comment-' . $cid
  )));
}

/**
 * Function to set a value on a comment.
 *
 * @param \EntityMetadataWrapper $wrapper
 *   The entityMetadataWrapper to set the value.
 * @param string $field
 *   The field to receive the value.
 * @param mixed $data
 *   The value or values to be set.
 *
 * @return mixed
 *   Only returns a value if an error has occurred.
 * @throws \ServicesException
 *   Throws an exception if the field could not be mapped.
 */
function _openlucius_services_comment_set_field(\EntityMetadataWrapper $wrapper, $field, $data) {
  // Switch on field.
  switch ($field) {

    case 'nid':
      $wrapper->{$field} = (int) $data['id'];
      break;

    // Set the body.
    case 'comment_body':

      // Set the body and filter the markup.
      $wrapper->{$field} = array(
        'value'  => check_markup($data['body'], 'plain_text'),
        'format' => 'plain_text',
      );
      break;

    // There's no implementation, so allow other modules to process the data.
    default:
      // Check if there's a module ready to process the data.
      if (count(module_implements('_openlucius_services_comment_set_field')) > 0) {
        module_invoke_all('_openlucius_services_comment_set_field', $wrapper, $field, $data);
      }
      // Return error as this field cannot yet be processed.
      else {
        openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_NOT_IMPLEMENTED_YET, array(
          "@type"   => $field,
          '@method' => '_openlucius_services_comment_set_field',
          '@see'    => 'includes/resource.comment.crud.inc',
        ));
      }
      break;
  }
}

/**
 * Function to attach files to the comment.
 *
 * @param int $cid
 *   The comment to attach the files to.
 * @param string $field_name
 *   The filed to attach the files to.
 * @param string $comment_type
 *   The type of comment.
 * @param bool $attach
 *   Whether the files should be appended or override existing files.
 *
 * @throws \ServicesException
 */
function _openlucius_services_comment_attach_files($cid, $field_name, $comment_type, $attach = TRUE) {
  // Load comment.
  $comment = comment_load($cid);

  // Get the field instance.
  $instance = field_info_instance('comment', $field_name, $comment_type);

  // Read the field.
  $field = field_read_field($field_name);

  // Fetch destination.
  $destination = file_field_widget_uri($field, $instance);
  if (isset($destination) && !file_prepare_directory($destination, FILE_CREATE_DIRECTORY)) {
    services_error(t('The upload directory %directory for the file field !name could not be created or is not accessible. A newly uploaded file could not be saved in this directory as a consequence, and the upload was canceled.', array(
      '%directory' => $destination,
      '!name'      => $field_name,
    )));
  }

  // Fetch the validators.
  $validators = array(
    'file_validate_extensions' => (array) $instance['settings']['file_extensions'],
    'file_validate_size'       => array(0 => parse_size($instance['settings']['max_filesize'])),
  );

  // Check if we need to override the old files or append the files.
  $counter = 0;
  if ($attach && !empty($comment->{$field_name}[LANGUAGE_NONE])) {
    $counter = count($comment->{$field_name}[LANGUAGE_NONE]);
  }
  else {
    $comment->{$field_name}[LANGUAGE_NONE] = array();
  }

  // Loop through the uploaded files.
  foreach ($_FILES['files']['name'] as $key => $val) {

    // Let the file module handle the upload and moving.
    if (!$file = file_save_upload($key, $validators, $destination, FILE_EXISTS_RENAME)) {
      services_error(t('Failed to upload file. @upload', array('@upload' => $key)), 406);
    }

    if (!empty($file->fid)) {
      $comment->{$field_name}[LANGUAGE_NONE][$counter]            = (array) $file;
      $comment->{$field_name}[LANGUAGE_NONE][$counter]['display'] = 1;
      $counter++;
    }
    else {
      services_error(t('An unknown error has occurred. As a result the fid is missing. Please contact your site admin.'), 500);
    }
  }

  // Save the comment.
  comment_save($comment);
}
