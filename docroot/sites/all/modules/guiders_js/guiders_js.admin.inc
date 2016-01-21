<?php

/**
 * @file
 * Administrations file for the Guiders-JS module.
 */

/**
 * builds the guiders packs list page's html.
 */
function guiders_js_packs_list_page() {
  $path_prefix = GUIDERS_JS_SETTINGS_PATH . '/manage/';

  // Columns for the table header
  $header = array(
    array('data' => t('Title'), 'id' => 'table-header-title'),
    array('data' => t('Name'), 'id' => 'table-header-name'),
    array('data' => t('Path'), 'id' => 'table-header-path'),
    array('data' => t('Actions'), 'id' => 'table-header-actions'),
  );

  // Get all of the guiders' packs
  $guiders_packs = _guiders_js_get('guiders_packs');

  // Building the table rows
  $rows = array();
  foreach ($guiders_packs as $guiders_pack) {
    // Getting the Packs' data
    $gp_data = !empty($guiders_pack->data) ? unserialize($guiders_pack->data) : "";
    $path = $path_prefix . $guiders_pack->gpid;
    $name = check_plain($guiders_pack->name);

    // Appending the pack's relevant data into the rows array
    $rows[] = array(
      check_plain($guiders_pack->title),
      $name,
      array(
        'data' => ($guiders_pack->path == '<front>' ? t('%front', array('%front' => '<front>')) : check_url($guiders_pack->path)),
        'id' => 'table-row-path-' . $name,
      ),
      (($gp_data && !$gp_data['active']) ?
        l(t('Enable'), $path . '/enable', array('attributes' => array('id' => 'table-row-path-' . $name . '-enable'))) :
        l(t('Disable'), $path . '/disable', array('attributes' => array('id' => 'table-row-path-' . $name . '-disable')))) . ' | ' .
      l(t('Edit'), $path . '/edit', array('attributes' => array('id' => 'table-row-path-' . $name . '-edit'))) . ' | ' .
      l(t('List'), $path . '/list', array('attributes' => array('id' => 'table-row-path-' . $name . '-list'))) . ' | ' .
      l(t('Delete'), $path . '/delete', array('attributes' => array('id' => 'table-row-path-' . $name . '-delete')))
    );
  }

  // Building the "add pack" action html
  $action_links = '<ul class="action-links">' . theme('menu_local_action', array(
    'element' => array(
      '#link' => array(
        'href' => GUIDERS_JS_SETTINGS_PATH . '/add',
        'title' => t('Add guider pack'),
      ),
    ),
  )) . '</ul>';

  // Themeing the table html
  $output = $action_links . theme('table', array('header' => $header, 'rows' => $rows)) . $action_links;

  return $output;
}

/**
 * Page to administer guiders within a pack
 *
 * Sets the weight of the guiders.
 *
 * @return
 *   A form setting the guiders weight.
 */
function guiders_js_form($form_state, $info) {
  $gpid = $info['build_info']['args'][0]->gpid;

  $path_prefix = GUIDERS_JS_SETTINGS_PATH . '/manage/';

  // Building the "add guider" action html
  $action_links = '<ul class="action-links">' . theme('menu_local_action', array(
    'element' => array(
      '#link' => array(
        'href' => $path_prefix . $gpid . '/add',
        'title' => t('Add guider to the pack'),
      ),
    ),
  )) . '</ul>';

  $form['action_links'] = array(
    '#markup' => $action_links,
  );

  // Get all of the guiders belonging to this pack
  $guiders = _guiders_js_get('guiders', array('gpid' => $gpid));
  foreach ($guiders as $guider) {
    $path = $path_prefix . $guider->gpid . '/' . $guider->gid;

    $form['guider:'. $guider->gpid . ':' . $guider->name] = array(
      '#weight' => isset($guider->weight) ? $guider->weight : 0,
      'title' => array(
        '#markup' => $guider->title,
      ),
      'weight' => array(
        '#type' => 'weight',
        '#default_value' => isset($guider->weight) ? $guider->weight : 0,
      ),
      'name' => array(
        '#markup' => $guider->name,
      ),
      // This is just to key off of in the theme function, so we don't render
      // elements that are not meant to be part of the table. There might be
      // a better way to handle this ??
      'gid' => array(
        '#type' => 'hidden',
        '#value' => $guider->gid,
      ),
      'actions' => array(
        'edit' => array(
          '#type' => 'link',
          '#title' => t('Edit'),
          '#href' => $path . '/edit',
          '#suffix' => ' | ',
        ),
        'delete' => array(
          '#type' => 'link',
          '#title' => t('Delete'),
          '#href' => $path . '/delete',
        ),
      ),
    );
  }

  $form['#tree'] = TRUE;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Submit handler for guiders_js_form()
 *
 * @see guiders_js_form()
 * @ingroup forms
 */
function guiders_js_form_submit(&$form, &$form_state) {
  foreach ($form_state['values'] as $key => $value) {
    if (stristr($key, 'guider:')) {
      _guiders_js_set('guiders', array('gid' => $value['gid']), array('weight' => $value['weight']));
    }
  }


  // Clearing the pack's cache so our change will happen immediately
  $gpack = $form_state['build_info']['args'][0];
  cache_clear_all('guiders-pack-' . $gpack -> gpid, 'cache_guiders');

  drupal_set_message(t('Your configuration has been saved.'));
}

// Theme the guiders listing form.
function theme_guiders_js_form($variables) {
  $form = $variables['form'];

  $action_links = drupal_render($form['action_links']);

  $output = $action_links;

  drupal_add_tabledrag('guiders-js-order', 'order', 'sibling', 'guiders-js-order-weight');

  $header = array(
    t('Title'),
    t('Name'),
    t('Weight'),
    t('Actions'),
  );

  // Build the table rows.
  $rows = array();
  foreach (element_children($form) as $item) {
    $element = &$form[$item];

    // Build a list of operations.
    $actions = array(drupal_render($element['actions']));

    // Add special class to be used with tabledrag.js
    if (isset($element['weight'])) {
      $element['weight']['#attributes']['class'] = array('guiders-js-order-weight');
    }

    if (isset($element['gid'])) {
      $row = array();
      $row[] = drupal_render($element['title']);
      $row[] = drupal_render($element['name']);
      $row[] = drupal_render($element['weight']);
      $row = array_merge($row, $actions);
      $row = array('data' => $row);
      $row['class'][] = 'draggable';
      $rows[] = $row;
    }
  }

  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'guiders-js-order')));
  $output .= $action_links;
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Form constructor for the guiders' pack add/edit form.
 *
 * @param object $guider_pack
 *   The guiders' pack we are editing.
 *
 * @see guiders_js_packs_add_form_validate()
 * @see guiders_js_packs_add_form_submit()
 * @ingroup forms
 */
function guiders_js_packs_add_form($form, &$form_state, $guider_pack = NULL) {
  if ($guider_pack) {
    // Unserialize the pack's data and put it into the guider as part of the object
    guiders_js_parse_data($guider_pack);
  }

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Guider pack name'),
    '#default_value' => isset($guider_pack->title) ? $guider_pack->title : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  $form['machine_name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine name'),
    '#default_value' => isset($guider_pack->name) ? $guider_pack->name : '',
    '#maxlength' => 30,
    '#description' => t('A unique name to construct the name for the guider. It must only contain lowercase letters, numbers and hyphens.'),
    '#machine_name' => array(
      'exists' => 'guiders_js_name_exists',
      'source' => array('title'),
      'label' => t('Machine name'),
      'replace_pattern' => '[^a-z0-9-]+',
      'replace' => '-',
    ),
    '#disabled' => FALSE,
  );
  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#description' => t('Relative drupal path without the starting "/" character, use %front for the front page.', array('%front' => '<front>')),
    '#default_value' => isset($guider_pack->path) ? $guider_pack->path : '',
    '#autocomplete_path' => GUIDERS_JS_SETTINGS_PATH . '/path/autocomplete',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
  );
  $form['settings']['active'] = array(
    '#type' => 'checkbox',
    '#title' => t('The pack is active.'),
    '#default_value' => isset($guider_pack->data['active']) ? $guider_pack->data['active'] : TRUE,
  );
  $form['settings']['start'] = array(
    '#type' => 'radios',
    '#title' => t('Start method'),
    '#options' => array(
      t('Auto start'),
      t('Delay'),
      t('Event'),
      t('Default block (make sure the block is active in the page of this pack)'),
    ),
    '#default_value' => isset($guider_pack->data['start']) ? $guider_pack->data['start'] : GUIDERS_JS_AUTO_START,
  );
  $form['settings']['delay_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Delay'),
    '#description' => t('Please use time in milliseconds, i.e: 1000 equals 1 second.'),
    '#default_value' => isset($guider_pack->data['delay_time']) ? $guider_pack->data['delay_time'] : '2000',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
    '#states' => array(
      // Only show this field when the 'start' radios has the option 'Delay' selected.
      'visible' => array(
        ':input[name="start"]' => array('value' => GUIDERS_JS_DELAY),
      ),
    ),
  );
  $form['settings']['event_type'] = array(
    '#type' => 'textfield',
    '#title' => t('Event type'),
    '#description' => t('A string containing one or more DOM event types, such as "click", "submit" or custom event names'),
    '#default_value' => isset($guider_pack->data['event_type']) ? $guider_pack->data['event_type'] : 'onclick',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
    '#states' => array(
      // Only show this field when the 'start' radios has the option 'Delay' selected.
      'visible' => array(
        ':input[name="start"]' => array('value' => GUIDERS_JS_EVENT),
      ),
    ),
  );
  $form['settings']['event_attach'] = array(
    '#type' => 'textfield',
    '#title' => t('Attach to'),
    '#description' => t('You can enter basic xpath synatx. e.g.: <strong>#my_element_id</strong>,
      <strong>.my_element_class</strong>, <strong>.my.element.classes</strong>, etc...'),
    '#default_value' => isset($guider_pack->data['event_attach']) ? $guider_pack->data['event_attach'] : '#content h2',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
    '#states' => array(
      // Only show this field when the 'start' radios has the option 'Delay' selected.
      'visible' => array(
        ':input[name="start"]' => array('value' => GUIDERS_JS_EVENT),
      ),
    ),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save pack'),
  );
  return $form;
}

/**
 * Form validation handler for guiders_js_packs_add_form().
 *
 * @see guiders_js_packs_add_form_submit()
 */
function guiders_js_packs_add_form_validate($form, &$form_state) {
  $path = $form_state['values']['path'];
  $guiders_packs = _guiders_js_get('guiders_packs', array('path' => $path));

  // Check if this path belongs to another guiders' pack
  if ($guiders_packs->rowCount() == 1) {

    // Get the guiders' pack
    $guiders_pack = $guiders_packs->fetchObject();

    // Check if this indeed another guider and not the one we are trying to save
    if ($guiders_pack->name != $form_state['values']['machine_name']) {
      form_set_error('path', t('You can\'t use the same path on more then one guider\'s pack'));
    }
  }

  // Check if the appropriate fields are set when their respective start method is chosen
  switch ($form_state['values']['start']) {
    case GUIDERS_JS_DELAY:
      // The "Delay" option was chosen
      if (empty($form_state['values']['delay_time'])) {
        form_set_error('delay_time', t('The delay time is required when you choose "Delay" as a start method'));
      }
      break;
    case GUIDERS_JS_EVENT:
      // The "Event" option was chosen
      if (empty($form_state['values']['event_type'])) {
        form_set_error('event_type', t('The event name is required when you choose "Event" as a start method'));
      }
      if (empty($form_state['values']['event_attach'])) {
        form_set_error('event_attach', t('The event element is required when you choose "Event" as a start method'));
      }
      break;
  }
}

/**
 * Form submission handler for guiders_js_packs_add_form().
 *
 * @see guiders_js_packs_add_form_validate()
 */
function guiders_js_packs_add_form_submit($form, &$form_state) {
  $name  = $form_state['values']['machine_name'];
  $title = $form_state['values']['title'];
  $path  = $form_state['values']['path'] == '/'  ? '<front>' : $form_state['values']['path'];

  $data  = serialize($form_state['values']);

  _guiders_js_set(
    'guiders_packs',
    array('name' => $name),
    array(
      'title' => $title,
      'path'  => $path,
      'data'  => $data,
    )
  );

  // Clearing the pack's cache so our change will happen immediately
  // Nothing to clear if this is a new pack
  if (!empty($form_state['build_info']['args'])) {
    $gpack = $form_state['build_info']['args'][0];
    cache_clear_all('guiders-pack-' . $gpack->gpid, 'cache_guiders');
  }

  // Adding a redirect so the form will return to the list page after submission
  $form_state['redirect'][] = GUIDERS_JS_SETTINGS_PATH;
}

/**
 * Form constructor for the guider add/edit form.
 *
 * @param object $guider_pack
 *   The guiders' pack this guider belongs to.
 *
 * @param object $guider
 *   The guider we are editing.
 *
 * @see guiders_js_add_form_validate()
 * @see guiders_js_add_form_submit()
 * @ingroup forms
 */
function guiders_js_add_form($form, &$form_state, $guider_pack = NULL, $guider = NULL) {
  if ($guider) {
    // Unserialize the guider's data and put it into the guider as part of the object
    guiders_js_parse_data($guider);
  }

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Guider title'),
    '#default_value' => $guider ? $guider->title : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  $form['machine_name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine name'),
    '#default_value' => $guider ? $guider->name : '',
    '#maxlength' => 30,
    '#description' => t('A unique name to construct the name for the guider. It must only contain lowercase letters, numbers and hyphens.'),
    '#machine_name' => array(
      'exists' => 'guiders_js_name_exists',
      'source' => array('title'),
      'label' => t('Machine name'),
      'replace_pattern' => '[^a-z0-9-]+',
      'replace' => '-',
    ),
    '#disabled' => FALSE,
  );
  $form['desc'] = array(
    '#type' => 'text_format',
    '#title' => t('Body'),
    '#default_value' => $guider ? $guider->data['desc']['value'] : '',
    '#format' => NULL,
    '#rows' => 10,
    '#description' => t('The content of the guider, plain text or html.'),
    '#required' => TRUE,
  );
  $form['attach'] = array(
    '#type' => 'textfield',
    '#title' => t('Attach to'),
    '#description' => t('Leave empty to center the guider to the window and display it without a tip,
      otherwise you can enter basic xpath synatx. <br /> e.g.: <strong>#my_element_id</strong>,
      <strong>.my_element_class</strong>, <strong>.my.element.classes</strong>, etc...'),
    '#default_value' => $guider ? $guider->data['attach'] : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
  );
  $form['position'] = array(
    '#type' => 'textfield',
    '#title' => t('Position'),
    '#description' => t('Use a 1 through 12 value, as in a clock hour handle.<br />
      Imagine the element you are attaching the guider to is in the center of the clock.'),
    '#default_value' => $guider ? $guider->data['position'] : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
    '#states' => array(
      // Only show this field when the 'attach' textfield was filled.
      'invisible' => array(
        ':input[name="attach"]' => array('value' => ''),
      ),
    ),
  );
  $form['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#description' => t('The guider box\'s width in px, emit the "px" suffix'),
    '#default_value' => $guider ? $guider->data['width'] : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
  );
  $form['overlay'] = array(
    '#type' => 'checkbox',
    '#title' => t('Overlay'),
    '#description' => t('when checked, an overlay will cover the page'),
    '#default_value' => $guider ? $guider->data['overlay'] : 1,
    '#required' => FALSE,
  );
  $form['highlight'] = array(
    '#type' => 'textfield',
    '#title' => t('Highlight'),
    '#description' => t('selector of the html element you want to highlight (will cause element to be above the overlay)'),
    '#default_value' => $guider ? $guider->data['highlight'] : '',
    '#required' => FALSE,
  );
  $form['autoFocus'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto Focus'),
    '#description' => t('if you want the browser to scroll to the position of the guider, set this to true'),
    '#default_value' => $guider ? $guider->data['autoFocus'] : FALSE,
    '#required' => FALSE,
  );

  $form['close'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show x button for the guider'),
    '#description' => t('when checked, there will be an X button on the right corner of the guider'),
    '#default_value' => $guider ? $guider->data['close'] : 1,
    '#required' => FALSE,
  );

  // Because we have many fields with the same values, we have to set
  // #tree to be able to access them.
  $form['#tree'] = TRUE;
  $form['buttons_fs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Buttons'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    // Set up the wrapper so that AJAX will be able to replace the fieldset.
    '#prefix' => '<div id="buttons-fs-wrapper">',
    '#suffix' => '</div>',
  );

  // Build the fieldset with the proper number of names. We'll use
  // $form_state['num_names'] to determine the number of textfields to build.
  if (empty($form_state['num_buttons'])) {
    // If we are rebuilding the form for adding or removing a button
    if (!empty($guider) && isset($guider->data['buttons_fs']) && count($guider->data['buttons_fs']) > 0) {
      // Then Update the number of buttons
      $form_state['num_buttons'] = count($guider->data['buttons_fs']);
    }
    else {
      // Otherwise initialize the number of buttons
      $form_state['num_buttons'] = 1;
    }
  }

  // Building the buttons' elements
  for ($i = 0; $i < $form_state['num_buttons']; $i++) {
    // Fix the buttons array for the "other" value
    if (!empty($guider) && isset($guider->data['buttons_fs'][$i]) && !in_array($guider->data['buttons_fs'][$i]['name'], array(t('Next'), t('Close'), t('other')))) {
      $guider->data['buttons_fs'][$i]['name'] = 'other';
    }

    $form['buttons_fs'][$i] = array(
      '#type' => 'container',
      '#prefix' => '<hr>',
    );
    $form['buttons_fs'][$i]['name'] = array(
      '#type' => 'radios',
      '#title' => t('Button text'),
      '#default_value' => (($guider && isset($guider->data['buttons_fs'][$i]['name'])) ? $guider->data['buttons_fs'][$i]['name'] : 'Next'),
      '#options' => array(
        'Next' => t('<b>Next</b> (will close this guider and open the next one in the pack)'),
        'Close' => t('<b>Close</b> (will close this one and end the guider)'),
        'other' => t('<b>Other</b> (set up your own button with a javascript function name to use as a callback)'),
      ),
      '#required' => FALSE,
    );
    $form['buttons_fs'][$i]['other'] = array(
      '#type' => 'textfield',
      '#title' => t('Text'),
      '#default_value' => (($guider && isset($guider->data['buttons_fs'][$i]['other'])) ? $guider->data['buttons_fs'][$i]['other'] : ''),
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => FALSE,
      '#states' => array(
        // Only show this field when the 'other' option was selected on the name field.
        'visible' => array(
          ':input[name="buttons_fs[' . $i . '][name]"]' => array('value' => 'other'),
        ),
      ),
    );
    $form['buttons_fs'][$i]['class'] = array(
      '#type' => 'textfield',
      '#title' => t('Class'),
      '#default_value' => (($guider && isset($guider->data['buttons_fs'][$i]['class'])) ? $guider->data['buttons_fs'][$i]['class'] : ''),
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => FALSE,
      '#states' => array(
        // Only show this field when the 'other' option was selected on the name field.
        'visible' => array(
          ':input[name="buttons_fs[' . $i . '][name]"]' => array('value' => 'other'),
        ),
      ),
    );
    $form['buttons_fs'][$i]['onclick'] = array(
      '#type' => 'textfield',
      '#title' => t('onClick'),
      '#description' => t('If the text is "Close" or "Next", onclick defaults to guiders.hideAll and guiders.next respectivelly'),
      '#default_value' => (($guider && isset($guider->data['buttons_fs'][$i]['onclick'])) ? $guider->data['buttons_fs'][$i]['onclick'] : ''),
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => FALSE,
      '#states' => array(
        // Only show this field when the 'other' option was selected on the name field.
        'visible' => array(
          ':input[name="buttons_fs[' . $i . '][name]"]' => array('value' => 'other'),
        ),
      ),
    );
  }
  $form['buttons_fs']['add_button'] = array(
    '#type' => 'submit',
    '#value' => t('Add button'),
    '#submit' => array('guiders_js_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'guiders_js_add_more_callback',
      'wrapper' => 'buttons-fs-wrapper',
    ),
  );
  if ($form_state['num_buttons'] > 1) {
    $form['buttons_fs']['remove_button'] = array(
      '#type' => 'submit',
      '#value' => t('Remove last'),
      '#submit' => array('guiders_js_add_more_remove_one'),
      '#ajax' => array(
        'callback' => 'guiders_js_add_more_callback',
        'wrapper' => 'buttons-fs-wrapper',
      ),
    );
  }

  if (isset($guider)) {
    $form['weight'] = array(
      '#type' => 'hidden',
      '#value' => $guider->weight,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save guider'),
  );
  return $form;
}

/**
 * Form validation handler for guiders_js_add_form().
 *
 * @see guiders_js_add_form_submit()
 */
function guiders_js_add_form_validate($form, &$form_state) {
  // Check for filter access
  if (!empty($form_state['values']['desc']['value']) && !filter_access(filter_format_load($form_state['values']['desc']['format']))) {
    form_set_error('desc', t('You are not allowed to use the given format'));
  }

  if (!empty($form_state['values']['attach'])) {
    // Position must be filled when attach is filled
    if (empty($form_state['values']['position'])) {
      form_set_error('position', t('The position field must be filled when the attach field is filled'));
    }
    // Check if position has invalid characters
    elseif (!is_numeric($form_state['values']['position']) || $form_state['values']['position'] < 1 || $form_state['values']['position'] > 12) {
      form_set_error('position', t('The position field must contain only the numbers 1 through 12'));
    }
  }

  // Check if width has invalid characters
  if (!empty($form_state['values']['width']) && !is_numeric($form_state['values']['width'])) {
    form_set_error('width', t('The width field must contain only numeric values'));
  }
}

/**
 * Form submission handler for guiders_js_add_form().
 *
 * @see guiders_js_add_form_submit()
 */
function guiders_js_add_form_submit($form, &$form_state) {
  $gpack = $form_state['build_info']['args'][0];
  $gpid  = $gpack -> gpid;
  $name  = $form_state['values']['machine_name'];
  $title = $form_state['values']['title'];

  // Clean the buttons array from the actions and leave only the buttons' data
  unset($form_state['values']['buttons_fs']['add_button']);
  unset($form_state['values']['buttons_fs']['remove_button']);
  foreach ($form_state['values']['buttons_fs'] as &$button) {
    switch ($button['name']) {
      case 'Next':
      case 'Close':
        // No need for these fields when we use the "Next" or "Close" buttons.
        // Their data is set automatically by the library
        unset($button['other']);
        unset($button['class']);
        unset($button['onclick']);
        break;
      case 'other':
        $button['name'] = $button['other'];
        break;
    }
  }

  if (isset($form_state['values']['weight'])) {
    $weight = $form_state['values']['weight'];
  }
  else {
    // Find out what the last guide weight is.
    $result = db_select('guiders', 'g')
      ->fields('g', array('weight'))
      ->condition('gpid', $gpid, '=')
      ->orderBy('weight', 'DESC')
      ->range(0, 1)
      ->execute()
      ->fetchAssoc();

    // Add one to the weight to push the new one to the bottom.
    $weight = $result['weight'] + 1;
  }

  $data  = serialize($form_state['values']);

  // Set the form's data in the db
  _guiders_js_set(
    'guiders',
    array('name' => $name),
    array(
      'gpid'  => $gpid,
      'title' => $title,
      'data'  => $data,
      'weight' => $weight,
    )
  );

  // Clearing the pack's cache so our change will happen immediately
  cache_clear_all('guiders-pack-' . $gpid, 'cache_guiders');

  // Adding a redirect so the form will return to the list page after submission
  $form_state['redirect'][] = GUIDERS_JS_SETTINGS_PATH . '/manage/' . $gpid;
}

/**
 * Builds the guider's confirm deletion page's html.
 *
 * @param object $guiders_pack
 *   The guiders' pack we are about to delete.
 *
 * @param object $guider
 *   The guider we are about to delete.
 *
 * @return
 *   The output html of this page.
 */
function guiders_js_delete_confirm($form, &$form_state, $guiders_pack, $guider = NULL) {
  $message = t('Are you sure you want to delete "%title"?', array('%title' => (!empty($guider) ? $guider->title : $guiders_pack->title)));
  return confirm_form($form, $message, GUIDERS_JS_SETTINGS_PATH, NULL, t('Delete'));
}

/**
 * Deletes the guider/guider's pack.
 */
function guiders_js_delete_confirm_submit($form, &$form_state) {
  // Getting the last argument, so in case it's a guider and not a pack we'll get it
  $guider = $form_state['build_info']['args'][count($form_state['build_info']['args']) - 1];

  if (!empty($guider)) {
    $table = isset($guider->gid) ? 'guiders' : 'guiders_packs';

    // Trying to delete from the db
    if (_guiders_js_del($table, array('name' => $guider->name)) === FALSE) {
      drupal_set_message(t('The delete action has failed due to not finding "%title"', array('%title' => $guider->title)));
    }
    else {
      drupal_set_message(t('The delete action was successful, "%title" is no more', array('%title' => $guider->title)));

      // Deleting the guider's pack's guiders from the guiders table to prevent the creation of orphans
      if ($table == 'guiders_packs') {
        _guiders_js_del('guiders', array('gpid' => $guider->gpid));
        drupal_set_message(t('The related guiders were also deleted'));
      }

      // Deleting this guider's pack data from the cache bin
      cache_clear_all('guiders-pack-' . $guider->gpid, 'cache_guiders');
    }
  }

  // Adding a redirect so the form will return to the list page after submission
  $redirect = GUIDERS_JS_SETTINGS_PATH;
  if (isset($guider->gid)) {
    // This means we need to redirect to the pack's guiders list and not the packs list
    $redirect = GUIDERS_JS_SETTINGS_PATH . '/' . $guider->gpid;
  }
  $form_state['redirect'][] = $redirect;
}

function guiders_js_packs_toggle_active($guider_pack) {
  guiders_js_parse_data($guider_pack);

  // Changing the value in field active to be the opposite
  $guider_pack->data['active'] = !$guider_pack->data['active'];

  $guider_pack->data = serialize($guider_pack->data);

  // Saving the updated pack
  _guiders_js_set(
    'guiders_packs',
    array('name' => $guider_pack->name),
    array(
      'data'  => $guider_pack->data,
    )
  );

  // Clearing the pack's cache so our change will happen immediately
  cache_clear_all('guiders-pack-' . $guider_pack->gpid, 'cache_guiders');

  // Adding a redirect so the form will return to the list page after submission
  drupal_goto(GUIDERS_JS_SETTINGS_PATH);
}

/**
 * Parses the guider serialized data and attach it to the passed guider param.
 */
function guiders_js_parse_data(&$guider) {
  $guider->data = unserialize($guider->data);
}

/**
 * Callback for both ajax-enabled buttons.
 *
 * This simply selects and returns the fieldset with the names in it.
 */
function guiders_js_add_more_callback($form, $form_state) {
  return $form['buttons_fs'];
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * It just increments the max counter and causes a rebuild.
 */
function guiders_js_add_more_add_one($form, &$form_state) {
  $form_state['num_buttons']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "remove one" button.
 *
 * Decrements the max counter and causes a form rebuild.
 */
function guiders_js_add_more_remove_one($form, &$form_state) {
  if ($form_state['num_buttons'] > 1) {
    $form_state['num_buttons']--;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Checkes for the uniqueness of the guider's name
 */
function guiders_js_name_exists($machine_name) {
  // Get the table name from the current path
  $table = (current_path() == GUIDERS_JS_SETTINGS_PATH . '/add') ? 'guiders_packs' : 'guiders';

  // Get the guider or pack that has this maching name
  $res = _guiders_js_get($table, array('name' => check_plain($machine_name)))->fetchObject();

  return !empty($res);
}

/*
 * Gets the autocomplete options for a given string.
 */
function guiders_js_path_autocomplete($string) {
  $matches = array();

  // Get the aliases from the url_alias table
  $query = db_select('url_alias', 'url');

  // Getting all the aliases
  $records = $query
    ->fields('url')
    ->condition('url.alias', '%' . db_like(check_plain($string)) . '%', 'LIKE')
    ->range(0, 10)
    ->execute();

  // add matches to $matches
  foreach ($records as $alias) {
    $matches[$alias->source] = check_plain($alias->alias);
  }

  // return for JS
  drupal_json_output($matches);
}
