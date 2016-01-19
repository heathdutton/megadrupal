<?php

/**
 * @file
 * Editor integration functions for ueditor.
 */

/**
 * Plugin implementation of hook_editor().
 */
function ueditor_ueditor_editor() {
  $editor['ueditor'] = array(
    'title' => 'Ueditor',
    'vendor url' => 'http://ueditor.baidu.com/website/index.html',
    'download url' => 'http://ueditor.baidu.com/website/download.html',
    'libraries' => array(
      '' => array(
        'title' => 'Source',
        'files' => array('ueditor.all.js'),
      ),
    ),
    'version callback' => 'ueditor_version',
    'settings callback' => 'ueditor_settings',
    'settings form callback' => 'ueditor_settings_form',
    'versions' => array(
      '1.4.3' => array(
        'js files' => array('ueditor.js'),
      ),
    ),
  );
  return $editor;
}

/**
 * Detect editor version.
 *
 * @param array $editor
 *   An array containing editor properties as returned from hook_editor().
 *
 * @return int
 *   The installed editor version.
 */
function ueditor_version($editor) {
  $script = $editor['library path'] . '/ueditor.all.js';
  if (!file_exists($script)) {
    return;
  }
  $script = fopen($script, 'r');
  $max_lines = 50;
  while ($max_lines && $line = fgets($script, 500)) {    
    if (preg_match('/^UE.*?"([\d\.]+)"/', $line, $version)) {
      fclose($script);
      return $version[1];
    }
    $max_lines--;
  }
  fclose($script);
}

/**
 * Enhances the editor profile settings form for UEditor.
 */
function ueditor_settings_form(&$form, &$form_state) {
  global $base_path;
  //load config js and css.
  if(module_exists('libraries')){
    $lib_path = libraries_get_path('ueditor');
    drupal_add_css($lib_path . '/themes/default/css/ueditor.css');
  }

  drupal_add_js(array('ueditor' => array('ueditor_enable_formula_editor' => variable_get('ueditor_enable_formula_editor',0))), 'setting');

  drupal_add_js(drupal_get_path('module', 'ueditor') . '/editors/js/ueditor.toolbars.js');

  // If enable kityformula support, add the icon css.
  if(variable_get('ueditor_enable_formula_editor',0)){
    drupal_add_css('.edui-default .edui-toolbar #id_kityformula .edui-icon {background: url("'. $base_path . $lib_path . '/kityformula-plugin/kf-icon.png") !important;}', 'inline');
  }

  if(!empty($form_state['wysiwyg_profile']->settings['toolbars'])){
    $settings = $form_state['wysiwyg_profile']->settings;
  }else{
    $settings = ueditor_config_default($form_state['wysiwyg_profile']->settings);
    $settings['toolbars'] = _ueditor_toolbars($settings['toolbars']);
  }

  $form['basic']['zindex'] = array(
    '#type' => 'textfield',
    '#title' => t('Editor zindex'),
    '#description' => t('The official website of the default zindex 900,<br />
      and Drupal overlay module ( #overlay= page ) conflict,
      so default change from 900 to 90 or you can customize.'),
    '#default_value' => $settings['zindex'],
    '#size' => 5,
    '#maxlength' => 4,
    '#required' => TRUE,
  );
  $form['basic']['initialFrameHeight'] = array(
    '#type' => 'textfield',
    '#title' => t('Editor Height'),
    '#description' => t('The default height is 320, you can change it.'),
    '#default_value' => $settings['initialFrameHeight'],
    '#size' => 5,
    '#maxlength' => 4,
    '#required' => TRUE,
  );
  $form['basic']['initial_content'] = array(
    '#type' => 'textfield',
    '#title' => t('Editor initial content'),
    '#description' => t('Editor initial content, after editor loading in the textarea.'),
    '#default_value' => $settings['initial_content'],
    '#maxlength' => 255,
  );
  $form['basic']['allowdivtop'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow Div Convert to P'),
    '#default_value' => $settings['allowdivtop'],
    '#description' => t('If enable, the Div tags converted to P tag.'),
  );
  $form['basic']['auto_height'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto Height'),
    '#default_value' => $settings['auto_height'],
    '#description' => t('If enable, the editor will auto height.'),
  );
  $form['basic']['auto_float'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto Float'),
    '#default_value' => $settings['auto_float'],
    '#description' => t('If enable, the editor will auto float.'),
  );
  $form['basic']['show_elementpath'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show element path'),
    '#default_value' => $settings['show_elementpath'],
    '#description' => t('If enable, It will show the element path under the editor.'),
  );
  $form['basic']['show_wordcount'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show word count'),
    '#default_value' => $settings['show_wordcount'],
    '#description' => t('If enable, It will show the word count under the editor.'),
  );
  $form['basic']['appearance'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Appearance'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,		
    '#attributes'			=> array('class' => array('edui-default')),
  );
  $form['basic']['appearance']['toolbars'] = array(
    '#type'           => 'textarea',
    '#title'          => t('Toolbars'),
    '#default_value'  => isset($settings['toolbars']) ? $settings['toolbars'] : '',
    '#description'    => t('Enter a comma separated list of toolbars.'),
  );

  drupal_add_js('Drupal.ueditor.admin_profile_form();', array('scope' => 'footer', 'type' => 'inline'));

  // config the upload path.
  $form['basic']['uploadpath'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Custom Upload Path'),
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
  );
  $form['basic']['uploadpath']['path_help'] = array(
    '#markup' => implode('<br>', array(
      '%b' => '<code>%b</code> - the base URL path of the Drupal installation (<code>'._ueditor_realpath('%b').'</code>)',
      '%m' => '<code>%m</code> - path where the UEditor module is stored (<code>'._ueditor_realpath('%m').'</code>)',
      '%l' => '<code>%l</code> - path to the libraries directory (<code>'._ueditor_realpath('%l').'</code>)',
      '%f' => '<code>%f</code> - the Drupal file system path where the files are stored (<code>'._ueditor_realpath('%f').'</code>)',
      '%d' => '<code>%d</code> - the server path to the document root (<code>'._ueditor_realpath('%d').'</code>)',
      '%u' => '<code>%u</code> - User ID (<code>'._ueditor_realpath('%u').'</code>)',
      '{yyyy}' => '<code>{yyyy}</code> - <a href="http://www.php.net/manual/en/function.date.php">The php date format</a>',
      '{mm}' => '<code>{mm}</code> - <a href="http://www.php.net/manual/en/function.date.php">The php date format</a>',
      '{dd}' => '<code>{dd}</code> - <a href="http://www.php.net/manual/en/function.date.php">The php date format</a>',
      '{time}' => '<code>{time}</code> - A timestamp',
      '{rand:6}' => '<code>{rand:6}</code> - A random number',
      '{transliteration_filename}' => '<code>{transliteration_filename}</code> - you need install <a href="https://www.drupal.org/project/transliteration">Transliteration</a> module',
      '<br>',
    )),
    '#prefix' => '<div class="region region-help"><div class="block block-system"><div class="content">',
    '#suffix' => '</div></div></div>',
  );

  $imagePathFormat = !empty($settings['imagePathFormat']) ? $settings['imagePathFormat'] : '';
  $form['basic']['uploadpath']['imagePathFormat'] = array(
    '#type'           => 'textfield',
    '#title'          => t('imagePathFormat'),
    '#default_value'  => $imagePathFormat,
    '#description'    => 'Current path:<code>'._ueditor_realpath($imagePathFormat).'</code>',
  );
  $scrawlPathFormat = !empty($settings['scrawlPathFormat']) ? $settings['scrawlPathFormat'] : '';
  $form['basic']['uploadpath']['scrawlPathFormat'] = array(
    '#type'           => 'textfield',
    '#title'          => t('scrawlPathFormat'),
    '#default_value'  => $scrawlPathFormat,
    '#description'    => 'Current path:<code>'._ueditor_realpath($scrawlPathFormat).'</code>',
  );
  $filePathFormat = !empty($settings['filePathFormat']) ? $settings['filePathFormat'] : '';
  $form['basic']['uploadpath']['filePathFormat'] = array(
    '#type'           => 'textfield',
    '#title'          => t('filePathFormat'),
    '#default_value'  => $filePathFormat,
    '#description'    => 'Current path:<code>'._ueditor_realpath($filePathFormat).'</code>',
  );
  $fileManagerListPath = !empty($settings['fileManagerListPath']) ? $settings['fileManagerListPath'] : '';
  $form['basic']['uploadpath']['fileManagerListPath'] = array(
    '#type'           => 'textfield',
    '#title'          => t('fileManagerListPath'),
    '#default_value'  => $fileManagerListPath,
    '#description'    => 'Current path:<code>'._ueditor_realpath($fileManagerListPath).'</code>',
  );
  $catcherPathFormat = !empty($settings['catcherPathFormat']) ? $settings['catcherPathFormat'] : '';
  $form['basic']['uploadpath']['catcherPathFormat'] = array(
    '#type'           => 'textfield',
    '#title'          => t('catcherPathFormat'),
    '#default_value'  => $catcherPathFormat,
    '#description'    => 'Current path:<code>'._ueditor_realpath($catcherPathFormat).'</code>',
  );
  $imageManagerListPath = !empty($settings['imageManagerListPath']) ? $settings['imageManagerListPath'] : '';
  $form['basic']['uploadpath']['imageManagerListPath'] = array(
    '#type'           => 'textfield',
    '#title'          => t('imageManagerListPath'),
    '#default_value'  => $imageManagerListPath,
    '#description'    => 'Current path:<code>'._ueditor_realpath($imageManagerListPath).'</code>',
  );
  $snapscreenPathFormat = !empty($settings['snapscreenPathFormat']) ? $settings['snapscreenPathFormat'] : '';
  $form['basic']['uploadpath']['snapscreenPathFormat'] = array(
    '#type'           => 'textfield',
    '#title'          => t('snapscreenPathFormat'),
    '#default_value'  => $snapscreenPathFormat,
    '#description'    => 'Current path:<code>'._ueditor_realpath($snapscreenPathFormat).'</code>',
  );
  $videoPathFormat = !empty($settings['videoPathFormat']) ? $settings['videoPathFormat'] : '';
  $form['basic']['uploadpath']['videoPathFormat'] = array(
    '#type'           => 'textfield',
    '#title'          => t('videoPathFormat'),
    '#default_value'  => $videoPathFormat,
    '#description'    => 'Current path:<code>'._ueditor_realpath($videoPathFormat).'</code>',
  );
  $form['basic']['uploadpath']['#element_validate'][] = 'ueditor_settings_form_validate_uploadpath';
}

/**
 * Deal with settings form submit.
 */
function ueditor_settings_form_validate_uploadpath($element, &$form_state) {
  $uploadpath = array();
  if(!empty($element['imagePathFormat']['#value'])){
    $uploadpath['imagePathFormat'] = $element['imagePathFormat']['#value'];
  }
  if(!empty($element['scrawlPathFormat']['#value'])){
    $uploadpath['scrawlPathFormat'] = $element['scrawlPathFormat']['#value'];
  }
  if(!empty($element['filePathFormat']['#value'])){
    $uploadpath['filePathFormat'] = $element['filePathFormat']['#value'];
  }
  if(!empty($element['fileManagerListPath']['#value'])){
    $uploadpath['fileManagerListPath'] = $element['fileManagerListPath']['#value'];
  }
  if(!empty($element['catcherPathFormat']['#value'])){
    $uploadpath['catcherPathFormat'] = $element['catcherPathFormat']['#value'];
  }
  if(!empty($element['imageManagerListPath']['#value'])){
    $uploadpath['imageManagerListPath'] = $element['imageManagerListPath']['#value'];
  }
  if(!empty($element['snapscreenPathFormat']['#value'])){
    $uploadpath['snapscreenPathFormat'] = $element['snapscreenPathFormat']['#value'];
  }
  if(!empty($element['videoPathFormat']['#value'])){
    $uploadpath['videoPathFormat'] = $element['videoPathFormat']['#value'];
  }

  if(!empty($uploadpath)){
    variable_set('ueditor_uploadpath_config', $uploadpath);
  }
}

/**
 * Return runtime editor settings for a given wysiwyg profile.
 *
 * @param array $editor
 *   A processed hook_editor() array of editor properties.
 * @param array $config
 *   An array containing wysiwyg editor profile settings.
 * @param string $theme
 *   The name of a theme/GUI/skin to use.
 *
 * @return array
 *   A settings array to be populated in
 *   Drupal.settings.wysiwyg.configs.{editor}
 */
function ueditor_settings($editor, $config, $theme) {
  // Settings.
  if(!isset($config['allowdivtop']) || !isset($config['auto_height']) || !isset($config['auto_float']) || !isset($config['initialFrameHeight']) || !isset($config['show_elementpath']) || !isset($config['show_wordcount'])){
    $settings = ueditor_config_default();
  }else{
    $settings = $config;
    if($config['allowdivtop'] == 1){
      $settings['allowdivtop'] = true;
    }else{
      $settings['allowdivtop'] = false;
    }

    if($config['auto_height'] == 1){
      $settings['auto_height'] = true;
    }else{
      $settings['auto_height'] = false;
    }

    if($config['auto_float'] == 1){
      $settings['auto_float'] = true;
    }else{
      $settings['auto_float'] = false;
    }

    if($config['show_elementpath'] == 1){
      $settings['show_elementpath'] = true;
    }else{
      $settings['show_elementpath'] = false;
    }

    if($config['show_wordcount'] == 1){
      $settings['show_wordcount'] = true;
    }else{
      $settings['show_wordcount'] = false;
    }
  }
  
  global $base_url;
  $settings['editorPath'] = $base_url.'/'.$editor['library path'] . '/';

  // Change the chinese language code.
  if(isset($settings['language']) && $settings['language'] == 'zh-hans'){
    $settings['language'] = 'zh-cn';
  }

  $settings['toolbars'] = (!empty($config['toolbars']) ? array(explode(',',$config['toolbars'])) : $settings['toolbars']);
  $settings['serverUrl'] = base_path().'?q=ueditor/controller';
  if(variable_get('ueditor_enable_paging', 0) == 1 && module_exists('paging')){
    $settings['pageBreakTag'] = htmlentities(variable_get('paging_separator', '<!--pagebreak-->'));
  }

  // Load config js.
  drupal_add_js($editor['library path'] . '/ueditor.config.js', array('weight' => -1, 'preprocess' => FALSE));  

  // Theme.
  // Ueditor temporarily does not support multiple themes,
  // Can customize CSS overwrite.
  drupal_add_css($editor['library path'] . '/themes/default/css/ueditor.css');
  drupal_add_css(drupal_get_path('module', 'ueditor') . '/css/ueditor-compatible.css');
  drupal_add_css($editor['library path'] . '/themes/default/iframe.css');

  // If enable kityformula support, load kityformula js.
  if(variable_get('ueditor_enable_formula_editor',0)){
    drupal_add_js($editor['library path'] . '/kityformula-plugin/addKityFormulaDialog.js', array('weight' => 5, 'preprocess' => FALSE));
    drupal_add_js($editor['library path'] . '/kityformula-plugin/getKfContent.js', array('weight' => 5, 'preprocess' => FALSE));
    drupal_add_js($editor['library path'] . '/kityformula-plugin/defaultFilterFix.js', array('weight' => 5, 'preprocess' => FALSE));
  }

  return $settings;
}

/**
 * Convert toolbars array of string.
 */
function _ueditor_toolbars($toolbars) {
  if(!empty($toolbars[0])) {
    $t = $toolbars[0];
    return implode(',', $t);
  }
  return '';
}
