<?php

/**
 * @file
 * Import all system specific blocks.
 */

/**
 * Form constructor for import system specific blocks.
 *
 * @see block_export_import_blocks_import_form_submit()
 * @see import_block_form_submit()
 */
function block_export_import_blocks_import_form($form, &$form_state) {
  if (empty($form_state['storage']['values'])) {
    $form['override_existing'] = array(
      '#type' => 'checkbox',
      '#title' => t('Replace an existing block if one exists with the same name'),
      '#return_value' => 1,
    );

    $form['bypass_theme'] = array(
      '#type' => 'checkbox',
      '#title' => t('Bypass theme/region validation'),
      '#description' => t('Bypass the validation of theme/region when importing the block(s).'),
      '#return_value' => 1,
    );

    $form['import_code'] = array(
      '#type' => 'textarea',
      '#title' => t('Paste block(s) code here'),
      '#required' => TRUE,
      '#rows' => 10,
      '#description' => t('Paste an exported block(s) array structure here.'),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  return $form;
}

/**
 * Validate handler for import_custom_blocks_form().
 */
function block_export_import_blocks_import_form_validate($form, &$form_state) {
  $bypass_theme = $form_state['values']['bypass_theme'];

  if ((int) $bypass_theme === 0) {
    // Get list of all currently available themes.
    $list_themes = list_themes($refresh = TRUE);
    $themes = array();
    foreach ($list_themes as $data) {
      if ($data->status) {
        $themes[$data->name] = $data->info['name'];
      }
    }
    // Transpose the theme key<>value.
    $themes_flip = array_flip((array) $themes);

    // Extract the code.
    $string = $form_state['values']['import_code'];
    @eval($string);
    foreach ($block as $data) {
      if (array_key_exists('theme', $data['block_custom'])) {
        $theme = $data['block_custom']['theme'];
        if (!in_array($theme, $themes_flip)) {
          form_set_error(' ', t('Sorry! theme with name %theme not exists or not enabled.', array('%theme' => $theme)));
        }
      }
    }
  }
}

/**
 * Submit handler for import_custom_blocks_form() to import/save a block.
 *
 * @see block_export_import_block_import_process()
 * @see _block_export_import_validate_import_data()
 */
function block_export_import_blocks_import_form_submit($form, &$form_state) {
  if (empty($form_state['storage']['values'])) {
    $values = $form_state['values'];
    $override_existing = (int) $values['override_existing'];
    $bypass_theme = (int) $values['bypass_theme'];
    $bsfields = _block_export_import_get_block_schema_fields();
    // Get list of all currently available themes.
    $list_themes = list_themes($refresh = TRUE);
    $themes_info = array();
    foreach ($list_themes as $data) {
      if ($data->status) {
        $themes_info[$data->name] = array(
          'name' => $data->name,
          'info' => array(
            'name' => $data->info['name'],
            'regions' => $data->info['regions'],
          ),
        );
      }
    }

    $block = array();
    $string = $form_state['values']['import_code'];
    @eval($string);

    if (is_array($block)) {
      $php_filter = module_exists('php');
      foreach ($block as $block_data) {
        $block_desc = trim($block_data['block_custom']['info']);
        $visibility = $block_data['block_custom']['visibility'];
        if (empty($block_desc)) {
          $error = t('Empty block description.');
          form_set_error('info', $error);
        }
        else {
          // The variable to display all types of messages which are produce in
          // the blocks creation process.
          $message = "";

          // It is use to verify related module are enabled or not if not enabled then
          // it throw an error,for a particular block.
          $bypass_phpfilter = TRUE;
          if ((int) $visibility === 2) {
            if (!$php_filter) {
              $bypass_phpfilter = FALSE;
            }
          }

          if ($bypass_phpfilter == TRUE) {
            $bfields = array();

            $result = _block_export_import_block_bypass_theme_check($bypass_theme, $block_data, $themes_info);
            if ($result['status'] == TRUE) {
              $query = "SELECT bid AS block_id FROM {block_custom} WHERE info = :info";
              $block_id = db_query($query, array(':info' => $block_desc))->fetchField();

              if ($block_id) {
                if ((int) $override_existing == 1) {
                  $block_data['block_custom']['block_id'] = $block_id;
                  $bfields = $result['block_fields'];
                  _block_export_import_block_update($form_state, $block_data, $bfields, $bsfields);
                }
              }
              else {
                _block_export_import_block_insert($form_state, $themes_info, $block_data, $bfields, $bsfields);
              }
            }
          }
          else {
            $block_desc = $block_data['block_custom']['info'];
            $message = t("The PHP filter module should be enabled to&nbsp;");
            $message .= t("override the block &nbsp;");
            $message .= t("'!block_info'", array('!block_info' => $block_desc));
            drupal_set_message(check_plain($message), 'error');
          }
        }
      }
      drupal_theme_rebuild();
      cache_clear_all('*', 'cache_block', TRUE);
      $form_state['rebuild'] = TRUE;
    }
  }
  drupal_goto('admin/structure/export-import-block/export');
}

/**
 * Validate region handler.
 */
function _block_export_import_block_bypass_theme_check($bypass_theme, $bdata, $tinfo) {
  $result['status'] = TRUE;
  $result['block_fields'] = array();
  $block_custom = $bdata['block_custom'];

  $info = $block_custom['info'];
  foreach ($tinfo as $theme_data) {
    if (array_key_exists('theme', $block_custom) && $theme_data['name'] == $block_custom['theme']) {
      if ($bypass_theme === 0) {
        if (array_key_exists($block_custom['region'], $theme_data['info']['regions']) || $block_custom['region'] == '-1') {
          $block_fields['theme'] = $block_custom['theme'];
          $block_fields['region'] = $block_custom['region'];
          $result['block_fields'] = $block_fields;
        }
        else {
          $result['status'] = FALSE;
          $place_holder = array(
            '%theme' => $theme_data['info']['name'],
            '%region' => $block_custom['region'],
          );
          drupal_set_message(t('There is no %region region under %theme theme.', $place_holder), 'warning');
        }
      }
    }
  }

  return $result;
}

/**
 * Callback to update existing block.
 *
 * @see _block_export_import_import_visibility_settings()
 */
function _block_export_import_block_update($form_state, $bdata, $bfields, $bsfields) {
  $values = $form_state['values'];
  $override_existing = $values['override_existing'];
  $bypass_theme = $values['bypass_theme'];
  // An array of block basic information.
  $block_custom = $bdata['block_custom'];
  $block_id = $block_custom['block_id'];
  // Block description.
  $block_desc = $block_custom['info'];
  // The filter_format format of the block body.
  $format = $block_custom['format'];
  // The Block contents.
  $body = stripslashes($block_custom['body']);

  // Update custom block's basic information.
  db_update('block_custom')->fields(
    array(
      'body' => $body,
      'format' => $format,
    )
  )->condition('info', $block_desc)->execute();

  // Update block's Visibility settings, title etc.
  $bfields['title'] = $block_custom['title'];
  $bfields['weight'] = $block_custom['weight'];
  $bfields['cache'] = $block_custom['cache'];
  $bfields['custom'] = (int) $block_custom['custom'];
  $bfields['visibility'] = (int) $block_custom['visibility'];
  $bfields['pages'] = $block_custom['pages'];
  $bfields['status'] = $block_custom['status'];
  if (module_exists('block_class') && in_array('css_class', $bsfields)) {
    if (array_key_exists('css_class', $block_custom)) {
      $bfields['css_class'] = $block_custom['css_class'];
    }
  }

  $update = db_update('block')->fields($bfields);
  if (array_key_exists('theme', $bfields)) {
    $update->condition('theme', $bfields['theme']);
  }
  $update->condition('module', 'block');
  $update->condition('delta', $block_id)->execute();

  // Detete the existing content type form same block to this block to
  // override by new settings.
  db_delete('block_node_type')->condition('module', 'block')->condition('delta', $block_id)->execute();

  // Detete the existing roles form same block to this block to
  // override by new settings.
  db_delete('block_role')->condition('module', 'block')->condition('delta', $block_id)->execute();

  // Call to override the block visibility settings (content type and roles).
  if (array_key_exists('visibility_settings', $block_custom)) {
    _block_export_import_import_visibility_settings($bdata, $block_id);
  }

  $message = t("Block '!block_info' has been override successfully.", array('!block_info' => $block_desc)
  );
  drupal_set_message(check_plain($message));
}

/**
 * Callback to create new block.
 */
function _block_export_import_block_insert($form_state, $tinfo, $bdata, $bfields, $bsfields) {
  $values = $form_state['values'];
  $override_existing = $values['override_existing'];

  // An array of block basic information.
  $block_custom = $bdata['block_custom'];
  // Block description.
  $block_desc = $block_custom['info'];
  // The filter_format format of the block body.
  $format = $block_custom['format'];
  // The Block contents.
  $body = stripslashes($block_custom['body']);
  $region = isset($block_custom['region']) ? $block_custom['region'] : '';

  // Create a new system specific blocks and return the current block id.
  $block_id = db_insert('block_custom')->fields(array(
      'info' => $block_desc,
      'body' => $body,
      'format' => $format,
      )
    )->execute();

  foreach ($tinfo as $theme_data) {
    $bfields['module'] = 'block';
    $bfields['delta'] = $block_id;
    $bfields['title'] = $block_custom['title'];
    $bfields['custom'] = (int) $block_custom['custom'];
    $bfields['visibility'] = (int) $block_custom['visibility'];
    $bfields['pages'] = $block_custom['pages'];
    $bfields['weight'] = $block_custom['weight'];
    $bfields['cache'] = $block_custom['cache'];
    $bfields['theme'] = $theme_data['name'];
    if (!empty($region)) {
      if (array_key_exists($region, $theme_data['info']['regions'])) {
        $bfields['region'] = $region;
      }
    }
    if (module_exists('block_class') && in_array('css_class', $bsfields)) {
      if (array_key_exists('css_class', $block_custom)) {
        $bfields['css_class'] = $block_custom['css_class'];
      }
    }

    $bfields['status'] = $block_custom['status'];
    // Insert the specific block entry into this table.
    db_insert('block')->fields($bfields)->execute();
  }

  // Set the block visibility settings.
  _block_export_import_import_visibility_settings($bdata, $block_id);

  $message = t("Block '!block_info' has been created successfully.", array('!block_info' => $block_desc));
  drupal_set_message(check_plain($message));
}

/**
 * Save handler for to save Visibility settings content types.
 *
 * @see _block_export_import_import_visibility_settings()
 */
function _block_export_import_import_node_type($node_types, $block_id) {
  $query = db_select('node_type', 'node_type');
  $query->fields('node_type', array('type'));
  $query->condition('node_type.type', $node_types, 'IN');
  $result = $query->execute();
  // Get the count value form the result set.
  $row_count = $result->rowCount();
  if ($row_count) {
    // Iterate through each database result set.
    foreach ($result as $data) {
      db_merge('block_node_type')->key(array('delta' => $block_id, 'type' => $data->type))->fields(array(
        'module' => 'block',
        'delta' => $block_id,
        'type' => $data->type,
        )
      )->execute();
    }
  }
}

/**
 * Save handler for to save Visibility settings roles.
 */
function _block_export_import_import_roles($roles, $block_id) {
  $query = db_select('role', 'role');
  $query->fields('role', array('rid'));
  $query->condition('role.rid', $roles, 'IN');
  $result = $query->execute();
  // Get the count value form the result set.
  $row_count = $result->rowCount();
  if ($row_count) {
    // Iterate through each database result set.
    foreach ($result as $data) {
      db_merge('block_role')->key(array('delta' => $block_id, 'rid' => $data->rid))->fields(array(
        'module' => 'block',
        'delta' => $block_id,
        'rid' => $data->rid,
        )
      )->execute();
    }
  }
}

/**
 * Save handler for to save Visibility settings content types and roles.
 *
 * @see _block_export_import_import_roles()
 * @see _block_export_import_import_node_type()
 */
function _block_export_import_import_visibility_settings($bdata, $block_id) {
  $visibility_settings = $bdata['visibility_settings'];
  if (count($visibility_settings)) {
    foreach ($visibility_settings as $key => $data) {
      switch ($key) {
        case 'roles':
          $roles = $visibility_settings['roles'];
          _block_export_import_import_roles($roles, $block_id);
          break;

        case 'node_types':
          $node_types = $visibility_settings['node_types'];
          _block_export_import_import_node_type($node_types, $block_id);
          break;
      }
    }
  }
}
