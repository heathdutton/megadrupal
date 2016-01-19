<?php
/**
 * @file
 *
 *
 *
 * @author Kálmán Hosszu - hosszu.kalman@gmail.com - http://www.kalman-hosszu.com
 */

/**
 * Build imagelens_settings_form form.
 *
 * @param array $form_state
 * @return array The created form.
 */
function imagelens_settings_form($form_state) {
  $form = array();

  $form['imagelens_always_add_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('Always include JavaScript file to the site.'),
    '#default_value' => variable_get('imagelens_always_add_js'),
  );


  return system_settings_form($form);
}

/**
 * Menu callback; Listing of all current imageLend styles.
 */
function imagelens_style_list() {
  $page = array();

  $styles = imagelens_styles();
  $page['imagelens_style_list'] = array(
    '#markup' => theme('imagelens_style_list', array('styles' => $styles)),
  );

  return $page;
}

/**
 * Build imagelens_style_form from.
 *
 * @param array $form
 * @param array $form_state
 * @param mixed $style
 * @return array 
 */
function imagelens_style_form($form, &$form_state, $style = NULL) {
  $form = array();
  
  $edit_form = !is_null($style);
  
  if ($edit_form) {
    drupal_set_title(t('Edit %name style', array('%name' => $style['name'])), PASS_THROUGH);
    $form['ilsid'] = array(
      '#type' => 'value',
      '#value' => $style['ilsid'],
    );
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Style name'),
    '#required' => TRUE,
    '#default_value' => $edit_form ? $style['name'] : NULL,
  );
  
  $form['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#description' => t('The width of the imageLens image. Leave it blank to stay the original size.'),
    '#size' => 10,
    '#default_value' => $edit_form ? $style['width'] : NULL,
  );
  
  $form['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#description' => t('The height of the imageLens image. Leave it blank to stay the original size.'),
    '#size' => 10,
    '#default_value' => $edit_form ? $style['height'] : NULL,
  );
  
  $form['actions'] = array('#type' => 'actions');
  
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Validate imagelens_style_form form.
 *
 * @param array $form
 * @param array $form_state 
 */
function imagelens_style_form_validate($form, &$form_state) {
  // If nothing added
  if (empty($form_state['values']['width']) && empty($form_state['values']['height'])) {
    form_error($form['width'], t('Add height or width parameter.'));
    form_error($form['height'], t('Add height or width parameter.'));
  }
  else {
    // If not a number
    if (!empty($form_state['values']['width']) && !is_numeric($form_state['values']['width'])) {
      form_error($form['width'], t('Please add a valid number.'));
    }
    
    if (!empty($form_state['values']['height']) && !is_numeric($form_state['values']['height'])) {
      form_error($form['height'], t('Please add a valid number.'));
    }
  }
}

/**
 * Submit imagelens_style_form form.
 *
 * @param array $form
 * @param array $form_state 
 */
function imagelens_style_form_submit($form, &$form_state) {
  $style = array(
    'name' => $form_state['values']['name'],
    'width' => $form_state['values']['width'],
    'height' => $form_state['values']['height'],
    'ilsid' => isset($form_state['values']['ilsid']) ? $form_state['values']['ilsid'] : '',
  );
  
  $form_state['redirect'] = 'admin/config/media/imagelens-styles/list';
  
  imagelens_style_save($style);
  
  drupal_set_message(t('The imageLens style was successfully saved.'));
}

/**
 * Build imagelens_style_delete_form form.
 * 
 * @param array $form_state
 * @return array The created form. 
 */
function imagelens_style_delete_form($form, &$form_state, $style) {
  $form['ilsid'] = array(
    '#type' => 'value',
    '#value' => $style['ilsid'],
  );


  return confirm_form(
    $form,
    t('Are you shure want to delet %style', array('%style' => $style['name'])),
    'admin/config/media/imagelens-styles/list',
    NULL,
    t('Delete')
  );
}

/**
 * Submit function for imagelens_style_delete_form form.
 *
 * @param array $form
 * @param array $form_state 
 */
function imagelens_style_delete_form_submit($form, &$form_state) {
  imagelens_style_delete($form_state['values']['ilsid']);
  
  drupal_set_message(t('The imageLens style was successfully deleted.'));
  
  $form_state['redirect'] = 'admin/config/media/imagelens-styles/list';
}

/**
 * Theme function form imagelens_style_list.
 *
 * @param array $styles
 *   An array with the list of imageLens style arrays.
 * @return string
 *   The rendered HTML table string. 
 */
function theme_imagelens_style_list($variables) {
  $styles = $variables['styles'];
  
  $header = array(t('Style name'), t('Width'), t('Height'), array('data' => t('Operations'), 'colspan' => 2));
  $rows = array();
  
  foreach ($styles as $ilsid => $style) {
    $rows[] = array(
      $style['name'],
      $style['width'],
      $style['height'],
      l(t('edit'), 'admin/config/media/imagelens-styles/edit/' . $ilsid),
      l(t('delete'), 'admin/config/media/imagelens-styles/delete/' . $ilsid),
    );
  }
  
  if (empty($rows)) {
    $rows[] = array(array(
      'colspan' => 5,
      'data' => t('There are currently no styles. <a href="!url">Add a new one</a>.', array('!url' => url('admin/config/media/imagelens-styles/add'))),
    ));
  }
  
  return theme('table', array('header' => $header, 'rows' => $rows));
}