<?php



/**
 * Ajax page callback for the toolbox.
 */
function oa_folders_toolbox_page_callback($op, $nid, $fid, $ajax = FALSE) {

  $type = ($fid ? 'file' : 'folder');
  $actions = oa_folders_toolbox_actions();
  $tool = $actions[$op . '-' . $type];

  // degrade to nojs
  if(!$ajax) {
    return drupal_get_form($tool['form callback'], $op, $nid, $fid);
  }

  ctools_include('modal');
  ctools_include('ajax');
  $form_state = array(
    'title' => $tool['title'],
    'ajax' => TRUE,
    'build_info' => array('args' => array($op, $nid, $fid)),
  );
  $output = ctools_modal_form_wrapper($tool['form callback'], $form_state);

  if (!empty($form_state['ajax_commands'])) {
    $output = $form_state['ajax_commands'];
  }
  if (!empty($form_state['executed'])) {
    $output[] = ctools_modal_command_dismiss(t('Success!'));
  }
  if (isset($tool['ajax callback']) && is_callable($tool['ajax callback'])) {
    $callback = $tool['ajax callback'];
    $output = array_merge($output, $callback($form_state));
  }

  print ajax_render($output);
  drupal_exit();
}

/**
 * File move form callback.
 */
function oa_folders_toolbox_move_form($form, $form_state) {

  $nid = $form_state['build_info']['args'][1];
  $options = oa_folders_all_folders_tree_recursive_options(oa_folders_get_folders_tree('update'), '--');

  $form['target_folder'] = array(
    '#type' => 'select',
    '#title' => t('Selected'),
    '#options' => $options,
    '#default_value' => $nid,
    '#description' => t('Select the target folder to move this file to.'),
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Move'));

  return $form;
}

/**
 * File move form submit callback.
 */
function oa_folders_toolbox_move_form_submit($form, &$form_state) {

  $fid = $form_state['build_info']['args'][2];
  $from_nid = $form_state['build_info']['args'][1];
  $to_nid = $form_state['values']['target_folder'];
  if ($from_nid != $to_nid) {
    oa_folders_move_file($fid, $from_nid, $to_nid);
  }
}

/**
 * File move ajax callback.
 */
function oa_folders_toolbox_move_ajax_callback($form_state) {
  $commands = array();
  $fid = $form_state['build_info']['args'][2];
  $from_nid = $form_state['build_info']['args'][1];
  $to_nid = $form_state['values']['target_folder'];
  if ($to_nid && $from_nid != $to_nid) {
    // Don't remove if the folder didn't change
    $commands[] = ajax_command_remove('.folder-file-' . $fid);
  }
  return $commands;
}

/**
 * File move form callback.
 */
function oa_folders_toolbox_delete_form($form, $form_state) {
  $nid = $form_state['build_info']['args'][1];

  $form['markup'] = array('#markup' => "The file will be deleted. Are you sure?");
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );

  return $form;
}

/**
 * File move form submit callback.
 */
function oa_folders_toolbox_delete_form_submit($form, &$form_state) {

  if ($form_state['triggering_element']['#value'] == 'Delete') {
    $fid = $form_state['build_info']['args'][2];
    $nid = $form_state['build_info']['args'][1];

    $file_key = NULL;
    $node = node_load($nid);

    foreach ($node->field_oa_media[LANGUAGE_NONE] as $key => $attachment) {
      // Find the relevant file
      if ($attachment['fid'] == $fid) {
        $file_key = $key;
        continue;
      }
    }
    if ($file_key) {
      // Remove the file from the old folder.
      $file_list = $node->field_oa_media[LANGUAGE_NONE];
      unset($file_list[$file_key]);
      $node->field_oa_media[LANGUAGE_NONE] = array_values($file_list);
      node_save($node);
    }
  }
}

/**
 * File move ajax callback.
 */
function oa_folders_toolbox_delete_ajax_callback($form_state) {
  $commands = array();
  $fid = $form_state['build_info']['args'][2];
  if ($form_state['triggering_element']['#value'] == 'Delete') {
    $commands[] = ajax_command_remove('.folder-file-' . $fid);
  }
  return $commands;
}

/**
 * File download callback.
 */
function oa_folders_toolbox_download_form($form, &$form_state) {

  $fid = $form_state['build_info']['args'][2];
  $file = file_load($fid);
  $nid = $form_state['build_info']['args'][1];
  $node = node_load($nid);

  if (empty($fid) || empty($file) || !$file->status) {
    return drupal_access_denied();
  }

  if (!oa_folders_check_file_access($file, $node, 'field_oa_media')) {
    // our function checks access to the file by checking whether it is
    // attached to the node and the user has field access to see the file.
    return drupal_access_denied();
  }

  $headers = array(
    'Content-Type'              => 'force-download',
    'Content-Disposition'       => 'attachment; filename="' . $file->filename . '"',
    'Content-Length'            => $file->filesize,
    'Content-Transfer-Encoding' => 'binary',
    'Pragma'                    => 'no-cache',
    'Cache-Control'             => 'must-revalidate, post-check=0, pre-check=0',
    'Expires'                   => '0',
    'Accept-Ranges'             => 'bytes'
  );

  // Send file to browser and exit().
  file_transfer($file->uri, $headers);
}
