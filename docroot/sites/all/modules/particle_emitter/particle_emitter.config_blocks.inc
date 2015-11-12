<?php
/**
 * @file
 * particle_emitter.config_blocks.inc.
 */

/**
 * Implemets hook_form().
 */
function particle_emitter_blocks_form($form, &$form_state) {

  $form['#attached']['css'][drupal_get_path('module', 'particle_emitter') . '/css/particle_emitter.css'] = array();

  $form['#tree'] = TRUE;
  $form['particle_emitter_blocks_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Block Management'),
    '#prefix' => '<div id="names-fieldset-wrapper">',
    '#suffix' => '</div>',
  );
  $kh_data_array = variable_get('particle_emitter_blocks', '');

  $data = array();
  if (!empty($kh_data_array)) {
    $data = $kh_data_array;
  }
  if (!isset($form_state['block_count']) && !empty($kh_data_array)) {
    $form_state['block_count'] = count($data);
  }

  if (empty($form_state['block_count'])) {
    $form_state['block_count'] = 1;
  }
  $_data = array_values($data);
  $data = $_data;

  $form['particle_emitter_blocks_fieldset']['description1'] = array(
    '#markup' => '<div>' . t('In this section you can manage your custom blocks. To add a new custom block click the "Add new block" button and give it a name and a internal description. To activate the blocks go to block page') . '</div>',
  );

  $form['particle_emitter_blocks_fieldset']['particle_emitter_description_br'] = array(
    '#markup' => '<br clear="all" />',
  );

  $form['particle_emitter_blocks_fieldset']['particle_emitter_add_block'] = array(
    '#type' => 'submit',
    '#value' => 'Add new block',
    '#title' => t('Add new block'),
    '#submit' => array('particle_emitter_blocks_form_add_block'),
    '#ajax' => array(
      'callback' => 'particle_emitter_blocks_form_callback',
      'wrapper' => 'names-fieldset-wrapper',
    ),
  );

  $form['particle_emitter_blocks_fieldset']['description2'] = array(
    '#markup' => '<div>' . t('Overview of custom blocks avaiable:') . '</div>',
  );

  for ($i = 0; $i < $form_state['block_count']; $i++) {

    $particle_emitter_block_name = (!empty($data[$i]['particle_emitter_block_id'])) ? $data[$i]['particle_emitter_block_id'] : '';
    $particle_emitter_block_desc = (!empty($data[$i]['particle_emitter_block_name'])) ? $data[$i]['particle_emitter_block_name'] : '';

    $form['particle_emitter_blocks_fieldset'][$i] = array(
      '#prefix' => /*($i == 0) ? $header : */'<div class="particle-emitter-blocks-block particle-emitter-blocks-block-' . $i . '">',
      '#suffix' => '</div>',
    );

    $form['particle_emitter_blocks_fieldset'][$i]['particle_emitter_block_id'] = array(
      '#type' => 'textfield',
      '#required' => TRUE,
      '#size' => 30,
      '#maxlength' => 50,
      '#title' => t('Block ID'),
      '#title_display' => 'invisible',
      '#default_value' => $particle_emitter_block_name,
      '#description' => t('Block ID'),
    );

    $form['particle_emitter_blocks_fieldset'][$i]['particle_emitter_block_name'] = array(
      '#type' => 'textfield',
      '#required' => TRUE,
      '#size' => 30,
      '#maxlength' => 150,
      '#title' => t('Block Name'),
      '#title_display' => 'invisible',
      '#default_value' => $particle_emitter_block_desc,
      '#description' => t('Block Name'),
    );

    $form['particle_emitter_blocks_fieldset'][$i]['particle_emitter_block_delete'] = array(
      '#type' => 'checkbox',
      '#title' => t('Remove this custom block'),
    );

    $form['particle_emitter_blocks_fieldset'][$i]['particle_emitter_br'] = array(
      '#markup' => '<br clear="all" />',
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  return $form;
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function particle_emitter_blocks_form_add_block($form, &$form_state) {
  $form_state['block_count']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the names in it.
 */
function particle_emitter_blocks_form_callback($form, $form_state) {
  return $form['particle_emitter_blocks_fieldset'];
}


/**
 * Implements hook_form_validate().
 */
function particle_emitter_blocks_form_validate($form, &$form_state) {
  $_data = $form_state['values']['particle_emitter_blocks_fieldset'];
  unset($_data['particle_emitter_add_block']);
  $data = array_values($_data);

  foreach ($data as $k => $v) {
    if (!empty($v['particle_emitter_block_id']) || !empty($v['particle_emitter_block_name'])) {
      if (!preg_match('/^[a-z]{1}[a-z0-9\_]{0,}$/', $v['particle_emitter_block_id'])) {
        form_set_error('url', t('Please provide a valid maschine name (Characters allowed: [a-z], [0-9], [_] underscore). ID must begin with a lowercase letter [a-z].'));
      }
      if (!preg_match('/^[A-Za-z0-9\_\.\ ]{1,}$/', $v['particle_emitter_block_name'])) {
        form_set_error('url', t('Please provide a valid block description. (Characters allowed: [A-Z], [a-z], [0-9], [_] underscore, [.] point, [ ] whitespace).'));
      }
    }
  }
}

/**
 * Implements hook_form_submit().
 */
function particle_emitter_blocks_form_submit($form, &$form_state) {

  if (isset($form_state['values']['particle_emitter_blocks_fieldset']['particle_emitter_add_block'])) {
    unset($form_state['values']['particle_emitter_blocks_fieldset']['particle_emitter_add_block']);
  }

  for ($i = 0; $i < count($form_state['values']['particle_emitter_blocks_fieldset']); $i++) {
    if ($form_state['values']['particle_emitter_blocks_fieldset'][$i]['particle_emitter_block_delete'] == '1') {
      unset($form_state['values']['particle_emitter_blocks_fieldset'][$i]);
    }
    else {
      unset($form_state['values']['particle_emitter_blocks_fieldset'][$i]['particle_emitter_block_delete']);
    }
  }
  variable_set("particle_emitter_blocks", $form_state['values']['particle_emitter_blocks_fieldset']);
  drupal_set_message('Your settings have been saved');
  cache_clear_all();
}
