<?php
/**
 * @file
 * Custom Help Text export_ui plugin.
 */

/**
 * Define this Export UI plugin.
 */
$plugin = array(
  'schema' => 'custom_help_text',
  // As defined in hook_schema().
  'access' => 'administer custom help text',
  // Define a permission users must have to access these pages.
  // Define the menu item.
  'menu' => array(
    'menu prefix' => 'admin/structure',
    'menu item' => 'custom-help-text',
    'menu title' => 'Custom Help Text',
    'menu description' => 'Administer Custom Help Text.',
  ),
  // Define user interface texts.
  'title singular' => t('Custom Help Text'),
  'title plural' => t('Custom Help Texts'),
  'title singular proper' => t('Custom Help Text'),
  'title plural proper' => t('Custom Help Texts'),
  'handler' => 'custom_help_text_export_ui',
);

/**
 * Class for Custom Help Text export UI.
 */
class custom_help_text_export_ui extends ctools_export_ui {

  function init($plugin) {
    $plugin['menu']['items']['list callback']['page callback'] = 'drupal_get_form';
    $plugin['menu']['items']['list callback']['page arguments'] = array('custom_help_text_list_form');
    $plugin['menu']['items']['list callback']['access callback'] = 'user_access';
    $plugin['menu']['items']['list callback']['access arguments'] = array('administer custom help text');

    $plugin['menu']['items']['list']['title'] = 'List';
    $plugin['menu']['items']['list']['page callback'] = 'drupal_get_form';
    $plugin['menu']['items']['list']['page arguments'] = array('custom_help_text_list_form');

    $plugin['menu']['items']['edit callback']['type'] = MENU_VISIBLE_IN_BREADCRUMB;

    return parent::init($plugin);
  }

  function hook_menu(&$items) {
    $stored_items = $this->plugin['menu']['items'];

    parent::hook_menu($items);

    $this->plugin['menu']['items'] = $stored_items;
  }

  /**
   * Provide the actual editing form.
   */
  function edit_form(&$form, &$form_state) {
    parent::edit_form($form, $form_state);

    $item = $form_state['item'];

    $form['info']['admin_title']['#required'] = TRUE;

    $form['path'] = array(
      '#type' => 'textarea',
      '#title' => t('Show the custom help text on specific pages'),
      '#default_value' => $item->path,
      '#maxlength' => 128,
      '#size' => 45,
      '#description' => t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array(
        '%blog' => 'blog',
        '%blog-wildcard' => 'blog/*',
        '%front' => '<front>'
      )),
      '#required' => TRUE,
    );

    $form['help'] = array(
      '#type' => 'textarea',
      '#title' => t('Custom Help Text'),
      '#default_value' => $item->help,
      '#description' => t('Specify a help message.'),
      '#required' => TRUE,
    );

    $form['options'] = array(
      '#type' => 'fieldset',
      '#title' => 'Options',
      '#collapsible' => TRUE,
      '#tree' => TRUE,
    );

    $item->options = unserialize($item->options);

    $roles = user_roles(FALSE, 'view custom help text');

    // Get all Authentiated roles
    if (isset($roles[DRUPAL_AUTHENTICATED_RID])) {
      $roles += array_diff(user_roles(TRUE), $roles);
    }

    if (user_access('administer permissions')) {
      $permission_link = l(t('View custom help text'), 'admin/people/permissions', array(
        'fragment' => 'module-custom_help_text'
      ));
    }
    else {
      $permission_link = t('View custom help text');
    }

    $form['options']['roles'] = array(
      '#type' => 'checkboxes',
      '#options' => $roles,
      '#title' => t('User roles that can view the custom help text'),
      '#required' => TRUE,
      '#description' => t('Check the roles that needs to view the help message and have currently the permission \'!permission_url\'.', array(
          '!permission_url' => $permission_link,
        )
      ),
    );

    if (!empty($item->options['roles'])) {
      foreach ($item->options['roles'] as $role_name) {
        if ($role = user_role_load_by_name($role_name)) {
          $form['options']['roles']['#default_value'][] = $role->rid;
        }
      }
    }
    else {
      $form['options']['roles']['#default_value'] = array();
    }
  }

  /**
   * Handle the submission of the edit form.
   */
  function edit_form_submit(&$form, &$form_state) {
    $this->serialize_options($form, $form_state);
    if (empty($form_state['item']->weight)) {
      $form_state['values']['weight'] = '-50';
    }
    parent::edit_form_submit($form, $form_state);
  }

  /**
   * Helper method to serialize options.
   */
  function serialize_options($form, &$form_state) {
    $roles = array();

    foreach ($form_state['values']['options']['roles'] as $rid) {
      if ($rid != 0 && $role = user_role_load($rid)) {
        $roles[] = $role->name;
      }
    }

    $form_state['values']['options']['roles'] = $roles;

    $form_state['values']['options'] = serialize($form_state['values']['options']);
  }
}