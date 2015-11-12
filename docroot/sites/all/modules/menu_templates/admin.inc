<?php
/**
 * @file
 * This file must have administrative functions for menu_templates
 */

/**
 * Lists the menu templates page
 * @return type string with the html to be rendered
 */
function menu_templates_list_page() {
  $templates = menu_templates_get_template_list();

  if (empty($templates)) {
    return t('There are no templates yet. !create_link', array(
      '!create_link' => l(t('Create one!'), 'admin/structure/menu-templates/add')
    ));
  }

  $table = array(
    '#theme' => 'table',
    '#header' => array(t('Name'), t('Menu'), t('Operations')),
    '#rows' => array(),
  );

  $menu_names = _menu_templates_get_menus();

  foreach ($templates as $mtid => $template) {
    $menu_name = $menu_names[$template->mid];

    $links = array(
      0 => l(t('Edit'), "admin/structure/menu-templates/$mtid/edit"),
      1 => l(t('Delete'), "admin/structure/menu-templates/$mtid/delete"),
    );

    $table['#rows'][] = array(
      $template->name,
      $menu_name,
      implode(' - ', $links),
    );
  }

  return '<p>' . l(t('Create new template'), 'admin/structure/menu-templates/add') . '</p>'
    . drupal_render($table);
}

/**
 * Returns a new menu template to be filled.
 * @return type Drupal Form API
 */
function menu_templates_new_page() {
  $form = drupal_get_form('menu_templates_form');

  return $form;
}

/**
 * Returns a page to edit a menu template.
 * @param type $mtid menu template ID
 * @return type Drupal Form API
 */
function menu_templates_edit_page($mtid) {
  $form = drupal_get_form('menu_templates_form', $mtid);

  return $form;
}

/**
 * Delete a menu template upon confirmation.
 * @param type $mtid menu template ID
 * @return type Drupal Form API
 */
function menu_templates_delete_page($mtid) {
  $template = menu_templates_get_template_details(array('mtid' => $mtid));

  if (!$template) {
    drupal_not_found();
  }

  $confirm = drupal_get_form('menu_templates_delete_confirm', $template);

  return $confirm;
}

/**
 * Implements hook_form
 * This is our menu template form to be filled in order to create a new template.
 */
function menu_templates_form($form, &$form_state, $mtid = NULL) {
  if (NULL !== $mtid) {
    // Pull template details from database
    $template = menu_templates_get_template_details(array('mtid' => $mtid));

    if (!$template) {
      // Are we editing a non existing template?!
      drupal_goto('admin/structure/menu-templates');
    }

    $form_state['menu_template'] = $template;
  }

  $form['instructions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Instructions'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['instructions']['guidelines'] = array(
    '#markup' => t('<p>You must include the entire markup for the menu items, as
      well as the wrappers.</p>
      <p>The Wrapper field customizes the HTML that goes around the menu tree
      (menu items). The Template field customizes the HTML that goes around the
      menu items (menu links).</p>
      <p>The examples below show how to create a template with the same output
      as Drupal\'s default</p>
      <p>
        <strong>Wrapper</strong><br />
        <code>@wrapper_code</code>
      </p>
      <p>
        <strong>Template</strong><br />
        <code>@template_code</code>
      </p>', array(
        '@wrapper_code' => '<ul class="menu">!tree</ul>',
        '@template_code' => '<li !attributes>!link !submenu</li>',
      )
    )
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#required' => TRUE,
    '#default_value' => isset($template["name"]) ? $template["name"] : '',
  );

  $form['mid'] = array(
    '#type' => 'select',
    '#title' => t('Menu to render'),
    '#required' => TRUE,
    '#default_value' => isset($template["mid"]) ? $template["mid"] : '',
    '#options' => _menu_templates_get_menus(),
  );

  // Create 3 groups of Wrapper and Template fields
  for ($i = 1; $i <= 3; $i++) {
    $form["l${i}_group"] = array(
      '#type' => 'fieldset',
      '#title' => t("Level $i"),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form["l${i}_group"]["l${i}_wrapper"] = array(
      '#type' => 'textarea',
      '#title' => t("Level $i wrapper"),
      '#description' => t("Available tokens: !tokens",
        array('!tokens' => "!tree")),
      '#required' => ($i == 1)?TRUE:FALSE,
      '#default_value' => isset($template["l${i}_wrapper"]) ? $template["l${i}_wrapper"] : '',
      '#rows' => 3,
    );
    $form["l${i}_group"]["l${i}_template"] = array(
      '#type' => 'textarea',
      '#title' => t("Level $i template"),
      '#description' => t("Available tokens: !tokens",
        array('!tokens' => "!link !attributes !submenu")),
      '#required' => ($i == 1)?TRUE:FALSE,
      '#default_value' => isset($template["l${i}_template"]) ? $template["l${i}_template"] : '',
      '#rows' => 3,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Implements hook_form_validate().
 */
function menu_templates_form_validate($form, &$form_state) {
  // TODO
}

/**
 * Implements hook_form_submit().
 */
function menu_templates_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  // Prepare a new template
  $template = array();

  foreach ($values as $field => $value) {
    // Filter Drupal control fields
    if ('op' != $field && 'submit' != $field && !preg_match('/^form_/', $field)) {
      $template[$field] = $value;
    }
  }

  if (isset($form_state['menu_template']['mtid'])) {
    // We are updating an existing template, so pass the mtid along
    $template['mtid'] = $form_state['menu_template']['mtid'];
  }

  $result = menu_templates_save_template($template);

  if ($result || 0 === $result) {
    drupal_set_message(t("Template %name saved succesfully",
      array('%name' => $template['name'])));

    $form_state['redirect'] = 'admin/structure/menu-templates';
  }
  else {
    drupal_set_message(t('An error occured while saving the template'), 'error');
  }

  // TODO
  // Watchdog logs
}

/**
 * This is the confirmation message with a form
 */
function menu_templates_delete_confirm($form, &$form_state, $template) {
  $form_state['menu_template'] = $template;

  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $template['name'])),
    'admin/structure/menu-templates',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel'));
}

/**
 * The process of deleting template upon confirmation
 */
function menu_templates_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $deleted = menu_templates_delete_template($form_state['menu_template']['mtid']);

    drupal_set_message(t('Template %name deleted',
      array('%name' => $form_state['menu_template']['name'])));

    $form_state['redirect'] = 'admin/structure/menu-templates';
  }
}
