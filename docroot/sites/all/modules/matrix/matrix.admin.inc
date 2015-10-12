<?php
// $Id

/**
 * @file
 * Configuration pages for matrix field
 */

/**
 *  Settings for for the text matrix
 */
function matrix_field_settings_form_text($field, $instance, $has_data) {
  $settings = (array)$field['settings'];
  
  $form['rows_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of rows'),
    '#description' => t('The number of rows to display in the grid.'),
    '#required' => TRUE,
    '#size' => 3,
    '#default_value' => $settings['rows_count'],
  );
  $form['cols_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of columns'),
    '#default_value' => $settings['cols_count'],
    '#description' => t('The number of columns to display in the grid.'),
    '#required' => TRUE,
    '#size' => 3,
  );
  $form['size'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#title' => t('Size of textfields'),
    '#default_value' => $settings['size'],
  );
  $form['spreadsheet_style'] = array(
    '#type' => 'checkbox',
    '#title' => t('Spreadsheet style'),
    '#description' => t('If selected columns will be labeld with A, B, C and rows with 1, 2, 3'),
    '#default_value' => $settings['spreadsheet_style'],
  );

  return $form;
}

/**
 *  Settings for for the custom matrix
 */
function matrix_field_settings_form_custom($field, $instance, $has_data) {
  global $user;
    
  //List of available matrix field types
  $matrix_field_types = matrix_field_types();
  
  //merge together the saved setting with those that have have come in via the ajax request
  //ideally thse should come from$form_state['values'] rather than $_POST, however $form_state is not available in this context.
  $saved_settings = (array)$field['settings'];
  $posted_settings = isset($_POST['field']['settings']) ? $_POST['field']['settings'] : array();
  $settings = array_merge($saved_settings, $posted_settings);
  
  $field_name = $field['field_name'];
  $cache_id = sprintf('matrix-cache-%s-%s', $field_name, $user->uid);
  
  $cached_settings = cache_get($cache_id, 'cache');
  
  if (isset($cached_settings->data)) {
    $settings['storage'] = $cached_settings->data;
  }
  else {
    $settings['storage'] = (array)unserialize($settings['settings']);
  }
  
  //$setings['preview'] is only a container for display purposes and can be merged back to the parent for simplicity sake
  if (isset($settings['preview'])) {
    $settings = array_merge($settings, $settings['preview']);
    unset($settings['preview']);
  }
  
  //set default value for $settings['define'] (to use rows or cols)
  if ($settings['cols_count'] == -1) {
    $settings['define'] = 'rows';
  }
  elseif ($settings['rows_count'] == -1) {
    $settings['define'] = 'cols';
  }
  elseif (empty($settings['define'])) {
    $settings['define'] = 'cols'; //default
  }
  
  //when using the settings form, work out if a row or column button was pressed and then which row/column
  if (isset($_POST['_triggering_element_name']) && strpos($_POST['_triggering_element_name'], 'matrix-settings') !== FALSE) {
    $parts = explode('-', $_POST['_triggering_element_name']);
    $op = $parts[2];
    $axis = $parts[3];
    $cell = $parts[4];
    
    if ($axis == 'rows') {
      $axis_name = t('Row');
      $other_axis_name = t('Column');
    }
    elseif ($axis == 'cols') {
      $axis_name = t('Column');
      $other_axis_name = t('Row');
    }
    
    if ($op == 'form') { //load out settings
      $field_settings = $settings['storage'][$axis][$cell];
      $show_settings_form = TRUE;
      if (empty($field_settings['field_type'])) {
        $field_settings['field_type'] = 'none';
      }
    }
    elseif ($op == 'save') { //put away settings
      $settings['storage'][$axis][$cell] = $settings['form'][$axis][$cell];
    }
  }
  
  //remove the col/row settings form from the settings as it's value are stored in $settings['storage'][$axis][$cell] 
  if (isset($settings['form'])) {
    unset($settings['form']);
  }
  if (isset($settings['preview'])) {
    unset($settings['preview']);
  }
  
  //reinitialize rows or columns if $define changes or $rows_count changes to -1 or $cols_count changes to -1
  if (($settings['cols_count'] == -1 && $settings['storage']['old_cols_count'] > 0) || ($settings['define'] == 'cols' && isset($settings['storage']['old_define']) && $settings['storage']['old_define'] == 'rows')) {
    $settings['storage']['cols'] = array();
  }
  if (($settings['rows_count'] == -1 && $settings['storage']['old_rows_count'] > 0) || ($settings['define'] == 'rows' && isset($settings['storage']['old_define']) && $settings['storage']['old_define'] == 'cols')) {
    $settings['storage']['rows'] = array();
  }
  
  //field settings to need to be as a form element so that the Drupal feilds api can save the data
  $form['define'] = array(
    '#type' => 'value',
    '#value' => $settings['define'],
  );
  $form['settings'] = array(
    '#type' => 'value',
    '#value' => serialize($settings['storage']),
  );
  
  //now for the form definition....
  $form['cols_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of columns'),
    '#default_value' => $settings['cols_count'],
    '#description' => t('The number of columns to display in the grid.'),
    '#required' => TRUE,
    '#size' => 3,
    '#ajax' => array(
      'callback' => 'matrix_settings_ajax_full_callback',
      'wrapper' => 'matrix-settings-ajax',
    ),
  );
  $form['rows_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of rows'),
    '#description' => t('The number of rows to display in the grid.'),
    '#required' => TRUE,
    '#size' => 3,
    '#default_value' => $settings['rows_count'],
    '#ajax' => array(
      'callback' => 'matrix_settings_ajax_full_callback',
      'wrapper' => 'matrix-settings-ajax',
    ),
  );
  $form['preview'] = array(
    '#type' => 'item',
    '#title' => t('Configuration'),
    '#theme' => 'matrix_preview',
    '#prefix' => '<div id="matrix-settings-ajax">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );

  //both cols and rows cannot be set to unlimited
  if ($settings['cols_count'] == -1 && $settings['rows_count'] == -1) {
    $form['preview']['message'] = array(
      '#type' => 'item',
      '#markup' => '<p>'. t('Both rows and columns cannot be set to unlimited') .'</p>',
    );
  }
  else {
    //if both rows and columns are of a defined number, need to prompt to find out which should define the types
    if ($settings['cols_count'] > 0 && $settings['rows_count'] > 0) {
      $form['preview']['define'] = array(
        '#type' => 'select',
        '#title' => t('Field type are defined by'),
        '#description' => t('Choose if columns or rows should choose the type of field (checkbox, textfield, select, etc)'),
        '#options' => array(
          'rows' => t('Rows'),
          'cols' => t('Columns'),
        ),
        '#default_value' => $settings['define'],
        '#ajax' => array(
          'callback' => 'matrix_settings_ajax_full_callback',
          'wrapper' => 'matrix-settings-ajax',
        ),
      );
    }
    //set up the preview
    if ($settings['cols_count'] > 0 && $settings['rows_count'] == -1) { //set by virtue that cols is set and rows is unlimited
      $setup = array(
        'cols' => array(
          'count' => $settings['cols_count'],
          'config' => 'full',
        ),
        'rows' => array(
          'count' => 3,
          'config' => 'none',
        ),
      );
    }
    elseif ($settings['cols_count'] == -1 && $settings['rows_count'] > 0) { //set by virtue that rows is set and cols is unlimited
      $setup = array(
        'cols' => array(
          'count' => 3,
          'config' => 'none',
        ),
        'rows' => array(
          'count' => $settings['rows_count'],
          'config' => 'full',
        ),
      );
    }
    elseif ($settings['cols_count'] > 0 && $settings['rows_count'] > 0) { //manually set rows_count and cols_count
      $setup = array(
        'cols' => array(
          'count' => $settings['cols_count'],
          'config' => $settings['define'] == 'cols' ? 'full' : 'limited',
        ),
        'rows' => array(
          'count' => $settings['rows_count'],
          'config' => $settings['define'] == 'rows' ? 'full' : 'limited',
        ),
      );
    }
    else { //fallback case (probably an error)
      $form['preview']['message'] = array(
        '#type' => 'item',
        '#markup' => '<p>'. t('Invalid number of rows and columns.') .'</p>',
      );
      $setup = array(
        'cols' => array(
          'count' => 0,
          'config' => 'none',
        ),
        'rows' => array(
          'count' => 0,
          'config' => 'none',
        ),
      );
    }
    
    $functions = module_invoke_all('matrix_functions');
    array_unshift($functions['allowed_values'], t('-none-'));
    $form['preview']['#setup'] = $setup; //will need this in the theme function

    //generate form items for managing the configuration
    for ($i = 1; $i <= $setup['cols']['count']; $i++) {
      if ($setup['cols']['config'] != 'none') {
        $wrapper_id = sprintf('matrix-settings-edit-col-%s', $i);
        
        $form['preview']['cols'][$i]['button'] = array(
          '#type' => 'button',
          '#name' => 'matrix-settings-form-cols-'. $i,
          '#value' => t('Change settings'),
          '#config_type' => $setup['cols']['config'],
          '#ajax' => array(
            'callback' => 'matrix_settings_ajax_edit_callback',
            'wrapper' => $wrapper_id,
          ),
        );
        $form['preview']['cols'][$i]['summary'] = array(
          '#type' => 'item',
          '#title' => isset($settings['storage']['cols'][$i]['title']) ? $settings['storage']['cols'][$i]['title'] : t('&lt;Title not set&gt;'),
          '#markup' => isset($settings['storage']['cols'][$i]['field_type']) ? $matrix_field_types['full'][$settings['storage']['cols'][$i]['field_type']] : t('&lt;Field type not set&gt;'),
        );
        $form['preview']['cols'][$i]['edit-wrapper'] = array(
          '#type'   => 'markup',
          '#prefix' => sprintf('<div id="%s">', $wrapper_id),
          '#suffix' => '</div>',
        );
      }
      else {
        $form['preview']['cols'][$i]['button'] = array(
          '#type' => 'item',
          '#markup' => '',
        );        
        $form['preview']['cols'][$i]['summary'] = array(
          '#type' => 'item',
          '#title' => t('Column %i', array('%i' => $i)),
        );
      }
    }
    for ($i = 1; $i <= $setup['rows']['count']; $i++) {
      if ($setup['rows']['config'] != 'none') {
        $wrapper_id = sprintf('matrix-settings-edit-row-%s', $i);
        
        $form['preview']['rows'][$i]['button'] = array(
          '#type' => 'button',
          '#name' => 'matrix-settings-form-rows-'. $i,
          '#value' => t('Change settings'),
          '#config_type' => $setup['rows']['config'],
          '#ajax' => array(
            'callback' => 'matrix_settings_ajax_edit_callback',
            'wrapper' => $wrapper_id,
          ),
        );
        $form['preview']['rows'][$i]['summary'] = array(
          '#type' => 'item',
          '#title' => isset($settings['storage']['rows'][$i]['title']) ? $settings['storage']['rows'][$i]['title'] : t('&lt;Title not set&gt;'),
          '#markup' => isset($settings['storage']['rows'][$i]['field_type']) ? $matrix_field_types['full'][$settings['storage']['rows'][$i]['field_type']] : t('&lt;Field type not set&gt;'),
        );
        $form['preview']['rows'][$i]['edit-wrapper'] = array(
          '#type'   => 'markup',
          '#prefix' => sprintf('<div id="%s">', $wrapper_id),
          '#suffix' => '</div>',
        );
      }
      else {
        $form['preview']['rows'][$i]['button'] = array(
          '#type' => 'item',
          '#markup' => '',
        );
        $form['preview']['rows'][$i]['summary'] = array(
          '#type' => 'item',
          '#title' => t('Row %i', array('%i' => $i)),
        );          
      }
    }

    //Generate the form for altering a row or a column     
    if (isset($show_settings_form) && $show_settings_form == TRUE) {
      $form['preview']['form'] = array(
        '#type' => 'fieldset',
        '#title' => t('Configuration for %axis %cell', array('%axis' => $axis_name, '%cell' => $cell)),
      );
      $form['preview']['form'][$axis][$cell]['field_type'] = array(
        '#type' => 'radios',
        '#title' => t('Field type'),
        '#default_value' => $field_settings['field_type'],
        '#required' => TRUE,
        '#options' => $matrix_field_types[$setup[$axis]['config']],
      );
      $form['preview']['form'][$axis][$cell]['title'] = array(
        '#type' => 'textfield',
        '#title' => t('Title'),
        '#description' => t('Title for %axis %cell', array('%axis' => $axis_name, '%cell' => $cell)),
        '#default_value' => $field_settings['title'],
      );
      
      //textfield
      $form['preview']['form'][$axis][$cell]['textfield']['required'] = array(
        '#type' => 'checkbox',
        '#title' => t('Required'),
        '#default_value' => $field_settings['required'],
        '#states' => array(
          'visible' => array(
            ':input[name="field[settings][preview][form]['. $axis .']['. $cell .'][field_type]"]' => array(
              'value' => 'textfield',
            ),
          ),
        ),
      );
      $form['preview']['form'][$axis][$cell]['textfield']['size'] = array(
        '#type' => 'textfield',
        '#size' => 5,
        '#title' => t('Size of textfields'),
        '#default_value' => $field_settings['textfield']['size'],
        '#states' => array(
          'visible' => array(
            ':input[name="field[settings][preview][form]['. $axis .']['. $cell .'][field_type]"]' => array(
              'value' => 'textfield',
            ),
          ),
        ),
      );
      $form['preview']['form'][$axis][$cell]['textfield']['prefix'] = array(
        '#type' => 'textfield',
        '#title' => t('Prefix'),
        '#size' => 5,
        '#description' => t('Prefix for textfield'),
        '#default_value' => $field_settings['textfield']['prefix'],
        '#states' => array(
          'visible' => array(
            ':input[name="field[settings][preview][form]['. $axis .']['. $cell .'][field_type]"]' => array(
              'value' => 'textfield',
            ),
          ),
        ),
      );
      $form['preview']['form'][$axis][$cell]['textfield']['suffix'] = array(
        '#type' => 'textfield',
        '#title' => t('Suffix'),
        '#size' => 5,
        '#description' => t('Suffix for textfield'),
        '#default_value' => $field_settings['textfield']['suffix'],
        '#states' => array(
          'visible' => array(
            ':input[name="field[settings][preview][form]['. $axis .']['. $cell .'][field_type]"]' => array(
              'value' => 'textfield',
            ),
          ),
        ),
      );
      
      //select
      $form['preview']['form'][$axis][$cell]['select']['required'] = array(
        '#type' => 'checkbox',
        '#title' => t('Required'),
        '#default_value' => $field_settings['select']['required'],
        '#states' => array(
          'visible' => array(
            ':input[name="field[settings][preview][form]['. $axis .']['. $cell .'][field_type]"]' => array(
              'value' => 'select',
            ),
          ),
        ),
      );
      $form['preview']['form'][$axis][$cell]['select']['allowed_values'] = array(
        '#type' => 'textarea',
        '#title' => t('Allowed values list'),
        '#default_value' => $field_settings['select']['allowed_values'],
        '#rows' => 5,
        '#description' => '<p>' . t('The possible values this field can contain. Enter one value per line, in the format "label" or "key|label". The key is the value that will be stored in the database') . '</p>',
        '#element_validate' => array('matrix_allowed_values_setting_validate'),
        '#states' => array(
          'visible' => array(
            ':input[name="field[settings][preview][form]['.$axis .']['. $cell .'][field_type]"]' => array(
              'value' => 'select',
            ),
          ),
        ),
      );
      $form['preview']['form'][$axis][$cell]['select']['allowed_values_function'] = array(
        '#type' => 'select',
        '#title' => t('Allowed values function'),
        '#description' => t('Instead of using a list of allowed values, you may use values specified by a php function. See the documentation on how to use this feature.'),
        '#options' => $functions['allowed_values'],
        '#default_value' => $field_settings['select']['allowed_values_function'],
        '#states' => array(
          'visible' => array(
            ':input[name="field[settings][preview][form]['.$axis .']['. $cell .'][field_type]"]' => array(
              'value' => 'select',
            ),
          ),
        ),
      );
      
      //radios
      $form['preview']['form'][$axis][$cell]['radios']['required'] = array(
        '#type' => 'checkbox',
        '#title' => t('Required'),
        '#default_value' => $field_settings['radios']['required'],
        '#states' => array(
          'visible' => array(
            ':input[name="field[settings][preview][form]['.$axis .']['. $cell .'][field_type]"]' => array(
              'value' => 'radios',
            ),
          ),
        ),
      );
      $form['preview']['form'][$axis][$cell]['radios']['allowed_values'] = array(
        '#type' => 'textarea',
        '#title' => t('Allowed values list'),
        '#default_value' => $field_settings['radios']['allowed_values'],
        '#rows' => 5,
        '#description' => '<p>' . t('The possible values this field can contain. Enter one value per line, in the format "label" or "key|label". The key is the value that will be stored in the database') . '</p>',
        '#element_validate' => array('matrix_allowed_values_setting_validate'),
        '#states' => array(
          'visible' => array(
            ':input[name="field[settings][preview][form]['.$axis .']['. $cell .'][field_type]"]' => array(
              'value' => 'radios',
            ),
          ),
        ),
      );
      $form['preview']['form'][$axis][$cell]['radios']['allowed_values_function'] = array(
        '#type' => 'select',
        '#title' => t('Allowed values function'),
        '#description' => t('Instead of using a list of allowed values, you may use values specified by a php function. See the documentation on how to use this feature.'),
        '#options' => $functions['allowed_values'],
        '#default_value' => $field_settings['radios']['allowed_values_function'],
        '#states' => array(
          'visible' => array(
            ':input[name="field[settings][preview][form]['.$axis .']['. $cell .'][field_type]"]' => array(
              'value' => 'radios',
            ),
          ),
        ),
      );
      
      //checkboxes
      $form['preview']['form'][$axis][$cell]['checkboxes']['required'] = array(
        '#type' => 'checkbox',
        '#title' => t('Required'),
        '#default_value' => $field_settings['checkboxes']['required'],
        '#states' => array(
          'visible' => array(
            ':input[name="field[settings][preview][form]['.$axis .']['. $cell .'][field_type]"]' => array(
              'value' => 'checkboxes',
            ),
          ),
        ),
      );
      $form['preview']['form'][$axis][$cell]['checkboxes']['allowed_values'] = array(
        '#type' => 'textarea',
        '#title' => t('Allowed values list'),
        '#default_value' => $field_settings['checkboxes']['allowed_values'],
        '#rows' => 5,
        '#description' => '<p>' . t('The possible values this field can contain. Enter one value per line, in the format "label" or "key|label". The key is the value that will be stored in the database') . '</p>',
        '#element_validate' => array('matrix_allowed_values_setting_validate'),
        '#states' => array(
          'visible' => array(
            ':input[name="field[settings][preview][form]['.$axis .']['. $cell .'][field_type]"]' => array(
              'value' => 'checkboxes',
            ),
          ),
        ),
      );
      $form['preview']['form'][$axis][$cell]['checkboxes']['allowed_values_function'] = array(
        '#type' => 'select',
        '#title' => t('Allowed values function'),
        '#description' => t('Instead of using a list of allowed values, you may use values specified by a php function. See the documentation on how to use this feature.'),
        '#options' => $functions['allowed_values'],
        '#default_value' => $field_settings['checkboxes']['allowed_values_function'],
        '#states' => array(
          'visible' => array(
            ':input[name="field[settings][preview][form]['.$axis .']['. $cell .'][field_type]"]' => array(
              'value' => 'checkboxes',
            ),
          ),
        ),
      );
      
      //calculation
      $form['preview']['form'][$axis][$cell]['calculation']['calculation'] = array(
        '#type' => 'select',
        '#title' => t('Type of calculation'),
        '#description' => t('Calculation to be performed on the %axis.', array('%axis' => $other_axis_name)),
        '#default_value' => $field_settings['calculation']['calculation'],
        '#options' => array(
          'sum' => t('Sum'),
          'average' => t('Average'),
          'max' => t('Maximum'),
          'min' => t('Minimum'),
        ),
        '#states' => array(
          'visible' => array(
            ':input[name="field[settings][preview][form]['.$axis .']['. $cell .'][field_type]"]' => array(
              'value' => 'calculation',
            ),
          ),
        ),
      );
      $form['preview']['form'][$axis][$cell]['calculation']['prefix'] = array(
        '#type' => 'textfield',
        '#title' => t('Prefix'),
        '#size' => 5,
        '#description' => t('Prefix for calculation'),
        '#default_value' => $field_settings['calculation']['prefix'],
        '#states' => array(
          'visible' => array(
            ':input[name="field[settings][preview][form]['. $axis .']['. $cell .'][field_type]"]' => array(
              'value' => 'calculation',
            ),
          ),
        ),
      );
      $form['preview']['form'][$axis][$cell]['calculation']['suffix'] = array(
        '#type' => 'textfield',
        '#title' => t('Suffix'),
        '#size' => 5,
        '#description' => t('Suffix for calculation'),
        '#default_value' => $field_settings['calculation']['suffix'],
        '#states' => array(
          'visible' => array(
            ':input[name="field[settings][preview][form]['. $axis .']['. $cell .'][field_type]"]' => array(
              'value' => 'calculation',
            ),
          ),
        ),
      );
      $form['preview']['form'][$axis][$cell]['calculation']['rounding'] = array(
        '#type' => 'textfield',
        '#title' => t('Rounding'),
        '#size' => 5,
        '#description' => t('Number of decimal places to round to'),
        '#default_value' => $field_settings['calculation']['rounding'],
        '#states' => array(
          'visible' => array(
            ':input[name="field[settings][preview][form]['.$axis .']['. $cell .'][field_type]"]' => array(
              'value' => 'calculation',
            ),
          ),
        ),
      );
      
      //custom
      $form['preview']['form'][$axis][$cell]['custom']['custom'] = array(
        '#type' => 'select',
        '#options' => $functions['calculation'],
        '#title' => t('Custom PHP function for processing'),
        '#description' => t('Custom php function which should be used for processing. See the documentation for details on how to use this feature.'),
        '#default_value' => $field_settings['custom']['custom'],
        '#states' => array(
          'visible' => array(
            ':input[name="field[settings][preview][form]['.$axis .']['. $cell .'][field_type]"]' => array(
              'value' => 'custom',
            ),
          ),
        ),
      );
      
      //submit
      $form['preview']['form']['submit'] = array(
        '#type' => 'button',
        '#name' => 'matrix-settings-save-'. $axis .'-'. $cell,
        '#value' => t('Update'),
        '#ajax' => array(
          'callback' => 'matrix_settings_ajax_full_callback',
          'wrapper' => 'matrix-settings-ajax',
        ),
      );
    }
  }
  
  //used for detecting when changing $define or if the rows or cols count goes to -1
  $settings['storage']['old_define'] = $settings['define'];
  $settings['storage']['old_rows_count'] = $settings['rows_count'];
  $settings['storage']['old_cols_count'] = $settings['cols_count'];
  cache_set($cache_id,  $settings['storage'], 'cache', CACHE_TEMPORARY);
  return $form;
}

/**
 * AJAX callback
 * Returns the form for altering the settings of columns/rows
 */
function matrix_settings_ajax_edit_callback($form, &$form_state) {
  return $form['field']['settings']['preview']['form'];
}

/**
 * AJAX callback
 * Returns the complete grid preview after a large-scale settings update.
 */
function matrix_settings_ajax_full_callback($form, &$form_state) {
  return $form['field']['settings']['preview'];
}

/**
 * Theme the display of the form
 */
function theme_matrix_preview($variables) {
  $form = $variables['form'];
  $cols_count = $form['#setup']['cols']['count'];
  $rows_count = $form['#setup']['rows']['count'];
  
  if ($rows_count == 0 || $cols_count == 0) {
    return ''; //show nothing at all if either rows or columns = 0
  }
  
  //render first row (headers)
  $items = array();
  $items[] = ''; // top left cell
  for ($i = 1; $i <= $cols_count; $i++) {
    $items[] = drupal_render($form['cols'][$i]['summary']) . drupal_render($form['cols'][$i]['button'])
             . drupal_render($form['cols'][$i]['edit-wrapper']);
  }
  $rows[] = $items;
  
  //render all subsequent rows
  for ($i = 1; $i <= $rows_count; $i++) {
    $items = array();
    $items[] = drupal_render($form['rows'][$i]['summary']) . drupal_render($form['rows'][$i]['button'])
             . drupal_render($form['rows'][$i]['edit-wrapper']);
    
    for ($j = 1; $j <= $cols_count; $j++) {
      $items[] = '';
    }
    $rows[] = $items;
  }  
  
  $output = drupal_render($form['message2']);
  $output .= drupal_render($form['define']);
  $output .= theme('table', array('header' => NULL, 'rows' => $rows, 'attributes' => array('border' => 1)));
  $output .= drupal_render($form['form']);
  return $output;
}

/**
 * Implementation of hook_form_FORM_ID_alter()
 */
function matrix_form_field_ui_field_edit_form_alter(&$form, &$form_state, $form_id) {
  if ($form['#field']['module'] != 'matrix') {
    return;
  }
  
  $form['#submit'][] = 'matrix_clear_cache_submit'; //Add a submit handler to ensure that the temp cache is cleared
  
  //The module can only work if cardinality is set to unlimited
  //This overrides the form element removing it from the UI
  $form['field']['cardinality'] = array(
    '#type' => 'value',
    '#value' => FIELD_CARDINALITY_UNLIMITED,
  );
  
  //The module can only work if required is set to FALSE.
  //Depending on the widget, the required attribute is controlled seperately
  $form['instance']['required'] = array(
    '#type' => 'value',
    '#value' => FALSE,
  );
}
