<?php
/**
 * @file
 * Necessary functionality for managing the front page.
 */

/**
 * A form to allow administrators to set an entity as a front page.
 */
function set_front_page_front_form($form, &$form_state, $entity, $entity_type) {
  // The current front page; defaults to the standard core 'node' page.
  $front_page = base_path() . variable_get('site_frontpage', 'node');

  // Work out the URL of the current page.
  $uri = entity_uri($entity_type, $entity);
  $this_page = $uri['path'];
  $this_page_alias = url($uri['path'], $uri['options']);

  // Load the default page.
  $default = variable_get('set_front_page_default', '');
  $default_front_path = !empty($default) ? $default : 'Not set';

  // Indicate if the current page is set as the front page, but only when the
  // form is being displayed, not when it is being processed - this will avoid
  // displaying the message twice when it's being assigned.
  if (empty($form_state['input']) && ($front_page == $this_page || $front_page == $this_page_alias)) {
    drupal_set_message(t('This page is being used as the front page.'));
  }

  $form['new_url'] = array(
    '#type' => 'value',
    '#value' => $this_page,
  );

  $header = array('Pages', 'Path');
  $pages = array(
    '0' => array(t('Current front page'), $front_page),
    '1' => array(t('Current page'), $this_page_alias),
    '2' => array(t('Default front page'), $default_front_path),
  );
  $page_table = theme('table', array('header' => $header, 'rows' => $pages));

  $form['page_table'] = array(
    '#markup' => $page_table,
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['change_to_new'] = array(
    '#type' => 'submit',
    '#value' => t('Use this page as the front page'),
    '#disabled' => ($front_page == $this_page || $front_page == $this_page_alias),
  );

  if (!empty($default)) {
    $form['actions']['change_to_default'] = array(
      '#type' => 'submit',
      '#value' => t('Revert to the default page'),
      '#disabled' => ($front_page == $default || $default == $this_page),
    );
  }

  return $form;
}

/**
 * Submit handler for set_front_page_node_front_form.
 */
function set_front_page_front_form_submit($form, &$form_state) {
  $new_url = '';

  // Work out which button was clicked.
  $button_label = $form_state['clicked_button']['#value'];

  // The 'reset' button was clicked.
  if ($button_label == t('Revert to the default page')) {
    $new_url = variable_get('set_front_page_default', '');
  }

  // The 'update' button was clicked.
  elseif ($button_label == t('Use this page as the front page')) {
    $new_url = $form_state['values']['new_url'];
  }

  if (empty($new_url)) {
    form_set_error('', 'Unable to change the front page.');
  }
  else {
    // Set as front page.
    variable_set('site_frontpage', $new_url);
    drupal_set_message(t('The front page has been set to @url.', array('@url' => url($new_url))));

    // Clear the front page cache.
    cache_clear_all(url('<front>', array('absolute' => TRUE)), 'cache_page');
  }
}
