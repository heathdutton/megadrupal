<?php

/**
 * @file
 * Export all system specific blocks.
 */

/**
 * Form constructor for export all system specific blocks array structure.
 *
 * @see block_export_import_blocks_export_form_submit()
 * @see _get_importable_custom_block_object()
 * @see _export_custom_blocks_form_validate()
 */
function block_export_import_blocks_export_form($form, &$form_state) {
  // A condition to verify form element export_code is empty.
  if (empty($form_state['storage']['values'])) {
    // Get list of all currently available themes.
    $list_themes = list_themes($refresh = TRUE);
    $themes = array();
    foreach ($list_themes as $data) {
      if ($data->status) {
        $themes[$data->name] = $data->info['name'];
      }
    }

    $form['theme_name'] = array(
      '#type' => 'select',
      '#title' => t('Available Themes'),
      '#options' => array('' => t('-- Select --')) + (array) $themes,
      '#description' => t('Use only if you have the same region(s) where you are going to import.'),
      '#ajax' => array(
        // Here we set up our AJAX callback handler.
        'callback' => '_block_export_import_theme_region_callback',
        'wrapper' => 'block-export-import-theme-region-wrapper',
        'method' => 'replace',
        // Tell FormAPI about our progress bar.
        'progress' => array(
          'type' => 'throbber',
          'message' => t('Please wait...'),
        ),
      ),
    );
    $theme_name = isset($form_state['values']['theme_name']) ? $form_state['values']['theme_name'] : '';

    $header = array();
    $header['info'] = array('data' => t('Block Name'), 'field' => 'bc.info', 'sort' => 'ASC');
    if (!empty($theme_name)) {
      $theme_regions = $list_themes[$theme_name]->info['regions'];
      $header['region'] = array('data' => t('Region'), 'field' => 'b.region');
      $header['weight'] = array('data' => t('Weight'), 'field' => 'b.weight');
    }

    $header['action'] = array('data' => t('Action'));

    // This query is used to fetch all system specific block.
    $query = db_select('block_custom', 'bc');
    $query->fields('bc', array('bid', 'info'));
    if (!empty($theme_name)) {
      $query->leftJoin('block', 'b', 'bc.bid = b.delta');
      $query->fields('b', array('region', 'weight'));
      $query->condition('b.theme', $theme_name, '=');
    }

    $query = $query->extend('TableSort')->orderByHeader($header);
    $result = $query->execute();

    // Get the count value form result set.
    $row_count = $result->rowCount();

    $rows = array();
    if ($row_count) {
      // Iterate over each element in our $form['custom_block'] array.
      foreach ($result as $data) {
        // The block’s block.bid.
        $bid = $data->bid;
        // Block Description.
        $info = $data->info;

        $action_url = 'admin/structure/block/manage/block/' . $bid;
        $action_link_attr = array('attributes' => array('target' => '_blank'));
        $link = l(t('Edit'), $action_url, $action_link_attr);
        $rows[$bid]['info'] = array('data' => $info);
        if (!empty($theme_name)) {
          $region = $data->region;
          $region = isset($theme_regions[$region]) ? $theme_regions[$region] : 'none';
          $weight = $data->weight;
          $rows[$bid]['region'] = array('data' => $region);
          $rows[$bid]['weight'] = array('data' => $weight);
        }
        $rows[$bid]['action'] = array('data' => $link);
      }

      $form['custom_block'] = array(
        '#type' => 'tableselect',
        '#header' => $header,
        '#options' => $rows,
        '#prefix' => '<div id="block-export-import-theme-region-wrapper">',
        '#suffix' => '</div>',
        '#empty' => t('No system specific block exists.'),
      );
    }

    $form['custom_block'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $rows,
      '#prefix' => '<div id="block-export-import-theme-region-wrapper">',
      '#suffix' => '</div>',
      '#empty' => t('No system specific block exists.'),
    );

    if ($row_count) {
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Export'),
        '#validate' => array('_block_export_import_blocks_export_form_validate'),
        '#prefix' => '<div class="export-block-submit">',
        '#suffix' => '</div>',
      );
    }
  }
  else {
    // Get Importable array structure as string.
    $export_data = _block_export_import_get_importable_object($form_state);

    $export_desc = "For importing copy the content of the text area and paste";
    $export_desc .= " it into the import page.";
    $form['export_code'] = array(
      '#type' => 'textarea',
      '#title' => t('Export'),
      '#default_value' => $export_data,
      '#rows' => 12,
      '#description' => check_plain($export_desc),
    );

    $link = l(t('<< Back to export other blocks'), 'admin/structure/export-import-block/export');
    $form['back'] = array(
      '#type' => 'item',
      '#markup' => $link,
    );
  }
  return $form;
}

/**
 * Callback to fetch the block theme name and region.
 */
function _block_export_import_theme_region_callback($form, &$form_state) {
  return $form['custom_block'];
}

/**
 * Use to validate user must be selected at least one block to export.
 */
function _block_export_import_blocks_export_form_validate($form, $form_state) {
  $custom_block = $form_state['values']['custom_block'];

  $flag = FALSE;
  foreach ($custom_block as $value) {
    if ($value) {
      $flag = TRUE;
      break;
    }
  }

  if ($flag == FALSE) {
    form_set_error('custom_block', t('Please select at least one block.'));
  }
}

/**
 * Export an array object of all selected system specific blocks.
 *
 * @return array
 *   Formatted structured array for import.
 */
function _block_export_import_get_importable_object($form_state) {
  $values = $form_state['storage']['values'];

  // Verify that input text area have some code.
  if (!empty($values)) {
    $theme_name = $form_state['values']['theme_name'];
    // Use to fetch all fields ffrom block table.
    $block_schema_fields = _block_export_import_get_block_schema_fields();
    // Use to store an structured array as an string.
    $export_data = '';
    // Append the blocks array.
    $export_data = "\$block = array();\n";

    // Iterate for each selected block.
    foreach ($form_state['storage']['values'] as $bid => $checked) {
      if ((int) $checked) {
        // This query is used to fetch basic or full information of
        // seleted blocks.
        $query = db_select('block_custom', 'bc');
        $query->fields('bc', array('info', 'body', 'format'));
        $query->innerJoin('block', 'b', 'b.delta = bc.bid');
        $query->leftJoin('block_node_type', 'bnt', 'bc.bid = bnt.delta');
        $query->fields('bnt', array('type'));
        $query->leftJoin('block_role', 'br', 'bc.bid = br.delta');
        $query->fields('br', array('rid'));
        $query->fields('b', $block_schema_fields);
        $query->condition('bc.bid', $bid);
        if (!empty($theme_name)) {
          $query->condition('b.theme', $theme_name, '=');
        }
        $result = $query->execute();

        // Get the count value form result set.
        $row_count = $result->rowCount();
        if ($row_count) {
          $block_obj = array();
          // Iterate through each database result.
          foreach ($result as $key => $data) {
            // The Block description.
            $block_obj['data']['info'] = $data->info;
            // Custom title for the block.
            $block_obj['data']['title'] = $data->title;
            // The Block body contents.
            $block_obj['data']['body'] = addslashes($data->body);
            // The filter_format.format of the block body.
            $block_obj['data']['format'] = $data->format;
            if (!empty($theme_name)) {
              $block_obj['data']['theme'] = $data->theme;
              $block_obj['data']['region'] = $data->region;
            }
            $block_obj['data']['weight'] = $data->weight;
            $block_obj['data']['cache'] = $data->cache;
            // The Flag to indicate how users may control visibility of
            // the block.
            $block_obj['data']['custom'] = $data->custom;
            // Flag to indicate how to show blocks on pages.
            $block_obj['data']['visibility'] = $data->visibility;
            // The Contents of the "Pages" block; contains either a list of
            // paths on which to include/exclude the block or PHP code,
            // depending on "visibility" setting.
            $block_obj['data']['pages'] = $data->pages;
            // Customize the styling of this block by adding CSS classes.
            if (array_key_exists('css_class', $data)) {
              $block_obj['data']['css_class'] = $data->css_class;
            }
            $block_obj['data']['status'] = $data->status;

            if ((int) $data->rid) {
              $rid = $data->rid;
              // The result may have some duplicate records but here we are
              // overriding it by same key of block_role array.
              // The user’s role ID.
              $block_obj['data']['visibility_settings']['role'][$rid] = $rid;
            }

            if ($data->type) {
              $type = $data->type;
              // The result may have some duplicate records but here we are
              // overriding it by same key of block_node_type array.
              // The machine-readable name of  conten type.
              $block_obj['data']['visibility_settings']['node_type'][$type] = $type;
            }
          }

          // Iterate through each block structured array.
          foreach ($block_obj as $data) {
            $export_data .= "\$block[] = array(\n";
            // Append Basic Information.
            $export_data .= "  'block_custom' => array(\n";
            $export_data .= "  'info' => '" . $data['info'] . "', \n";
            $export_data .= "  'format' => '" . $data['format'] . "', \n";
            $export_data .= "  'title' => '" . $data['title'] . "', \n";
            $export_data .= "  'body' => '" . $data['body'] . "', \n";
            $export_data .= "  'weight' => '" . $data['weight'] . "', \n";
            $export_data .= "  'cache' => '" . $data['cache'] . "', \n";
            $export_data .= "  'custom' => '" . $data['custom'] . "', \n";
            $export_data .= "  'visibility' => '" . $data['visibility'] . "', \n";
            $export_data .= "  'pages' => '" . $data['pages'] . "', \n";
            if (array_key_exists('theme', $data)) {
              $export_data .= "  'theme' => '" . $data['theme'] . "', \n";
              $export_data .= "  'region' => '" . $data['region'] . "', \n";
            }
            //Customize the styling of this block by adding CSS classes
            if (array_key_exists('css_class', $data)) {
              $export_data .= "  'css_class' => '" . $data['css_class'] . "', \n";
            }
            $export_data .= "  'status' => '" . $data['status'] . "', \n";
            $export_data .= "  ),\n";

            if (array_key_exists('visibility_settings', $data)) {
              $visibility_data = $data['visibility_settings'];
              $export_data .= '// Visibility settings  --------------' . "\n";

              // Append Visibility settings.
              $export_data .= " 'visibility_settings' =>  array(";
              $export_data .= "\n";
              foreach ($visibility_data as $key => $vdata) {
                switch ($key) {
                  case 'role':
                    $export_data .= "  'roles' => array(\n";
                    foreach ($vdata as $role_id) {
                      $export_data .= "  " . (int) $role_id . ", \n";
                    }
                    $export_data .= "  ),\n";
                    break;

                  case 'node_type':
                    $export_data .= "  'node_types' => array(\n";
                    foreach ($vdata as $node_type) {
                      $export_data .= "  '" . $node_type . "', \n";
                    }
                    $export_data .= "  ),\n";
                    break;
                }
              }
              $export_data .= " ), \n";
            }
            $export_data .= ");\n\n";
          }
        }
      }
    }
    return $export_data;
  }
}

/**
 * Submit handler for blockexport_export_blocks_form_submit() to save a blocks.
 */
function block_export_import_blocks_export_form_submit($form, &$form_state) {
  if (empty($form_state['storage']['values'])) {
    // If there is no previous values redraw for next step.
    $form_state['storage']['values'] = $form_state['values']['custom_block'];
    $form_state['rebuild'] = TRUE;
  }
}
