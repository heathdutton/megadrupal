<?php


/**
 * @file
 * Hooks provided by the Open Atrium Folders module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Provide toolbox actions to be used on folders.
 *
 * Open Atrium Folders will add a link in the folders toolbox and open it by
 * default in a modal view. It can either be used with form callbacks or with
 * a path directly.
 *
 * - op: machine name for the operation.
 * - type: file or folder; the type of toolbox the action is for.
 * - title: name of the action
 * - description: used as hover text
 * - glyphicon: bootstrap icon used for the action
 * - form callback: function name used to create the form
 * - ajax callback: function name used to add ajax functionality
 * - path: path used instead of the form. !fid and !nid tokens are replaced
 * - node access: Optional; access check for the action defaults to "update".
 * - access callback: Optional; Instead of node_access a custom function can be
 *     called. It takes the toolbox item, the node and the fid as arguments.
 * - attributes: Optional; link attributes, default to open the modal.
 * - verify: Optional; check the availability and access to the link.
 */
function hook_folder_toolbox() {
  $items[] = array(
    'op' => 'move',
    'type' => 'file',
    'title' => t('Move'),
    'description' => t('Move the file to another folder.'),
    'glyphicon' => 'glyphicon-share icon-share',
    'form callback' => 'oa_folders_toolbox_move_form',
    'ajax callback' => 'oa_folders_toolbox_move_ajax_callback',
  );
  $items[] = array(
    'op' => 'edit',
    'type' => 'file',
    'title' => t('Edit'),
    'description' => t('Edit the file informations.'),
    'glyphicon' => 'glyphicon-edit icon-edit',
    'path' => 'media/!fid/edit/nojs',
  );
  $items[] = array(
    'op' => 'download',
    'type' => 'file',
    'title' => t('Download'),
    'description' => t('Download the file.'),
    'glyphicon' => 'glyphicon-download-alt icon-download-alt',
    'form callback' => 'oa_folders_toolbox_download_form',
    'access callback' => 'oa_folders_toolbox_download_access',
    'node access' => 'view',
    'attributes' => array(),
  );
  return $items;
}

/**
 * @} End of "addtogroup hooks".
 */
