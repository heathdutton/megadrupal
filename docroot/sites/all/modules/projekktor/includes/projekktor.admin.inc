<?php

/**
 * @file
 * Administrative page callbacks for the projekktor module.
 */

/**
 * Menu callback; Listing of all current presets.
 */
function projekktor_page_preset_list() {
  $presets = projekktor_presets();

  $header = array(
    t('Preset Name'),
    array(
      'data' => t('Operations'),
      'colspan' => 2,
    ),
  );
  $rows = array();
  foreach ($presets as $name => $preset) {
    $rows[] = array(
      l($preset['title'], 'admin/config/media/projekktor/edit/' . $name),
      l(t('edit'), 'admin/config/media/projekktor/edit/' . $name),
      // Hide the delete link for the 'default' set.
      ($name == 'default') ? '' : l(t('delete'), 'admin/config/media/projekktor/delete/' . $name),
    );
  }

  return theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'empty' => t('There are currently no presets. <a href="!url">Add a new one</a>.', array('!url' => url('admin/config/media/projekktor/add'))),
    ));
}

/**
 * Form builder; Form for adding a new preset.
 */
function projekktor_form_preset_add($form, &$form_state) {
  $form['title'] = array(
    '#type' => 'textfield',
    '#maxlength' => '255',
    '#title' => t('Title'),
    '#description' => t('A human-readable title for this preset.'),
    '#required' => TRUE,
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#maxlength' => '255',
    '#machine_name' => array(
      'source' => array('title'),
      'exists' => 'projekktor_preset_exists',
    ),
    '#required' => TRUE,
  );

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Create new preset'),
    ),
    'cancel' => array(
      '#type' => 'link',
      '#title' => t('Cancel'),
      '#href' => 'admin/config/media/projekktor',
    ),
  );

  return $form;
}

/**
 * Submit handler for adding a new preset.
 */
function projekktor_form_preset_add_submit($form, &$form_state) {
  $preset = array(
    'name' => $form_state['values']['name'],
    'title' => $form_state['values']['title'],
    'options' => array(
      'height' => 320,
      'width' => 640,
    ),
  );
  $preset = projekktor_preset_save($preset, TRUE);
  drupal_set_message(t('preset %name was created.', array('%name' => $preset['name'])));
  $form_state['redirect'] = 'admin/config/media/projekktor/edit/' . $preset['name'];
}

/**
 * Theme to embed tables into forms.
 */
function theme_projekktor_form_table($variables) {
  $form = $variables['form'];

  $rows = array();
  foreach (element_children($form) as $row_key) {
    $row = array();
    foreach (element_get_visible_children($form[$row_key]) as $cell_key) {
      $cell = array('data' => drupal_render($form[$row_key][$cell_key]));
      if (!empty($form[$row_key][$cell_key]['#table_attributes'])) {
        $cell += $form[$row_key][$cell_key]['#table_attributes'];
      }
      $row[] = $cell;
    }
    $rows[] = $row;
  }

  $variables = array();
  foreach ($form as $key => $value) {
    if (element_property($key)) {
      $variables[substr($key, 1)] = $value;
    }
  }
  $variables['rows'] = $rows;

  return theme('table', $variables);
}

/**
 * This function returns an array defining the form elements used to edit
 * the different options.
 */
function projekktor_option_elements() {
  return array(
    'autoplay' => array(
      '#type' => 'checkbox',
      '#title' => t('Autoplay'),
      '#description' => t('Automatically start playback once page has loaded -  will be overwritten by autoplay-attribute of the replaced @video tag.', array('@video' => '<video>')),
      '#default_value' => TRUE,
    ),
    'continuous' => array(
      '#type' => 'checkbox',
      '#title' => t('Continuous'),
      '#description' => t('If more than one item is scheduled, true will automatically start playback of the next item in line once current one completed.'),
      '#default_value' => TRUE,
    ),
    'controls' => array(
      '#type' => 'checkbox',
      '#title' => t('Controls'),
      '#description' => t('Enable/disable controls -  will be overwritten by controls-attribute of the replaced @video tag.', array('@video' => '<video>')),
      '#default_value' => TRUE,
    ),
    'height' => array(
      '#type' => 'textfield',
      '#title' => t('Height'),
      '#description' => t('The height of the player in pixels.'),
      '#element_validate' => array('_projekktor_validate_integer'),
      '#default_value' => 640,
    ),
    'ignoreAttributes' => array(
      '#type' => 'checkbox',
      '#title' => t('Continuous'),
      '#description' => t('If set true during instanciation the player will ignore all behavior relevant attributes of the destination @video or @audio tag like loop, autoplay, controls.', array('@video' => '<video>', '@audio' => '<audio>')),
      '#default_value' => TRUE,
    ),
    'imageScaling' => array(
      '#type' => 'select',
      '#title' => t('Image scaling'),
      '#description' => t('Scaling used for images (playlist items and posters) "fill" or "aspectratio"'),
      '#options' => array(
        'fill' => t('Fill the stage'),
        'aspectratio' => t('Scale down to aspect ratio'),
      ),
      '#default_value' => 'aspectratio',
    ),
    'loop' => array(
      '#type' => 'checkbox',
      '#title' => t('Loop'),
      '#description' => t('For looping scheduled media elements -  will be overwritten by loop-attribute of a replaced @video tag.', array('@video' => '<video>')),
      '#default_value' => TRUE,
    ),
    'poster' => array(
      '#type' => 'textfield',
      '#title' => t('Poster'),
      '#description' => t('Path to the item that will be used for the background poster.'),
      '#default_value' => file_create_url(libraries_get_path('projekktor') . '/media/intro.png'),
    ),
    'useYTIframeAPI' => array(
      '#type' => 'checkbox',
      '#title' => t('Use YouTube iframe API'),
      '#description' => t('Youtube offers two different player APIs: fLaSh and "iFrame" for HTML5 . Make your choice here. For mobile devices this is forced to TRUE'),
      '#default_value' => TRUE,
    ),
    'videoScaling' => array(
      '#type' => 'select',
      '#title' => t('Video scaling'),
      '#description' => t('Scaling used for videos (flash and native, not youtube) "fill" or "aspectratio".'),
      '#options' => array(
        'fill' => t('Fill the stage'),
        'aspectratio' => t('Scale down to aspect ratio'),
      ),
      '#default_value' => 'aspectratio',
    ),
    'volume' => array(
      '#type' => 'textfield',
      '#title' => t('Volume'),
      '#description' => t('The volume level when a video is started. Set between 0 and 1'),
      '#element_validate' => array('_projekktor_validate_volume'),
      '#default_value' => 0.8,
    ),
    'width' => array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#description' => t('The width of the player in pixels.'),
      '#element_validate' => array('_projekktor_validate_integer'),
      '#default_value' => 640,
    ),
  );
}

/**
 * Returns the form element to use to edit the given option.
 */
function projekktor_option_element($option, $value) {
  $elements = projekktor_option_elements();
  $element = isset($elements[$option]) ? $elements[$option] : array('#type' => 'textfield');

  if ($value !== NULL) {
    if ($element['#type'] == 'select') {
      if ($value === TRUE) {
        $value = 'true';
      }
      elseif ($value === FALSE) {
        $value = 'false';
      }
    }
    $element['#default_value'] = $value;
  }

  return $element;
}

/**
 * Form  builder; Form to edit a given preset.
 */
function projekktor_form_preset_edit($form, &$form_state, $preset) {
  if (empty($form_state['preset'])) {
    $form_state['preset'] = $preset;
  }
  else {
    $preset = $form_state['preset'];
  }

  // Title.
  $form['title'] = array(
    '#type' => 'textfield',
    '#maxlength' => '255',
    '#title' => t('Title'),
    '#default_value' => $preset['title'],
    '#description' => t('A human-readable title for this preset.'),
    '#required' => TRUE,
  );

  // Option table.
  $form['options'] = array(
    '#theme' => 'projekktor_form_table',
    '#tree' => TRUE,
    '#header' => array(t('Name'), t('Value'), t('Operations')),
  );

  $i = 0;
  foreach ($preset['options'] as $key => $value) {
    $option_element = projekktor_option_element($key, $value);

    $form['options'][] = array(
      'name' => array(
        '#type' => 'item',
        '#title' => check_plain($key),
        '#description' => isset($option_element['#title']) ? $option_element['#title'] : '',
      ),
      'value' => $option_element + array(
        '#option_name' => $key,
        '#title_display' => 'none',
      ),
      'delete' => array(
        '#type' => 'submit',
        '#name' => 'button_del_' . $i++,
        '#value' => t('Delete'),
        '#submit' => array('projekktor_form_preset_edit_submit_delete'),
        '#limit_validation_errors' => array(),
      ),
    );
  }

  // 'Add option' row at the end of the table.
  $options = array_diff(array_keys(projekktor_option_elements()), array_keys($preset['options']));
  $options = empty($options) ? array() : array_combine($options, $options);
  $form['options'][] = array(
    'add_option_row' => array(
      '#table_attributes' => array('colspan' => '3', 'class' => array('container-inline')),
      '#tree' => FALSE,
      'new_option' => array(
        '#type' => 'select',
        '#options' => $options,
        '#empty_option' => t('Select or enter:'),
      ),
      'new_option_custom' => array(
        '#type' => 'textfield',
        '#states' => array(
          'visible' => array(
            ':input[name="new_option"]' => array('value' => ''),
          ),
        ),
      ),
      'button_add' => array(
        '#type' => 'submit',
        '#name' => 'add_option',
        '#value' => t('Add option'),
        '#submit' => array('projekktor_form_preset_edit_submit_add'),
        '#limit_validation_errors' => array(
          array('new_option'),
          array('new_option_custom'),
        ),
      ),
    ),
  );

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#name' => 'submit',
      '#value' => t('Save preset'),
    ),
    'cancel' => array(
      '#type' => 'link',
      '#title' => t('Cancel'),
      '#href' => 'admin/config/media/projekktor',
    ),
  );

  return $form;
}

/**
 * Validate a form element that should have an integer value.
 */
function _projekktor_validate_integer($element, &$form_state) {
  $value = $element['#value'];
  if ($value !== '' && (!is_numeric($value) || intval($value) != $value || $value < 0)) {
    form_error($element, t('%name must be a positive integer.', array('%name' => $element['#title'])));
  }
}

/**
 * Validate a form element that should have a number as value.
 */
function _projekktor_validate_number($element, &$form_state) {
  $value = $element['#value'];
  if ($value !== '' && !is_numeric($value)) {
    form_error($element, t('%name must be a number.', array('%name' => $element['#option_name'])));
  }
}

/**
 * Validate a form element that should have a value between 0 and 1.
 */
function _projekktor_validate_volume($element, &$form_state) {
  $value = $element['#value'];
  if ($value !== '' && (!is_numeric($value) || $value < 0 || $value > 1)) {
    form_error($element, t('%name must be a value between 0 and 1.', array('%name' => $element['#option_name'])));
  }
}

/**
 * Submit handler for 'Add option' button; Add a new option to the set.
 */
function projekktor_form_preset_edit_submit_add($form, &$form_state) {
  $preset = &$form_state['preset'];

  if (!empty($form_state['values']['new_option'])) {
    $new_option_element = 'new_option';
  }
  elseif (!empty($form_state['values']['new_option_custom'])) {
    $new_option_element = 'new_option_custom';
  }

  if (isset($new_option_element)) {
    $new_option = $form_state['values'][$new_option_element];
    if (!array_key_exists($new_option, $preset['options'])) {
      // Add the new option with a NULL value.
      // The input element cares for a default value.
      $preset['options'][$new_option] = NULL;
      // Reset the input field.
      $form_state['input'][$new_option_element] = '';
      drupal_set_message(t('Option %name added.', array('%name' => $new_option)));
    }
    else {
      form_set_error($new_option_element, t('This set already includes the %name option.', array('%name' => $new_option)));
    }
  }

  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for 'Delete' buttons; Delete an option from the set.
 */
function projekktor_form_preset_edit_submit_delete($form, &$form_state) {
  $preset = &$form_state['preset'];

  $rowindex = $form_state['triggering_element']['#parents'][1];
  $option = $form['options'][$rowindex]['value']['#option_name'];

  unset($preset['options'][$option]);
  drupal_set_message(t('Option %name removed.', array('%name' => $option)));

  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for 'Save preset' button; Save the changed preset.
 */
function projekktor_form_preset_edit_submit($form, &$form_state) {
  $preset = &$form_state['preset'];

  $preset['title'] = $form_state['values']['title'];
  foreach ($form_state['values']['options'] as $index => $values) {
    $element = $form['options'][$index]['value'];
    $value = $values['value'];

    if ($value !== '') {
      // Do some typeguessing here...
      if ($element['#type'] == 'checkbox') {
        $value = (bool) $value;
      }
      elseif (is_numeric($value)) {
        $value = (float) $value;
      }
      elseif (strcasecmp($value, 'true') == 0) {
        $value = TRUE;
      }
      elseif (strcasecmp($value, 'false') == 0) {
        $value = FALSE;
      }
    }

    $option = $element['#option_name'];
    $preset['options'][$option] = $value;
  }

  projekktor_preset_save($preset);
  drupal_set_message(t('preset %name changed.', array('%name' => $preset['name'])));
  $form_state['redirect'] = 'admin/config/media/projekktor';
}

/**
 * Form builder; Form to delete a given preset.
 */
function projekktor_preset_form_delete($form, &$form_state, $preset) {
  $form_state['preset'] = &$preset;

  return confirm_form(
    $form,
    t('Are you sure you want to delete the preset %name?', array('%name' => $preset['name'])),
    'admin/config/media/projekktor',
    NULL,
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submit handler for deleting an preset.
 */
function projekktor_preset_form_delete_submit($form, &$form_state) {
  $preset = &$form_state['preset'];

  if ($preset['name'] == 'default') {
    // Prevent deletion of the default set so we can fall back to it.
    drupal_set_message(t('The default preset may not be deleted!', 'error'));
  }
  else {
    projekktor_preset_delete($preset);
    drupal_set_message(t('preset %name was deleted.', array('%name' => $preset['name'])));
  }

  $form_state['redirect'] = 'admin/config/media/projekktor';
}

/**
 * Form builder; Form for sitewide module settings.
 */
function projekktor_embed_settings_form($form, &$form_state) {
  $form = array();

  $form['sitewide'] = array(
    '#type' => 'fieldset',
    '#title' => 'Projekktor sitewide settings',
  );
  $form['sitewide']['projekktor_sitewide_video'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Projekktor site wide'),
    '#default_value' => variable_get('projekktor_sitewide_video', FALSE),
  );

  // Show select box for the preset.
  $presets = array();
  foreach (projekktor_presets() as $name => $preset) {
    $presets[$name] = check_plain($preset['title']);
  }
  $form['sitewide']['projekktor_sitewide_video_preset'] = array(
    '#title' => t('Preset'),
    '#type' => 'select',
    '#options' => $presets,
    '#default_value' => variable_get('projekktor_sitewide_video_preset', 'default'),
    '#states' => array(
      'visible' => array(
        ':input[name="projekktor_sitewide_video"]' => array('checked' => TRUE),
      ),
    ),
  );

  // Show the theme select box if there is more than one theme.
  $themes = array_keys(projekktor_get_themes(TRUE));
  if (count($themes) == 1) {
    $form['sitewide']['projekktor_sitewide_video_theme'] = array(
      '#type' => 'hidden',
      '#value' => $themes[0],
    );
  }
  elseif (count($themes) > 1) {
    asort($themes);

    $form['sitewide']['projekktor_sitewide_video_theme'] = array(
      '#type' => 'select',
      '#title' => t('Theme'),
      '#options' => array_combine($themes, $themes),
      '#default_value' => variable_get('projekktor_sitewide_video_theme', 'default'),
      '#states' => array(
        'visible' => array(
          ':input[name="projekktor_sitewide_video"]' => array('checked' => TRUE),
        ),
      ),
    );
  }

  if (variable_get('projekktor_sitewide_video', FALSE)) {
    $form['test'] = array(
      '#markup' => '<video id="demoplayer" class="projekktor" title="PLASTER SESSION" src="http://www.youtube.com/watch?v=dP9et4xlnSE" type="video/youtube" controls></video>',
    );
  }

  return system_settings_form($form);
}

/**
 * Form builder; Form for library summary.
 */
function projekktor_form_settings() {
  $form = array();

  $form['library'] = array(
    '#type' => 'fieldset',
    '#title' => 'Projekktor library',
  );
  $form['library']['lib_path'] = array(
    '#type' => 'item',
    '#title' => t('Projekktor library path'),
    '#markup' => t('<code>@file</code>', array('@file' => libraries_get_path('projekktor'))),
  );
  $form['library']['lib_file'] = array(
    '#type' => 'item',
    '#title' => t('Projekktor library file'),
    '#markup' => t('<code>@file</code>', array('@file' => projekktor_get_library_file())),
    '#description' => t('This file is cached until the file is deleted.'),
  );
  $form['library']['lib_version'] = array(
    '#type' => 'item',
    '#title' => t('Projekktor version'),
    '#markup' => t('<code>@file</code>', array('@file' => projekktor_get_library_version())),
    '#description' => t('This is the projekktor version of the cached library file.'),
  );

  $form['library']['themes_title'] = array(
    '#type' => 'item',
    '#title' => t('Projekktor controlbar themes'),
    '#description' => t('The following themes have been detected. Clear the cache to search for new ones. The css file in contributed themes must be named <em>projekktor.style.css</em> for the module to recognize them.'),
  );
  $form['library']['themes'] = array(
    '#theme' => 'projekktor_form_table',
    '#header' => array(t('Name'), t('CSS path')),
    '#empty' => t('No themes found!'),
  );
  foreach (projekktor_get_themes() as $theme => $file) {
    $form['library']['themes'][] = array(
      array(
        '#markup' => check_plain($theme),
      ),
      array(
        '#markup' => t('<code>@file</code>', array('@file' => $file)),
      ),
    );
  }

  $form['library']['button_clearcache'] = array(
    '#type' => 'submit',
    '#name' => 'button_clearcache',
    '#value' => t('Clear cache'),
    '#submit' => array('projekktor_form_settings_submit_clearcache'),
  );

  return $form;
}

/**
 * Submit handler for the projekktor library summary form button 'Clear cache'.
 */
function projekktor_form_settings_submit_clearcache($form, &$form_state) {
  cache_clear_all('projekktor_', 'cache', TRUE);
  drupal_set_message(t('Cache cleared.'));
}

/**
 * Submit handler for the projekktor library summary settings.
 */
function projekktor_form_settings_submit($form, &$form_state) {
  drupal_set_message(t('NYI: Nothing done.'));
}

/**
 * Form builder; Form for jcarousel library summary.
 */
function projekktor_jcarousel_form_settings() {
  $form = array();

  $form['library'] = array(
    '#type' => 'fieldset',
    '#title' => 'jCarousel library',
  );

  $file = libraries_get_path('jquery.jcarousel');
  if (!is_dir($file)) {
    drupal_set_message(t('jCarousel library directory not found: <code>@file</code>', array('@file' => $file)), 'error', FALSE);
  }
  $form['library']['lib_path'] = array(
    '#type' => 'item',
    '#title' => t('jCarousel library path'),
    '#markup' => t('<code>@file</code>', array('@file' => $file)),
  );

  $file = projekktor_jcarousel_get_library_file();
  $form['library']['lib_file'] = array(
    '#type' => 'item',
    '#title' => t('jCarousel library file'),
    '#markup' => $file ? t('<code>@file</code>', array('@file' => $file)) : t('Unknown'),
    '#description' => t('This filename is cached until the file is deleted.'),
  );

  $form['library']['themes_title'] = array(
    '#type' => 'item',
    '#title' => t('jCarousel themes'),
    '#description' => t('The following themes are currently known to the module. Clear the cache to search for new ones.'),
  );
  $form['library']['themes'] = array(
    '#theme' => 'projekktor_form_table',
    '#header' => array(t('Name'), t('CSS Path')),
    '#empty' => t('No themes found!'),
  );
  foreach (projekktor_jcarousel_get_themes() as $theme => $file) {
    $form['library']['themes'][] = array(
      array(
        '#markup' => check_plain($theme),
      ),
      array(
        '#markup' => t('<code>@file</code>', array('@file' => $file)),
      ),
    );
  }

  $form['library']['button_clearcache'] = array(
    '#type' => 'submit',
    '#name' => 'button_clearcache',
    '#value' => t('Clear cache'),
    '#submit' => array('projekktor_jcarousel_form_settings_submit_clearcache'),
  );

  return $form;
}

/**
 * Submit handler for the carousel library settings form button 'Clear cache'.
 */
function projekktor_jcarousel_form_settings_submit_clearcache($form, &$form_state) {
  cache_clear_all('projekktor_jcarousel_', 'cache', TRUE);
  drupal_set_message(t('Cache cleared.'));
}

/**
 * Submit handler for the carousel library settings.
 */
function projekktor_jcarousel_form_settings_submit($form, &$form_state) {
  drupal_set_message(t('NYI: Nothing done.'));
}
