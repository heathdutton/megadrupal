<?php
// $Id$

/**
 * Form constructor for admin jQuery UI themes settings form
 * 
 * @see jqueryui_theme_settings_form_submit()
 * @ingroup $forms
 */
function jqueryui_theme_settings_form($form, &$form_state) {
  $form['#tree'] = TRUE;
  $form['#theme'] = 'jqueryui_theme_settings';
  $form['#attached'] = array(
    'js' => array(drupal_get_path('module', 'jqueryui_theme') . '/jqueryui_theme_settings.js'),
  );
  
  $themes_infos = _jqueryui_theme_get_themes_infos();
  if (is_array($themes_infos)) {
    foreach ($themes_infos as $theme =>  $data) {
      
      $form['themes'][$theme]['checkbox'] = array(
        '#type' => 'checkbox',
        '#attributes' => array('class' => array('box-row', 'box-row-' . $theme)),
      );
      
      $form['themes'][$theme]['name'] = array(
        '#type' => 'textfield',
        '#default_value' => $data->human_name,
        '#size' => 20,
        '#maxlength' => 64,
        '#attributes' => array('class' => array('textfield-name', 'textfield-jqueryui-' . $theme)),
      );
      
      $drupal_themes = _jqueryui_theme_drupal_themes_availables(TRUE);
      foreach ($drupal_themes as $theme_name => $theme_available) {
        $form['themes'][$theme]['assoc'][$theme_name] = array(
          '#type' => 'checkbox',
          '#title' => $theme_available['status'] ? $theme_name : $theme_name . t('(disable)'),
          '#default_value' => in_array($theme_name, $data->drupal_theme_attach),
          '#attributes' => array('class' => array(
            'box-drupal-theme', 
            'box-cell-' . $theme_name,
            'box-jqueryui-' . $theme)),
        );
      }
      
      $form['themes'][$theme]['uri'] = array(
        '#type' => 'markup',
        '#markup' => $data->uri,
      );
      
      $form['themes'][$theme]['version'] = array(
        '#type' => 'markup',
        '#markup' => $data->version,
      );
      
      $form['themes'][$theme]['params'] = array(
        '#type' => 'markup',
        '#markup' => valid_url($data->params, TRUE) ? l(t('parameters'), $data->params) : $data->params,
      );
      
      $form['themes'][$theme]['infos'] = array(
        '#type' => 'value',
        '#value' => $data,
      );
    }
    
    $form['action'] = array(
      '#type' => 'select',
      '#title' => t('Choose an action'),
      '#options' => array(
        'update' => t('Update'),
        'delete' => t('Delete'),
      ),
    );
    
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
      '#attributes' => array('class' => array('submit-jqueryui-settings')),
    );
  
  }
  return $form;
}

/**
 * Form submission handler for jqueryui_theme_settings_form().
 */
function jqueryui_theme_settings_form_submit($form, $form_state) {
  $handle_themes = array();
  foreach ($form_state['values']['themes'] as $key => $data) {
    if ($data['checkbox']) {
      $handle_themes[$key] = $form_state['values']['themes'][$key];
    }
  }
  
  if (!empty($handle_themes)) {
    $action = $form_state['values']['action'];
    if ($action == 'update') {
      
      $menu_rebuild = FALSE;
      $trx = db_transaction();
      try {
        foreach ($handle_themes as $theme) {
          
          if ($theme['name'] != $theme['infos']->human_name) {
            _jqueryui_theme_rename($theme['infos']->machine_name, filter_xss($theme['name']));
          }
          
          foreach ($theme['assoc'] as $theme_name => $check) { 
            if (!$check && in_array($theme_name, $theme['infos']->drupal_theme_attach)) {
                _jqueryui_theme_dissoc($theme['infos']->machine_name, $theme_name);
                $menu_rebuild = TRUE;
            }
          }
        }
        
        //second loop if drupal theme have been available during process
        foreach ($handle_themes as $theme) {
          foreach ($theme['assoc'] as $theme_name => $check) {
            if ($check) {
              if (array_key_exists($theme_name, _jqueryui_theme_drupal_themes_availables())) {
                _jqueryui_theme_assoc($theme['infos']->machine_name, $theme_name);
                $menu_rebuild = TRUE;
              }
              else {
                $themes_used = _jqueryui_theme_drupal_themes_used($theme_name);
                if ($themes_used[$theme_name]['jqueryui_theme'] != $theme['infos']->machine_name) {
                  drupal_set_message(t('Unable to attach jQuery UI "@theme" with @drupal_theme (already use by "@theme_used").', array(
                    '@theme' => $theme['infos']->human_name,
                    '@drupal_theme' => $theme_name,
                    '@theme_used' => $themes_used[$theme_name]['jqueryui_theme'],
                  )), 'error');
                }
              }
            }
          }
        }
        unset($trx);
      } catch (PDOException $e) {
        $trx->rollback();
        watchdog_exception('jqueryui_theme update', $e);
        throw $e;
      }
      
      drupal_set_message(t('The configuration have been saved'));
    } 
    elseif ($action == 'delete') {
      
      $trx = db_transaction();
      try {
        foreach ($handle_themes as $theme) {
          _jqueryui_theme_delete($theme['infos']->machine_name, $theme['infos']->uri);
        }
        unset($trx);
        $menu_rebuild = TRUE;
      } catch (PDOException $e) {
          $trx->rollback();
          watchdog_exception('jqueryui_theme delete', $e);
          throw $e;
      }
      drupal_set_message(format_plural(sizeof($handle_themes), '1 jQuery UI theme has been deleted', '@count jQuery UI themes have been deleted'));
      
    }
    
    if ($menu_rebuild) {
      menu_rebuild();
    }
  }
  else {
    drupal_set_message(t('No jQuery UI theme selected'), 'error');
  }
}

/**
 * Form constructor for admin jQuery UI themes import form
 * 
 * @see jqueryui_theme_import_form_validate()
 * @see jqueryui_theme_import_form_submit()
 * @ingroup $forms
 */
function jqueryui_theme_import_form($form, &$form_state) {
  
  if (!isset($form_state['next_step']['step_two'])) {
    
    $form['help_text'] = array(
      '#prefix' => '<p>',
      '#markup' => t('You can find jQuery UI themes on <a href="@link">jqueryui.com</a>. ' .
        'The following file extensions are supported: @extensions. ' .
        'Maximun size upload : @max_size', array(
          '@link' => 'http://jqueryui.com/themeroller/',
          '@max_size' => round(file_upload_max_size()/(1024*1024), 2) . 'M',
          '@extensions' => archiver_get_extensions(),
      )),
      '#suffix' => '</p>',
    );
    
    $options = _jqueryui_theme_default_jqueryui_themes_exist(); 
    if ($options) {
      
      $form['default_themes'] = array(
        '#type' => 'radios',
        '#title' => t('Import default jQuery UI themes'),
        '#options' => $options,
        '#default_value' => reset(array_keys($options)),
      );
      
      $form['information_one'] = array(
        '#prefix' => '<strong>',
        '#markup' => t('Or'),
        '#suffix' => '</strong>',
      );
    }
    
    $form['themes_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Upload jQuery UI themes from an internal or external URL'),
      '#description' => t('For example: @url', array('@url' => 'http://jquery-ui.googlecode.com/files/jquery-ui-themes-1.7.zip')),
    );
    
    $form['information_two'] = array(
      '#prefix' => '<strong>',
      '#markup' => t('Or'),
      '#suffix' => '</strong>',
    );
    
    $form['upload_themes'] = array(
      '#type' => 'file',
      '#title' => t('Upload an archive containing multiples jQuery UI Themes'),
      '#description' => t('For example: @filename from your local computer', array('@filename' => 'name.tar.gz')),
    );
    
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Import'),
    );
  
  } 
  else {
    
    $form['#tree'] = TRUE;
    $form['#theme'] = 'jqueryui_theme_import_confirm';
    $form['#attached'] = array(
      'js' => array(drupal_get_path('module', 'jqueryui_theme') . '/jqueryui_theme_settings.js'),
    );
    
    //if page is refresh, last process still be execute
    $themes = $form_state['next_step']['themes_infos'];
    foreach ($themes as $theme) {
      
      $form['themes'][$theme['name']]['checkbox'] = array(
        '#type' => 'checkbox',
        '#default_value' => 1,
        '#attributes' => array('class' => array('box-row')),
      );
      
      $form['themes'][$theme['name']]['human_name'] = array(
        '#type' => 'textfield',
        '#default_value' => $theme['name'],
        '#maxlength' => 64,
      );
      
      $form['themes'][$theme['name']]['machine_name'] = array(
        '#type' => 'machine_name',
        '#title_display' => 'invisible',
        '#maxlength' => 64,
        '#default_value' => '',
        '#machine_name' => array(
          'exists' => 'jqueryui_theme_machine_name_exist',
          'source' => array('themes', $theme['name'], 'human_name'),
        ),
      );
      
      $form['themes'][$theme['name']]['display_version'] = array(
        '#type' => 'markup',
        '#markup' => $theme['version'], 
      );
      
      $form['themes'][$theme['name']]['version'] = array(
        '#type' => 'value',
        '#value' => $theme['version'], 
      );
      
      $form['themes'][$theme['name']]['files'] = array(
        '#type' => 'value',
        '#value' => $theme['files'],
      );
      
      $form['themes'][$theme['name']]['directory'] = array(
        '#type' => 'value',
        '#value' => $theme['directory'],
      );
    }
    unset($form_state['next_step']['themes_infos']);
    
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
      '#weight' => 10,
    );
  }
  return $form;
}

/**
 * Checks if URL to jQuery UI default theme
 * are still available and tarballs could be retrieve.
 * 
 * @return
 *   An array keyed by url to default jQuery UI theme default version
 *   as options expected by Form API.
 *   NULL otherwise.
 */
function _jqueryui_theme_default_jqueryui_themes_exist() {
  $options = array('none' => t('None'));
  $sources = array(
    'jQuery UI 1.7' => 'http://jquery-ui.googlecode.com/files/jquery-ui-themes-1.7.zip',
    'jQuery UI 1.8' => 'http://jquery-ui.googlecode.com/files/jquery-ui-themes-1.8.zip',
  );

  foreach ($sources as $key => $src) {
    $options[$src] = $key;
  }
  
  if (sizeof($options) > 1) {
    return $options;
  }
  return;
}

/**
 * Form validation handler for jqueryui_theme_import_form().
 *
 * @see jqueryui_theme_import_form_submit()
 */
function jqueryui_theme_import_form_validate($form, &$form_state) {
  if (!isset($form_state['next_step']['step_two'])) {
    if (empty($_FILES['files']['name']['upload_themes'])
      && empty($form_state['values']['themes_url']) 
      && (!isset($form_state['values']['default_themes']) || $form_state['values']['default_themes'] == 'none')) {
      form_set_error('', t('No jQuery UI themes submitted'));
    }
    if ($form_state['values']['themes_url']) {
      if (!valid_url($form_state['values']['themes_url'], TRUE)) {
        form_set_error('themes_url', t('The provided URL is invalid.'));
      }
      $form_state['values']['themes_url'] = check_url($form_state['values']['themes_url']);
    }
  }
  else {
    $name = array();
    $values =& $form_state['values']['themes'];
    foreach ($values as $theme_name => $theme) {
      if ($theme['checkbox']) {
        if (empty($theme['machine_name']) || empty($theme['human_name'])) {
          form_set_error('themes][' . $theme_name, t('You must enter a human name or/and a machine name'));
        }
        $name[] = $theme['machine_name'];
      }
      $values[$theme_name]['human_name'] = filter_xss($theme['human_name']);
    }
    if (sizeof($name) != sizeof(array_unique($name))) {
      form_set_error('', t('Machine name must be unique'));
    }
  }
}

/**
 * Form validation handler for jqueryui_theme_import_form().
 *
 * @see jqueryui_theme_import_form_validate()
 */
function jqueryui_theme_import_form_submit($form, &$form_state) {
  
  if (!isset($form_state['next_step']['step_two'])) {   
    $destination = 'temporary://jqueryui_theme/' . uniqid();
    $form_state['next_step']['destination_source'] = $destination;
    
    if (FALSE === (file_prepare_directory($destination, FILE_CREATE_DIRECTORY))) {
      drupal_set_message(t('Unable to create directory in : !destination.', array('!destination' => $destination)), 'error');
      return;
    }
    
    if (!empty($_FILES['files']['name']['upload_themes'])) {
      $field = 'upload_themes';
      $validators = array('file_validate_extensions' => array(archiver_get_extensions()), 'file_validate_size' => array((int)file_upload_max_size()));
      if (!($finfo = file_save_upload($field, $validators, $destination, FILE_EXISTS_REPLACE))) {
        return;
      }
      $form_state['next_step']['finfo'] = $finfo;
      $local_cache = $finfo->uri;
    }
    else {
      if ($form_state['values']['themes_url']) {
        $field = 'themes_url';
        $url = check_url($form_state['values']['themes_url']);
      }
      else {
        $field = 'default_themes';
        $url =  $form_state['values']['default_themes'];  
      }
      $local_cache = _jqueryui_theme_get_file_url($url, $destination);
      if (!$local_cache) {
        form_set_error($field, t('Unable to retrieve jQuery UI themes from @url.', array('@url' => $url)));
        return;
      }
    }
    
    try {
      $archiver = archiver_get_archiver($local_cache);
      if (!$archiver) {
        form_set_error($field, t('Cannot extract file uploaded, no archiver have been detected.'));
        return;
      }
    }
    catch (Exception $e) {
      form_set_error($field, $e->getMessage());
      return;
    }
    
    if ($themes = _jqueryui_theme_detect_themes($archiver->listContents(), $destination)) {
      $archiver->extract($destination);
      foreach ($themes as $key => $theme) {
        $themes[$key]['version'] = _jqueryui_theme_get_version($theme['directory'], $theme['files'], $theme['version']);
      }
      
      $form_state['next_step']['themes_infos'] = $themes;
      $form_state['next_step']['step_two'] = TRUE;
      $form_state['rebuild'] = TRUE;
      drupal_set_message(format_plural(sizeof($themes), '1 jQuery UI theme has been detected.', '@count jQuery UI themes have been detected.'));
    }
    else {
      drupal_set_message(t('No "all.css" files detected.'), 'error');
    }
  }
  else {
    //second step form, move jQuery UI themes selected
    $themes = $form_state['values']['themes'];
    $imported = 0;
    
    foreach ($themes as $theme) {
      if ($theme['checkbox']) {
        $params = _jqueryui_theme_get_params($theme['directory'], $theme['files']);
        if (_jqueryui_theme_move_tmp($theme)) {
          _jqueryui_theme_insert_theme($theme['machine_name'], $theme['human_name'], $theme['version'], $params);
          $imported++;
        }
      }
    }
          
    //clear temp dir
    if (isset($form_state['next_step']['finfo'])) {
      file_delete($form_state['next_step']['finfo'], TRUE);  
    }
    file_unmanaged_delete_recursive($form_state['next_step']['destination_source']);
    $form_state['redirect'] = 'admin/appearance/jqueryui-theme/settings';
    drupal_set_message(format_plural($imported, '1 jQuery UI theme has been imported', '@count jQuery UI themes have been imported'));
  }
}

/**
 * Implements hook_form_FORM_ID_alter()
 */
function jqueryui_theme_form_system_theme_settings_alter(&$form, &$form_state, $form_id) {
  $theme_name = preg_replace(array('/theme_/', '/_?settings/'), '', $form['var']['#value']);
  
  if (!empty($theme_name)) {
    $form['jqueryui_theme'] = array(
      '#type' => 'fieldset',
      '#title' => t('Attach one jqueryui theme import to !theme', array('!theme' => $theme_name)),
      '#weight' => 20,
    );
    
    $default_value = '';
    $options = _jqueryui_theme_get_options_themes($theme_name, $default_value);
    if (empty($options)) {
      $form['jqueryui_theme']['markup'] = array(
        '#type' => 'markup',
        '#markup' => t('There is no jQuery UI themes imported.'),
      );
    }
    else {
      $form['jqueryui_theme']['radios'] = array(
        '#type' => 'radios',
        '#options' => $options,
        '#default_value' => $default_value,
      );
      if ($default_value != 'none') {
        $form['jqueryui_theme']['example_link'] = array(
          '#type' => 'markup',
          '#markup' => l(t('See on @theme with @jquery_theme', array('@theme' => $theme_name, '@jquery_theme' => $default_value)),  
                           'admin/appearance/jqueryui-theme/example/' . $theme_name),
        );
      }
    }
    
    $form_state['storage']['default_radio'] = $default_value;
    $form['jqueryui_theme']['theme_name'] = array(
      '#type' => 'value',
      '#value' => $theme_name,
    );
    $form['#submit'][] = 'jqueryui_theme_attach_submit';
  }
}

/**
 * Form submission handler for jqueryui_theme_form_system_theme_settings_alter().
 * 
 * @see jqueryui_theme_form_system_theme_settings_alter()
 */
function jqueryui_theme_attach_submit($form, &$form_state) {
  $theme_name = $form_state['values']['theme_name'];
  $radio = $form_state['values']['radios'];
  $default = $form_state['storage']['default_radio'];
  
  if ($radio != $default) {
    db_delete('jqueryui_theme_assoc')
      ->condition('drupal_theme', $theme_name)
      ->execute();
    if ($radio != 'none') {
      db_insert('jqueryui_theme_assoc')
        ->fields(array(
          'drupal_theme' => $theme_name,
          'machine_name' => $radio,
        ))
        ->execute();
    }
    menu_rebuild();
  }
}

/**
 * Returns HTML for jquery UI themes example on Drupal themes selected
 * 
 * @param $theme_name
 *   A string containing a Drupal theme name for displaying
 *   a preview with a jQuery UI theme associated.
 */
function jqueryui_theme_example($theme_name) {
  $widgets = array('accordion', 'button', 'datepicker', 'dialog', 'autocomplete', 'progressbar', 'slider', 'tabs');
  foreach ($widgets as $widget) {
    drupal_add_library('system', 'ui.' . $widget);
  }
  drupal_add_js(drupal_get_path('module', 'jqueryui_theme') . '/jqueryui_theme_example.js');
  drupal_add_css(drupal_get_path('module', 'jqueryui_theme') . '/jqueryui_theme_example.css');
  return theme('jqueryui_theme_example');
}