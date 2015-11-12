<?php

/**
 * @file
 * Administrative page callbacks for the gotwo module.
 */

/**
 * Implements hook_settings().
 */
function gotwo_admin_settings_form($form, &$form_state) {
  $form['gotwo_filter_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global filter configuration'),
  );
  $form['gotwo_filter_settings']['gotwo_numeric'] = array(
    '#type' => 'checkbox',
    '#title' => t('Numerical url'),
    '#description' => t('Use numbers instead of a human readable URL. Use "go/1234" instead of "go/some/location".'),
    '#default_value' => variable_get('gotwo_numeric', FALSE),
  );

  $transliterate_dependencies = '<div class="admin-dependencies">';
  $transliterate_dependencies .= t('Depends on: !dependencies', array('!dependencies' => (module_exists('transliteration') ? t('@module (<span class="admin-enabled">enabled</span>)', array('@module' => 'Transliteration')) : t('@module (<span class="admin-disabled">disabled</span>)', array('@module' => 'Transliteration')))));
  $transliterate_dependencies .= '</div>';

  $form['gotwo_filter_settings']['gotwo_transliteration'] = array(
    '#type' => 'checkbox',
    '#title' => t('Transliterate URLs'),
    '#description' => t('Enables <a href="!url">transliteration</a> of URLs. Generally spoken, it takes Unicode text and tries to represent it in US-ASCII characters (universally displayable, unaccented characters) by attempting to transliterate the pronunciation expressed by the text in some other writing system to Roman letters.', array('!url' => 'http://drupal.org/project/transliteration')) . $transliterate_dependencies,
    '#default_value' => variable_get('gotwo_transliteration', TRUE),
    '#disabled' => (module_exists('transliteration') ? FALSE : TRUE),
  );
  $form['gotwo_filter_settings']['gotwo_separator'] = array(
    '#type' => 'select',
    '#title' => t('URL separator'),
    '#description' => t('All spaces in titles can be replaced with an underscore or hyphen. If set to "-", an title of "Lorem ipsum dolor" becomes an URL of "Lorem-ipsum-dolor".'),
    '#default_value' => variable_get('gotwo_separator', '-'),
    '#options' => array(
      '-' => t('Hyphen (-)'),
      '_' => t('Underscore (_)'),
    ),
  );
  $form['gotwo_filter_settings']['gotwo_max_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum length of target labels'),
    '#description' => t('Target labels are the parts after the "go/" part of the shown url. The absolute maximum is 128.'),
    '#default_value' => variable_get('gotwo_max_length', 128),
    '#element_validate' => array('_gotwo_element_max_length_validate'),
    '#size' => 10,
  );

  $form['disclaimer'] = array(
    '#type' => 'fieldset',
    '#title' => t('Disclaimer settings'),
  );
  $form['disclaimer']['gotwo_disclaimer_boolean'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add disclaimer'),
    '#description' => t('Check to add a disclaimer before redirecting to the Gotwo links.'),
    '#default_value' => variable_get('gotwo_disclaimer_boolean', FALSE),
  );
  $form['disclaimer']['gotwo_disclaimer_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Disclaimer page title'),
    '#description' => t('The page title that is shown on the disclaimer page.'),
    '#default_value' => variable_get('gotwo_disclaimer_title', 'Disclaimer'),
  );
  $form['disclaimer']['gotwo_disclaimer_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Disclaimer text'),
    '#description' => t('The disclaimer that will be presented to the user before they are redirected.<br /><strong>Variables available:</strong><br /> %url = URL to be redirected to <br />%seconds = Number of seconds until page redirects'),
    '#default_value' => variable_get('gotwo_disclaimer_text', ''),
  );
  $form['disclaimer']['gotwo_disclaimer_time'] = array(
    '#type' => 'select',
    '#title' => t('Disclaimer timeout'),
    '#options' => drupal_map_assoc(array(0, 1, 2, 3, 4, 5, 10, 15, 30, 45, 60)),
    '#description' => t('Number of seconds until the page will be redirected to the requested URL, 0 means no refresh.'),
    '#default_value' => variable_get('gotwo_disclaimer_time', 0),
    '#field_suffix' => t('seconds'),
  );

  return system_settings_form($form);
}

/**
 * Shows the list of go redirects.
 */
function _gotwo_list() {
  $access = user_access('edit gotwo redirects');

  $header = array(
    'gid' => array('data' => t('ID'), 'field' => 'gid'),
    'src' => array('data' => t('Label'), 'field' => 'src'),
    'dst' => array('data' => t('Destination'), 'field' => 'dst'),
    'cnt' => array('data' => t('Counter'), 'field' => 'cnt', 'sort' => 'desc'),
  );

  if ($access) {
    $header[] = array('data' => t('Operations'), 'colspan' => 2);
  }

  $query = db_select('gotwo', 'g')->extend('PagerDefault')->extend('TableSort');
  $result = $query
    ->fields('g')
    ->limit(50)
    ->orderByHeader($header)
    ->execute();

  $rows = array();
  foreach ($result as $go) {
    $rows[] = array(
      'data' => array(
        $go->gid,
        check_plain($go->src),
        l($go->dst, $go->dst),
        $go->cnt,
        $access ? l(t('Reset counter'), 'admin/structure/gotwo/reset/' . $go->gid) : '&nbsp;',
        $access ? l(t('Delete'), 'admin/structure/gotwo/delete/' . $go->gid) : '&nbsp;',
      ),
    );
  }

  $build['gotwo_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No redirects have been found.'),
  );
  $build['gotwo_pager'] = array('#theme' => 'pager');

  return $build;
}

/**
 * Create confirmation form for click counter item reset.
 */
function gotwo_reset_form($form, &$form_state, $edit) {
  $form['gid'] = array(
    '#type' => 'value',
    '#value' => $edit->gid,
  );

  return confirm_form(
    $form,
    t('Reset the counter for %label', array('%label' => $edit->src)),
    'admin/structure/gotwo',
    '<p>' . t('Are you sure you want to reset the click counter for %label? This action cannot be undone.', array('%label' => $edit->src)) . '</p>',
    t('Reset'),
    t('Cancel')
  );
}

/**
 * Form submit handler for click counter item reset.
 */
function gotwo_reset_form_submit($form, &$form_state) {
  db_update('gotwo')
    ->fields(array(
      'cnt' => 0,
    ))
    ->condition('gid', $form_state['values']['gid'])
    ->execute();

  $form_state['redirect'] = 'admin/structure/gotwo';
}

/**
 * Build delete form.
 */
function gotwo_delete_form($form, &$form_state, $edit) {
  $form['gid'] = array(
    '#type' => 'value',
    '#value' => $edit->gid,
  );

  return confirm_form(
    $form,
    t('Delete go redirect %label', array('%label' => $edit->src)),
    'admin/structure/gotwo',
    '<p>' . t('Are you sure you want to delete the go redirect %label? This action cannot be undone. The link will become broken, a new one might be automatically created when a node linking to it is edited.', array('%label' => $edit->src)) . '</p>',
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Delete redirect form has been submitted.
 */
function gotwo_delete_form_submit($form, &$form_state) {
  db_delete('gotwo')
    ->condition('gid', $form_state['values']['gid'])
    ->execute();

  $form_state['redirect'] = 'admin/structure/gotwo';
}

/**
 * Manually add a go redirect.
 */
function gotwo_add_form($form, &$form_state) {

  $form['src'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#description' => t('The label used in the go url, this will automatically be made suitable.'),
    '#required' => TRUE,
  );
  $form['dst'] = array(
    '#type' => 'textfield',
    '#title' => t('Destination'),
    '#description' => t('The target url. Can be a relative drupal url or an absolute url.'),
    '#required' => TRUE,
    '#maxlength' => 255,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );

  return $form;
}

/**
 * Go redirect has been submitted.
 */
function gotwo_add_form_submit($form, &$form_state) {
  $res = _gotwo_get($form_state['values']['dst'], $form_state['values']['src']);
  $form_state['redirect'] = 'admin/structure/gotwo';
}
