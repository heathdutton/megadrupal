<?php

/**
 * @file
 * Admin callbacks.
 */

/**
 * Admin overview callback page.
 */
function cta_blocks_admin_list() {
  $cta_blocks = cta_blocks_cta_block_load();

  $header = array(
    t('Name'),
    t('Title'),
    t('Link'),
    t('Status'),
    array('data' => '', 'colspan' => 3),
  );

  $rows = array();
  $links = '';

  foreach ($cta_blocks as $key => $block) {
    $row = array();
    $row[0] = check_plain($block->name);
    $row[1] = check_plain($block->title);
    $row[2] = l($block->link, $block->link);
    $row[4] = l(
      t('Edit'),
      'admin/structure/block/cta-blocks/edit/' . $block->machine_name,
      array(
        'attributes' => array(
          'class' => array(
            'link-edit',
          ),
        ),
      )
    );

    switch ($block->export_type) {
      case 1:
        // Normal type.
        $row[3] = '<em>' . t('Normal') . '</em>';
        $row[5] = l(
          t('Delete'),
          'admin/structure/block/cta-blocks/delete/' . $block->machine_name,
          array(
            'attributes' => array(
              'class' => array(
                'link-delete',
              ),
            ),
          )
        );
        break;

      case 3:
        // Overridden.
        $row[3] = '<em>' . t('Overridden') . '</em>';
        $row[5] = l(
          t('Revert'),
          'admin/structure/block/cta-blocks/delete/' . $block->machine_name,
          array(
            'attributes' => array(
              'class' => array(
                'link-delete',
              ),
            ),
          )
        );
        break;

      default:
        // Default.
        $row[3] = '<em>' . t('Default') . '</em>';
        $row[5] = '';
        break;
    }
    $row[6] = l(
      t('Export'),
      'admin/structure/block/cta-blocks/export/' . $block->machine_name,
      array(
        'attributes' => array(
          'class' => array(
            'link-export',
          ),
        ),
      )
    );

    ksort($row);

    $rows[] = $row;
  }

  $table = theme(
    'table',
    array(
      'header'     => $header,
      'rows'       => $rows,
      'attributes' => array(),
      'sticky'     => FALSE,
      'empty'      => t('There are no CTA Blocks available.'),
    )
  );

  return $table;
}

/**
 * Add/edit form function.
 */
function cta_blocks_admin_block($form, &$form_state, $cta_block = NULL) {
  if ($cta_block) {
    drupal_set_title(t('@name settings', array('@name' => check_plain($cta_block->name))));
  }
  else {
    drupal_set_title(t('Add CTA Block'));
    $cta_block               = new stdClass();
    $cta_block->machine_name = '';
    $cta_block->name         = '';
    $cta_block->title        = '';
    $cta_block->subtitle     = '';
    $cta_block->link         = '';
  }

  $form['existing'] = array(
    '#type'  => 'hidden',
    '#value' => ($cta_block->machine_name) ? TRUE : FALSE,
  );

  $form['name'] = array(
    '#title'         => t('Name'),
    '#description'   => t('Human-readable name for this CTA Block.'),
    '#type'          => 'textfield',
    '#default_value' => $cta_block->name,
    '#required'      => TRUE,
  );

  $form['machine_name'] = array(
    '#type'          => 'machine_name',
    '#default_value' => $cta_block->machine_name,
    '#maxlength'     => 28,
    '#machine_name'  => array(
      'exists' => 'cta_blocks_machine_name_exists',
    ),
    // Prevent changing a machine name for an existing cta block.
    '#disabled'      => $cta_block->machine_name ? TRUE : FALSE,
  );

  $form['title'] = array(
    '#title'         => t('Title'),
    '#description'   => t('Call to action title.'),
    '#required'      => TRUE,
    '#type'          => 'textfield',
    '#maxlength'     => 255,
    '#default_value' => $cta_block->title,
  );

  $form['subtitle'] = array(
    '#title'         => t('Subtitle'),
    '#description'   => t('Optional call to action subtitle.'),
    '#type'          => 'textfield',
    '#maxlength'     => 1024,
    '#default_value' => $cta_block->subtitle,
  );

  $form['link'] = array(
    '#title'         => t('Link/Path'),
    '#description'   => t('Full URL or internal path to link to with this call to action block.'),
    '#type'          => 'textfield',
    '#maxlength'     => 512,
    '#default_value' => $cta_block->link,
    '#required'      => TRUE,
  );

  $form['actions'] = array(
    'submit' => array(
      '#type'  => 'submit',
      '#value' => $cta_block->machine_name ? t('Update') : t('Save'),
    ),
    'cancel' => array(
      '#type'  => 'link',
      '#title' => t('Cancel'),
      '#href'  => 'admin/structure/block/cta-blocks',
    ),
  );

  return $form;
}

/**
 * Validate a cta block form.
 */
function cta_blocks_admin_block_validate($form, &$form_state) {
  // Check for illegal characters in machine_name field.
  if (preg_match('/[^0-9a-zA-Z_-]/', $form_state['values']['machine_name'])) {
    form_set_error('machine_name', t('Please use only alphanumeric characters, underscores (_) and hyphens (-) for machine names.'));
  }
}

/**
 * Submit handler for cta block form.
 */
function cta_blocks_admin_block_submit($form, &$form_state) {

  if ($form_state['values']['existing']) {
    $block = cta_blocks_cta_block_load($form_state['values']['machine_name']);
    if ($block && $block->export_type == 2) {
      $res = drupal_write_record('cta_blocks', $form_state['values']);
    }
    else {
      $res = drupal_write_record('cta_blocks', $form_state['values'], 'machine_name');
    }
  }
  else {
    $res = drupal_write_record('cta_blocks', $form_state['values']);
  }

  if ($res) {
    drupal_set_message(t('Saved CTA Block %name.', array('%name' => $form_state['values']['name'])), 'status');
  }
  else {
    drupal_set_message(t('Failed to save CTA Block %name.', array('%name' => $form_state['values']['name'])), 'warning');
  }

  drupal_goto('admin/structure/block/cta-blocks');
}

/**
 * Callback function to test for existing machine name.
 */
function cta_blocks_machine_name_exists($machine_name) {
  $exists = db_query_range('SELECT 1 FROM {cta_blocks} WHERE machine_name = :machine_name', 0, 1, array(':machine_name' => $machine_name))->fetchField();

  return $exists || cta_blocks_cta_block_load($machine_name);
}


/**
 * Delete cta_block form.
 */
function cta_blocks_admin_block_delete($form, &$form_state, $cta_block) {
  $form['machine_name'] = array(
    '#type'  => 'hidden',
    '#value' => $cta_block->machine_name,
  );

  $form['name'] = array(
    '#type'  => 'hidden',
    '#value' => $cta_block->name,
  );

  $form['export_type'] = array(
    '#type'  => 'hidden',
    '#value' => $cta_block->export_type,
  );

  switch ($cta_block->export_type) {
    case 1:
      // "Normal" type.
      $message = t('Are you sure you want to delete %title?', array('%title' => $cta_block->name));
      $button  = t('Delete');
      break;

    case 3:
      // "Overridden" type.
      $message = t('Are you sure you want to revert %title?', array('%title' => $cta_block->name));
      $button  = t('Revert');
      break;

    default:
      // There are no other export types that can be deleted.
      drupal_goto('admin/structure/block/cta-blocks');
      return array();
  }

  return confirm_form(
    $form,
    $message,
    isset($_GET['destination']) ? $_GET['destination'] : 'admin/structure/block/cta-blocks',
    t('This action cannot be undone.'),
    $button,
    t('Cancel')
  );
}

/**
 * Submit handler to delete a cta block.
 */
function cta_blocks_admin_block_delete_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    cta_blocks_cta_block_delete($form_state['values']['machine_name']);
    // Adjust message depending on export type.
    $message = $form_state['values']['export_type'] == 1 ?
      'Deleted preset %name.' :
      'Reverted preset %name.';
    drupal_set_message(t($message, array('%name' => $form_state['values']['name'])));
  }
  $form_state['redirect'] = 'admin/structure/block/cta-blocks';
}

/**
 * Page callback: export a cta block.
 */
function cta_blocks_admin_block_export($form, &$form_state, $cta_block) {
  drupal_set_title(t('Export of CTA Block !name', array('!name' => $cta_block->name)));
  $form = array();

  // Get export code with Ctools.
  $export = ctools_export_object('cta_blocks', $cta_block);
  $export = "{$export}return serialize(\$cta_block);";

  // Create the export code textarea.
  ctools_include('export');
  $form['export'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Code'),
    '#rows'          => 20,
    '#default_value' => $export,
  );

  return $form;
}

/**
 * Page callback: import cta_block.
 */
function cta_blocks_admin_block_import($form, &$form_state) {
  $form['import'] = array(
    '#type' => 'textarea',
    '#rows' => 10,
  );
  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Import'),
  );

  return $form;
}

/**
 * Validate a cta block import.
 */
function cta_blocks_admin_block_import_validate($form, &$form_state) {
  // Run the import code, which should return a serialized $preset object.
  if (substr($form_state['values']['import'], 0, 5) == '<?php') {
    $form_state['values']['import'] = substr($form_state['values']['import'], 5);
  }
  ob_start();
  eval($form_state['values']['import']);
  ob_end_clean();

  if (empty($cta_block) || !is_object($cta_block) || empty($cta_block->machine_name)) {
    form_set_error('import', t('The submitted CTA Block code could not be interperated.'));
  }
  elseif (cta_blocks_cta_block_load($cta_block->machine_name)) {
    form_set_error('import', t('A CTA Block with that machine name already exists.'));
  }
  else {
    // Pass the parsed object on to the submit handler.
    $form_state['values']['import_parsed'] = $cta_block;
  }
}

/**
 * Submit handler to import a cta block.
 */
function cta_blocks_admin_block_import_submit($form, &$form_state) {
  $cta_block = (array) $form_state['values']['import_parsed'];

  if (drupal_write_record('cta_blocks', $cta_block)) {
    drupal_set_message(t('Imported CTA Block %name.', array('%name' => $cta_block['name'])));
  }
  else {
    drupal_set_message(t('Failed to import the CTA Block.'), 'warning');
  }

  $form_state['redirect'] = 'admin/structure/block/cta-blocks';
}
