<?php

/**
 * BAWStats drupal module
 *
 * BAWStats is a drupal module written in php for integration of
 * the BetterAWStats presentation and processing of AWStats data into drupal.
 * Copyright (C) 2008  Andrew Gillies (anaru@equivocation.org)
 *
 * For more information about BetterAWStats see:
 * http://betterawstats.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or, at
 * your option, any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

include_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'bawstats') . '/includes/bawstats.config.inc';

/**
 * Menu callback; presents the bawstats admin settings page.
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function bawstats_admin_settings($form, &$form_state) {
  include_once DRUPAL_ROOT . '/' . './includes/install.inc';
  global $BAW_CONF, $BAW_LIB;
  $form = array();

  // build the form for the settings

  $form['bawstats_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('AWStats configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => -1,
  );
  $bawstats_data['path'] = variable_get('bawstats_data', '/var/lib/awstats');
  $bawstats_data['installed'] = is_dir($bawstats_data['path']);

  $bawstats_lib['path'] = variable_get('bawstats_lib', '/usr/share/awstats/lib');
  $bawstats_lib['installed'] = is_dir($bawstats_lib['path']);

  $bawstats_lang['path'] = variable_get('bawstats_lang', '/usr/share/awstats/lang');
  $bawstats_lang['installed'] = is_dir($bawstats_lang['path']);
  $bawstats_icon = array();
  $bawstats_icon['path'] = libraries_get_path('bawstats') . '/icon';
  $bawstats_icon['installed'] = is_dir($bawstats_icon['path']);
  $status = array();
  $status['requirements']['bawstats_data'] = array(
    'title' => t('AWStats data directory'),
    'severity' => ($bawstats_data['installed'] ? REQUIREMENT_OK : REQUIREMENT_ERROR),
    'value' => ($bawstats_data['installed'] ? t('The directory <code>@dir</code> exists', array('@dir' => $bawstats_data['path'])) : t('Set the AWStats data directory below.'))
  );

  $status['requirements']['bawstats_lib'] = array(
    'title' => t('AWStats library directory'),
    'severity' => ($bawstats_lib['installed'] ? REQUIREMENT_OK : REQUIREMENT_ERROR),
    'value' => ($bawstats_lib['installed'] ? t('The directory <code>@dir</code> exists.', array('@dir' => $bawstats_lib['path'])) : t('Set the AWStats library directory below.'))
  );
  $status['requirements']['bawstats_lang'] = array(
    'title' => t('AWStats language directory'),
    'severity' => ($bawstats_lang['installed'] ? REQUIREMENT_OK : REQUIREMENT_ERROR),
    'value' => ($bawstats_lang['installed'] ? t('The directory <code>@dir</code> exists.', array('@dir' => $bawstats_lang['path'])) : t('Set the AWStats language directory below.'))
  );
  $status['requirements']['installed'] = array(
    'severity' => ($bawstats_icon['installed'] ? REQUIREMENT_OK : REQUIREMENT_ERROR),
    'value' => $bawstats_icon['installed'] ?  t('The directory <code>@dir</code> exists.', array('@dir' => $bawstats_icon['path'])) : t('The AWStats icon directory is not in place. In a standard Debian/Ubuntu AWStats installation copy or linke the directory  <code>/usr/share/awstats/icon</code> to <code>sites/all/libraries/bawstats/icon</code>. Otherwise copy or link the directory <code>wwwroot/icon</code> under the AWStats tree to the same location.'),
    'title' => t('AWStats icon directory'),
  );
  $status['requirements']['format_number'] = array(
    'title' => t('Format Number API'),
    'severity' => (module_exists('format_number') ? REQUIREMENT_OK : REQUIREMENT_INFO),
    'value' => module_exists('format_number') ?  t('Format Number API is installed. <a href="@url">Configure site wide number format settings</a>.', array('@url' => url('admin/config/regional/format_number'))) : t('Install <a href="@url">Format Number API</a> to configure presentation of numbers in statistics.', array('@url' => url('https://www.drupal.org/project/format_number'))),
  );

  $form['bawstats_config']['requirements'] = array(
    '#markup' =>  theme('status_report', $status)
  );
  $form['bawstats_config']['bawstats_data'] = array(
    '#type' => 'textfield',
    '#size' => 40,
    '#maxlength' => 80,
    '#title' => t('AWStats data directory'),
    '#default_value' =>  ($bawstats_data['installed'] ? $bawstats_data['path'] : ''),
    '#description' => t('Set this value to the directory where AWStats saves its database files into. In a standard Debian/Ubuntu AWStats installation this should be <code>/var/lib/awstats</code> or one of its subdirectories. In general it is given by the AWStats <code>DirData</code> configuration variable.'),
    '#weight' => 1,
  );
  $form['bawstats_config']['bawstats_lib'] = array(
    '#type' => 'textfield',
    '#title' => t('AWStats library directory'),
    '#size' => 40,
    '#maxlength' => 80,
    '#default_value' =>  ($bawstats_lib['installed'] ? $bawstats_lib['path'] : ''),
    '#description' => t('Set this value to the directory where the AWStats library files are located. In a standard Debian/Ubuntu AWStats installation this should be <code>/usr/share/awstats/lib</code>. Otherwise it is the <code>lib</code> directory under the AWStats tree.'),
    '#weight' => 2,
  );
  $form['bawstats_config']['bawstats_lang'] = array(
    '#type' => 'textfield',
    '#title' => t('AWStats language directory'),
    '#size' => 40,
    '#maxlength' => 80,
    '#default_value' =>  ($bawstats_lang['installed'] ? $bawstats_lang['path'] : ''),
    '#description' => t('Set this value to the directory where the AWStats language files are located. In a standard Debian/Ubuntu AWStats installation this should be <code>/usr/share/awstats/lang</code>. Otherwise it is the <code>lang</code> directory under the AWStats tree.'),
    '#weight' => 3,
  );
  $form['bawstats_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('BAWStats general settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 0,
  );
  $form['bawstats_settings']['bawstats_page_view'] = array(
    '#type' => 'checkbox',
    '#title' => t('Split page view'),
    '#default_value' => variable_get('bawstats_page_view', 0),
    '#description' => t('By default statistics for a single month are displayed on a single large page. Selecting this option splits the statistics across multiple pages, which are accessed via tabs.'),
    '#weight' => 1,
  );
  // turn off display control if we are not yet configured
  //if ($configured==0) $form['bawstats_settings']['bawstats_page_view']['#disabled']=TRUE;

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['buttons']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
  );

  if (!empty($_POST) && form_get_errors()) {
    drupal_set_message(t('The settings have not been saved because of the errors.'), 'error');
  }

  $form['#submit'][] = 'bawstats_admin_settings_submit';
  $form['#theme'] = 'system_settings_form';

  return $form;
}

function _bawstats_remove_slash($path) {
  if (substr($path, strlen($path) -1, 1) == '/') {
    return substr($path, 0, strlen($path) -1);
  }
  return $path;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function bawstats_admin_settings_validate($form, &$form_state) {
  global $BAW_CONF, $BAW_LIB, $BAW_CONF_DIS_DEF;
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  if ($op != t('Reset to defaults')) {
    // check the key paths

    $settingOK = TRUE;
    $data_dir = _bawstats_remove_slash($form_state['values']['bawstats_data']);

    if (is_dir($data_dir)) {
      if (!is_readable($data_dir)) {
        form_set_error('bawstats_data', t('AWStats data directory "') . $data_dir . t('" is not accessible.'));
        $settingOK = FALSE;
      }
    }
    else {
      form_set_error('bawstats_data', t('AWStats data location "') . $data_dir . t('" is not a directory.'));
      $settingOK = FALSE;
    }

    if (!$settingOK) {
      variable_del('bawstats_data'); // this is evil doing this in validate, but ensures clean fall-over
    }

    $settingOK = TRUE;
    $lib_dir = _bawstats_remove_slash($form_state['values']['bawstats_lib']);

    if (is_dir($lib_dir)) {
      if (is_readable($lib_dir)) {
        // directory is there and readable, so check it contains the AWStats lib stuff
        if (!is_readable($lib_dir . '/domains.pm')) {
          form_set_error('bawstats_lib', t('AWStats library directory "') . $lib_dir . t('" does not contain the necessary library files?'));
          $settingOK = FALSE;
        }
      }
      else {
        form_set_error('bawstats_lib', t('AWStats library directory "') . $lib_dir . t('" is not accessible.'));
        $settingOK = FALSE;
      }
    }
    else {
      form_set_error('bawstats_lib', t('AWStats library location "') . $lib_dir . t('" is not a directory.'));
      $settingOK = FALSE;
    }

    if (!$settingOK) {
      variable_del('bawstats_lib'); // this is evil doing this in validate, but ensures clean fall-over
    }

    $settingOK = TRUE;
    $lang_dir = _bawstats_remove_slash($form_state['values']['bawstats_lang']);

    if (is_dir($lang_dir)) {
      if (is_readable($lang_dir)) {
        // directory is there and readable, so check it contains the AWStats lib stuff
        if (!is_readable($lang_dir . '/awstats-en.txt')) {
          form_set_error('bawstats_lang', t('AWStats library directory "') . $lang_dir . t('" does not contain the necessary language files?'));
          $settingOK = FALSE;
        }
      }
      else {
        form_set_error('bawstats_lang', t('AWStats language directory "') . $lang_dir . t('" is not accessible.'));
        $settingOK = false;
      }
    }
    else {
      form_set_error('bawstats_lang', t('AWStats language location "') . $lang_dir . t('" is not a directory.'));
      $settingOK = FALSE;
    }

    if (!$settingOK) {
      variable_del('bawstats_lang'); // this is evil doing this in validate, but ensures clean fall-over
    }
  }
}


/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function bawstats_admin_settings_submit($form, &$form_state) {
  global $BAW_CONF, $BAW_LIB, $BAW_CONF_DIS_DEF;
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  // Exclude unnecessary elements.
  unset($form_state['values']['submit'], $form_state['values']['reset'], $form_state['values']['form_id'], $form_state['values']['op'], $form_state['values']['form_token']);

  foreach ($form_state['values'] as $key => $value) {
    if ($op == t('Reset to defaults')) {
      variable_del($key);
    }
    else {
      if (is_array($value) && isset($form_state['values']['array_filter'])) {
        $value = array_keys(array_filter($value));
      }
      // Test the key is correct:

      if ($key == 'bawstats_data') {
        // assume validation has taken care of testing for directory/readable
        $value = _bawstats_remove_slash($value);
        // do some further checking
        $count = count(glob($value . "/*.txt"));
        if ($count == 0) {
          drupal_set_message(t('Warning, the AWStats data directory "') . $value . t('" does not contain any text data files'), 'warning');
        }
      }

      if ($key == 'bawstats_lib') {
        // assume validation has taken care of testing for directory/readable
        $value = _bawstats_remove_slash($value);
      }

      if ($key == 'bawstats_lang') {
        // assume validation has taken care of testing for directory/readable
        $value = _bawstats_remove_slash($value);
      }

      if ($key == 'bawstats_icon') {
        // assume validation has taken care of testing for directory/readable
        $value = _bawstats_remove_slash($value);
        $testvalue = substr($value, 1);
        // do some further checking
        if (is_dir($testvalue)) {
          if (is_readable($testvalue)) {
            // directory is there and readable, so check it contains the AWStats lib stuff
            if (!is_readable($testvalue . '/browser')) {
              drupal_set_message(t('Warning, the AWStats icon path "') . $value . t('" does not appear to contain icon directories.'), 'warning');
            }
          }
          else {
            drupal_set_message(t('Warning, the AWStats icon path "') . $value . t('" is not accessible.'), 'warning');
          }
        }
        else {
          drupal_set_message(t('Warning, the AWStats icon path "') . $value . t('" is not a directory.'), 'warning');
        }
      }

      // set the actual variable
      variable_set($key, $value);
    }
  }

  if ($op == t('Reset to defaults')) {
    drupal_set_message(t('The configuration options have been reset to their default values.'));
  }
  else {
    drupal_set_message(t('The configuration options have been saved.'));
  }

  menu_rebuild();
}
