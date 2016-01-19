<?php

class TemplateApiFileInputType extends TemplateApiInputType {
  public function form($input_definition, $input_values) {
    $form = array();

    if (module_exists('media')) {
      $form['file_fid'] = array(
        '#type' => 'media',
        '#title' => $input_definition['label'],
        '#default_value' => !empty($input_values['file_fid']) ? $input_values['file_fid'] : NULL,
        // Kill notice in media module.
        '#description' => NULL,
        '#attributes' => array(
          'class' => array('media-widget'),
        ),
      );
    }
    else {
      $form['file_fid'] = array(
        '#type' => 'managed_file',
        '#title' => $input_definition['label'],
        '#default_value' => isset($input_values['file_fid']['fid']) ? $input_values['file_fid']['fid'] : '',
        '#upload_location' => 'public://template_images/',
      );
    }

    return $form;
  }

  public function validate($input_definition, $input_values, &$form_state) {
    // Stash the fid
    //$form_state['storage'][$input_definition['name']]['fid'] = $input_values['file_fid']['fid'];
  }


  public function getVariable($input_definition, $input_values) {
    if (!empty($input_values['file_fid']['fid']) && ($file = file_load($input_values['file_fid']['fid']))) {
      return file_create_url($file->uri);
    }
  }

  public function isEmpty($input_definition, $input_values) {
    if (!isset($input_values['file_fid']['fid'])) {
      return TRUE;
    }

    if ($file = file_load($input_values['file_fid']['fid'])) {
      return FALSE;
    }

    return TRUE;
  }

  function update($input_definition, $input_values) {
    if (!empty($input_values['file_fid']['fid'])) {
      // Load the file if it exists
      if ($file = file_load($input_values['file_fid']['fid'])) {
        $file->status =  FILE_STATUS_PERMANENT;

        file_save($file);
        // @TODO something better here?
        file_usage_add($file, 'template_api', 'file_input', 0);
      }
    }
    // @TODO handle if the file is removed from the field
  }

  function insert($input_definition, $input_values) {
    if (!empty($input_values['file_fid']['fid'])) {
      // Load the file if it exists
      if ($file = file_load($input_values['file_fid']['fid'])) {
        $file->status =  FILE_STATUS_PERMANENT;

        file_save($file);
        // @TODO something better here?
        file_usage_add($file, 'template_api', 'file_input', 0);
      }
    }
  }

  function delete($input_definition, $input_values) {
    if (!empty($input_values['file_fid']['fid'])) {
      // Load the file if it exists
      if ($file = file_load($input_values['file_fid']['fid'])) {
        file_usage_delete($file, 'template_api', 'file_input', 0);

        file_delete($file);
      }
    }
  }
}
