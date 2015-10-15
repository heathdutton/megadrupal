<?php

/**
 * @file
 * Admin pages for the block manager module
 */

/**
 * Builds the form for block manager's configuration page.
 *
 * @see block_manager_admin_settings_validate()
 * @see block_manager_admin_settings_submit()
 *
 * @ingroup forms
 */
function block_manager_admin_settings($form, $form_state = array()) {
  $form = array();

  // Get all of the configuration options from the database.
  $config = _block_manager_get_full_config();

  $form['instructions'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' . t('Select the regions that should be managed by the block manager instead of the default block module') . '</p>',
  );

  $form['block_manager'] = array(
    '#tree' => TRUE,
  );

  $theme_default = variable_get('theme_default', 'bartik');

  // Get all active themes.
  $active_themes = array();
  foreach (list_themes() as $theme) {
    $theme->is_default = ($theme->name == $theme_default);
    if ($theme->status == 1) {
      $active_themes[] = $theme;
    }
  }

  foreach ($active_themes as $theme) {
    // Build up regions table.
    foreach ($theme->info['regions'] as $key => $name) {
      if (array_key_exists($theme->name, $config)) {
        $form['block_manager'][$theme->name][$key] = _block_manager_build_row(
          $key,
          $name,
          $config[$theme->name][$key]['manage'],
          $config[$theme->name][$key]['layout'],
          $config[$theme->name][$key]['limit_types'],
          $config[$theme->name][$key]['allowed_types']
        );
      }
      else {
        $form['block_manager'][$theme->name][$key] = _block_manager_build_row(
          $key,
          $name,
          FALSE,
          'vertical',
          FALSE,
          array()
        );
      }
    }

    $header_array = array(
      array('data' => t('Manage'), 'class' => array('checkbox')),
      t('Region Name'),
      t('Admin Page Layout'),
    );

    if (module_exists('nodeblock')) {
      $header_array = array_merge($header_array, array(
        array('data' => t('Limit Allowed Blocks'), 'class' => array('checkbox')),
        t('Blocks Allowed in Region'),
        ));
    }

    $form['block_manager'][$theme->name] += array(
      '#type' => 'fieldset',
      '#title' => $theme->name,
      '#collapsible' => TRUE,
      '#theme' => 'block_manager_theme_fieldset',
      '#header' => $header_array,
      // Ensure that the default theme fieldset comes first.
      '#weight' => $theme->is_default ? -10 : NULL,
    );
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Custom validation function for the block manager configuration form.
 *
 * Requires the allowed_types field only if limit is selected.
 *
 * @see block_manager_admin_settings_submit()
 */
function block_manager_admin_settings_validate($form, &$form_state) {
  foreach ($form_state['values']['block_manager'] as $theme_name => $region_config) {
    foreach ($region_config as $region_name => $fields) {
      if ($fields['limit_types'] && !$fields['allowed_types']) {
        form_set_error(
          "block_manager[$theme_name][$region_name][allowed_types]", t('You must select at least one content type to be allowed in type-limited regions.')
        );
      }
    }
  }
}

/**
 * Handles submissions for the block manager configuration form.
 *
 * @see block_manager_admin_settings_validate()
 */
function block_manager_admin_settings_submit($form, &$form_state) {

  // Collect the current configuration options.
  $config = _block_manager_get_full_config();

  foreach ($form_state['values']['block_manager'] as $theme_name => $theme) {
    foreach ($theme as $region_name => $region) {
      if (array_key_exists($theme_name, $config) && array_key_exists($region_name, $config[$theme_name])) {
        // Only update the record if it changed.
        if ($config[$theme_name][$region_name]['manage'] !== $region['manage']
          || $config[$theme_name][$region_name]['layout'] !== $region['layout']
          || $config[$theme_name][$region_name]['limit_types'] !== $region['limit_types']
          || $config[$theme_name][$region_name]['allowed_types'] !== $region['allowed_types']) {
          db_update('block_manager_config')
            ->fields(array(
              'manage' => $region['manage'],
              'layout' => $region['layout'],
              'limit_types' => $region['limit_types'],
              'allowed_types' => serialize($region['allowed_types']),
            ))
            ->condition('theme', $theme_name)
            ->condition('region', $region_name)
            ->execute();
          if ($region['manage'] == 1) {
            // Pull in the block configuration for this region
            // from the block module.
            _block_manager_copy_block_db($theme_name, $region_name);
          }
          else {
            // Remove the block configuration for this region.
            db_delete('block_manager')
              ->condition('theme', $theme_name)
              ->condition('region', $region_name)
              ->execute();
          }
        }
      }
      else {
        db_insert('block_manager_config')
          ->fields(array(
            'theme' => $theme_name,
            'region' => $region_name,
            'manage' => $region['manage'],
            'layout' => $region['layout'],
            'limit_types' => $region['limit_types'],
            'allowed_types' => serialize($region['allowed_types']),
          ))
          ->execute();
        if ($region['manage'] == 1) {
          // Pull in the block configuration for this region
          // from the block module.
          _block_manager_copy_block_db($theme_name, $region_name);
        }
      }
    }
  }
}

/**
 * Returns HTML for the block manager configuration page.
 *
 * @param array $vars
 *   An associative array containing:
 *   - form: A render element representing the form
 *
 * @ingroup themeable
 */
function theme_block_manager_theme_fieldset($vars) {
  $form = $vars['form'];

  // Individual table headers.
  $rows = array();

  // Iterate through all the regions, which are children of this fieldset.
  foreach (element_children($form) as $key) {
    // Stick it into $region for easier accessing.
    $region = $form[$key];
    $row = array();

    // Manage Checkbox.
    unset($region['manage']['#title']);
    $row[] = array('class' => array('checkbox'), 'data' => drupal_render($region['manage']));

    // Region Name.
    $label = '<label';
    if (isset($region['manage']['#id'])) {
      $label .= ' for="' . $region['manage']['#id'] . '"';
    }
    $row[] = $label . '><strong>' . drupal_render($region['region_name']) . '</strong></label>';

    // Layout Selector.
    unset($region['layout']['#title']);
    $row[] = drupal_render($region['layout']);

    if (module_exists('nodeblock')) {
      // Limit Checkbox.
      unset($region['limit_types']['#title']);
      $row[] = array('class' => array('checkbox'), 'data' => drupal_render($region['limit_types']));

      // Allowed Blocks Selector.
      unset($region['allowed_types']['#title']);
      $row[] = drupal_render($region['allowed_types']);
    }

    $rows[] = $row;
  }

  return theme('table', array('header' => $form['#header'], 'rows' => $rows));
}

/**
 * Create form to remove all blocks for a given page and region.
 *
 * @param String $region
 *   Machine name of the region from which to remove blocks
 *
 * @see block_manager_remove_blocks_form_submit()
 *
 * @ingroup forms
 */
function block_manager_remove_blocks_form($form, $form_state, $region = NULL) {
  $page = $_GET['page'];

  $extra_fields = array(
    'region' => array(
      '#type' => 'hidden',
      '#value' => $region,
    ),
    'page' => array(
      '#type' => 'hidden',
      '#value' => $page,
    ),
  );

  // Make the home page read as "<front>" rather than "node".
  if ('node' === $page) {
    $page = t('<front>');
  }

  return confirm_form(
    $extra_fields,
    t(
      'Delete all blocks from the %region region on the page %page?',
      array('%region' => $region,'%page' => $page)
    ),
    $page
  );
}

/**
 * Handle form submissions for the "Remove Blocks" form.
 */
function block_manager_remove_blocks_form_submit($form, $form_state) {
  $pages[] = $form_state['values']['page'];
  if ($pages[0] == 'node') {
    $pages[] = '<front>';
  }
  if (($drupal_path = drupal_lookup_path('source', $pages[0]))) {
    $pages[] = $drupal_path;
  }

  // Remove all sidebar items, then redirect back to the page we came from.
  $query = db_delete('block_manager')
    ->condition('url', $pages, 'IN')
    ->condition('region', $form_state['values']['region'])
    ->execute();
}

/**
 * Create the management form for blocks.
 *
 * @param String $region
 *   Region on the page that is being managed
 *
 * @see block_manager_manage_blocks_submit()
 *
 * @ingroup forms
 */
function block_manager_manage_blocks($form, $form_state, $region) {
  include_once drupal_get_path('module', 'block') . '/block.admin.inc';
  $default_theme = variable_get('theme_default', 'bartik');

  $page = $_GET['page'];

  $pages = array();
  if ($page == 'node') {
    $pages[] = '<front>';
  }
  $pages[] = $page;
  $drupal_path = NULL;
  if (($drupal_path = drupal_lookup_path('source', $page))) {
    $pages[] = $drupal_path;
  }

  $extra_fields = array(
    'region' => array(
      '#type' => 'value',
      '#value' => $region,
    ),
    'page' => array(
      '#type' => 'value',
      '#value' => $page,
    ),
    'theme' => array(
      '#type' => 'value',
      '#value' => $default_theme,
    ),
  );

  // Get the configuration record for this region.
  $config = _block_manager_get_config($region, $default_theme);
  // There should be only one result.
  $config = $config[$region];

  if ($config['limit_types']) {
    $allowed_nids = db_select('node', 'n')
      ->fields('n', array('nid'))
      ->condition('type', $config['allowed_types'], 'IN')
      ->addTag('node_access')
      ->execute()
      ->fetchAll();

    $allowed_nodes = array();
    foreach ($allowed_nids as $object) {
      $allowed_nodes[] = $object->nid;
    }
  }

  drupal_add_library('system', 'ui.sortable');
  drupal_add_library('system', 'ui.draggable');

  $form['#attached']['css'] = array(drupal_get_path('module', 'block_manager') . '/css/block_manager.css');
  $form['#attached']['js'] = array(drupal_get_path('module', 'block_manager') . '/js/block_manager.js');

  $form['#tree'] = TRUE;

  $form['available_blocks'] = array(
    '#type' => 'fieldset',
    '#title' => t('Available Blocks'),
    '#id' => 'available_blocks',
    '#attributes' => array(
      'class' => array('block_container', 'first', $config['layout']),
    ),
  );
  $form['available_blocks']['inner'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('inner'),
    ),
  );
  $form['managed_blocks'] = array(
    '#type' => 'fieldset',
    '#title' => t('Blocks to Display'),
    '#id' => 'managed_blocks',
    '#attributes' => array(
      'class' => array('block_container', 'last', $config['layout']),
    ),
  );
  $form['managed_blocks']['inner'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('inner'),
    ),
  );

  // Get and render all available blocks.
  $blocks = block_admin_display_prepare_blocks($default_theme);

  // Get all blocks from this region.
  $managed_blocks = _block_manager_get_region_blocks($region, $pages, $default_theme);
  if (array_key_exists($region, $managed_blocks)) {
    $managed_blocks = $managed_blocks[$region];
  }
  else {
    $managed_blocks = array();
  }

  foreach ($blocks as $b) {
    if ($config['limit_types']) {
      // Limit to the nodes found from the types available.
      if ($b['module'] !== 'nodeblock' || !in_array($b['delta'], $allowed_nodes)) {
        continue;
      }
    }
    elseif ($b['module'] === 'system') {
      continue;
    }

    $key = $b['module'] . '_' . $b['delta'];

    if (array_key_exists($b['bid'], $managed_blocks)) {
      // We're managing this block - add it to the managed blocks container.
      $block_region = 'managed_blocks';
      $weight = $managed_blocks[$b['bid']]['weight'];
    }
    else {
      // This is an available block - add it to the available blocks container.
      $block_region = 'available_blocks';
      $weight = $b['weight'];
    }
    $form[$block_region]['inner'][$key] = array(
      '#type' => 'container',
      '#weight' => $weight,
      '#attributes' => array(
        'class' => array(
          'block',
        ),
      ),
    );
    $form[$block_region]['inner'][$key]['remove'] = array(
      '#type' => 'markup',
      '#markup' => '<a href="#" title="' . t('Remove Block') . '" class="remove">' . t('Remove Block') . '</a>',
    );
    $form[$block_region]['inner'][$key]['info'] = array(
      '#markup' => check_plain($b['info']),
    );
    $form[$block_region]['inner'][$key]['bid'] = array(
      '#type' => 'value',
      '#default_value' => $b['bid'],
      '#attributes' => array(
        'class' => array('bid'),
      ),
    );
    $form[$block_region]['inner'][$key]['manage'] = array(
      '#type' => 'hidden',
      '#default_value' => $block_region == 'managed_blocks' ? 1 : 0,
      '#attributes' => array(
        'class' => array('manage'),
      ),
    );
    $form[$block_region]['inner'][$key]['weight'] = array(
      '#type' => 'hidden',
      '#default_value' => $weight,
      '#attributes' => array(
        'class' => array('weight'),
      ),
    );
  }

  $form += $extra_fields;

  $form['actions'] = array(
    '#tree' => FALSE,
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save blocks'),
  );

  return $form;
}

/**
 * Handle form submissions for the manage page.
 */
function block_manager_manage_blocks_submit($form, $form_state) {

  $region = $form_state['values']['region'];
  $page = $form_state['values']['page'];
  $theme = $form_state['values']['theme'];

  $pages = array();
  if ($page == 'node') {
    $pages[] = '<front>';
  }
  $pages[] = $page;
  $drupal_path = NULL;
  if (($drupal_path = drupal_lookup_path('source', $page))) {
    $pages[] = $drupal_path;
  }

  // Use only the node/:nid version of the path for the DB.
  $page = $pages[count($pages) - 1];

  // Get the currently managed blocks for this page and region.
  $managed_blocks = _block_manager_get_region_blocks($region, $pages, $theme);

  if (!empty($managed_blocks)) {
    $managed_blocks = $managed_blocks[$region];
  }

  $blocks = array();
  if (array_key_exists('available_blocks', $form_state['values'])) {
    $blocks += $form_state['values']['available_blocks']['inner'];
  }
  if (array_key_exists('managed_blocks', $form_state['values'])) {
    $blocks += $form_state['values']['managed_blocks']['inner'];
  }

  // List the blocks by bid.
  foreach ($blocks as $key => $b) {
    unset($blocks[$key]);
    $blocks[$b['bid']] = $b;
  }

  foreach ($managed_blocks as $bid => $info) {
    if ($blocks[$bid]['manage'] == 1 && $blocks[$bid]['weight'] != $info['weight']) {
      // Update the weight.
      db_update('block_manager')
        ->fields(array(
          'weight' => $blocks[$bid]['weight'],
        ))
        ->condition('bid', $bid)
        ->condition('url', $page)
        ->condition('region', $region)
        ->condition('theme', $theme)
        ->execute();
    }
    elseif ($blocks[$bid]['manage'] == 0) {
      // Remove the block from the block manager table.
      db_delete('block_manager')
        ->condition('bid', $bid)
        ->condition('url', $page)
        ->condition('region', $region)
        ->condition('theme', $theme)
        ->execute();
    }
  }

  // Add any new blocks to the DB.
  foreach ($blocks as $bid => $info) {
    if ($blocks[$bid]['manage'] == 1 && !array_key_exists($bid, $managed_blocks)) {
      db_insert('block_manager')
        ->fields(array(
          'bid' => $bid,
          'url' => $page,
          'region' => $region,
          'weight' => $info['weight'],
          'theme' => $theme,
        ))
        ->execute();
    }
  }
}

/**
 * Copy the blocks from the block database table to the block_manager table.
 *
 * @param string $theme
 *   theme to copy block data for
 * @param string $region
 *   region to copy block data for
 */
function _block_manager_copy_block_db($theme = NULL, $region = NULL) {

  $blocks_query = db_query(
    'SELECT b.bid, b.weight, b.theme, b.region, b.pages FROM {block} b ' .
      'WHERE b.status = :status AND b.visibility = :visibility ' .
      'AND b.pages <> :pages AND b.theme = :theme AND b.region = :region ' .
      'ORDER BY delta',
    array(
      ':status' => 1,
      ':visibility' => 1,
      ':pages' => '',
      ':theme' => $theme,
      ':region' => $region,
    )
  );

  $page_blocks = $blocks_query->fetchAll();

  $rows = array();
  foreach($page_blocks as $block) {
    // Create a new entry for each page for this block.
    foreach (preg_split('/[\n\r]/', $block->pages, NULL, PREG_SPLIT_NO_EMPTY) as $page) {
      if ($page == '<front>') {
        $url = '<front>';
      }
      else if (menu_get_item($page)) { // The given path is already valid.
        $url = $page;
      }
      else { // Generate the system path from the given alias, if possible.
        $url = drupal_lookup_path('source', $page);
      }

      if ($url) {
        $values = array(
          'bid' => $block->bid,
          'url' => $url,
          'region' => $region,
          'weight' => $block->weight,
          'theme' => $theme,
        );
        $rows[] = $values;
      }
    }
  }

  $query = db_insert('block_manager')
    ->fields(array('bid', 'url', 'region', 'weight', 'theme'));
  foreach ($rows as $row) {
    $query->values($row);
  }
  $query->execute();
}

/**
 * Build a row for a region in the block manager config page.
 *
 * @param String $key
 *   Machine name of the region this row represents
 * @param String $name
 *   Human-readable name of the region this row represents
 * @param Boolean $enabled
 *   Whether this region is already managed by Block Manager
 * @param String $layout
 *   "vertical" or "horizontal" - the layout to use for the Block Manager page
 * @param Boolean $limit
 *   Whether to limit this region to specific nodeblock types
 * @param array $nodes
 *   An array of strings representing the node types allowed in this region
 */
function _block_manager_build_row($key, $name, $enabled, $layout, $limit = NULL, $nodes = array()) {
  static $all_types = FALSE;

  $form = array(
    '#tree' => TRUE,
  );

  $form['manage'] = array(
    '#type' => 'checkbox',
    '#title' => t('Manage'),
    '#default_value' => $enabled,
    '#id' => $key,
  );
  $form['region_name'] = array(
    '#markup' => check_plain($name),
  );
  $form['layout'] = array(
    '#type' => 'select',
    '#title' => t('Layout'),
    '#options' => array(
      'vertical' => t('vertical'),
      'horizontal' => t('horizontal'),
    ),
    '#default_value' => $layout,
  );

  if (module_exists('nodeblock')) {
    // Get all available node types, but only do it once per execution.
    if (FALSE === $all_types) {
      $all_types = array();
      foreach (node_type_get_types() as $type => $info) {
        if (variable_get('nodeblock_' . $type, 0) != 0) {
          $all_types[$type] = $info->name;
        }
      }
    }

    if (empty($all_types)) {
      $disable = TRUE;
    }
    else {
      $disable = FALSE;
    }

    // Add form fields.
    $form['limit_types'] = array(
      '#type' => 'checkbox',
      '#title' => t('Limit Allowed Blocks'),
      '#default_value' => $limit,
      '#disabled' => $disable,
    );

    $form['allowed_types'] = array(
      '#type' => 'select',
      '#multiple' => TRUE,
      '#default_value' => empty($nodes) ? NULL : $nodes,
      '#options' => $all_types,
      '#disabled' => $disable,
    );
  }
  else {
    $form['limit_types'] = array(
      '#type' => 'value',
      '#value' => 0,
    );
    $form['allowed_types'] = array(
      '#type' => 'value',
      '#value' => '',
    );
  }

  return $form;
}

/**
 * Get all configuration options for block manager as an array.
 */
function _block_manager_get_full_config() {
  $config_result = db_query('SELECT * FROM {block_manager_config}')->fetchAll();

  $config = array();
  foreach ($config_result as $c) {
    $config[$c->theme][$c->region] = array(
      'manage' => (bool) $c->manage,
      'layout' => $c->layout,
      'limit_types' => $c->limit_types,
      'allowed_types' => unserialize($c->allowed_types),
    );
  }

  return $config;
}
