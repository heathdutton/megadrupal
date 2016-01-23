<?php

/**
 * @file
 * Webform module file component.
 */

/**
 * Implementation of _webform_defaults_component().
 */
function _webform_defaults_multifile() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'mandatory' => 0,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'filtering' => array(
        'types' => array('gif', 'jpg', 'png'),
        'addextensions' => '',
        'size' => 800,
      ),
      'max_amount' => -1,
      'scheme' => 'public',
      'directory' => '',
      'progress_indicator' => 'throbber',
      'width' => '',
      'title_display' => 0,
      'description' => '',
      'attributes' => array(),
      'private' => FALSE,
    ),
  );
}

/**
 * Implementation of _webform_theme_component().
 */
function _webform_theme_multifile() {
  return array(
    'webform_edit_multifile' => array(
      'render element' => 'form',
    ),
    'webform_render_multifile' => array(
      'render element' => 'element',
    ),
    'webform_display_multifile' => array(
      'render element' => 'element',
    ),
  );
}

/**
 * Implementation of _webform_edit_component().
 */
function _webform_edit_multifile($component) {
    webform_component_include('file');
    $form = array();
    $form['#theme'] = 'webform_edit_multifile';
    $form['#element_validate'] = array('_webform_edit_multifile_check_directory');
    $form['#after_build'] = array('_webform_edit_multifile_check_directory');
    $options[-1] = t('Unlimited');
    $options += drupal_map_assoc(range(1, 10));
    $form['validation']['max_amount'] = array(
      '#title' => t('File limit'),
      '#description' => t('The number of files the user is allowed to upload per submission with this component.'),
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $component['extra']['max_amount'],
      '#parents' => array('extra', 'max_amount'),
    );

    $form['validation']['filtering'] = array(
      '#element_validate' => array('_webform_edit_multifile_filtering_validate'),
      '#parents' => array('extra', 'filtering'),
    );

    // Find the list of all currently valid extensions.
    $current_types = isset($component['extra']['filtering']['types']) ? $component['extra']['filtering']['types'] : array();

    $types = array('gif', 'jpg', 'png');
    $form['validation']['filtering']['types']['webimages'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Web Images'),
      '#options' => drupal_map_assoc($types),
      '#default_value' => array_intersect($current_types, $types),
    );

    $types = array('bmp', 'eps', 'tif', 'pict', 'psd');
    $form['validation']['filtering']['types']['desktopimages'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Desktop Images'),
      '#options' => drupal_map_assoc($types),
      '#default_value' => array_intersect($current_types, $types),
    );

    $types = array('txt', 'rtf', 'html', 'odf', 'pdf', 'doc', 'docx', 'ppt', 'pptx', 'xls', 'xlsx', 'xml');
    $form['validation']['filtering']['types']['documents'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Documents'),
      '#options' => drupal_map_assoc($types),
      '#default_value' => array_intersect($current_types, $types),
    );

    $types = array('avi', 'mov', 'mp3', 'ogg', 'wav');
    $form['validation']['filtering']['types']['media'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Media'),
      '#options' => drupal_map_assoc($types),
      '#default_value' => array_intersect($current_types, $types),
    );

    $types = array('bz2', 'dmg', 'gz', 'jar', 'rar', 'sit', 'tar', 'zip');
    $form['validation']['filtering']['types']['archives'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Archives'),
      '#options' => drupal_map_assoc($types),
      '#default_value' => array_intersect($current_types, $types),
    );

    $form['validation']['filtering']['addextensions'] = array(
      '#type' => 'textfield',
      '#title' => t('Additional Extensions'),
      '#default_value' => $component['extra']['filtering']['addextensions'],
      '#description' => t('Enter a list of additional file extensions for this upload field, seperated by commas.<br /> Entered extensions will be appended to checked items above.'),
      '#size' => 60,
      '#weight' => 3,
      '#default_value' => $component['extra']['filtering']['addextensions'],
    );

    $form['validation']['filtering']['size'] = array(
      '#type' => 'textfield',
      '#title' => t('Max Upload Size'),
      '#default_value' => $component['extra']['filtering']['size'],
      '#description' => t('Enter the max file size a user may upload (in KB).'),
      '#size' => 10,
      '#weight' => 3,
      '#field_suffix' => t('KB'),
      '#default_value' => $component['extra']['filtering']['size'],
      '#parents' => array('extra', 'filtering', 'size'),
      '#element_validate' => array('_webform_edit_multifile_size_validate'),
    );
    $scheme_options = array();
    foreach (file_get_stream_wrappers(STREAM_WRAPPERS_WRITE_VISIBLE) as $scheme => $stream_wrapper) {
      $scheme_options[$scheme] = $stream_wrapper['name'];
    }
    $form['extra']['scheme'] = array(
      '#type' => 'radios',
      '#title' => t('Upload destination'),
      '#options' => $scheme_options,
      '#default_value' => $component['extra']['scheme'],
      '#description' => t('Private file storage has significantly more overhead than public files, but restricts file access to users who can view submissions.'),
      '#weight' => 4,
      '#access' => count($scheme_options) > 1,
    );
    $form['extra']['directory'] = array(
      '#type' => 'textfield',
      '#title' => t('Upload directory'),
      '#default_value' => $component['extra']['directory'],
      '#description' => t('You may optionally specify a sub-directory to store your files.'),
      '#weight' => 5,
      '#field_prefix' => 'sites/default/files/webform/',
    );
    $form['display']['progress_indicator'] = array(
      '#type' => 'radios',
      '#title' => t('Progress indicator'),
      '#options' => array(
        'throbber' => t('Throbber'),
        'bar' => t('Bar with progress meter'),
      ),
      '#default_value' => $component['extra']['progress_indicator'],
      '#description' => t('The throbber display does not show the status of uploads but takes up less space. The progress bar is helpful for monitoring progress on large uploads.'),
      '#weight' => 16,
      '#access' => file_progress_implementation(),
      '#parents' => array('extra', 'progress_indicator'),
    );
    $form['display']['width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#default_value' => $component['extra']['width'],
      '#description' => t('Width of the file field.') . ' ' . t('Leaving blank will use the default size.'),
      '#size' => 5,
      '#maxlength' => 10,
      '#weight' => 4,
      '#parents' => array('extra', 'width')
    );
    return $form;
}

/**
 * A Form API element validate function to check filesize is numeric.
 */
function _webform_edit_multifile_size_validate($element) {
  if (!empty($element['#value'])) {
    if (!is_numeric($element['#value']) || intval($element['#value']) != $element['#value']) {
      form_error($element, t('Max upload size must be a number in KB.'));
    }
  }
}

/**
 * A Form API after build function.
 *
 * Ensure that the destination directory exists and is writable.
 */
function _webform_edit_multifile_check_directory($element) {
  $scheme = $element['extra']['scheme']['#value'];
  $directory = $element['extra']['directory']['#value'];

  $destination_dir = file_stream_wrapper_uri_normalize($scheme . '://' . $directory . '/webform');

  // Sanity check input to prevent use parent (../) directories.
  if (preg_match('/\.\.[\/\\\]/', $destination_dir . '/')) {
    form_error($element['extra']['directory'], t('The save directory %directory is not valid.', array('%directory' => $directory)));
  }
  else {
    $destination_success = file_prepare_directory($destination_dir, FILE_CREATE_DIRECTORY);
    if (!$destination_success) {
      form_error($element['extra']['directory'], t('The save directory %directory could not be created. Check that the webform files directory is writable.', array('%directory' => $directory)));
    }
  }

  return $element;
}

/**
 * A Form API element validate function.
 *
 * Change the submitted values of the component so that all filtering extensions
 * are saved as a single array.
 */
function _webform_edit_multifile_filtering_validate($element, &$form_state) {
  // Predefined types.
  $extensions = array();
  foreach (element_children($element['types']) as $category) {
    foreach (array_keys($element['types'][$category]['#value']) as $extension) {
      if ($element['types'][$category][$extension]['#value']) {
        $extensions[] = $extension;
      }
    }
  }

  // Additional types.
  $additional_extensions = explode(',', $element['addextensions']['#value']);
  foreach ($additional_extensions as $extension) {
    $clean_extension = drupal_strtolower(trim($extension));
    if (!empty($clean_extension) && !in_array($clean_extension, $extensions)) {
      $extensions[] = $clean_extension;
    }
  }

  form_set_value($element['types'], $extensions, $form_state);
}

function theme_webform_edit_multifile($variables) {
  $form = $variables['form'];
  // Add a little JavaScript to check all the items in one type.
  $javascript = '
    function check_category_boxes() {
      var checkValue = !document.getElementById("edit-extra-filtering-types-"+arguments[0]+"-"+arguments[1]).checked;
      for(var i=1; i < arguments.length; i++) {
        document.getElementById("edit-extra-filtering-types-"+arguments[0]+"-"+arguments[i]).checked = checkValue;
      }
    }
 ';
  drupal_add_js($javascript, 'inline');

  // Format the components into a table.
  $per_row = 6;
  $rows = array();
  foreach (element_children($form['validation']['filtering']['types']) as $key => $filtergroup) {
    $row = array();
    $first_row = count($rows);
    if ($form['validation']['filtering']['types'][$filtergroup]['#type'] == 'checkboxes') {
      // Add the title.
      $row[] = $form['validation']['filtering']['types'][$filtergroup]['#title'];
      $row[] = '&nbsp;';
      // Convert the checkboxes into individual form-items.
      $checkboxes = form_process_checkboxes($form['validation']['filtering']['types'][$filtergroup]);
      // Render the checkboxes in two rows.
      $checkcount = 0;
      $jsboxes = '';
      foreach (element_children($checkboxes) as $key) {
        $checkbox = $checkboxes[$key];
        if ($checkbox['#type'] == 'checkbox') {
          $checkcount++;
          $jsboxes .= "'" . $checkbox['#return_value'] . "',";
          if ($checkcount <= $per_row) {
            $row[] = array('data' => drupal_render($checkbox));
          }
          elseif ($checkcount == $per_row + 1) {
            $rows[] = array('data' => $row, 'style' => 'border-bottom: none;');
            $row = array(array('data' => '&nbsp;'), array('data' => '&nbsp;'));
            $row[] = array('data' => drupal_render($checkbox));
          }
          else {
            $row[] = array('data' => drupal_render($checkbox));
          }
        }
      }
      // Pretty up the table a little bit.
      $current_cell = $checkcount % $per_row;
      if ($current_cell > 0) {
        $colspan = $per_row - $current_cell   + 1;
        $row[$current_cell + 1]['colspan'] = $colspan;
      }
      // Add the javascript links.
      $jsboxes = drupal_substr($jsboxes, 0, drupal_strlen($jsboxes) - 1);
      $rows[] = array('data' => $row);
      $select_link = ' <a href="javascript:check_category_boxes(\'' . $filtergroup . '\',' . $jsboxes . ')">(select)</a>';
      $rows[$first_row]['data'][1] = array('data' => $select_link, 'width' => 40);
      unset($form['validation']['filtering']['types'][$filtergroup]);
    }
    elseif ($filtergroup != 'size') {
      // Add other fields to the table (ie. additional extensions).
      $row[] = $form['validation']['filtering']['types'][$filtergroup]['#title'];
      unset($form['validation']['filtering']['types'][$filtergroup]['#title']);
      $row[] = array(
        'data' => drupal_render($form['validation']['filtering']['types'][$filtergroup]),
        'colspan' => $per_row + 1,
      );
      unset($form['validation']['filtering']['types'][$filtergroup]);
      $rows[] = array('data' => $row);
    }
  }
  $header = array(array('data' => t('Category'), 'colspan' => '2'), array('data' => t('Types'), 'colspan' => $per_row));

  // Create the table inside the form.
  $form['validation']['filtering']['types']['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  $output = drupal_render_children($form);

  return $output;
}

/**
 * Implementation of _webform_render_component().
 */
function _webform_render_multifile($component, $value = NULL, $filter = TRUE) {
  $id = 'MultiFile-identifier-' . str_replace('_', '-', $component['form_key']);
  $component['extra']['attributes']['class'][] = 'form-item multi ' . $id;

  $node = node_load($component['nid']);
  $form_key = implode('_', webform_component_parent_keys($node, $component));
  $current_types = isset($component['extra']['filtering']['types']) ? $component['extra']['filtering']['types'] : array();
  $element[$form_key] = array(
    '#type' => 'file',
    '#translatable' => array('title', 'description'),
    '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    //'#required' => $component['mandatory'], // Drupal core bug with required file uploads.
    '#description' => $filter ? _webform_filter_descriptions($component['extra']['description']) : $component['extra']['description'],
    '#attributes' => $component['extra']['attributes'],
    '#tree' => FALSE, // file_check_upload assumes a flat $_FILES structure.
    '#suffix' => "<div class='outer'><div class='inner'></div></div>",
    "#attached" => array(
      'js' => array(
        drupal_get_path('module', 'webform_multifile') . '/multifile/jquery.MultiFile.js' => array('type' => 'file'),
        drupal_get_path('module', 'webform_multifile') . '/webform_multifile.js' => array('type' => 'file', 'scope' => 'footer'),
        "(function ($) { \n" .
        // Stores all the settings for each field in an array so the javascript files can load later
        "  if (typeof MultiFile_fields == 'undefined') {MultiFile_fields = []} \n" .
        "  MultiFile_fields.push(\n  {\n    id :'$id',\n    properties : {\n" .
          "        max:" . $component['extra']['max_amount'] . ",\n" .
          "        accept:'" . join('|', $current_types) . "',\n        STRING: {\n" .
          "          remove:'" . t('Remove') . "',\n" .
          "          denied:'" . t('You are kindly asked not to submit !ext files in this form.', array('!ext' => '$ext')) . "',\n" .
          "          duplicate:'" . t('The file !file has already been selected and will be uploaded once you submit this form.', array('!file' => '$file')) . "'\n" .
          "        }\n" .
        "      }\n    }\n  );\n}) (jQuery);" => array('type' => 'inline'),
      )
    ),
    '#element_validate' => array(
      '_webform_validate_multifile',
      '_webform_required_multifile',  // Custom required routine.
    ),
    '#pre_render' => array('webform_element_title_display'),
    '#webform_component' => $component,
    '#upload_location' => $component['extra']['scheme'] . '://webform/' . $component['extra']['directory'],
  );
  $element['#webform_required'] = $component['mandatory'];
  $element['#webform_form_key'] = $form_key;
  $element['#weight'] = $component['weight'];
  $element['#theme'] = 'webform_render_multifile';
  $element['#theme_wrappers'] = array('webform_element');
  $element['#webform_component'] = $component;

  // Change the 'width' option to the correct 'size' option.
  if ($component['extra']['width'] > 0) {
    $element[$form_key]['#size'] = $component['extra']['width'];
  }

  // Add a hidden element to store the FID for new files.
  $element['_fids'] = array(
    '#type' => 'hidden',
    '#default_value' => '',
  );

  // Add a hidden element to store the FID for existing files.
  $element['_old_fids'] = array(
    '#type' => 'hidden',
    '#value' => isset($value[0]) ? $value[0] : NULL,
  );
  return $element;
}

/**
 * Render a File component.
 */
function theme_webform_render_multifile($variables) {
  $element = $variables['element'];
  // Add information about the existing file, if any.
  if (isset($element['#default_value'])) {
    $element['_fids']['#value'] = $element['#default_value'];
  }
  $value = $element['_fids']['#value'] ? $element['_fids']['#value'] : $element['_old_fids']['#value'];

  $webform_nid = $element['#webform_component']['nid'];
  $component_id = $element['#webform_component']['cid'];
  $submission_id = arg(3);
  if ($fids = unserialize($value)) {
    $suffix = '';
    foreach (webform_get_multifile($fids) as $file) {
      $suffix .= '<div class="multifile-file"> ';
      $suffix .= l(t('Download !filename', array('!filename' => webform_multifile_name($file->uri))), webform_multifile_url($file->uri));
      $suffix .= ' ( ' . l(t('Delete'), "node/$webform_nid/submission/$submission_id/multifile_delete/$component_id/$file->fid") . ' )';
      $suffix .= ' </div>';
    }
    $firstchild = array_shift(array_keys($element));
    $element[$firstchild]['#suffix'] = $suffix;
    $element[$firstchild]['#suffix'] .= isset($element['#suffix']) ? $element['#suffix'] : '';
  }

  // Add the required asterisk.
  if ($element['#webform_required']) {
    $element[$element['#webform_form_key']]['#required'] = TRUE;
  }

  $output = '';
  $output = drupal_render_children($element);
  return $output;
}

/**
 * A Form API element validate function.
 *
 * Fix Drupal core's handling of required file fields.
 */
function _webform_required_multifile($element, $form_state) {
  $component = $element['#webform_component'];
  $parents = $element['#array_parents'];
  array_pop($parents);
  $form_key = implode('_', $parents);

  // Do not validate requiredness on back or draft button.
  if (isset($form_state['clicked_button']['#validate']) && empty($form_state['clicked_button']['#validate'])) {
    return;
  }

  // Check if a value is already set in the hidden field.
  $values = $form_state['values'];
  $key = array_shift($parents);
  $found = FALSE;
  while (isset($values[$key])) {
    if (isset($values[$key])) {
      $values = $values[$key];
      $found = TRUE;
    }
    else {
      $found = FALSE;
    }
    $key = array_shift($parents);
  }
  if (!$found || (empty($values['_fids']) && empty($values['_old_fids']))) {
    if (empty($_FILES['files']['name'][$form_key]) && $component['mandatory']) {
      form_error($element, t('%field field is required.', array('%field' => $component['name'])));
    }
  }
}

/**
 * A Form API element validate function.
 *
 * Ensure that the uploaded file matches the specified file types.
 */
function _webform_validate_multifile(&$element, &$form_state) {
  $component = $element['#webform_component'];
  $form_key = implode('_', $element['#parents']);
  if (empty($_FILES['files']['size'][$form_key]) || !isset($_FILES['files']['size'][$form_key][0]) || empty($_FILES['files']['size'][$form_key][0])) {
    return;
  }

  // Build a human readable list of extensions:
  $extensions = $component['extra']['filtering']['types'];
  $extension_list = '';
  if (count($extensions) > 1) {
    for ($n = 0; $n < count($extensions) - 1; $n++) {
      $extension_list .= $extensions[$n] . ', ';
    }
    $extension_list .= 'or ' . $extensions[count($extensions) - 1];
  }
  elseif (!empty($extensions)) {
    $extension_list = $extensions[0];
  }

  if (in_array('jpg', $extensions)) {
    $extensions[] = 'jpeg';
  }

  foreach ($_FILES['files']['name'][$form_key] as $key => $filename) {
    $filename = _webform_multifile_get_files_array_value($filename, $form_key);
    $dot = strrpos($filename, '.');
    $extension = drupal_strtolower(drupal_substr($filename, $dot + 1));
    $file_error = FALSE;
    if (!empty($extensions) && !in_array($extension, $extensions)) {
      form_error($element, t("Files with the '%ext' extension are not allowed, please upload a file with a %exts extension.", array('%ext' => $extension, '%exts' => $extension_list)));
      $file_error = TRUE;
    }
  }
  foreach ($_FILES['files']['size'][$form_key] as $key => $size) {
    $size = (int)_webform_multifile_get_files_array_value($size, $form_key);
    // Now let's check the file size (limit is set in KB).
    if ($size > $component['extra']['filtering']['size'] * 1024) {
      form_error($element, t("The file '%filename' is too large (%filesize KB). Please upload a file %maxsize KB or smaller.", array('%filename' => $_FILES['files']['name'][$form_key][$key], '%filesize' => (int) ($size/1024), '%maxsize' => $component['extra']['filtering']['size'])));
      $file_error = TRUE;
    }
  }

  // Save the file to a temporary location.
  if (!$file_error) {
    $upload_dir = $component['extra']['scheme'] . '://webform/' . $component['extra']['directory'];
    if (file_prepare_directory($upload_dir, FILE_CREATE_DIRECTORY)) {
      $keys = _webform_multifile_convert_files_array($form_key);
      $fids = array();
      foreach ($keys as $key) {
        $file = file_save_upload($key, array('file_validate_extensions' => array(implode(' ', $extensions))), $upload_dir);
        if ($file) {
          @chmod(drupal_realpath($file->uri), 0664);
          $fids[] = $file->fid;
        }
        else {
          drupal_set_message(t('The uploaded file was unable to be saved. The destination directory may not be writable.'), 'error');
        }
      }
      // Set the hidden field value.
      $parents = $element['#array_parents'];
      array_pop($parents);
      $parents[] = '_fids';
      form_set_value(array('#parents' => $parents), $fids, $form_state);
    }
    else {
      drupal_set_message(t('The uploaded file was unable to be saved. The destination directory does not exist.'), 'error');
    }
  }
}

/**
 * Implementation of _webform_submit_component().
 */
function _webform_submit_multifile($component, $value) {
  $old_fids = isset($value['_old_fids']) ? unserialize($value['_old_fids']) : NULL;

  if ($fids = $value['_fids']) {
    $files = webform_get_multifile($fids);
    foreach ($files as $file) {
      // Save any new files permanently.
      $file = (object)$file;
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
    }

    if ($old_fids) {
      $fids = array_merge($old_fids, $fids);
    }
  }
  else {
    $fids = $old_fids;
  }

  if ($fids) {
    return serialize($fids);
  }
}

/**
 * Implementation of _webform_display_component().
 */
function _webform_display_multifile($component, $value, $format = 'html') {
  $fids = isset($value[0]) ? unserialize($value[0]) : NULL;
  return array(
     '#title' => $component['name'],
     '#value' => $fids ? webform_get_multifile($fids) : NULL,
     '#weight' => $component['weight'],
     '#theme' => 'webform_display_multifile',
     '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
     '#webform_component' => $component,
     '#format' => $format,
  );
}

/**
 * Format the output of text data for this component
 */
function theme_webform_display_multifile($variables) {
  $element = $variables['element'];
  $output = '';
  if (isset($element['#value']) && !empty($element['#value'])) {
    foreach ($element['#value'] as $file) {
      if (!empty($file)) {
        $url = webform_multifile_url($file->uri);
        if ($element['#format'] == 'html') {
          $output .= '<div class="multifile-file"> ';
          $output .= l($file->filename, $url);
          $output .= ' </div>';
        }
        else {
          $output .= $file->filename . ': ' . $url . "\n";
        }
      }
    }
  }
  return $output;
}

/**
 * Implementation of _webform_delete_component().
 */
function _webform_delete_multifile($component, $value) {
  // Delete a set of files on an individual submission.
  $fids = isset($value[0]) ? unserialize($value[0]) : NULL;
  foreach (webform_get_multifile($fids) as $file) {
    file_delete($file);
  }
}

/**
 * Implementation of _webform_analysis_component().
 */
function _webform_analysis_multifile($component, $sids = array()) {

  $q = db_select('webform_submitted_data', 'wsd')
    ->fields('wsd', array('data'))
    ->condition('nid', $component['nid'])
    ->condition('cid', $component['cid']);

  if (!empty($sids)) {
    $q->condition('sid', $sids);
  }

  $result = $q->execute();

  $nonblanks = 0;
  $sizetotal = 0;
  $submissions = 0;
  $numfiles = 0;

  while ($data = $result->fetchAssoc()) {
    if ($fids = unserialize($data['data'])) {
      $counter = 0;
      foreach (webform_get_multifile($fids) as $file) {
        if (isset($file->filesize)) {
          $counter++;
          $sizetotal += $file->filesize;
        }
      }
      if ($counter) {
        $numfiles += $counter;
        $nonblanks++;
      }
      $submissions++;
    }
  }

  $rows[0] = array(t('Left Blank'), ($submissions - $nonblanks));
  $rows[1] = array(t('User uploaded file'), $nonblanks);
  $rows[2] = array(t('Average uploaded files'), ($nonblanks == 0) ? 0 : $numfiles / $nonblanks);
  $rows[3] = array(t('Average uploaded file size'), (($sizetotal != 0 && $numfiles != 0) ? (int) (($sizetotal/$numfiles)/1024) . ' KB' : '0'));
  return $rows;
}

/**
 * Implementation of _webform_table_component().
 */
function _webform_table_multifile($component, $value) {
  $links = array();
  if ($fids = (isset($value[0]) ? unserialize($value[0]) : FALSE)) {
    foreach (webform_get_multifile($fids) as $file) {
      if (!empty($file->fid)) {
        $link = l(webform_multifile_name($file->uri), webform_multifile_url($file->uri));
        $link .= ' (' . (int) ($file->filesize/1024) . ' KB)';
        $links[] = $link;
      }
    }
  }
  return implode("<br />\n", $links);
}

/**
 * Implementation of _webform_csv_headers_component().
 *
 * Note: This function is identical to _webform_csv_headers_file().
 */
function _webform_csv_headers_multifile($component, $export_options) {
  $header = array();
  // Two columns in header.
  $header[0] = array('', '');
  $header[1] = array($component['name'], '');
  $header[2] = array(t('Name'), t('Filesize (KB)'));
  return $header;
}

/**
 * Implementation of _webform_csv_data_component().
 */
function _webform_csv_data_multifile($component, $export_options, $value) {
  $filenames = array();
  $sizes = array();
  if ($fids = (isset($value[0]) ? unserialize($value[0]) : FALSE)) {
    foreach (webform_get_multifile($fids) as $file) {
      $filenames[] = webform_multifile_url($file->uri);
      $sizes[] = (int) ($file->filesize/1024);
    }
  }

  if (empty($filenames)) {
    return array('', '');
  }
  else {
    return array(implode("\n", $filenames), implode("\n", $sizes));
  }
}

/**
 * Implementation of _webform_get_files_component().
 */
function _webform_get_files_multifile($value) {
  $files = array();
  if ($fids = (isset($value[0]) ? unserialize($value[0]) : FALSE)) {
    foreach (webform_get_multifile($fids) as $file) {
      $files[] = $file;
    }
  }

  return $files;
}

/**
 * Helper function to create proper file names for uploaded file.
 *
 * Note: This is identical to webform_file_name().
 */
function webform_multifile_name($filepath) {
  if (!empty($filepath)) {
    $info = pathinfo($filepath);
    $file_name = $info['basename'];
  }
  return isset($file_name) ? $file_name : '';
}

/**
 * Helper function to create proper URLs for uploaded file.
 *
 * Note: This function is identical to webform_file_url
 */
function webform_multifile_url($uri) {
  if (!empty($uri)) {
    $file_url = file_create_url($uri);
  }
  return isset($file_url) ? $file_url : '';
}

/**
 * Helper function to load a file from the database.
 */
function webform_get_multifile($fids) {
  static $files;
  $allfiles = array();
  if (empty($fids)) return array();
  foreach ($fids as $fid) {
    if (!isset($files[$fid])) {
      if (empty($fid)) {
        $files[$fid] = FALSE;
      }
      else {
        $files[$fid] = file_load($fid);
      }
    }
    $allfiles[$fid] = $files[$fid];
  }
  return $allfiles;
}

function _webform_multifile_convert_files_array($form_key) {
  // file_save_upload expects the usual Forms API _FILES structure, which is
  // incompatible to jquery.MultiFile.js
  $keys = array();
  $file_properties = array('type', 'tmp_name', 'error', 'size', 'orig_name');
  foreach ($_FILES['files']['type'][$form_key] as $key => $empty) {
    $newkey = $form_key . $key;
    if (!empty($_FILES['files']['orig_name'][$form_key][$key])) {
      $_FILES['files']['name'][$newkey] = transliteration_clean_filename(_webform_multifile_get_files_array_value($_FILES['files']['orig_name'][$form_key][$key], $form_key));
    }
    else {
      $_FILES['files']['name'][$newkey] = transliteration_clean_filename(_webform_multifile_get_files_array_value($_FILES['files']['name'][$form_key][$key], $form_key));
    }
    foreach ($file_properties as $file_property) {
      $_FILES['files'][$file_property][$newkey] = _webform_multifile_get_files_array_value($_FILES['files'][$file_property][$form_key][$key], $form_key);
    }
    $keys[] = $newkey;
  }
  unset($_FILES['files']['name'][$form_key]);
  foreach ($file_properties as $file_property) {
    unset($_FILES['files'][$file_property][$form_key]);
  }
  return $keys;
}

/**
 * Helper function.
 */
function _webform_multifile_get_files_array_value($value, $form_key) {
  return (is_array($value) && isset($value[$form_key])) ? $value[$form_key] : $value;
}

/**
 * Implements _webform_attachments_component().
 */
function _webform_attachments_multifile($component, $value) {
  static $files = array();
  $fids = unserialize($value[0]);
  $return_files = array();
  foreach ($fids as $fid) {
    if (!isset($files[$fid])) {
      $file = file_load($fid);
      $file->filepath = $file->uri;
      $files[$fid] = $file;
    }
    $return_files[] = $files[$fid];
  }
  return $return_files;
}
