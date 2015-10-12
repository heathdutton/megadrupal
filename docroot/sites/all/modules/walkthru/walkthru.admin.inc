<?php

/**
 * @file
 * Administrations file for the walkthru module.
 */

/**
 * builds the walkthru list page.
 */
function walkthru_list() {

  // Columns for the table header
  $header = array(t('Title'), t('Name'), t('Actions'));

  // Get all of the walkthrus.
  $walkthrus = db_select('walkthru', 'w')
    ->fields('w')
    ->execute();

  // Building the table rows
  $rows = array();
  $path_prefix = 'admin/structure/walkthru';
  foreach ($walkthrus as $walkthru) {

    // Appending the walkthru's relevant data into the rows array
    $rows[] = array(
      check_plain($walkthru->title),
      check_plain($walkthru->name),
      l(t('Edit'), $path_prefix . '/edit/' . $walkthru->name) . ' | ' . l(t('List'), $path_prefix . '/' . $walkthru->name . '/list') . ' | ' . l(t('Delete'), $path_prefix . '/delete/' . $walkthru->name)
    );
  }

  // Building the "add pack" action html
  $action_links = '<ul class="action-links">' . theme('menu_local_action', array(
    'element' => array(
      '#link' => array(
        'href' => $path_prefix . '/add',
        'title' => t('Add a Walkthru'),
      ),
    ),
  )) . '</ul>';

  // Themeing the table html
  $output = $action_links . theme('table', array('header' => $header, 'rows' => $rows)) . $action_links;

  return $output;
}

/**
 * Form constructor for the walkthru add/edit form.
 *
 * @param $walkthru
 *   The walkthru we are editing.
 *
 * @see walkthru_add_form_validate()
 * @see walkthru_add_form_submit()
 * @ingroup forms
 */
function walkthru_add_form($form, &$form_state, $walkthru = NULL) {
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Walkthru Name'),
    '#default_value' => $walkthru ? $walkthru->title : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  $form['machine_name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine Name'),
    '#default_value' => $walkthru ? $walkthru->name : '',
    '#maxlength' => 30,
    '#description' => t('A unique name to construct the name for the guider. It must only contain lowercase letters, numbers and hyphens.'),
    '#machine_name' => array(
      'exists' => 'walkthru_name_exists',
      'source' => array('title'),
      'label' => t('Machine Name'),
      'replace_pattern' => '[^a-z0-9-]+',
      'replace' => '-',
    ),
    '#disabled' => FALSE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Walkthru'),
  );
  return $form;
}

/**
 * Form validation handler for walkthru_add_form().
 *
 * @see walkthru_add_form_submit()
 */
function walkthru_add_form_validate($form, &$form_state) {
  $machine_name = $form_state['values']['machine_name'];
  if (in_array($machine_name, array('add', 'edit', 'delete', 'list'))) {
    form_set_error('machine_name', t('The machine name chosen is one that is not allowed. Restricted machine names are add, edit, delete, and list'));
  }
}

/**
 * Form submission handler for walkthru_add_form().
 *
 * @see walkthru_add_form_validate()
 */
function walkthru_add_form_submit($form, &$form_state) {
  $name  = $form_state['values']['machine_name'];
  $title = $form_state['values']['title'];

  db_merge('walkthru')
    ->key(array('name' => $name))
    ->fields(array('title' => $title))
    ->execute();

  // Adding a redirect so the form will return to the list page after submission
  $form_state['redirect'][] = 'admin/structure/walkthru';
}

/**
 * Checkes for the uniqueness of the walkthru's name
 */
function walkthru_name_exists($machine_name) {
  // Get the walkthru that has this matching name
  $result = walkthru_load(check_plain($machine_name));
  return !empty($result);
}

/**
 * Builds the walkthru's confirm deletion page.
 *
 * @param $walkthru
 * The walkthru we are about to delete.
 *
 * @return
 * The output html of this page.
 *
 */
function walkthru_delete_confirm($form, &$form_state, $guiders_pack) {
  $message = t('Are you sure you want to delete "%title"?', array('%title' => $walkthru->title));
  return confirm_form($form, $message, 'admin/structure/walkthru', NULL, t('Delete'));
}

/**
 * Deletes a walkthru.
 */
function walkthru_delete_confirm_submit($form, &$form_state, $walkthru) {
  // Trying to delete from the db
  if (db_delete('walkthru')->condition('name', $walkthru->name)->execute() === FALSE) {
    drupal_set_message(t('The delete action has failed due to not finding "%title"', array('%title' => $walkthru->title)));
  }
  else {
    drupal_set_message(t('The delete action was successful, "%title" is no more', array('%title' => $walkthru->title)));

    // Deleting this guider's pack data from the cache bin
    cache_clear_all('walkthru-' . $walkthru->name, 'cache_walkthru');
  }
  $form_state['redirect'][] = 'admin/structure/walkthru';
}

/**
 * Page to administer steps within a walkthru
 *
 * Sets the order of the steps.
 *
 * @return
 *  A form setting the order of a walkthru's steps.
 */
function walkthru_steps_manage_form($form_state, $info, $walkthru) {
  drupal_set_title(t('Manage @walkthru steps', array('@walkthru' => $walkthru->title)));
  $walkthru_name = $info['build_info']['args'][0]->name;

  $path_prefix = 'admin/structure/walkthru';

  // Building the "add steps" action html
  $action_links = '<ul class="action-links">' . theme('menu_local_action', array(
    'element' => array(
      '#link' => array(
        'href' => $path_prefix . '/' . $walkthru_name . '/add',
        'title' => t('Add a Step to the Walkthru'),
      ),
    ),
  )) . '</ul>';

  $form['action_links'] = array(
    '#markup' => $action_links,
  );

  // Get all of the steps belonging to this walkthru
  $steps = db_select('walkthru_steps', 'ws')
    ->fields('ws')
    ->condition('walkthru_name', $walkthru_name)
    ->execute();
  foreach ($steps as $step) {
    $path = $path_prefix . '/' . $step->walkthru_name;

    $form['walkthru:'. $step->walkthru_name . ':' . $step->name] = array(
      '#weight' => $step->weight,
      'title' => array(
        '#markup' => $step->title,
      ),
      'weight' => array(
        '#type' => 'weight',
        '#default_value' => $step->weight,
      ),
      'name' => array(
        '#markup' => $step->name,
      ),
      // This is just to key off of in the theme function, so we don't render
      // elements that are not meant to be part of the table. There might be
      // a better way to handle this ??
      'step_name' => array(
        '#type' => 'hidden',
        '#value' => $step->name,
      ),
      'actions' => array(
        'edit' => array(
          '#type' => 'link',
          '#title' => t('Edit'),
          '#href' => $path . '/edit/' . $step->name,
          '#suffix' => ' | ',
        ),
        'delete' => array(
          '#type' => 'link',
          '#title' => t('Delete'),
          '#href' => $path . '/delete/' . $step->name,
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
 * Submit handler for walkthru_steps_manage_form()
 *
 * @see walkthru_steps_manage_form()
 * @ingroup forms
 */
function walkthru_steps_manage_form_submit(&$form, &$form_state) {
  foreach ($form_state['values'] as $key => $value) {
    if (stristr($key, 'walkthru:')) {
      db_merge('walkthru_steps')
        ->key(array('name' => $value['step_name']))
        ->fields(array('weight' => $value['weight']))
        ->execute();
    }
  }
  drupal_set_message(t('Your configuration has been saved.'));
}

// Theme the steps listing form.
function theme_walkthru_steps_manage_form($variables) {
  $form = $variables['form'];

  // Render our action links now so we can use them in multiple places.
  $action_links = drupal_render($form['action_links']);

  $output = $action_links;

  drupal_add_tabledrag('walkthru-steps-order', 'order', 'sibling', 'walkthru-steps-order-weight');

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

    if (isset($element['step_name'])) {
      // Build a list of operations.
      $actions = array(drupal_render($element['actions']));

      // Add special class to be used with tabledrag.js
      if (isset($element['weight'])) {
        $element['weight']['#attributes']['class'] = array('walkthru-steps-order-weight');
      }

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

  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'walkthru-steps-order')));
  $output .= $action_links;
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Form constructor for the walkthru step add/edit form.
 *
 * @param $walkthru
 *   The walkthru this step belongs to.
 *
 * @param $step
 *   The step we are editing.
 *
 * @see walkthru_step_add_form_validate()
 * @see walkthru_step_add_form_submit()
 * @ingroup forms
 */
function walkthru_step_add_form($form, &$form_state, $walkthru = NULL, $step = NULL) {
  if ($step) {
    // Unserialize the step's data and put it into the step as part of the object
    $step->step = unserialize($step->data);
  }

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Step Title'),
    '#default_value' => $step ? $step->title : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  $form['machine_name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine Name'),
    '#default_value' => $step ? $step->name : '',
    '#maxlength' => 30,
    '#description' => t('A unique name to construct the name for the step. It must only contain lowercase letters, numbers and hyphens.'),
    '#machine_name' => array(
      'exists' => 'walkthru_step_name_exists',
      'source' => array('title'),
      'label' => t('Machine Name'),
      'replace_pattern' => '[^a-z0-9-]+',
      'replace' => '-',
    ),
    '#disabled' => FALSE,
  );
  $form['desc'] = array(
    '#type' => 'text_format',
    '#title' => t('Body'),
    '#default_value' => $step ? $step->step['desc']['value'] : '',
    '#format' => NULL,
    '#rows' => 10,
    '#description' => t('The content of the step, plain text or html.'),
    '#required' => TRUE,
  );
  $form['attach'] = array(
    '#type' => 'textfield',
    '#title' => t('Attach To'),
    '#description' => t('Leave empty to center the step dialog to the window and display it without a tip,
      otherwise you can enter basic xpath synatx. <br /> e.g.: <strong>#my_element_id</strong>,
      <strong>.my_element_class</strong>, <strong>.my.element.classes</strong>, etc...'),
    '#default_value' => $step ? $step->step['attach'] : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
  );
  $form['position'] = array(
    '#type' => 'textfield',
    '#title' => t('Position'),
    '#description' => t('Use a 1 through 12 value, as in a clock hour handle.<br />
      Imagine the element you are attaching the step dialog to is in the center of the clock.'),
    '#default_value' => $step ? $step->step['position'] : '',
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
    '#description' => t('The step dialog\'s width in px, omit the "px" suffix'),
    '#default_value' => $step ? $step->step['width'] : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
  );
  $form['overlay'] = array(
    '#type' => 'checkbox',
    '#title' => t('Overlay'),
    '#description' => t('when checked, an overlay will cover the page'),
    '#default_value' => $step ? $step->step['overlay'] : 1,
    '#required' => FALSE,
  );
  $form['button_minimize_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimize Button Text'),
    '#description' => t('The text of the minimize button.'),
    '#default_value' => $step ? $step->step['button_minimize_text'] : 'Minimize',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
  );
  $form['button_previous_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Previous Button Text'),
    '#description' => t('The text of the previous button.'),
    '#default_value' => $step ? $step->step['button_previous_text'] : 'Previous',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
  );
  $form['button_next_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Next Button Text'),
    '#description' => t('The text of the next button.'),
    '#default_value' => $step ? $step->step['button_next_text'] : 'Next',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
  );
  $form['button_close_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Close Button Text'),
    '#description' => t('The text of the close button.'),
    '#default_value' => $step ? $step->step['button_close_text'] : 'Close',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
  );

  // These elements will be used to restrict a step to certain pages.
  $form['force_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL Requirement'),
    '#description' => t('Enter a url that this step is required to be on. If a user gets to this step
                        and they are not at the url then an error window will appear.  Ex. node/add/article or node/add/*'),
    '#default_value' => $step ? $step->step['force_url'] : '',
  );
  $form['force_url_error_message'] = array(
    '#type' => 'text_format',
    '#title' => t('Invalid URL Message'),
    '#default_value' => $step ? $step->step['force_url_error_message']['value'] : '',
    '#format' => NULL,
    '#rows' => 10,
    '#description' => t('The message to display if the user is at the wrong url.'),
  );

  // Only add the weight form element if it's an edit form.
  if (isset($step)) {
    $form['weight'] = array(
      '#type' => 'hidden',
      '#value' => $step->weight,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Step'),
  );
  return $form;
}

/**
 * Form validation handler for walkthru_step_add_form().
 *
 * @see walkthru_step_add_form_submit()
 */
function walkthru_step_add_form_validate($form, &$form_state) {
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

  if (!empty($form_state['values']['force_url'])) {
    if (!empty($form_state['values']['force_url_error_message']['value'])) {
      // Check for filter access
      if (!filter_access(filter_format_load($form_state['values']['force_url_error_message']['format']))) {
        form_set_error('force_url_error_message', t('You are not allowed to use the given format'));
      }
    }
    else {
      form_set_error('force_url_error_message', t('If you add a url you must specify an error message.'));
    }
  }
}

/**
 * Form submission handler for walkthru_step_add_form().
 *
 * @see walkthru_step_add_form_submit()
 */
function walkthru_step_add_form_submit($form, &$form_state) {
  $walkthru_name = $form_state['build_info']['args'][0]->name;
  $name  = $form_state['values']['machine_name'];
  $title = $form_state['values']['title'];

  // Set the weight to previous weight if it's there. Set it to show up after
  // the last step.
  if (isset($form_state['values']['weight'])) {
    $weight = $form_state['values']['weight'];
  }
  else {
    // Find out what the last step weight is.
    $result = db_select('walkthru_steps', 'ws')
      ->fields('ws', array('weight'))
      ->condition('walkthru_name', $walkthru_name, '=')
      ->orderBy('weight', 'DESC')
      ->range(0, 1)
      ->execute()
      ->fetchAssoc();

    // Add one to the weight to push the new one to the bottom.
    $weight = $result['weight'] + 1;
  }

  $data  = serialize($form_state['values']);

  // Set the form's data in the db
  db_merge('walkthru_steps')
    ->key(array('name' => $name))
    ->fields(array('walkthru_name' => $walkthru_name, 'title' => $title, 'data' => $data, 'weight' => $weight))
    ->execute();

  cache_clear_all('walkthru-' . $walkthru_name, 'cache_walkthru');

  // Adding a redirect so the form will return to the list page after submission
  $form_state['redirect'][] = 'admin/structure/walkthru/' . $walkthru_name;
}

/**
 * Checkes for the uniqueness of the step's name
 */
function walkthru_step_name_exists($machine_name) {
  // Get the step that has this maching name
  $result = walkthru_step_load(check_plain($machine_name));
  return !empty($result);
}

/**
 * Builds the step's confirm deletion page.
 *
 * @param $walkthru_step
 * The step we are about to delete.
 *
 * @return
 * The output html of this page.
 *
 */
function walkthru_step_delete_confirm($form, &$form_state, $walkthru, $walkthru_step) {
  $message = t('Are you sure you want to delete "%title"?', array('%title' => $walkthru_step->title));
  return confirm_form($form, $message, 'admin/structure/walkthru/' . $walkthru->name, NULL, t('Delete'));
}

/**
 * Deletes the walkthru step.
 */
function walkthru_step_delete_confirm_submit($form, &$form_state, $walkthru, $walkthru_step) {
  // Trying to delete from the db
  if (db_delete('walkthru_steps')->condition('name', $walkthru_step->name)->execute() === FALSE) {
    drupal_set_message(t('The delete action has failed due to not finding "%title"', array('%title' => $walkthru_step->title)));
  }
  else {
    drupal_set_message(t('The delete action was successful, "%title" is no more', array('%title' => $walkthru_step->title)));

    // Deleting this guider's pack data from the cache bin
    cache_clear_all('walkthru-' . $walkthru->name, 'cache_walkthru');
  }

  $form_state['redirect'][] = 'admin/structure/walkthru/' . $walkthru->name;
}
