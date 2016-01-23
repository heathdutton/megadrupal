<?php
/**
 * @file
 * Administrative UI file.
 */

/**
 * Displays an overview of image derivative access controls.
 */
function image_style_access_overview() {
  // Display help text.
  $flush_link_text = t('the derivative is flushed');
  $output['help'] = array(
    '#theme_wrappers' => array('container'),
    '#attributes' => array('class' => array('help')),
    'text' => array(
      '#type' => 'html_tag',
      '#tag' => 'p',
      '#value' => t('Access to images and their derivatives can be controlled by their location, image style, and other options depending what type of access control is used. The final configuration matched (ordered by specificity as listed in the table) is used to determine whether to allow or deny access to the image. The priority of each configuration allows ordering of a particular access configuration.'),
    ),
    '#weight' => -105,
  );
  $output['help_addendum'] = array(
    '#theme_wrappers' => array('container'),
    '#attributes' => array('class' => array('help')),
    'text' => array(
      '#type' => 'html_tag',
      '#tag' => 'p',
      '#value' => t('Note: publicly accessible files, i.e. images and their derivatives under the public files directory, are not affected by these access controls (because accessing public files bypasses Drupal). Once a public image derivative is generated and placed in the public files directory, access to the derivative is not controlled by the following configuration until !flush_link.', array(
        '!flush_link' => module_exists('imagestyleflush') ?
          l($flush_link_text, 'admin/config/media/image-styles') :
          $flush_link_text,
      )),
    ),
    '#weight' => -100,
  );

  // Add access simulation form.
  $output['simulate'] = drupal_get_form('image_style_access_overview_simulate_form');
  $output['simulate']['#weight'] = -50;

  // List configurations by file scheme.
  $simulate = isset($output['simulate']['#image_style_access_simulate']) ?
    $output['simulate']['#image_style_access_simulate'] :
    array();
  $output['overview'] = drupal_get_form('image_style_access_overview_form', $simulate);

  return $output;
}

/**
 * Form builder to display a filter form for simulating access.
 *
 * When the form is submitted, the resulting form array contains an array of
 * values to simulate in '#image_style_access_simulate'.
 */
function image_style_access_overview_simulate_form($form, &$form_state) {
  form_load_include($form_state, 'inc', 'image_style_access', 'image_style_access.admin');

  // Prepare filter values.
  $simulate = array();
  $filtering = FALSE;
  foreach (array('style', 'target', 'scheme', 'show_disabled') as $key) {
    if (isset($_GET[$key])) {
      $simulate[$key] = $_GET[$key];
      $filtering = TRUE;
    }
    else {
      $simulate[$key] = NULL;
    }
  }
  if ($filtering) {
    $form['#image_style_access_simulate'] = $simulate;
  }

  // Build form.
  $form['simulate'] = array(
    '#type' => 'fieldset',
    '#title' => t('Simulate access'),
    '#collapsible' => TRUE,
    '#collapsed' => !$filtering,
    '#attributes' => array(
      'class' => array('image-style-access-overview-simulate'),
    ),
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'image_style_access') . '/image_style_access.css'),
    ),
  );
  $options = array();
  foreach (image_style_access_list_accessible_sources() as $scheme => $scheme_info) {
    $options[$scheme] = $scheme_info['name'];
  }
  $form['simulate']['style'] = array(
    '#type' => 'select',
    '#title' => t('Image style'),
    '#options' => array('' => t('Original image')) + image_style_options(),
    '#default_value' => $simulate['style'],
  );
  $form['simulate']['scheme'] = array(
    '#type' => 'select',
    '#title' => t('File source'),
    '#options' => $options,
    '#default_value' => $simulate['scheme'],
  );
  $form['simulate']['target'] = array(
    '#type' => 'textfield',
    '#title' => t('File location'),
    '#default_value' => $simulate['target'],
    '#size' => 40,
  );
  $form['simulate']['show_disabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show disabled'),
    '#default_value' => $simulate['show_disabled'],
  );
  $form['simulate']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Simulate'),
    '#name' => '',
  );
  $form['simulate']['reset'] = array(
    '#type' => 'link',
    '#title' => t('Reset'),
    '#href' => current_path(),
  );

  return $form;
}

/**
 * Implements hook_form_FORM_ID_alter() for image_style_access_overview_simulate_form.
 */
function image_style_access_form_image_style_access_overview_simulate_form_alter(&$form, &$form_state) {
  $form['#method'] = 'get';
  $form['form_build_id']['#access'] = FALSE;
  $form['form_token']['#access'] = FALSE;
  $form['form_id']['#access'] = FALSE;
}

/**
 * Form builder to display a table of configurations.
 *
 * The status of each configuration can be managed with the checkboxes and the
 * submit button to save changes.
 */
function image_style_access_overview_form($form, &$form_state, $simulate = array()) {
  // Prepare overview prerequisite data.
  $wrappers = image_style_access_list_accessible_sources();
  $action_titles = array(
    IMAGE_STYLE_ACCESS_ALLOW => t('Allow'),
    IMAGE_STYLE_ACCESS_DENY => t('Deny'),
  );
  $operation_link_base = array('query' => drupal_get_destination());
  $headers = array(
    'enabled' => t('Enabled'),
    'title' => t('Title'),
    'scheme' => t('File source'),
    'target_base' => t('File location base'),
    'style' => t('Image style'),
    'type' => t('Access type'),
    'action' => t('Action'),
    'priority' => t('Priority'),
    'operations' => t('Operations'),
  );
  $any = '<em>' . t('Any') . '</em>';
  $unknown = '<em>' . t('Unknown') . '</em>';

  // Build overview table.
  $form['configs'] = array(
    '#type' => 'image_style_access_overview',
    '#tree' => TRUE,
    '#header' => $headers,
    '#empty' => t('There is no access configuration.'),
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'image_style_access') . '/image_style_access.css'),
    ),
  );
  if (empty($simulate)) {
    $configs = image_style_access_config_controller()->loadMultiple();
  }
  else {
    $simulate += array(
      'style' => NULL,
      'scheme' => NULL,
      'target' => '',
    );
    $configs = image_style_access_config_controller()->loadMultipleByDownload($simulate['style'], $simulate['scheme'], empty($simulate['show_disabled']));
    $configs = image_style_access_checker()->matchTarget($configs, $simulate['target']);
  }
  $type_info = image_style_access_get_type_info();
  foreach ($configs as $cid => $config) {
    // Add checkbox for configuring config enabled status.
    $form['configs'][$cid]['enabled'] = array(
      '#type' => 'checkbox',
      '#default_value' => $config->enabled,
    );
    // Add config details.
    $form['configs'][$cid]['title'] = array(
      '#markup' => check_plain($config->title),
    );
    $form['configs'][$cid]['scheme'] = array(
      '#markup' => isset($config->scheme) ?
        (isset($wrappers[$config->scheme]) ? check_plain($wrappers[$config->scheme]['name']) : $unknown) :
        $any,
    );
    $form['configs'][$cid]['target_base'] = array(
      '#markup' => strlen($config->target_base) ?
        ('<span class="image-style-access-target">/' . check_plain($config->target_base) . '</span>') :
        $any,
    );
    $form['configs'][$cid]['style'] = array(
      '#markup' => isset($config->style) ?
        (strlen($config->style) ? check_plain($config->style) : t('Original image')) :
        $any,
    );
    $form['configs'][$cid]['type'] = array(
      '#markup' => isset($type_info[$config->type]) ?
        check_plain($type_info[$config->type]['label']) :
        $unknown,
    );
    $form['configs'][$cid]['action'] = array(
      '#type' => 'html_tag',
      '#tag' => 'span',
      '#value' => check_plain($action_titles[$config->action]),
      '#attributes' => array(
        'class' => array('image-style-access-action', drupal_html_class('image-style-access-' . $config->action)),
      ),
    );
    $form['configs'][$cid]['priority'] = array(
      '#markup' => check_plain($config->priority),
    );
    // Add configuration operations.
    $form['configs'][$cid]['operations'] = array(
      '#theme' => 'links__image_style_access_operations',
      '#links' => array(
        'config-edit' => array(
          'title' => t('Edit'),
          'href' => 'admin/config/media/image-styles/access/edit/' . $config->cid,
        ) + $operation_link_base,
        'config-delete' => array(
          'title' => t('Delete'),
          'href' => 'admin/config/media/image-styles/access/delete/' . $config->cid,
        ) + $operation_link_base,
      ),
      '#attributes' => array(
        'class' => array('image-style-access-operations'),
      ),
    );
  }

  // Add form action button.
  $form['actions'] = array(
    '#type' => 'actions',
    '#access' => (bool) element_children($form['configs']),
  );
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
  );

  return $form;
}

/**
 * Saves configuration overview.
 */
function image_style_access_overview_form_submit($form, &$form_state) {
  // Detect changes.
  $save = array();
  foreach ($form_state['values']['configs'] as $cid => $data) {
    $default_enabled = $form['configs'][$cid]['enabled']['#default_value'];
    if ($form_state['values']['configs'][$cid]['enabled'] != $default_enabled) {
      $save[$cid]['enabled'] = (int) $form_state['values']['configs'][$cid]['enabled'];
    }
  }

  // Save changes.
  foreach ($save as $cid => $changes) {
    db_update('image_style_access_config')
      ->fields($changes)
      ->condition('cid', $cid)
      ->execute();
  }
  image_style_access_clear_cache();
  drupal_set_message(t('The changes have been saved.'));
}

/**
 * Renders a configuration overview table.
 */
function theme_image_style_access_overview_table($variables) {
  $element = $variables['element'];

  // Build table.
  $header = $element['#header'];
  $rows = array();
  foreach (element_children($element) as $child) {
    $row = array();
    foreach (array_keys($header) as $header_cell) {
      // Build cell.
      $cell = isset($element[$child][$header_cell]) ? $element[$child][$header_cell] : '';
      if (is_array($cell)) {
        $row[] = drupal_render($cell);
      }
      else {
        $row[] = $cell;
      }
    }
    $rows[] = $row;
  }
  $table = array(
    'header' => array_values($header),
    'rows' => $rows,
  );
  if (!empty($element['#empty'])) {
    $table['empty'] = $element['#empty'];
  }

  return theme('table', $table);
}

/**
 * Form builder to add an access configuration.
 */
function image_style_access_config_add_form($form, &$form_state, $destination = NULL, $type = NULL) {
  // Build basic initial form.
  if (!isset($type)) {
    $form['heading'] = array(
      '#prefix' => '<p><strong>',
      '#markup' => t('Select an access control:'),
      '#suffix' => '</strong></p>',
    );
    $form['type'] = array(
      '#theme' => 'image_style_access_config_add_list',
      '#destination' => $destination,
    );
  }
  // Build config form.
  else {
    // Initialize form controller.
    if (!isset($form_state['form_controller'])) {
      $form_state['form_controller'] = image_style_access_config_controller()->getFormController($type);
    }
    /** @var $form_controller ImageStyleAccessConfigFormController */
    $form_controller = $form_state['form_controller'];

    // Build form with controller.
    $config = new ImageStyleAccessConfig(array('type' => $type));
    $form = $form_controller->form($form, $form_state, $config);
    $form['#validate'][] = 'image_style_access_config_form_validate';
    $form['#submit'][] = 'image_style_access_config_form_submit';
  }

  return $form;
}

/**
 * Renders a list of types with which to create configuration.
 */
function theme_image_style_access_config_add_list($variables) {
  $element = $variables['element'];
  $destination = !empty($element['#destination']) ? array('destination' => $element['#destination']) : drupal_get_destination();
  $output = '<dl class="image-style-access-type-list">';
  foreach (image_style_access_get_type_info() as $type => $info) {
    $href = 'admin/config/media/image-styles/access/add/' . $type;
    $output .= '<dt>' . l($info['label'], $href, array('query' => $destination)) . '</dt>';
    $description = isset($info['description']) ? $info['description'] : '';
    $output .= '<dd>' . $description . '</dd>';
  }
  $output .= '</dl>';

  return $output;
}

/**
 * Form builder to edit an access configuration.
 */
function image_style_access_config_form($form, &$form_state, ImageStyleAccessConfig $config) {
  // Initialize form controller.
  if (!isset($form_state['form_controller'])) {
    $form_state['form_controller'] = image_style_access_config_controller()->getFormController($config->type);
  }
  /** @var $form_controller ImageStyleAccessConfigFormController */
  $form_controller = $form_state['form_controller'];

  // Build form with controller.
  $form = $form_controller->form($form, $form_state, $config);

  return $form;
}

/**
 * Validates config form submission.
 */
function image_style_access_config_form_validate($form, &$form_state) {
  if (isset($form_state['form_controller'])) {
    /** @var $form_controller ImageStyleAccessConfigFormController */
    $form_controller = $form_state['form_controller'];
    $form_controller->validate($form, $form_state, $form_state['config']);
  }
}

/**
 * Processes config form submission.
 */
function image_style_access_config_form_submit($form, &$form_state) {
  if (isset($form_state['form_controller'])) {
    /** @var $form_controller ImageStyleAccessConfigFormController */
    $form_controller = $form_state['form_controller'];
    $form_controller->submit($form, $form_state, $form_state['config']);
  }
}

/**
 * Confirmation form to delete an access configuration.
 */
function image_style_access_config_delete_form($form, &$form_state, ImageStyleAccessConfig $config) {
  $form_state['cid'] = $config->cid;
  $question = t('Are you sure you want to delete this access control?');
  return confirm_form($form, $question, 'admin/config/media/image-styles/access');
}

/**
 * Deletes an access configuration.
 */
function image_style_access_config_delete_form_submit($form, &$form_state) {
  image_style_access_config_controller()->delete($form_state['cid']);
  drupal_set_message(t('The access configuration has been deleted.'));
}

/**
 * Default access configuration form controller.
 */
class ImageStyleAccessConfigFormController {
  /**
   * Builds the edit form for a configuration.
   */
  public function form($form, &$form_state, ImageStyleAccessConfig $config) {
    // Initialize config.
    if (!isset($form_state['config'])) {
      $form_state['config'] = $config;
    }
    else {
      $config = $form_state['config'];
    }

    // Build form.
    $options = array('' => t('- Any -'));
    foreach (image_style_access_list_accessible_sources() as $scheme => $scheme_info) {
      $options[$scheme] = $scheme_info['name'];
    }
    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Administrative title'),
      '#default_value' => $config->title,
      '#maxlength' => 255,
      '#description' => t('Enter descriptive text to summarize this configuration.'),
    );
    $form['action'] = array(
      '#type' => 'radios',
      '#title' => t('Action to take'),
      '#options' => array(
        IMAGE_STYLE_ACCESS_ALLOW => t('Allow'),
        IMAGE_STYLE_ACCESS_DENY => t('Deny'),
      ),
      '#default_value' => $config->action,
      '#required' => TRUE,
      '#description' => t('Select the action to take for access to the image being checked when this configuration is matched.'),
    );
    $form['scheme'] = array(
      '#type' => 'select',
      '#title' => t('File source'),
      '#options' => $options,
      '#default_value' => $config->scheme,
      '#description' => t('Select the source of the image file applicable to this configuration.'),
    );
    $form['target_base'] = array(
      '#type' => 'textfield',
      '#title' => t('Location base'),
      '#field_prefix' => '/',
      '#default_value' => $config->target_base,
      '#maxlength' => 255,
      '#description' => t('Enter the base path of the image file location on which to match this configuration. If the provided path is empty, all locations will match this configuration. You may use the percentage sign (%) as a placeholder for one or more characters not including forward slash (/).'),
    );
    $options = array(
      -1 => t('- Any -'),
      '' => t('Original image'),
    ) + image_style_options();
    $form['style'] = array(
      '#type' => 'select',
      '#title' => t('Image style'),
      '#options' => $options,
      '#default_value' => isset($config->style) ? $config->style : -1,
      '#description' => t('Select the image style applicable to this configuration.'),
    );
    $form['settings'] = array(
      '#tree' => TRUE,
    );
    $form['priority'] = array(
      '#type' => 'weight',
      '#title' => t('Priority'),
      '#delta' => 30,
      '#default_value' => $config->priority,
      '#description' => t('Set the priority of this configuration compared to others. The configuration with a higher priority is applied last and overrides previous matched configurations.'),
    );

    // Add form actions.
    $form['actions'] = array(
      '#type' => 'actions',
      '#weight' => 5,
    );
    $form['actions']['save'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );

    return $form;
  }

  /**
   * Builds the config from a form submission.
   */
  protected function buildConfig($form, &$form_state) {
    $values = $form_state['values'];

    // Build config from clone of original.
    /** @var $config ImageStyleAccessConfig */
    $config = clone $form_state['config'];
    $config->title = $values['title'];
    $config->scheme = !empty($values['scheme']) ? $values['scheme'] : NULL;
    $config->target_base = $values['target_base'];
    $config->style = $values['style'] == -1 ? NULL : $values['style'];
    $config->settings = !empty($values['settings']) ? $values['settings'] : array();
    $config->action = $values['action'];
    $config->priority = $values['priority'];

    return $config;
  }

  /**
   * Validates an edit form submission.
   */
  public function validate($form, &$form_state) {
    $values = $form_state['values'];
    if ($values['scheme'] == 'public' && $values['style'] === '') {
      form_error($form['style'], t('Original images cannot be access controlled in the public file system.'));
    }
  }

  /**
   * Processes an edit form submission.
   */
  public function submit($form, &$form_state) {
    $config = $this->buildConfig($form, $form_state);
    image_style_access_config_controller()->save($config);
    drupal_set_message(t('The access configuration has been saved.'));
  }
}

/**
 * Form controller for configuration user access control.
 */
class ImageStyleAccessUserConfigFormController extends ImageStyleAccessConfigFormController {
  /**
   * {@inheritdoc}
   */
  public function form($form, &$form_state, ImageStyleAccessConfig $config) {
    $form = parent::form($form, $form_state, $config);
    form_load_include($form_state, 'inc', 'image_style_access', 'image_style_access.admin');

    // Add user autocomplete.
    $username = '';
    if (!empty($config->settings['uid']) && $user = user_load($config->settings['uid'])) {
      $username = $user->name;
    }
    $form['settings']['uid'] = array(
      '#type' => 'textfield',
      '#title' => t('User'),
      '#maxlength' => 60,
      '#autocomplete_path' => 'user/autocomplete',
      '#default_value' => $username,
      '#required' => TRUE,
      '#description' => t('Enter the user to match for this configuration.'),
    );

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  protected function buildConfig($form, &$form_state) {
    $config = parent::buildConfig($form, $form_state);
    $values = $form_state['values'];

    // Add user setting.
    if ($user = user_load_by_name($values['settings']['uid'])) {
      $config->settings['uid'] = $user->uid;
    }

    return $config;
  }

  /**
   * Validates entered user.
   */
  public function validate($form, &$form_state) {
    parent::validate($form, $form_state);
    $values = $form_state['values'];
    if (isset($values['settings']['uid']) && !user_load_by_name($values['settings']['uid'])) {
      form_error($form['settings']['uid'], t('The specific user name does not correspond to a valid user.'));
    }
  }
}

/**
 * Form controller for configuration user role access control.
 */
class ImageStyleAccessUserRoleConfigFormController extends ImageStyleAccessConfigFormController {
  /**
   * {@inheritdoc}
   */
  public function form($form, &$form_state, ImageStyleAccessConfig $config) {
    $form = parent::form($form, $form_state, $config);

    // Add user role selector.
    $config->settings += array('roles' => array());
    $form['settings']['roles'] = array(
      '#type' => 'checkboxes',
      '#title' => t('User roles'),
      '#options' => user_roles(),
      '#default_value' => $config->settings['roles'],
      '#required' => TRUE,
      '#description' => t('Select user roles to match for this configuration.'),
    );

    return $form;
  }
}
