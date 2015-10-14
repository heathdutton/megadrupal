<?php

/**
 * @file
 * Administrative pages and callbacks.
 */

/**
 * Overview of all of the current categories in the system.
 */
function book_library_category_admin_overview() {
  $rows = array();

  $headers = array(t('Category'), t('Operations'));

  // Add any categories to the list.
  foreach (book_library_get_categories() as $category => $name) {
    $rows[] = array(check_plain($name), l(t('edit'), 'admin/content/book/category/' . $category));
  }

  return theme('table', array('header' => $headers, 'rows' => $rows, 'empty' => t('No categories available.')))
    . l(t('Add Category'), 'admin/content/book/category/add', array('query' => drupal_get_destination()));
}

/**
 * Form for adding or editing a category.
 */
function book_library_category_admin_edit($form, $form_state, $category = NULL) {
  $form = array();

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t("Category"),
    '#required' => TRUE,
    '#maxlength' => 255,
    '#default_value' => $category ? $category->name : NULL,
  );
  $form['category'] = array(
    '#type' => 'machine_name',
    '#title' => t("Machine Name"),
    '#required' => TRUE,
    '#maxlength' => 255,
    '#default_value' => $category ? $category->category : NULL,
    '#machine_name' => array(
      'exists' => 'book_library_category_load',
      'source' => array('name'),
    ),
  );
  if ($category) {
    $form += book_library_manage_books_form(array(), $form_state, $category);
    $form['books']['#type'] = 'fieldset';
    $form['books']['#title'] = t('Associated Books');
    unset($form['submit']);
  }
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t("Save"),
  );
  $form['#library_category'] = $category;
  return $form;
}

/**
 * Theme the category administrative form.
 */
function theme_book_library_category_admin_edit(&$vars) {
  if ($vars['form']['#library_category']) {
    $vars['books_only'] = TRUE;
    $vars['form']['books']['table'] = array(
      '#type' => 'markup',
      '#markup' => theme_book_library_manage_books_form($vars),
    );
    return drupal_render($vars['form']);
  }
}

function book_library_category_admin_edit_submit($form, &$form_state) {
  $category = $form['#library_category'];
  // Category machine_name changed. Update our mappings.
  if ($category && $form_state['values']['category'] != $category->category) {
    db_update('book_library')
      ->fields(array('category' => $form_state['values']['category']))
      ->condition('category', $category->category)
      ->execute();
  }

  // Update the title of the category.
  db_merge('book_library_category')
    ->key(array('category' => $category ? $category->category : $form_state['values']['category']))
    ->fields(array(
      'name' => $form_state['values']['name'],
      'category' => $form_state['values']['category'],
    ))
    ->execute();

  drupal_set_message(t('Successfully saved category.'));

  $form_state['redirect'] = 'admin/content/book/category/' . $form_state['values']['category'];

  // Handle the submission of the list form.
  book_library_manage_books_form_submit($form, $form_state);
}
