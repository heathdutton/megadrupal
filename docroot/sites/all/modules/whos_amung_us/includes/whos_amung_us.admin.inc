<?php

/**
 * @file
 * Administration pages provided by Whos.amung.us module.
 *
 * @author Francisco J. Cruz Romanos <grisendo@gmail.com>
 */

/**
 * Menu callback for admin/structure/block/add-whos-amung-us-block.
 */
function whos_amung_us_configuration_new_form() {

  $form_state = array(
    'values' => array(),
  );
  return whos_amung_us_configuration_form(array(), $form_state, 'newblock');

}

/**
 * Menu callback for admin/config/services/whos-amung-us.
 */
function whos_amung_us_configuration_config_form() {

  $form_state = array(
    'values' => array(),
  );
  return whos_amung_us_configuration_form(array(), $form_state, 'general');

}

/**
 * Returns the configuration form for a whos.amung.us block/widget.
 *
 * @param array $form
 *   Nested array of form elements that comprise the form.
 * @param array $form_state
 *   An associated array of configuration options should be present in the
 *   'values' key. If none are given, default configuration is assumed.
 * @param string $id
 *   The 'flavour' of the form, in order to identify which module is using
 *   it (views, ctools...) or which form it is (block form, config form...).
 * @param string $sublevel
 *   The base #tree level where the form is, so the JS in Form API #states
 *   is well declared using the correct triggers.
 *
 * @return array
 *   The form in Form API format.
 */
function whos_amung_us_configuration_form($form, $form_state, $id = 'general', $sublevel = NULL) {

  $form = array();

  if ($id != 'general') {
    if (isset($form_state['values'])) {
      $config = $form_state['values'];
    }
  }
  else {
    $config = variable_get('whos_amung_us_configuration', whos_amung_us_default_form_values());
  }
  if (isset($config) && $config && is_array($config)) {
    $config += whos_amung_us_default_form_values();
  }
  else {
    $config = whos_amung_us_default_form_values();
  }

  $widgets = array(
    'general' => t('General'),
    'classic' => t('Classic'),
    'colored' => t('Colored'),
    'small' => t('Small'),
    'tab' => t('Tab'),
    'map' => t('Map'),
  );

  $default = $config['widget'];
  if ($id == 'general') {
    unset($widgets['general']);
    $form['apikey'] = array(
      '#type' => 'textfield',
      '#title' => t('Api Key'),
      '#max_length' => 64,
      '#required' => TRUE,
      '#default_value' => variable_get('whos_amung_us_key', ''),
    );
  }

  $form['#attached'] = array(
    'library' => array(
      array('system', 'farbtastic'),
    ),
    'js' => array(
      drupal_get_path('module', 'whos_amung_us') . '/js/whos_amung_us.js',
    ),
  );

  if (($id == 'newblock') || ($id == 'block')) {

    $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#max_length' => 64,
      '#default_value' => $config['name'],
    );

  }

  if ($id == 'newblock') {

    $form['mname'] = array(
      '#type' => 'machine_name',
      '#title' => t('Machine name'),
      '#required' => TRUE,
      '#default_value' => $config['mname'],
      '#max_length' => 64,
      '#machine_name' => array(
        'exists' => 'whos_amung_us_name_exists',
        'source' => array('name'),
      ),
    );
  }

  $form['widget'] = array(
    '#type' => 'select',
    '#title' => t('Widget type'),
    '#required' => TRUE,
    '#multiple' => FALSE,
    '#default_value' => $default,
    '#options' => $widgets,
  );

  if ($sublevel) {
    $jname = ':input[name="' . $sublevel . '[widget]"]';
  }
  else {
    $jname = ':input[name="widget"]';
  }

  $form['background'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color'),
    '#default_value' => $config['background'],
    '#attributes' => array(
      'class' => array('wau-has-color-picker'),
    ),
    '#states' => array(
      'visible' => array(
        $jname => array('value' => 'colored'),
      ),
      'required' => array(
        $jname => array('value' => 'colored'),
      ),
    ),
  );

  $form['foreground'] = array(
    '#type' => 'textfield',
    '#title' => t('Text color'),
    '#default_value' => $config['foreground'],
    '#attributes' => array(
      'class' => array('wau-has-color-picker'),
    ),
    '#states' => array(
      'visible' => array(
        $jname => array('value' => 'colored'),
      ),
      'required' => array(
        $jname => array('value' => 'colored'),
      ),
    ),
  );

  $form['position'] = array(
    '#type' => 'radios',
    '#title' => t('Position'),
    '#default_value' => $config['position'],
    '#options' => array(
      'left-upper' => t('Left upper'),
      'left-middle' => t('Left middle'),
      'left-lower' => t('Left lower'),
      'bottom-left' => t('Bottom left'),
      'bottom-center' => t('Bottom center'),
      'bottom-right' => t('Bottom right'),
      'right-upper' => t('Right upper'),
      'right-middle' => t('Right middle'),
      'right-lower' => t('Right lower'),
    ),
    '#states' => array(
      'visible' => array(
        $jname => array('value' => 'tab'),
      ),
      'required' => array(
        $jname => array('value' => 'tab'),
      ),
    ),
  );

  $form['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => $config['width'],
    '#states' => array(
      'visible' => array(
        $jname => array('value' => 'map'),
      ),
      'required' => array(
        $jname => array('value' => 'map'),
      ),
    ),
  );

  $form['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => $config['height'],
    '#states' => array(
      'visible' => array(
        $jname => array('value' => 'map'),
      ),
      'required' => array(
        $jname => array('value' => 'map'),
      ),
    ),
  );

  $form['map'] = array(
    '#type' => 'radios',
    '#title' => t('Map'),
    '#default_value' => $config['map'],
    '#options' => array(
      'natural' => t('Natural'),
      'green' => t('Green'),
      'neosat' => t('Neosat'),
      'classic' => t('Classic'),
      'night' => t('Night'),
      'red' => t('Red'),
      'cart' => t('Cart'),
      'textbook' => t('Textbook'),
      'girly' => t('Girly'),
    ),
    '#states' => array(
      'visible' => array(
        $jname => array('value' => 'map'),
      ),
      'required' => array(
        $jname => array('value' => 'map'),
      ),
    ),
  );

  $form['marker'] = array(
    '#type' => 'radios',
    '#title' => t('Marker'),
    '#default_value' => $config['marker'],
    '#options' => array(
      'default-black' => t('Default black'),
      'default-brown' => t('Default brown'),
      'default-red' => t('Default red'),
      'default-orange' => t('Default orange'),
      'default-yellow' => t('Default yellow'),
      'default-green' => t('Default green'),
      'default-blue' => t('Default blue'),
      'default-pink' => t('Default pink'),
      'target-black' => t('Target black'),
      'target-brown' => t('Target brown'),
      'target-red' => t('Target red'),
      'target-orange' => t('Target orange'),
      'target-yellow' => t('Target yellow'),
      'target-green' => t('Target green'),
      'target-blue' => t('Target blue'),
      'target-pink' => t('Target pink'),
      'cross-black' => t('Cross black'),
      'cross-brown' => t('Cross brown'),
      'cross-red' => t('Cross red'),
      'cross-orange' => t('Cross orange'),
      'cross-yellow' => t('Cross yellow'),
      'cross-green' => t('Cross green'),
      'cross-blue' => t('Cross blue'),
      'cross-pink' => t('Cross pink'),
      'diamond-black' => t('Diamond black'),
      'diamond-brown' => t('Diamond brown'),
      'diamond-red' => t('Diamond red'),
      'diamond-orange' => t('Diamond orange'),
      'diamond-yellow' => t('Diamond yellow'),
      'diamond-green' => t('Diamond green'),
      'diamond-blue' => t('Diamond blue'),
      'diamond-pink' => t('Diamond pink'),
      'star-black' => t('Star black'),
      'star-brown' => t('Star brown'),
      'star-red' => t('Star red'),
      'star-orange' => t('Star orange'),
      'star-yellow' => t('Star yellow'),
      'star-green' => t('Star green'),
      'star-blue' => t('Star blue'),
      'star-pink' => t('Star pink'),
      'heart-black' => t('Heart black'),
      'heart-brown' => t('Heart brown'),
      'heart-red' => t('Heart red'),
      'heart-orange' => t('Heart orange'),
      'heart-yellow' => t('Heart yellow'),
      'heart-green' => t('Heart green'),
      'heart-blue' => t('Heart blue'),
      'heart-pink' => t('Heart pink'),
      'triangle-black' => t('Triangle black'),
      'triangle-brown' => t('Triangle brown'),
      'triangle-red' => t('Triangle red'),
      'triangle-orange' => t('Triangle orange'),
      'triangle-yellow' => t('Triangle yellow'),
      'triangle-green' => t('Triangle green'),
      'triangle-blue' => t('Triangle blue'),
      'triangle-pink' => t('Triangle pink'),
      'square-black' => t('Square black'),
      'square-brown' => t('Square brown'),
      'square-red' => t('Square red'),
      'square-orange' => t('Square orange'),
      'square-yellow' => t('Square yellow'),
      'square-green' => t('Square green'),
      'square-blue' => t('Square blue'),
      'square-pink' => t('Square pink'),
      'spinner-black' => t('Spinner black'),
      'spinner-brown' => t('Spinner brown'),
      'spinner-red' => t('Spinner red'),
      'spinner-orange' => t('Spinner orange'),
      'spinner-yellow' => t('Spinner yellow'),
      'spinner-green' => t('Spinner green'),
      'spinner-blue' => t('Spinner blue'),
      'spinner-pink' => t('Spinner pink'),
    ),
    '#states' => array(
      'visible' => array(
        $jname => array('value' => 'map'),
      ),
      'required' => array(
        $jname => array('value' => 'map'),
      ),
    ),
  );

  if (($id == 'general') || ($id == 'newblock')) {
    $form['save'] = array(
      '#type' => 'submit',
      '#default_value' => t('Save'),
    );
  }

  if ($id == 'newblock') {
    $form['#submit'] = array('whos_amung_us_configuration_form_submit');
  }
  elseif ($id == 'general') {
    $form['#submit'] = array('whos_amung_us_configuration_form_general_submit');
  }

  return $form;

}

/**
 * Submit handler for the options whos_amung_us_configuration_form() form.
 */
function whos_amung_us_configuration_form_general_submit($form, &$form_state) {

  $config = array(
    'widget' => $form_state['values']['widget'],
    'foreground' => $form_state['values']['foreground'],
    'background' => $form_state['values']['background'],
    'position' => $form_state['values']['position'],
    'map' => $form_state['values']['map'],
    'marker' => $form_state['values']['marker'],
    'width' => $form_state['values']['width'],
    'height' => $form_state['values']['height'],
  );
  $config += whos_amung_us_default_form_values();
  variable_set('whos_amung_us_configuration', $config);
  variable_set('whos_amung_us_key', $form_state['values']['apikey']);
  drupal_set_message(t('Options saved.'));

}

/**
 * Submit handler for the new-block whos_amung_us_configuration_form() form.
 */
function whos_amung_us_configuration_form_submit($form, &$form_state) {

  $config = array(
    'widget' => $form_state['values']['widget'],
    'foreground' => $form_state['values']['foreground'],
    'background' => $form_state['values']['background'],
    'position' => $form_state['values']['position'],
    'map' => $form_state['values']['map'],
    'marker' => $form_state['values']['marker'],
    'width' => $form_state['values']['width'],
    'height' => $form_state['values']['height'],
  );

  db_insert('whos_amung_us_block')
    ->fields(
      array(
        'info' => $form_state['values']['name'],
        'machine_name' => $form_state['values']['mname'],
        'config' => serialize($config),
      )
    )
    ->execute();

  drupal_set_message(t('Block saved.'));
  drupal_goto('admin/structure/block');

}

/**
 * Auxiliar function for hook_block_configure().
 */
function _whos_amung_us_block_configure($delta = '') {

  $form = array();
  $item = whos_amung_us_item_load($delta);
  if ($item) {
    $form_state = array('values' => unserialize($item['config']));
    $form_state['values']['name'] = $item['info'];
    $form_state['values']['mname'] = $item['machine_name'];
    return whos_amung_us_configuration_form(array(), $form_state, 'block');
  }
  return $form;

}

/**
 * Auxiliar function for hook_block_save().
 */
function _whos_amung_us_block_save($delta = '', $edit = array()) {

  $config = array(
    'widget' => $edit['widget'],
    'foreground' => $edit['foreground'],
    'background' => $edit['background'],
    'position' => $edit['position'],
    'map' => $edit['map'],
    'marker' => $edit['marker'],
    'width' => $edit['width'],
    'height' => $edit['height'],
  );

  db_update('whos_amung_us_block')
    ->fields(
      array(
        'info' => $edit['name'],
        'config' => serialize($config),
      )
    )
    ->condition('machine_name', $delta)
    ->execute();

}

/**
 * Menu callback for admin/structure/block/delete-whos-amung-us-block/%.
 */
function whos_amung_us_configuration_delete_form($form, &$form_state, $item) {

  $form['delta'] = array('#type' => 'hidden', '#value' => $item['machine_name']);
  $form['block_title'] = array('#type' => 'hidden', '#value' => $item['info']);
  return confirm_form(
    $form,
    t(
      'Are you sure you want to delete the "%name" block?',
      array(
        '%name' => $item['info'],
      )
    ),
    'admin/structure/block',
    NULL,
    t('Delete'),
    t('Cancel')
  );

}

/**
 * Submit handler for the whos_amung_us_configuration_delete_form() form.
 */
function whos_amung_us_configuration_delete_form_submit($form, &$form_state) {

  db_delete('whos_amung_us_block')
    ->condition('machine_name', $form_state['values']['delta'])
    ->execute();
  drupal_set_message(t('The block "%name" has been removed.', array('%name' => $form_state['values']['block_title'])));
  cache_clear_all();
  $form_state['redirect'] = 'admin/structure/block';

}

/**
 * Auxiliar function for hook_form_FORM_ID_alter().
 */
function _whos_amung_us_form_block_admin_display_form_alter(&$form, $form_state) {

  $sql = db_select('whos_amung_us_block', 'b')
    ->fields('b', array('machine_name'))
    ->execute();
  while ($row = $sql->fetchAssoc()) {
    $form['blocks']['whos_amung_us_' . $row['machine_name']]['delete'] = array(
      '#type' => 'link',
      '#title' => t('delete'),
      '#href' => 'admin/structure/block/delete-whos-amung-us-block/' . $row['machine_name'],
    );
  }

}

/**
 * Try to get a new Whos.Amung.us API key via HTTP request.
 *
 * @return string
 *   The new key, or NULL if the key couldn't be retrieved.
 */
function whos_amung_us_get_new_key() {

  $request = drupal_http_request(
    'http://whos.amung.us',
    array(
      'timeout' => 5,
    )
  );
  if (($request->code == 200) && ($request->data)) {
    $counter = preg_match("/k: \'(.*?)\',/si", $request->data, $matches);
    if ($counter == 1) {
      return $matches[1];
    }
  }
  return NULL;

}
