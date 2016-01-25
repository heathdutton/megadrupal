<?php

/**
 * @file
 * This file contains all the access callback for admin page
 * and all other admin operations.
 */

/**
 * Delete List,  Confirm Form.
 */
function mark_as_read_delete_list_confirm($form, &$form_state, $list_id) {
  $list = mark_as_read_db_get_list_by_id($list_id);
  $form['#list_id'] = $list_id;
  return confirm_form($form,
    t('Are you sure you want to delete %title ?', array('%title' => $list->list_name)),
    MARK_AS_READ_ADMIN_URL,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Delete List confirm access callback.
 */
function mark_as_read_delete_list_confirm_submit($form, &$form_state) {
  mark_as_read_db_delete_list($form['#list_id']);
  $form_state['redirect'] = MARK_AS_READ_ADMIN_URL;
}

/**
 * Admin form to add/edit Mark as Read List/Menu.
 */
function mark_as_read_admin_add_edit_form($form, &$form_state, $edit_id) {

  if (!isset($edit_id)) {
    // Assume as ORM object.
    $list = new MarkAsReadListORM();
  }
  else {
    $list = new MarkAsReadListORM($edit_id);
  }

  // If there is already user entry for menu item then disable all the fields.
  $disabled = FALSE;
  if (!$list->isNew && count(mark_as_read_db_get_user_activity($edit_id))) {
    $disabled = TRUE;
  }

  $form['list_name'] = array(
    '#type' => 'textfield',
    '#title' => t('List/Menu Name'),
    '#required' => TRUE,
    '#description' => t('For Eg: MainMenu or User Navigation or Primary Menu'),
    '#default_value' => $list->list_name,
    '#disabled' => $disabled,
  );
  $form['css_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('Css Selector of the List/Menu Item'),
    '#required' => TRUE,
    '#description' => t('Css selector of the list/menu which items are marked as read on click. For eg. For main-menu items it would be:
    	<strong>ul.main-menu li a</strong> or <strong>ul.main-menu li span</strong>'),
    '#default_value' => $list->css_selector,
    '#disabled' => $disabled,
  );

  $form['attribute_name'] = array(
    '#type' => 'textfield',
    '#title'  => t('Attribute Name Of the List/Menu items that holds unique value. If empty, "href" will be used'),
    '#description'  => t('Name of the attribute that represents each list/menu item uniquely.
     <div><strong>Note: If anchor(a) tag is not the html element provided above, you can provide any custom or default attribute which represents item uniquely in the list. For eg, <em>data-id</em> or <em>id</em></strong></div>'),
    '#default_value'  => $list->attribute_name,
    '#disabled' => $disabled,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['list'] = array(
    '#type' => 'value',
    '#value' => $list,
  );
  $form['disabled'] = array(
    '#type' => 'value',
    '#value' => $disabled,
  );
  return $form;
}

/**
 * Admin settings form.
 */
function mark_as_read_admin_settings_form() {
  $form = array();
  $form['mark_as_read_html_behaviour'] = array(
    '#type' => 'fieldset',
    '#title' => t('HTML Behaviour') ,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['mark_as_read_html_behaviour']['mark_as_read_make_bold'] = array(
    '#type' => 'checkbox',
    '#title' => t('Set unread item bold by default'),
    '#description' => t('Other than adding the class <strong> @mark_as_read_class / @mark_as_unread_class</strong>, it makes the list item bold if its unread by default.', array(
      '@mark_as_read_class' => MARK_AS_READ_READ_DEFAULT_CLASS,
      '@mark_as_unread_class' => MARK_AS_READ_UNREAD_DEFAULT_CLASS,
      )
    ),
    '#default_value' => variable_get('mark_as_read_make_bold', 1),
  );
  $form['mark_as_read_html_behaviour']['mark_as_read_unread_class'] = array(
    '#type' => 'textfield',
    '#title' => t('CSS class for Unread/Un-Visited HTML element'),
    '#description' => t('If empty <strong>@mark_as_unread_class</strong> is used.', array('@mark_as_unread_class' => MARK_AS_READ_UNREAD_DEFAULT_CLASS)),
    '#default_value' => variable_get('mark_as_read_unread_class'),
  );

  $form['mark_as_read_html_behaviour']['mark_as_read_read_class'] = array(
    '#type' => 'textfield',
    '#title' => t('CSS class for Read/Visited HTML Element'),
    '#description' => t('If empty <strong>@mark_as_read_class</strong> is used.', array('@mark_as_read_class' => MARK_AS_READ_READ_DEFAULT_CLASS)),
    '#default_value' => variable_get('mark_as_read_read_class'),
  );
  $form['mark_as_read_access'] = array(
    '#type' => 'fieldset',
    '#title' => t('Access conditions'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $user_roles = user_roles();
  // Remove anonymous user and authenticated user.
  unset($user_roles[1]);
  unset($user_roles[2]);
  $form['mark_as_read_access']['mark_as_read_user_roles'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#options' => $user_roles,
    '#title' => t('User roles not to apply.'),
    '#description' => t('Javascript will be loaded for every loggedin users regardless their roles. For optimization purpose its good to select user roles if it does not apply to any.
      This will have precedence over the page list provided below.'),
    '#default_value' => variable_get('mark_as_read_user_roles', array('3')),
  );
  $form['mark_as_read_access']['mark_as_read_list_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Render Javascript on all the pages specified'),
    '#attributes' => array('rows' => 3),
    '#description' => t('List of all the pages where Javascript will be rendered based on the List Item/Menu Item provided. If empty, Javscript will be loaded on all the pages other than list of pages provided below.
      This will have precedence over pages provide below.<div><em>Specify pages by using their paths. Enter one path per line. The \'*\' character is a wildcard.
      Example paths are blog for the blog page and blog/* for every personal blog. is the front page.</em></div>'),
    '#default_value' => variable_get('mark_as_read_list_pages'),
  );
  $form['mark_as_read_access']['mark_as_read_except_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Render Javascript on all the pages except'),
    '#attributes' => array('rows' => 3),
    '#description' => t("List all the pages where we don\'t require to load Javascript file. This can be only used for optimization as it does not have any
      impact on the behaviour of MARK AS READ. Pages defined in the individual list will have precedence over pages provide here.<div>
      <em>Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard.
      Example paths are blog for the blog page and blog/* for every personal blog. is the front page.</em></div>"),
    '#default_value' => variable_get('mark_as_read_except_pages'),
  );
  $form['mark_as_read_logging'] = array(
    '#type' => 'fieldset',
    '#title' => t('Logging'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['mark_as_read_logging']['mark_as_read_enable_logging'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log read activity in the watchdog.'),
    '#description' => t('If unread html element is clicked, then it will log the data with attribute name and its value to the watchdog.'),
    '#default_value' => variable_get('mark_as_read_enable_logging', 0),
  );
  return system_settings_form($form);
}

/**
 * Default page callback which lists all the available lists.
 */
function mark_as_read_admin_list_form() {
  $form = array();

  //Get all the lists
  // Existing columns.
  $items = mark_as_read_db_get_all_lists();
  // @todo Add Info.
  $form['lists']['existing'] = array(
    '#markup' => theme('mark_as_read_lists_table', array('items' => $items)),
  );
  return $form;
}

/**
 * Validate callback.
 *
 * Check if submitted list name already exists or not. It is used as the unique
 * identifier in the features export/import.
 */
function mark_as_read_admin_add_edit_form_validate(&$form, $form_state) {

  // Throw error only if list name already exists, but when we edit,
  // check if list name has changed or not, otherwise we will always
  // get this error while editing.
  if (mark_as_read_db_get_list_by_name($form_state['values']['list_name']) && $form_state['values']['list_name'] != $form_state['values']['list']->list_name) {
    form_set_error('list_name', t('List name already exists.'));
  }
}

/**
 * Submit Handler for List add/edit form.
 */
function mark_as_read_admin_add_edit_form_submit(&$form, &$form_state) {

  $list = $form_state['values']['list'];
  $disabled = $form_state['values']['disabled'];

  // If disabled, then $list object will have all the value already in it.
  if (!$disabled) {
    $list->list_name = $form_state['values']['list_name'];
    $list->css_selector = $form_state['values']['css_selector'];
    $list->attribute_name = $form_state['values']['attribute_name'];
  }

  // Provide default attribute name if empty.
  if (empty($list->attribute_name)) {
    $list->attribute_name = MARK_AS_READ_DEFAULT_ATTRIBUTE_NAME;
  }
  if ($list->save()) {
    drupal_set_message(t('Data for <em>@name</em> successfully saved.', array('@name' => $list->list_name)));
  }

  $form_state['redirect'] = MARK_AS_READ_ADMIN_URL;
}

/**
 * Theme callback.
 *
 * Generate the tabular theme to list all the lists.
 */
function theme_mark_as_read_lists_table($variables) {
  $header = array();
  $header[] = array('data' => t('Name'));
  $header[] = array('data' => t('Css Selector'));
  $header[] = array('data' => t('Attribute Name'));
  $header[] = array('data' => t('Operations'), 'colspan' => 2);

  $row = array();
  $i = 0;
   foreach ($variables['items'] as $item) {
    $row[$i][] = check_plain($item->list_name);
    $row[$i][] = check_plain($item->css_selector);
    $row[$i][] = check_plain($item->attribute_name);
    $row[$i][] = l(t('Edit'), MARK_AS_READ_ADMIN_URL . "/list/{$item->list_id}/edit");
    $row[$i][] = l(t('Delete'), MARK_AS_READ_ADMIN_URL . "/list/{$item->list_id}/delete");
    $i++;
  }

  return theme('table', array(
    'header' => $header,
    'rows' => $row,
    'empty' => t('There are no list available. Please add the new list from the link above.'),
  ));
}
