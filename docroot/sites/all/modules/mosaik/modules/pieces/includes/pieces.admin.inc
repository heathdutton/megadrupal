<?php

/**
 * Pieces settings form
 */
function pieces_settings_form($form, &$form_state, $mosaik) {
  $form['#mosaik'] = $mosaik;
  $data = _pieces_get_form_data($mosaik);
  $form['pieces']['#tree'] = TRUE;

  uasort($mosaik['pieces'], 'pieces_sort');

  foreach ($mosaik['pieces'] as $piece_name => $piece) {
    $form['pieces'][$piece_name] = array(
      'machine_name' => array(
        '#markup' => check_plain($piece_name),
      ),
      'title' => array(
        '#markup' => pieces_is_block($piece) ? $piece['info'] : l($piece['title'], MOSAIK_ADMIN_MENU . '/piece/' . $piece_name . '/edit', array('query' => drupal_get_destination())),
      ),
      'region' => array(
        '#type' => 'select',
        '#options' => $data['regions'],
        '#default_value' => $piece['region']
      ),
      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => $piece['weight'],
        '#delta' => 10,
        '#title-display' => 'invisible',
      ),
    );
  }

  foreach ($data['disabled'] as $piece_name => $piece) {
    $form['pieces'][$piece_name] = array(
      'machine_name' => array(
        '#markup' => check_plain($piece_name),
      ),
      'title' => array(
        '#markup' => pieces_is_block($piece) ? $piece['info'] : l($piece['title'], MOSAIK_ADMIN_MENU . '/piece/' . $piece_name . '/edit', array('query' => drupal_get_destination())),
      ),
      'region' => array(
        '#type' => 'select',
        '#options' => $data['regions'],
        '#default_value' => PIECES_DISABLED_REGION
      ),
      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => $piece['weight'],
        '#delta' => 10,
        '#title-display' => 'invisible',
      ),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Theme implementation
 *
 * @see pieces_settings_form()
 */
function theme_pieces_settings_form($variables) {
  drupal_add_css(drupal_get_path('module', 'block') . '/block.css', array('preprocess' => FALSE));
  drupal_add_js(drupal_get_path('module', 'block') . '/block.js');

  $form = $variables['form'];
  $mosaik = $form['#mosaik'];
  $pieces = element_children($form['pieces']);

  $data = _pieces_get_form_data($mosaik);
  $table_id = 'blocks';
  $header = array(t('Title'), t('Machine name'), t('Region'), t('Weight'));
  $rows = array();
  $output = '';

  $disabled_label = array_shift($data['regions']);
  $data['regions'][PIECES_DISABLED_REGION] = $disabled_label;

  foreach ($data['regions'] as $region => $region_label) {
    $rows[] = array(
      'data' => array(
        array(
          'data' => ($region != PIECES_DISABLED_REGION) ? ucfirst($region) : t('Disabled'),
          'colspan' => 4,
        )
      ),
      'class' => array(
        'region-title',
        'region-title-' . $region,
        'region-' . $region,
      ),
    );

    $pfr = array();

    foreach ($pieces as $id) {
      $piece_region = $form['pieces'][$id]['region']['#value'];

      if ($region == $piece_region) {
        $form['pieces'][$id]['region']['#attributes']['class'] = array(
          'block-region-select',
          'block-region-' . $region
        );
        $form['pieces'][$id]['weight']['#attributes']['class'] = array(
          'block-weight',
          'block-weight-' . $region
        );

        $pfr[] = array(
          'data' => array(
            drupal_render($form['pieces'][$id]['title']),
            drupal_render($form['pieces'][$id]['machine_name']),
            drupal_render($form['pieces'][$id]['region']),
            drupal_render($form['pieces'][$id]['weight']),
          ),
          'class' => array('draggable'),
        );
      }
    }

    $region_message_class = count($pfr) ? 'region-populated' : 'region-empty';
    $rows[] = array(
      'data' => array(
        array('data' => t('No pieces in this region'), 'colspan' => 4)
      ),
      'class' => array(
        'region-message',
        'region-' . $region . '-message',
        $region_message_class,
      )
    );

    $rows = array_merge($rows, $pfr);

    drupal_add_tabledrag($table_id, 'match', 'sibling', 'block-region-select', 'block-region-' . $region, NULL, FALSE);
    drupal_add_tabledrag($table_id, 'order', 'sibling', 'block-weight', 'block-weight-' . $region);

  }

  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => $table_id),
    'sticky' => TRUE,
    'caption' => t('Layout: @layout', array('@layout' => $data['layout']['template'])),
  ));

  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Submit callback
 *
 * @see pieces_settings_form()
 */
function pieces_settings_form_submit($form, &$form_state) {
  pieces_save_associations($form['#mosaik'], $form_state['values']['pieces']);
  drupal_set_message(t('Pieces saved'));
}

/**
 * Helper function: get necessary data for pieces forms
 */
function _pieces_get_form_data($mosaik) {
  $layout = mosaik_layout_load($mosaik['layout']);
  $regions = array(PIECES_DISABLED_REGION => t('- None -')) + array_combine(array_keys($layout['variables']), array_keys($layout['variables']));
  $pieces = pieces_get_all();

  foreach (_block_rehash() as $block) {
    $blocks[$block['module'] . ':' . $block['delta']] = $block;
  }

  $pieces = array_merge($pieces, $blocks);

  $disabled_pieces_names = array_diff(array_keys($pieces), array_keys($mosaik['pieces']));
  $disabled_pieces = array();

  foreach ($disabled_pieces_names as $piece_name) {
    $disabled_pieces[$piece_name] = $pieces[$piece_name];
  }

  return array(
    'mosaik' => $mosaik,
    'layout' => $layout,
    'regions' => $regions,
    'pieces' => $mosaik['pieces'],
    'disabled' => $disabled_pieces,
  );
}

/**
 * Pieces Form
 */
function pieces_form($form, &$form_state, $piece = NULL) {

  if (!isset($form_state['piece'])) {
    $form_state['piece'] = $piece;
  }
  else {
    $piece = $form_state['piece'];
  }

  $form['pieces'] = array(
    '#title' => !empty($piece['title']) ? $piece['title'] : t('Add Piece'),
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['pieces']['settings'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['pieces']['settings']['piece'] = array(
    '#title' => t('Piece'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['pieces']['settings']['piece']['name'] = array(
    '#title' => t('Name'),
    '#type' => 'machine_name',
    '#default_value' => !empty($piece['name']) ? $piece['name'] : '',
    '#maxlength' => 64,
    '#machine_name' => array(
      'exists' => 'pieces_name_exists',
    ),
    '#disabled' => !empty($piece['name']),
  );

  $form['pieces']['settings']['piece']['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#default_value' => !empty($piece['title']) ? $piece['title'] : '',
    '#required' => TRUE,
  );

  $form['pieces']['settings']['callback'] = array(
    '#title' => t('Callback'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['pieces']['settings']['callback']['wrapper'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="pieces-callback-wrapper">',
    '#suffix' => '</div>',
  );

  $callbacks = pieces_callbacks();
  $callback_types = array();

  foreach ($callbacks as $type => $info) {
    module_load_include($info['file type'], $info['module'], $info['file']);

    $info['callback_form']($form['pieces']['settings']['callback']['wrapper'][$type], $form_state);
    $callback_type = !empty($form_state['values']['callback_type']) ? $form_state['values']['callback_type'] : _pieces_get_callback_type_by_callback($piece['callback']);
    $form['pieces']['settings']['callback']['wrapper'][$type]['#access'] = ($callback_type == $type);
    $callback_types[$type] = $type;
  }

  $form['pieces']['settings']['callback']['wrapper']['callback_type'] = array(
    '#title' => t('Callback type'),
    '#type' => 'select',
    '#options' => $callback_types,
    '#weight' => -1,
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => 'pieces_form_ajax_callback',
      'wrapper' => 'pieces-callback-wrapper',
    ),
    '#default_value' => !empty($callback_type) && !empty($piece['callback']) ? $callback_type : NULL,
  );

  $form['pieces']['settings']['callback']['wrapper']['callback'] = array(
    '#title' => t('Callback'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#element_validate' => array('pieces_validate_callback'),
    '#default_value' => !empty($callbacks[$callback_type]['callback']) ? $callbacks[$callback_type]['callback'] : $piece['callback'],
    '#disabled' => TRUE,
    '#access' => !empty($form_state['values']['callback_type']),
  );

  if (empty($callbacks[$callback_type]['callback'])) {
    $form['pieces']['settings']['callback']['wrapper']['callback']['#description'] = t('Enter a valid function name without parentheses (eg.: node_add).');
    $form['pieces']['settings']['callback']['wrapper']['callback']['#disabled'] = FALSE;
  }

  $form['pieces']['settings']['callback']['wrapper']['args'] = array(
    '#type' => 'fieldset',
    '#title' => t('Callback arguments'),
    '#prefix' => '<div id="args-fieldset-wrapper">',
    '#suffix' => '</div>',
    '#access' => !empty($form_state['values']['callback_type']),
  );

  if (empty($form_state['values']['callback_arguments'])) {
    $callback_arguments = !empty($piece['callback_arguments']) ? $piece['callback_arguments'] : array(0 => '');
  }
  else {
    $callback_arguments = $form_state['values']['callback_arguments'];
  }

  $form['pieces']['settings']['callback']['wrapper']['args']['callback_arguments'] = array(
    '#tree' => TRUE,
  );

  foreach ($callback_arguments as $i => $callback_argument) {
    $form['pieces']['settings']['callback']['wrapper']['args']['callback_arguments'][$i] = array(
      '#type' => is_numeric($i) ? 'textarea' : 'textfield',
      '#rows' => 1,
      '#default_value' => $callback_argument,
      '#disabled' => !is_numeric($i),
      '#access' => user_access('access pieces debug'),
    );
  }

  $form['pieces']['settings']['callback']['wrapper']['args']['add_argument'] = array(
    '#type' => 'submit',
    '#value' => t('Add another argument'),
    '#submit' => array('pieces_form_add_argument'),
    '#ajax' => array(
      'callback' => 'pieces_form_arguments_callback',
      'wrapper' => 'args-fieldset-wrapper',
    ),
  );

  if (count($callback_arguments) > 1) {
    $form['pieces']['settings']['callback']['wrapper']['args']['remove_argument'] = array(
      '#type' => 'submit',
      '#value' => t('Remove last argument'),
      '#submit' => array('pieces_form_remove_argument'),
      '#ajax' => array(
        'callback' => 'pieces_form_arguments_callback',
        'wrapper' => 'args-fieldset-wrapper',
      ),
    );
  }

  if (module_exists('token')) {
    $form['pieces']['settings']['callback']['wrapper']['token_help'] = array(
      '#theme' => 'token_tree',
      '#dialog' => TRUE,
      '#access' => !empty($form_state['values']['callback_type']),
    );
  }

  $form['pieces']['settings']['callback']['file_inclusion'] = array(
    '#title' => t('File inclusion'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['pieces']['settings']['callback']['file_inclusion']['file'] = array(
    '#title' => t('File'),
    '#type' => 'textfield',
    '#description' => t('Enter the name of a file to include (eg. node.pages.inc)'),
    '#default_value' => !empty($piece['file']) ? $piece['file'] : '',
  );

  $form['pieces']['settings']['callback']['file_inclusion']['file_path'] = array(
    '#title' => t('Filepath'),
    '#type' => 'textfield',
    '#description' => t('Enter the path to the file directory to include (eg. modules/node)'),
    '#default_value' => !empty($piece['file_path']) ? $piece['file_path'] : '',
  );

  $form['pieces']['settings']['access'] = array(
    '#title' => t('Access'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['pieces']['settings']['access']['access_type'] = array(
    '#title' => t('Restrict access by'),
    '#type' => 'radios',
    '#options' => array(
      'roles' => t('Roles'),
      'permission' => t('Permission'),
    ),
    '#default_value' => !empty($piece['access_arguments']) ? $piece['access_arguments'][0] : 'roles',
  );

  $form['pieces']['settings']['access']['access_by_roles'] = array(
    '#title' => t('Roles'),
    '#type' => 'checkboxes',
    '#options' => user_roles(),
    '#states' => array(
      'visible' => array('input[name="access_type"]' => array('value' => 'roles')),
    ),
    '#default_value' => (!empty($piece['access_arguments']) && $piece['access_arguments'][0] == 'roles') ? $piece['access_arguments'][1] : array(),
  );

  foreach (module_implements('permission') as $module) {
    $perms[$module] = array();
    $function = $module . '_permission';
    $permissions = $function();
    asort($permissions);

    foreach ($permissions as $perm => $info) {
      $perms[$module][$perm] = strip_tags($info['title']);
    }
  }

  ksort($perms);

  $form['pieces']['settings']['access']['access_by_permission'] = array(
    '#title' => t('Permissions'),
    '#type' => 'select',
    '#options' => $perms,
    '#default_value' => (!empty($piece['access_arguments']) && $piece['access_arguments'][0] == 'permission') ? $piece['access_arguments'][1] : array(),
    '#states' => array(
      'visible' => array('input[name="access_type"]' => array('value' => 'permission')),
    ),
  );

  $form['pieces']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

function pieces_form_ajax_callback($form, $form_state) {
  return $form['pieces']['settings']['callback']['wrapper'];
}

/**
 * Ajax Callback for both ajax-enabled callback arguments buttons.
 *
 * @see pieces_form()
 */
function pieces_form_arguments_callback($form, $form_state) {
  return $form['pieces']['settings']['callback']['wrapper']['args'];
}

/**
 * Submit handler for the "add-another-argument" button.
 *
 * @see pieces_form()
 */
function pieces_form_add_argument($form, &$form_state) {
  $form_state['values']['callback_arguments'][] = '';
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "remove one" button.
 *
 * @see pieces_form()
 */
function pieces_form_remove_argument($form, &$form_state) {
  if (count($form_state['values']['callback_arguments']) > 1 && is_numeric(end(array_keys($form_state['values']['callback_arguments'])))) {
    array_pop($form_state['values']['callback_arguments']);
  }

  $form_state['rebuild'] = TRUE;
}

/**
 * Returns whether a mosaik piece name already exists.
 */
function pieces_name_exists($value) {
  $pieces = pieces_get_all();

  if (!empty($pieces)) {
    return isset($pieces[$value]) ? TRUE : FALSE;
  }

  return FALSE;
}

/**
 * Validation callback: check if a function exists
 */
function pieces_validate_callback($element, &$form_state) {
  if (!empty($form_state['values']['file']) && !empty($form_state['values']['file_path'])) {
    $file_name = implode('/', array(
      $form_state['values']['file_path'],
      $form_state['values']['file'],
    ));

    if (file_exists($file_name)) {
      require_once $file_name;
    }
  }

  if (!function_exists($element['#value'])) {
    form_error($element, t('Undefined callback function "!name()". If you need to include a file for this function please use File and Filepath fields.', array('!name' => t($element['#value']))));
  }
}

/**
 * Submit callback
 *
 * @see pieces_form()
 */
function pieces_form_submit($form, &$form_state) {
  $piece = $form_state['values'];

  $piece['access_arguments'] = array();
  $access = 'access_by_' . $piece['access_type'];

  if (in_array($piece['access_type'], array('roles', 'permission'))) {
    $piece['access_callback'] = 'mosaik_access';
    $piece['access_arguments'] = array($piece['access_type'], $piece[$access]);
  }
  else {
    $piece['access_callback'] = $access;
    $piece['access_arguments'] = $piece[$access];
  }

  $action = !empty($form_state['piece']) ? t('updated') : t('saved');
  pieces_save($piece);

  drupal_set_message(t('Piece @title @action', array(
    '@title' => $piece['title'],
    '@action' => $action
  )));

  $form_state['redirect'] = MOSAIK_ADMIN_MENU . '/pieces';
}

/**
 * Form constructor for the piece deletion confirmation form.
 */
function pieces_delete_confirm($form, &$form_state, $piece) {
  $form['#piece'] = $piece;

  $form['name'] = array('#type' => 'value', '#value' => $piece['name']);

  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $piece['title'])),
    MOSAIK_ADMIN_MENU,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Executes piece deletion.
 *
 * @see pieces_delete_confirm()
 */
function pieces_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $piece = piece_load($form_state['values']['name']);
    piece_delete($piece);
    watchdog('pieces', 'Piece: deleted %title.', array('%title' => $piece['title']));
    drupal_set_message(t('Piece %title has been deleted.', array('%title' => $piece['title'])));
  }

  $form_state['redirect'] = MOSAIK_ADMIN_MENU . '/pieces';
}

/**
 * Defines default Piece callback types.
 */
function pieces_callbacks() {
  $default = array(
    'file' => 'includes/pieces.callbacks',
    'file type' => 'inc',
    'module' => 'pieces',
  );

  $callbacks = array(
    'block' => array(
        'callback' => 'pieces_render_block',
        'callback_form' => 'pieces_callback_form_block',
      ) + $default,
    'entity' => array(
        'callback' => 'pieces_render_entity',
        'callback_form' => 'pieces_callback_form_entity',
      ) + $default,
    'function' => array(
        'callback' => '',
        'callback_form' => 'pieces_callback_form_function',
      ) + $default,
    'views' => array(
        'callback' => 'views_embed_view',
        'callback_form' => 'pieces_callback_form_views',
      ) + $default,
  );

  // Allow other modules to alter defined callbacks.
  drupal_alter('pieces_callbacks', $callbacks);

  return $callbacks;
}

/**
 * Helper function: get piece callback type by callback name.
 */
function _pieces_get_callback_type_by_callback($callback) {
  $types = pieces_callbacks();
  $callback_type = 'function';

  foreach ($types as $name => $type) {
    if ($type['callback'] == $callback) {
      $callback_type = $name;
    }
  }

  return $callback_type;
}
