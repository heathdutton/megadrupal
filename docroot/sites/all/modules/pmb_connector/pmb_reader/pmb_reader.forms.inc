<?php

/**
 * @file
 * PMB reader forms.
 */

/**
 * Manage reader cart form.
 *
 * @ingroup forms
 * @see pmb_reader_cart_form_validate()
 * @see pmb_reader_cart_form_submit()
 */
function pmb_reader_cart_form($form, &$form_state, $notices) {
  $form['notices_ids'] = array(
    '#type'  => 'value',
    '#value' => $notices,
  );

  $options = array();
  foreach ($notices as $anotice_id => $notice_content) {
    $options[$anotice_id] = '';
  }

  $form['checked_notices'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => array(),
  );

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['delete_selected'] = array(
    '#type' => 'submit',
    '#value' => t('Remove selected elements from cart'),
    '#name' => 'delete_selected',
  );

  $form['actions']['empty_cart'] = array(
    '#type' => 'submit',
    '#value' => t('Empty cart'),
    '#name' => 'empty_cart',
  );

  return $form;
}

/**
 * @see pmb_reader_cart_form()
 */
function pmb_reader_cart_form_validate($form, &$form_state) {
  switch ($form_state['triggering_element']['#name']) {
    case 'delete_selected' :
      if (!array_sum($form_state['values']['checked_notices'])) {
        form_set_error('title', t('You must select records before removing.'));
      }
      return;
    case 'empty_cart' :
      return;
    default:
      form_set_error('title', t('Unknown operation'));
  }
}

/**
 * @see pmb_reader_cart_form()
 */
function pmb_reader_cart_form_submit($form, &$form_state) {
  global $user;

  require_once(drupal_get_path('module', 'pmb_reader') . '/pmb_reader.cart.inc');

  switch ($form_state['triggering_element']['#name']) {
    case 'delete_selected' :
      $notices_ids = array();
      foreach ($form_state['values']['checked_notices'] as $anotice_id => $value) {
        if ($value) {
          $notices_ids[] = $anotice_id;
        }
      }
      pmb_reader_delete_notices_from_cart($user, $notices_ids);

      return;
    case 'empty_cart' :
      pmb_reader_empty_cart($user);
      return;
  }
}

/**
 * Add a notice to reader cart form.
 *
 * @ingroup forms
 * @see pmb_reader_add_notice_to_cart_form_validate()
 * @see pmb_reader_add_notice_to_cart_form_submit()
 */
function pmb_reader_add_notice_to_cart_form($form, &$form_state, $notice_id) {
  $form['notice_id'] = array(
    '#type'  => 'value',
    '#value' => $notice_id,
  );

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['add_notice_to_cart'] = array(
    '#type' => 'submit',
    '#value' => t('Add notice to cart'),
    '#name' => 'add_notice_to_cart',
  );

  return $form;
}

/**
 * @see pmb_reader_add_notice_to_cart_form()
 */
function pmb_reader_add_notice_to_cart_form_validate($form, &$form_state) {
  if ($form_state['triggering_element']['#name'] == 'add_notice_to_cart') {
    if ($form_state['values']['notice_id'] == 0)
      form_set_error('title', t('Blank record id.'));
    return;
  }
  form_set_error('title', t('Could not understand query.'));
}

/**
 * @see pmb_reader_add_notice_to_cart_form()
 */
function pmb_reader_add_notice_to_cart_form_submit($form, &$form_state) {
  global $user;

  require_once(drupal_get_path('module', 'pmb_reader') . '/pmb_reader.cart.inc');
  $notices_ids = array($form_state['values']['notice_id']);
  pmb_reader_add_notices_to_cart($user, $notices_ids);
  drupal_goto(_pmb_p('reader/') . $user->uid . _pmb_p('/cart'));
}

/**
 * Add a notice to reader reading list form.
 *
 * @ingroup forms
 * @see pmb_reader_add_cart_to_reading_list_form_validate()
 * @see pmb_reader_add_cart_to_reading_list_form_submit()
 */
function pmb_reader_add_cart_to_reading_list_form($form, &$form_state, $list_id) {
  global $user;

  $pmb_data = new pmb_data();
  if (!$pmb_data->set_reader($user)) {
    return FALSE;
  }

  $form['list_id'] = array(
    '#type'  => 'value',
    '#value' => $list_id,
  );

  $cart_content = $pmb_data->reader_get_cart();

  $form['cart_content'] = array(
    '#type'  => 'value',
    '#value' => $cart_content,
  );

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['add_cart_to_reading_list'] = array(
    '#type' => 'submit',
    '#value' => t('Add your cart to this reading list'),
    '#name' => 'add_cart_to_reading_list',
    '#disabled' => !($cart_content && count($cart_content)),
  );

  return $form;
}

/**
 * @see pmb_reader_add_cart_to_reading_list_form()
 */
function pmb_reader_add_cart_to_reading_list_form_validate($form, &$form_state) {
  if ($form_state['triggering_element']['#name'] == 'add_cart_to_reading_list') {
    if ($form_state['values']['list_id'] == 0)
      form_set_error('title', t('Blank reading list.'));
    return;
  }
  form_set_error('title', t('Could not understand query.'));
}

/**
 * @see pmb_reader_add_cart_to_reading_list_form()
 */
function pmb_reader_add_cart_to_reading_list_form_submit($form, &$form_state) {
  global $user;

  require_once(drupal_get_path('module', 'pmb_reader') . '/pmb_reader.reading_lists.inc');
  $list_id = $form_state['values']['list_id'];
  pmb_reader_add_notices_to_reading_list($user, $list_id, $form_state['values']['cart_content']);
  drupal_goto(_pmb_p('reader/') . $user->uid . _pmb_p('/reading_list/') . $list_id);
}

/**
 * Manage reader reading list form.
 *
 * @ingroup forms
 * @see pmb_reader_reading_list_form_validate()
 * @see pmb_reader_reading_list_form_submit()
 */
function pmb_reader_reading_list_form($form, &$form_state, $list, $notices) {
  global $user;

  $form['reading_list'] = array(
    '#type'  => 'value',
    '#value' => $list,
  );

  $form['notices_ids'] = array(
    '#type'  => 'value',
    '#value' => $notices,
  );

  $options = array();
  foreach ($notices as $anotice_id => $notice_content) {
    $options[$anotice_id] = '';
  }

  $form['checked_notices'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => array(),
  );

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['delete_selected'] = array(
    '#type' => 'submit',
    '#value' => t('Remove selected elements from list'),
    '#name' => 'delete_selected',
    '#disabled' => !($notices && count($notices)),
  );

  $form['actions']['empty_list'] = array(
    '#type' => 'submit',
    '#value' => t('Empty list'),
    '#name' => 'empty_list',
    '#disabled' => !($notices && count($notices)),
  );


  $pmb_data = new pmb_data();
  if ($pmb_data->set_reader($user)) {
    $cart_content = $pmb_data->reader_get_cart();

    $form['cart_content'] = array(
      '#type'  => 'value',
      '#value' => $cart_content,
    );

    $form['actions']['add_cart_to_list'] = array(
      '#type' => 'submit',
      '#value' => t('Add the content of your cart to this list'),
      '#name' => 'add_cart_to_list',
      '#disabled' => !($cart_content && count($cart_content)),
    );
  }

  return $form;
}

/**
 * @see pmb_reader_reading_list_form()
 */
function pmb_reader_reading_list_form_validate($form, &$form_state) {
  switch ($form_state['triggering_element']['#name']) {
    case 'delete_selected' :
      if (!array_sum($form_state['values']['checked_notices'])) {
        form_set_error('title', t('You must select records to remove.'));
      }
      return;
    case 'empty_list' :
      return;
    case 'add_cart_to_list' :
      return;
    default:
      form_set_error('title', t('Unknown operation'));
  }
}

/**
 * @see pmb_reader_reading_list_form()
 */
function pmb_reader_reading_list_form_submit($form, &$form_state) {
  global $user;

  require_once(drupal_get_path('module', 'pmb_reader') . '/pmb_reader.reading_lists.inc');

  switch ($form_state['triggering_element']['#name']) {
    case 'delete_selected' :
      $notices_ids = array();
      foreach ($form_state['values']['checked_notices'] as $anotice_id => $value) {
        if ($value) {
          $notices_ids[] = $anotice_id;
        }
      }
      pmb_reader_remove_notices_from_reading_list($user, $form_state['values']['reading_list']->reading_list_id, $notices_ids);
      return;
    case 'add_cart_to_list' :
      $list_id = $form_state['values']['reading_list']->reading_list_id;
      pmb_reader_add_notices_to_reading_list($user, $list_id, $form_state['values']['cart_content']);
      return;
    case 'empty_list' :
      pmb_reader_empty_reading_list($user, $form_state['values']['reading_list']->reading_list_id);
      return;
  }
}

/**
 * Add/edit reader suggestion form.
 *
 * @ingroup forms
 * @see pmb_reader_suggestion_edit_form_validate()
 * @see pmb_reader_suggestion_edit_form_submit()
 */
function pmb_reader_suggestion_edit_form($form, &$form_state, $suggestion, $categ_and_sources) {
  if (is_object($suggestion)) {
    $can_edit = ($suggestion->sugg_state == 1) || !$suggestion->sugg_id;
    $can_delete = $suggestion->sugg_id;
  }
  else {
    $can_edit = TRUE;
    $can_delete = FALSE;
  }

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#size' => 50,
    '#maxlength' => 255,
    '#default_value' => isset($suggestion->sugg_title) ? $suggestion->sugg_title : '',
    '#description' => t('The title of the document'),
    '#required' => TRUE,
    '#disabled' => !$can_edit,
  );
  $form['author'] = array(
    '#type' => 'textfield',
    '#title' => t('Author'),
    '#size' => 50,
    '#maxlength' => 255,
    '#default_value' => isset($suggestion->sugg_author) ? $suggestion->sugg_author : '',
    '#description' => t('The author of the document'),
    '#required' => TRUE,
    '#disabled' => !$can_edit,
  );
  $form['publisher'] = array(
    '#type' => 'textfield',
    '#title' => t('Publisher'),
    '#size' => 50,
    '#maxlength' => 255,
    '#default_value' => isset($suggestion->sugg_editor) ? $suggestion->sugg_editor : '',
    '#description' => t('The publisher of the document'),
    '#required' => TRUE,
    '#disabled' => !$can_edit,
  );
  $form['barcode'] = array(
    '#type' => 'textfield',
    '#title' => t('ISBN / Bar code'),
    '#size' => 50,
    '#maxlength' => 255,
    '#default_value' => isset($suggestion->sugg_barcode) ? $suggestion->sugg_barcode : '',
    '#description' => t('The bar code of the document'),
    '#disabled' => !$can_edit,
  );
  $form['price'] = array(
    '#type' => 'textfield',
    '#title' => t('Price'),
    '#size' => 50,
    '#maxlength' => 255,
    '#default_value' => isset($suggestion->sugg_price) ? $suggestion->sugg_price : '',
    '#description' => t('The price of the document'),
    '#disabled' => !$can_edit,
  );
  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#size' => 50,
    '#maxlength' => 255,
    '#default_value' => isset($suggestion->sugg_url) ? $suggestion->sugg_url : '',
    '#description' => t('The website of the document'),
    '#disabled' => !$can_edit,
  );
  $form['date_pub'] = array(
    '#type' => 'textfield',
    '#title' => t('Publication date'),
    '#size' => 50,
    '#maxlength' => 255,
    '#default_value' => isset($suggestion->sugg_date) ? $suggestion->sugg_date : '',
    '#description' => t('The date of the document'),
    '#disabled' => !$can_edit,
  );
  $form['comment'] = array(
    '#type' => 'textarea',
    '#base_type' => 'textarea',
    '#title' => t('Comment'),
    '#default_value' => isset($suggestion->sugg_comment) ? $suggestion->sugg_comment : '',
    '#format' => NULL,
    '#description' => t('The comment of the suggestion'),
    '#disabled' => !$can_edit,
  );
  $form['source'] = array(
    '#type' => 'select',
    '#title' => t('Source'),
    '#default_value' => isset($suggestion->sugg_source) ? $suggestion->sugg_source : 0,
    '#description' => t('The source of the suggestion'),
    '#disabled' => !$can_edit,
  );
  $form['source']['#options'] = array();
  $form['source']['#options'][0] = t('No source');
  if (!empty($categ_and_sources)) {
    foreach ($categ_and_sources->sources as $asource) {
      $form['source']['#options'][$asource->source_id] = $asource->source_caption;
    }
  }

  $form['category'] = array(
    '#type' => 'select',
    '#title' => t('Category'),
    '#default_value' => isset($suggestion->sugg_category) ? $suggestion->sugg_category : 0,
    '#description' => t('The category of the suggestion'),
    '#disabled' => !$can_edit,
  );
  $form['category']['#options'] = array();
  $form['category']['#options'][0] = t('No category');
  if (!empty($categ_and_sources)) {
    foreach ($categ_and_sources->categories as $acateg) {
      $form['category']['#options'][$acateg->category_id] = $acateg->category_caption;
    }
  }

  $form['suggestion_id'] = array(
    '#type' => 'hidden',
    '#value' => isset($suggestion->sugg_id) ? $suggestion->sugg_id : 0,
  );

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#disabled' => !$can_edit,
  );
  if ($can_delete) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#disabled' => !$can_edit,
    );
  }
  return $form;
}

/**
 * @see pmb_reader_suggestion_edit_form()
 */
function pmb_reader_suggestion_edit_form_validate($form, &$form_state) {
  if ($form_state['values']['title'] == '') {
    form_set_error('title', t('You must enter a title.'));
  }
  if ($form_state['values']['author'] == '') {
    form_set_error('title', t('You must enter an author.'));
  }
  if ($form_state['values']['publisher'] == '') {
    form_set_error('title', t('You must enter a publisher.'));
  }
}

/**
 * @see pmb_reader_suggestion_edit_form()
 */
function pmb_reader_suggestion_edit_form_submit($form, &$form_state) {
  global $user;

  $suggestion = array(
    'sugg_id' => $form_state['values']['suggestion_id'],
    'sugg_title' => $form_state['values']['title'],
    'sugg_author' => $form_state['values']['author'],
    'sugg_editor' => $form_state['values']['publisher'],
    'sugg_barcode' => $form_state['values']['barcode'],
    'sugg_price' => $form_state['values']['price'],
    'sugg_comment' => $form_state['values']['comment'],
    'sugg_url' => $form_state['values']['url'],
    'sugg_date' => $form_state['values']['date_pub'],
    'sugg_source' => $form_state['values']['source'],
    'sugg_category' => $form_state['values']['category'],
  );

  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);
  if ($form_state['triggering_element']['#id'] == 'edit-delete') {
    if ($suggestion['sugg_id']) {
      $result = $pmb_data->reader_delete_suggestion($suggestion['sugg_id']);

      if ($result) {
        drupal_set_message(t('Your suggestion has been deleted.'));
      }
      else {
        drupal_set_message(t('Your suggestion has not been deleted.'), 'error');
      }
    }
    drupal_goto(_pmb_p('reader/') . $user->uid . _pmb_p('/suggestion'));
  }
  else {
    $result = (!$suggestion['sugg_id']) ?
      $pmb_data->reader_add_suggestion($suggestion) :
      $pmb_data->reader_edit_suggestion($suggestion);

    if ($result) {
      drupal_set_message(t('Your suggestion has been saved.'));
    }
    else {
      drupal_set_message(t('Your suggestion has not been saved.'), 'error');
    }
    drupal_goto(_pmb_p('reader/') . $user->uid . _pmb_p('/suggestion'));
  }
}
