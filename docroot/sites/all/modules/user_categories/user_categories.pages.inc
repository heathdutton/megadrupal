<?php
/**
 * @file
 * Category entity administration pages.
 */

/**
 * List all categories for administration.
 */
function user_categories_manage() {
  $categories = _user_categories();
  $items = array();
  foreach ($categories as $category) {
    if ($category['name'] == 'account') {
      continue;
    }
    $items[] = l($category['title'], 'admin/config/people/categories/' . $category['name']);
  }
  $build[] = array();
  $build[] = array(
    '#theme' => 'item_list',
    '#items' => $items,
  );
  $build[] = drupal_get_form('user_categories_edit_category');
  return $build;
}

/**
 * Category configuration form
 * used for both creating and editing a category
 *
 * @param string $category
 *  category machine name
 */
function user_categories_edit_category($form, &$form_state, $category = NULL) {
  $categories = variable_get('user_categories_list', array());
  $edit['categories'] = array(
    '#type' => 'value',
    '#value' => $categories,
  );
  $edit['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Machine name'),
    '#description' => t('Category machine name'),
    '#required' => TRUE,
  );
  $edit['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('Category title'),
    '#required' => TRUE,
  );
  $edit['weight'] = array(
    '#type' => 'textfield',
    '#title' => t('Weight'),
    '#default_value' => count($categories),
    '#required' => TRUE,
    '#description' => t('Category order'),
  );
  $edit['save'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );
  if ($category) {
    $edit['name']['#default_value'] = $category;
    $edit['name']['#disabled'] = TRUE;
    $edit['title']['#default_value'] = $categories[$category]['title'];
    $edit['weight']['#default_value'] = $categories[$category]['weight'];
    $edit['save']['#value'] = t('Save');
    $edit['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('user_categories_delete_category'),
    );
    if (!is_array($form)) {
      $form = array();
    }
    $form += $edit;
  }
  else {
    $form['edit'] = array(
      '#title' => t('Add new category'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
    );
    $form['edit'] += $edit;
  }
  return $form;
}

/**
 * sets categories list variable.
 * invokes hook_entity_info to refresh  bundles,
 * and hook_menu to build tabs under user/edit
 */
function user_categories_edit_category_submit($form, &$form_state) {
  $categories = $form_state['values']['categories'];
  $new_cat = $form_state['values'];
  $name = $form_state['values']['name'];
  $categories[$name] = array(
    'name' => $new_cat['name'],
    'title' => $new_cat['title'],
    'weight' => $new_cat['weight'],
  );
  variable_set('user_categories_list', $categories);
  field_cache_clear();
  menu_cache_clear_all();
  menu_rebuild();
}

/**
 * deleting a category by unseting it from categories variable
 * also invokes entity info and rebuilds menu
 */
function user_categories_delete_category($form, &$form_state) {
  $categories = $form_state['values']['categories'];
  $name = $form_state['values']['name'];
  $weight = $form_state['values']['weight'];
  unset($categories[$name]);
  foreach ($categories as $name => &$category) {
    if ($category['weight'] > $weight) {
      $category['weight']--;
    }
  }
  variable_set('user_categories_list', $categories);
  field_attach_delete_bundle('user_category', $name);
  field_cache_clear();
  menu_cache_clear_all();
  menu_rebuild();
  $form_state['redirect'] = 'admin/config/people/categories';
}

/**
 * Configure a category entity
 *
 * @param string $category
 *  category machine name
 */
function user_categories_configure($category) {
  $categories = variable_get('user_categories_list', array());
  if (isset($categories[$category])) {
    return drupal_get_form('user_categories_edit_category', $category);
  }
  else {
    $format = array('@category' => check_plain($category));
    $message = t('Unmanaged category : @category', $format);
    return array('#markup' => $message);
  }
}
