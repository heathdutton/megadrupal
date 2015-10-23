<?php

/**
 * @file
 * Admin Interface UI
 *
 * This file represents the admin UI of the module.
 */

function _pdfck_pdf_elements_name($type, $id, $link = true) {
  switch ($type) {
    case 'node':
      $node = node_load($id);
      return $link ? l($node->title, 'node/' . $id) : check_plain($node->title);
      break;
    case 'view':
      $views = views_get_view($id);
      return $link ? l($views->name, 'admin/structure/views/view/' . $id) : check_plain($views->name);
      break;
  }
}

/**
 * Deletes both the file from the disk and the database.
 */
function pdfck_pdf_file_delete($fid) {
  $file = file_load($fid);
  file_delete($file);
  db_delete('pdfck_files')
  ->condition('fid', $fid)
  ->execute();
}

/**
 * Menu callback. Main overview page.
 */
function pdfck_overview() {
  $headers = array();
  $headers[] = t('Name');
  $headers[] = t('Description');
  $headers[] = t('Operations');

  $result = db_select('pdfck_pdf', 'p')
    ->fields('p', array('pdfid', 'name', 'description'))
    ->execute();
  
  $rows = array();
  foreach ($result as $row) {
    $r = array();
    $r[] = $row->name;
    $r[] = $row->description;
    $links = array();
    $links[] = l(t('edit'), 'admin/content/pdfck/manage/' . $row->pdfid . '/edit');
    $links[] = l(t('elements'), 'admin/content/pdfck/manage/' . $row->pdfid . '/elements');
    $links[] = l(t('generate'), 'admin/content/pdfck/manage/' . $row->pdfid . '/generate');
    $links[] = l(t('files'), 'admin/content/pdfck/manage/' . $row->pdfid . '/files');
    $links[] = l(t('delete'), 'admin/content/pdfck/manage/' . $row->pdfid . '/delete');
    $r[] = implode($links, ' | ');
    $rows[] = $r;
  }
  $output = theme('table', array('header' => $headers, 'rows' => $rows, 'empty' => t('There are currently no PDFs. Please go ahead and <a href="!add_uri">add one</a>.', array('!add_uri' => url('admin/content/pdfck/add')))));

  return $output;
}

/**
 * Form constructor for each PDF settings
 *
 * @ingroup forms
 */
function pdfck_pdf_form($form, &$form_state, $pdfck = NULL) {
  $form = array();
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('A human readable identifier of the PDF.'),
    '#required' => TRUE,
    '#default_value' => isset($pdfck->name) ? $pdfck->name : '',
  );

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#description' => t('The description of the PDF.'),
    '#default_value' => isset($pdfck->description) ? $pdfck->description : '',
  );


  $scheme_options = array();
  foreach (file_get_stream_wrappers(STREAM_WRAPPERS_WRITE_VISIBLE) as $scheme => $stream_wrapper) {
    $scheme_options[$scheme] = $stream_wrapper['name'];
  }
  $form['uri_scheme'] = array(
    '#type' => 'radios',
    '#title' => t('Destination'),
    '#options' => $scheme_options,
    '#default_value' => isset($pdfck->uri_scheme) ? $pdfck->uri_scheme : file_default_scheme(),
    '#description' => t('Select where the final files should be stored. Private file storage has significantly more overhead than public files, but allows restricted access to files within this field.'),
  );

  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#description' => t('The path within the upload destination in which the pdf will be generated.'),
    '#default_value' => isset($pdfck->path) && !empty($pdfck->path) ? $pdfck->path : 'pdfck',
  );

  $form['filename'] = array(
    '#type' => 'textfield',
    '#title' => t('Filename (no .pdf extension)'),
    '#description' => t('The filename of the PDF. If blank, the identifier above will be used. Tokens may be used to build the filename. The .pdf extension will be appended automatically.'),
    '#default_value' => isset($pdfck->filename) ? $pdfck->filename : '',
  );

  if (module_exists('token')) {
    $form['token_help'] = array(
      '#title' => t('Replacement patterns'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['token_help']['help'] = array(
      '#markup' => theme('token_tree'),
    );
  }

  $form['keep'] = array(
    '#type' => 'radios',
    '#title' => t('Keep'),
    '#options' => array(
      FILE_EXISTS_REPLACE => t('Replace'),
      FILE_EXISTS_RENAME => t('Keep'),
    ),
    '#description' => t('Wether the generated file should replace the previous ones, or if every generated file will be kept. If kept, and if the filename is the same, it will be automatically renamed.'),
    '#default_value' => isset($pdfck->keep) ? $pdfck->keep : 0,
  );

  if (isset($pdfck->pdfid) && $pdfck->pdfid) {
    $form['pdfid'] = array(
      '#type' => 'hidden',
      '#value' => isset($pdfck->pdfid) ? $pdfck->pdfid : '',
    );
  }

  if (isset($pdfck->name) && !empty($pdfck->name)) {
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete PDF'),
      '#weight' => 45,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Form submit handler for pdfck_pdf_form().
 *
 * @see pdfck_pdf_form()
 */
function pdfck_pdf_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  if ($op == t('Delete PDF')) {
    $form_state['redirect'] = 'admin/content/pdfck/manage/' . $form_state['values']['pdfid'] . '/delete';
    return;
  }

  $object = (object) $form_state['values'];

  $update = array();
  if (isset($object->pdfid) && $object->pdfid) {
    $update[] = 'pdfid';
  }
  $ret = drupal_write_record('pdfck_pdf', $object, $update);

  if ($ret == SAVED_NEW) {
    drupal_set_message(t('PDF created'));
  }
  if ($ret == SAVED_UPDATED) {
    drupal_set_message(t('PDF updated'));
  }

  $form_state['redirect'] = 'admin/content/pdfck';
  return;
}

/**
 * Form constructor for adding elements to the PDF
 *
 * @ingroup forms
 */
function pdfck_pdf_elements_form($form, &$form_state, $pdfck) {
  // Store the default weights as we meet them, to be able to put the
  //'add new' rows after them.
  $weights = array(0);

  $form = array(
    '#tree' => TRUE,
    '#pdf_elements' => array(),
  );

  $result = db_select('pdfck_elements', 'e')
    ->fields('e', array('elid', 'type', 'id', 'weight'))
    ->condition('pdfid', $pdfck->pdfid)
    ->orderBy('e.weight')
    ->execute();

  foreach ($result as $row) {
    $form[$row->elid] = array(
      'type' => array(
        '#markup' => check_plain($row->type),
      ),
      'id' => array(
        '#markup' => check_plain($row->id),
      ),
      'item' => array(
        '#type' => 'markup',
        '#markup' => _pdfck_pdf_elements_name($row->type, $row->id),
      ),
      'weight' => array(
        '#type' => 'textfield',
        '#default_value' => $row->weight,
        '#size' => 3,
      ),
      'remove' => array('#markup' => l(t('delete'), 'admin/content/pdfck/manage/' . $pdfck->pdfid . '/elements/' . $row->elid . '/delete')),
    );
    $form['#pdf_elements'][] = $row->elid;

    $weights[] = $row->weight;
  }

  $form['pdfid'] = array(
    '#type' => 'value',
    '#value' => $pdfck->pdfid,
  );

  $form['add_new_node'] = array(
    '#title' => t('Add new node'),
    '#type' => 'textfield',
    '#autocomplete_path' => 'pdfck/autocomplete/nodes',
    '#description' => t('Start typing the tile of the node.'),
  );

  $types = variable_get('pdfck_content_types', array());
  $types_filtered = array_filter($types);

  if (!empty($types_filtered)) {
    $content_types = node_type_get_types();
    $types_selected = array();
    foreach ($types_filtered as $type) {
      $types_selected[] = $content_types[$type]->name;
    }
    $form['add_new_node']['#description'] .= ' ' . t('<a href="!admin_uri">Search will be performed <strong>only</strong> on the following content types: %content_types.</a>', array('%content_types' => implode(', ', $types_selected), '!admin_uri' => url('admin/config/media/pdfck', array('query' => drupal_get_destination()))));
  }

  if (module_exists('views')) {
    $options = array('' => t('Select one'));
    $views = views_get_all_views();
    foreach ($views as $view) {
      if ($view->base_table == 'node') {
        $options[$view->name] = $view->name;
      }
    }
    $form['add_new_view'] = array(
      '#title' => t('Add new view'),
      '#type' => 'select',
      '#options' => $options,
      '#description' => t('Select a view. Only the views that has the node table as the base table will be shown here.'),
    );
  }

  $form['add_new_weight'] = array(
    '#type' => 'value',
    '#value' => max($weights),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Form submit handler for pdfck_pdf_elements_form().
 *
 * @see pdfck_pdf_elements_form()
 */
function pdfck_pdf_elements_form_submit($form, &$form_state) {
  if (!empty($form_state['values']['add_new_node'])) {
    // Store node element
    $object = new stdClass();
    $object->pdfid = $form_state['values']['pdfid'];
    $object->type = 'node';
    if (preg_match('/\[nid:([0-9]+)\]$/', $form_state['values']['add_new_node'], $matches)) {
      $object->id = $matches[1];
      $object->weight = $form_state['values']['add_new_weight'];
      $node = node_load($object->id);
      drupal_write_record('pdfck_elements', $object);
    }
  }
  if (!empty($form_state['values']['add_new_view'])) {
    // Store node element
    $object = new stdClass();
    $object->pdfid = $form_state['values']['pdfid'];
    $object->type = 'view';
    $object->id = $form_state['values']['add_new_view'];
    $object->weight = $form_state['values']['add_new_weight'];
    drupal_write_record('pdfck_elements', $object);
  }

  foreach ($form['#pdf_elements'] as $elid) {
    $object = new stdClass();
    $object->elid = $elid;
    $object->weight = $form_state['values'][$elid]['weight'];
    drupal_write_record('pdfck_elements', $object, 'elid');
  }
}

/**
 * Menu callback. Form constructor to confirm a delete of a PDF.
 *
 * @ingroup forms
 */
function pdfck_pdf_delete_confirm_form($form, &$form_state, $pdfck) {
  $form = array();
  $form['pdfid'] = array(
    '#type' => 'value',
    '#value' => $pdfck->pdfid,
  );
  $form['name'] = array(
    '#type' => 'value',
    '#value' => $pdfck->name,
  );

  $message = t('Are you sure you want to delete the pdf %pdf?', array('%pdf' => check_plain($pdfck->name)));
  $caption = '<p>' . t('This action cannot be undone. <b>This will also delete all of the generated files.</b>') . '</p>';

  return confirm_form($form, $message, 'admin/content/pdfck', $caption, t('Delete'));
}


/**
 * Form submit handler for pdfck_pdf_delete_confirm_form().
 *
 * Removes everything of a PDF, including files.
 *
 * @see pdfck_pdf_delete_confirm_form()
 */
function pdfck_pdf_delete_confirm_form_submit($form, &$form_state) {

  db_delete('pdfck_pdf')
    ->condition('pdfid', $form_state['values']['pdfid'])
    ->execute();
  
  db_delete('pdfck_elements')
    ->condition('pdfid', $form_state['values']['pdfid'])
    ->execute();

  // Delete files
  $result = db_select('pdfck_files', 'f')
    ->fields('f', array('fid'))
    ->condition('pdfid', $form_state['values']['pdfid'])
    ->execute();
  
  foreach($result as $row) {
    pdfck_pdf_file_delete($row->fid);
  }

  $t_args = array('%name' => $form_state['values']['name']);
  drupal_set_message(t('The PDF %name has been deleted.', $t_args));
  watchdog('pdfck', 'Deleted PDF %name.', $t_args, WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/content/pdfck';
  return;
}

/**
 * Menu callback. Form constructor to confirm a delete of a PDF element.
 *
 * @ingroup forms
 */
function pdfck_pdf_element_delete_confirm_form($form, &$form_state, $pdfck, $element) {
  $form = array();
  $form['pdfid'] = array(
    '#type' => 'value',
    '#value' => $pdfck->pdfid,
  );
  $form['elid'] = array(
    '#type' => 'value',
    '#value' => $element->elid,
  );
  $form['name'] = array(
    '#type' => 'value',
    '#value' => _pdfck_pdf_elements_name($element->type, $element->id, false),
  );

  $message = t('Are you sure you want to delete the element %name from the PDF %pdf?', array('%name' => $form['name']['#value'], '%pdf' => $pdfck->name));
  $caption = '<p>' . t('This action cannot be undone.') . '</p>';

  return confirm_form($form, $message, 'admin/content/pdfck/manage/' . $pdfck->pdfid . '/elements', $caption, t('Delete'));
}

/**
 * Form submit handler for pdfck_pdf_element_delete_confirm_form().
 *
 * @see pdfck_pdf_element_delete_confirm_form()
 */
function pdfck_pdf_element_delete_confirm_form_submit($form, &$form_state) {
  db_delete('pdfck_elements')
    ->condition('elid', $form_state['values']['elid'])
    ->execute();

  $t_args = array('%name' => $form_state['values']['name']);
  drupal_set_message(t('The element %name has been removed.', $t_args));

  $form_state['redirect'] = 'admin/content/pdfck/manage/' . $form_state['values']['pdfid'] . '/elements';
  return;
}

/**
 * Menu callback. Overview page of the generated files for each PDF
 */
function pdfck_pdf_files($pdfck) {
  $output = '';

  $headers = array();
  $headers[] = t('File');
  $headers[] = t('Operations');

  $result = db_select('pdfck_files', 'f')
    ->fields('f')
    ->condition('f.pdfid', $pdfck->pdfid)
    ->extend('PagerDefault')
    ->limit('10')
    ->execute();
  
  $rows = array();
  foreach ($result as $row) {
    $r = array();
    $file = file_load($row->fid);
    $path = file_create_url($file->uri);
    $r[] = $file->filename;
    $links = array();
    $links[] = l(t('download'), $path);
    $links[] = l(t('delete'), 'admin/content/pdfck/manage/' . $row->pdfid . '/files/' . $file->fid . '/delete');
    $r[] = implode($links, ' | ');
    $rows[] = $r;
  }

  $output .= theme('table', array('header' => $headers, 'rows' => $rows, 'empty' => t('A PDF has not been generated yet.') ));
  $output .= theme('pager');

  return $output;
}

/**
 * Menu callback. Form constructor to confirm a delete of a PDF file.
 *
 * @ingroup forms
 */
function pdfck_pdf_file_delete_confirm_form($form, &$form_state, $pdfck, $file) {
  $form = array();
  $form['pdfid'] = array(
    '#type' => 'value',
    '#value' => $pdfck->pdfid,
  );
  $form['fid'] = array(
    '#type' => 'value',
    '#value' => $file->fid,
  );
  $form['filename'] = array(
    '#type' => 'value',
    '#value' => $file->filename,
  );

  $message = t('Are you sure you want to delete the file %name from the PDF %pdf?', array('%name' => $file->filename, '%pdf' => $pdfck->name));
  $caption = '<p>' . t('This action cannot be undone.') . '</p>';

  return confirm_form($form, $message, 'admin/content/pdfck/manage/' . $pdfck->pdfid . '/files', $caption, t('Delete'));
}

/**
 * Form submit handler for pdfck_pdf_element_delete_confirm_form().
 *
 * @see pdfck_pdf_element_delete_confirm_form()
 */
function pdfck_pdf_file_delete_confirm_form_submit($form, &$form_state) {
  pdfck_pdf_file_delete($form_state['values']['fid']);

  $t_args = array('%name' => $form_state['values']['filename']);
  drupal_set_message(t('The file %name has been removed.', $t_args));

  $form_state['redirect'] = 'admin/content/pdfck/manage/' . $form_state['values']['pdfid'] . '/files';
  return;
}

/**
 * Theme the elements admin UI form.
 *
 * @ingroup themable
 *
 */
function theme_pdfck_pdf_elements_form($variables) {
  $form = $variables['form'];
  $headers = array();
  $headers[] = t('Type');
  $headers[] = t('Weight');
  $headers[] = t('ID');
  $headers[] = t('Item');
  $headers[] = t('Operations');
  $rows = array();
  foreach ($form['#pdf_elements'] as $elid) {
    $r = array();
    $r[] = drupal_render($form[$elid]['type']);
    $form[$elid]['weight']['#attributes']['class'] = array("pdfck-elements-weight");
    $r[] = drupal_render($form[$elid]['weight']);
    $r[] = drupal_render($form[$elid]['id']);
    $r[] = drupal_render($form[$elid]['item']);
    $r[] = drupal_render($form[$elid]['remove']);
    $rows[] = array(
      'data' => $r,
      'class' => array('draggable'),
    );
  }
  $output = '';
  if (count($rows)) {
    $output .= theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('id' => 'pdfck-elements-table')));
  }
  drupal_add_tabledrag('pdfck-elements-table', 'order', 'sibling', 'pdfck-elements-weight');
  $output .= drupal_render_children($form);

  return $output;
}
