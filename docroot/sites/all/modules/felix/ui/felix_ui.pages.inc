<?php
/**
 * @file
 * Functions relating to menu callbacks for the Felix UI module.
 */

/**
 * Menu callback for the regions page.
 */
function felix_ui_regions_page() {
  $output = '';

  $header = array(t('Title'), t('Context'), t('Region'), t('Operations'));
  $rows = array();

  $regions = db_select('felix_region', 'cbr')
          ->fields('cbr', array('name', 'title', 'context', 'region'))
          ->orderBy('title')
          ->execute();
  while ($region = $regions->fetchObject()) {
    $operations = array(
      l(t('Edit'), "admin/structure/felix/regions/{$region->name}/edit"),
      l(t('Delete'), "admin/structure/felix/regions/{$region->name}/delete"),
    );
    $rows[] = array(
      check_plain($region->title),
      check_plain($region->context),
      check_plain($region->region),
      implode(' ', $operations),
    );
  }

  if ($rows) {
    $output .= theme('table', array('header' => $header, 'rows' => $rows));
  }
  else {
    $output .= '<p>' . t('There are no regions defined yet.') . '</p>';
  }

  return $output;
}

/**
 * Add / edit region form callback.
 */
function felix_ui_region_form($form, &$form_state, $region = NULL) {
  if ($region) {
    $region = db_select('felix_region', 'cbr')
      ->fields('cbr', array('name', 'title', 'region', 'weight', 'context', 'block_set', 'data'))
      ->condition('cbr.name', $region)
      ->execute()
      ->fetchObject();
    if (!$region) {
      drupal_not_found();
      module_invoke_all('exit');
      exit;
    }

    $region->data = unserialize($region->data);
    $form['#felix_region'] = $region;
  }

  if (empty($form_state['storage'])) {
    $values = array();
    $rehash = FALSE;
  }
  else {
    $values = $form_state['storage']['values'];
    $rehash = !empty($form_state['storage']['rehash']);
  }

  if ($rehash) {
    // Build the second page (rehash review / options).

    $added = $form_state['storage']['hash_added'];
    $removed = $form_state['storage']['hash_removed'];
    $form['info'] = array(
      '#markup' => '<p>' . t('You have changed the differentiate content option. This requires us to update all blocks in a batch process. This proces will start after clicking the submit button. Please review the settings below.') . '</p>',
    );
    $form['added'] = array(
      '#markup' => '<p>' . t('Added') . ': ' . ($added ? check_plain(implode(', ', $added)) : t('none')) . '</p>',
    );
    $form['removed'] = array(
      '#markup' => '<p>' . t('Removed') . ': ' . ($removed ? check_plain(implode(', ', $removed)) : t('none')) . '</p>',
    );
    if ($added) {
      $form['info_parts'] = array(
        '#markup' => '<p>' . t('The information for the added parts is not available for existing blocks. You need to provide a default value for these fields.') . '</p>',
      );
      $form['parts'] = array(
        '#tree' => TRUE,
      );
      foreach ($added as $part) {
        $form['parts'][$part] = array(
          '#type' => 'textfield',
          '#title' => t('Default value for %part', array('%part' => $part)),
          '#default_value' => !empty($values['parts']) && !empty($values['parts'][$part]) ? $values['parts'][$part] : '',
        );
      }
    }

    $form['rehash'] = array(
      '#type' => 'submit',
      '#value' => t('Save and rehash'),
    );
  }
  else {
    // Build the first page.

    $block_sets = db_select('felix_block_set', 'cbs')
            ->fields('cbs', array('name', 'title'))
            ->orderBy('title')
            ->execute()
            ->fetchAllKeyed();
    if (!$block_sets) {
      $form['info'] = array(
        '#markup' => '<p>' . t('You have to <a href="@url">create a block set</a> first before you can create a new region.', array(
          '@url' => url('admin/structure/felix/sets/add'),
        )) . '</p>',
      );
      return $form;
    }

    $contexts = context_context_list();
    if (!$contexts) {
      $form['info'] = array(
        '#markup' => '<p>' . t('You have to <a href="@url">create a context</a> before you can create a new region.', array(
          '@url' => url('admin/structure/context/add'),
        )) . '</p>',
      );
      return $form;
    }

    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => $region ? $region->title : '',
      '#required' => TRUE,
    );

    $form['name'] = array(
      '#type' => 'machine_name',
      '#title' => t('Machine name'),
      '#default_value' => $region ? $region->name : '',
      '#maxlength' => 64,
      '#required' => TRUE,
      '#machine_name' => array(
        'exists' => 'felix_ui_region_name_exists',
        'source' => array('title'),
        'label' => t('Name'),
        'replace_pattern' => '[^a-z0-9-]+',
        'replace' => '-',
      ),
      '#disabled' => empty($region) ? FALSE : TRUE,
    );

    $options = array();
    $themes = system_get_info('theme');
    foreach ($themes as $theme) {
      $options[$theme['name']] = $theme['regions'];
    }
    $form['region'] = array(
      '#type' => 'select',
      '#title' => t('Region'),
      '#options' => $options,
      '#default_value' => $region ? $region->region : '',
    );

    $form['context'] = array(
      '#type' => 'select',
      '#title' => t('Context'),
      '#options' => $contexts,
      '#default_value' => $region ? $region->context : '',
    );

    $form['weight'] = array(
      '#type' => 'textfield',
      '#title' => t('Weight'),
      '#size' => 6,
      '#default_value' => $region ? $region->weight : 0,
      '#required' => TRUE,
    );

    $form['block_set'] = array(
      '#type' => 'select',
      '#title' => t('Block set'),
      '#options' => $block_sets,
      '#default_value' => $region ? $region->block_set : '',
    );

    $form['hash'] = array(
      '#tree' => TRUE,
      '#type' => 'fieldset',
      '#title' => t('Differentiate content per'),
      '#description' => t('This setting allows you to stick blocks to pages matching specified criteria. For example, if you choose nodetype, all article pages will have the same blocks, but blog pages will have different blocks.'),
      '#collapsible' => FALSE,
    );
    $options = module_invoke_all('felix_hash_options');
    foreach ($options as $name => $description) {
      $form['hash'][$name] = array(
        '#prefix' => '<div class="felix-ui-hash">',
        '#suffix' => '</div>',
      );
      $form['hash'][$name]['enabled'] = array(
        '#type' => 'checkbox',
        '#title' => $description,
        '#default_value' => $region && in_array($name, $region->data['hash']),
      );
      $values = $region && !empty($region->data['hash_config'][$name]) ? $region->data['hash_config'][$name] : array();
      $config = module_invoke_all('felix_hash_config', $name, $values);
      if ($config) {
        $form['hash'][$name]['config'] = $config;
        $form['hash'][$name]['config']['#prefix'] = '<div class="felix-ui-hash-config">';
        $form['hash'][$name]['config']['#suffix'] = '</div>';
      }
    }

    $form['#attached']['js'] = array(
      drupal_get_path('module', 'felix_ui') . '/felix_ui.js',
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
  }

  return $form;
}

/**
 * Form validate callback for the region form.
 */
function felix_ui_region_form_validate($form, &$form_state) {
  if (isset($form_state['values']['weight'])) {
    $weight = $form_state['values']['weight'];
    if (!is_numeric($weight) || $weight < -127 || $weight > 128) {
      form_set_error('weight', t('Weight must be a number between -127 and 128.'));
    }
  }
}

/**
 * Form submit callback for the region form.
 */
function felix_ui_region_form_submit($form, &$form_state) {
  $values = empty($form_state['storage']) ? array() : $form_state['storage']['values'];
  $values += $form_state['values'];

  $region = new stdClass();
  $region->name = $values['name'];
  $region->title = $values['title'];
  $region->region = $values['region'];
  $region->weight = $values['weight'];
  $region->context = $values['context'];
  $region->block_set = $values['block_set'];

  $data = empty($form['#felix_region']) ? array() : $form['#felix_region']->data;
  $data['hash'] = array();
  $data['hash_config'] = array();
  foreach (element_children($values['hash']) as $name) {
    $item = $values['hash'][$name];
    if ($item['enabled']) {
      $data['hash'][] = $name;
      if (!empty($item['config'])) {
        $data['hash_config'][$name] = $item['config'];
      }
    }
  }
  if (empty($data['hash_config'])) {
    // No need to save this value.
    // Remove it to keep consistence with old feature modules.
    unset($data['hash_config']);
  }
  $region->data = serialize($data);

  $rehash = FALSE;
  if (!empty($form['#felix_region'])) {
    $removed = array_values(array_diff($form['#felix_region']->data['hash'], $data['hash']));
    $added = array_values(array_diff($data['hash'], $form['#felix_region']->data['hash']));
    $rehash = $removed || $added;
  }

  if ($rehash) {
    if ($form_state['clicked_button']['#value'] == t('Save and rehash')) {
      // Save region and start batch to rehash blocks.
      drupal_write_record('felix_region', $region, empty($form['#felix_region']) ? array() : array('name'));
      module_load_include('inc', 'felix', 'felix.rehash');
      felix_batch_rehash_start($region, $added, $removed, isset($values['parts']) ? $values['parts'] : array());
    }
    else {
      // Rebuild the form for the second step.
      $form_state['rebuild'] = TRUE;
      $form_state['storage'] = array(
        'values' => $form_state['values'],
        'rehash' => TRUE,
        'hash_added' => $added,
        'hash_removed' => $removed
      );
    }
  }
  else {
    drupal_write_record('felix_region', $region, empty($form['#felix_region']) ? array() : array('name'));
    drupal_set_message(t('The region has been saved succesfully.'));
    $form_state['redirect'] = 'admin/structure/felix';
  }
}

/**
 * Delete region form callback.
 */
function felix_ui_delete_region_form($form, &$form_state, $region) {
  $region = db_select('felix_region', 'cbr')
    ->fields('cbr', array('name', 'title'))
    ->condition('cbr.name', $region)
    ->execute()
    ->fetchObject();
  if (!$region) {
    drupal_not_found();
    module_invoke_all('exit');
    exit;
  }

  $form['#felix_region'] = $region;

  $info = '<p>' . t('Are you sure you want to delete the region %title?', array('%title' => $region->title)) . '</p>';

  $form['info'] = array(
    '#type' => 'markup',
    '#markup' => $info,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );

  $form['cancel'] = array(
    '#type' => 'markup',
    '#markup' => l(t('cancel'), 'admin/structure/felix'),
  );

  return $form;
}

/**
 * Form submit handler for the delete region form.
 */
function felix_ui_delete_region_form_submit($form, &$form_state) {
  db_delete('felix_region')->condition('name', $form['#felix_region']->name)->execute();
  db_delete('felix_block')->condition('region', $form['#felix_region']->name)->execute();
  drupal_set_message(t('The region %title has been deleted.', array('%title' => $form['#felix_region']->title)));
  $form_state['redirect'] = 'admin/structure/felix';
}

/**
 * Menu callback for the block sets page.
 */
function felix_ui_sets_page() {
  $output = '';

  $header = array(t('Title'), t('Operations'));
  $rows = array();

  $sets = db_select('felix_block_set', 'cbs')
          ->fields('cbs', array('name', 'title'))
          ->orderBy('title')
          ->execute();
  while ($set = $sets->fetchObject()) {
    $operations = array(
      l(t('Edit'), "admin/structure/felix/sets/{$set->name}/edit"),
      l(t('Delete'), "admin/structure/felix/sets/{$set->name}/delete"),
    );
    $rows[] = array(
      check_plain($set->title),
      implode(' ', $operations),
    );
  }

  if ($rows) {
    $output .= theme('table', array('header' => $header, 'rows' => $rows));
  }
  else {
    $output .= '<p>' . t('There are no block sets defined yet.') . '</p>';
  }

  return $output;
}

/**
 * Add / edit block set form callback.
 */
function felix_ui_set_form($form, &$form_state, $set = NULL) {
  if ($set) {
    $set = db_select('felix_block_set', 'cbs')
      ->fields('cbs', array('name', 'title'))
      ->condition('cbs.name', $set)
      ->execute()
      ->fetchObject();
    if (!$set) {
      drupal_not_found();
      module_invoke_all('exit');
      exit;
    }

    $res = db_select('felix_block_set_block', 'cbsb')
      ->fields('cbsb', array('module', 'delta'))
      ->condition('block_set', $set->name)
      ->execute();
    $set->blocks = array();
    while ($block = $res->fetchObject()) {
      $set->blocks[$block->module][] = $block->delta;
    }

    $set->nodetypes = db_select('felix_block_set_nodetype', 'cbsn')
      ->fields('cbsn', array('nodetype'))
      ->condition('block_set', $set->name)
      ->execute()
      ->fetchCol();

    $set->viewmodes = db_select('felix_block_set_viewmode', 'cbsv')
      ->fields('cbsv', array('viewmode'))
      ->condition('block_set', $set->name)
      ->execute()
      ->fetchCol();

    $form['#felix_set'] = $set;
  }

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $set ? $set->title : '',
    '#required' => TRUE,
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine name'),
    '#default_value' => $set ? $set->name : '',
    '#maxlength' => 64,
    '#required' => TRUE,
    '#machine_name' => array(
      'exists' => 'felix_ui_set_name_exists',
      'source' => array('title'),
      'label' => t('Name'),
      'replace_pattern' => '[^a-z0-9-]+',
      'replace' => '-',
    ),
    '#disabled' => empty($set) ? FALSE : TRUE,
  );

  foreach (module_implements('block_info') as $module) {
    $info = system_get_info('module', $module);
    $name = $info['name'];
    $package = $info['package'];

    if (!isset($form['packages'][$package])) {
      $form['packages'][$package] = array(
        '#type' => 'fieldset',
        '#title' => t($package),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
    }

    $options = array(
      '*' => '<em>' . t('All') . '</em>',
    );
    $blocks = module_invoke($module, 'block_info');
    foreach ($blocks as $delta => $block) {
      $options[$delta] = $block['info'];
    }

    $form['packages'][$package]["module_$module"] = array(
      '#type' => 'checkboxes',
      '#title' => check_plain($name),
      '#options' => $options,
      '#default_value' => empty($set) || empty($set->blocks[$module]) ? array() : drupal_map_assoc($set->blocks[$module]),
    );
  }

  ksort($form['packages']);

  $options = array();
  foreach (node_type_get_types() as $name => $type) {
    $options[$name] = $type->name;
  }
  asort($options);
  $form['nodetypes'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node types'),
    '#options' => $options,
      '#default_value' => empty($set) ? array() : drupal_map_assoc($set->nodetypes),
    '#description' => t('Check the nodetypes which can be used in this set.')
  );

  $options = array();
  $entity_info = entity_get_info('node');
  foreach ($entity_info['view modes'] as $view_mode => $info) {
    $options[$view_mode] = $info['label'];
  }
  $form['viewmodes'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#title' => t('View modes'),
    '#default_value' => empty($set) ? array('felix' => 'felix') : drupal_map_assoc($set->viewmodes),
    '#description' => t('Check the view modes which can be used in this set.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Validate handler for the set form.
 */
function felix_ui_set_form_validate($form, &$form_state) {
  // If you have selected 1+ content types, you should also have enabled at least one view mode.
  $content_types_selected = array_filter($form_state['values']['nodetypes']);
  $view_modes_selected = array_filter($form_state['values']['viewmodes']);
  if ($content_types_selected && !$view_modes_selected) {
    form_set_error('viewmodes', t('If you enable content types for this block set, you should enable at least one view mode to render the content types in.'));
  }
}

/**
 * Submit handler for the set form.
 */
function felix_ui_set_form_submit($form, &$form_state) {
  $set = new stdClass();
  $set->name = $form_state['values']['name'];
  $set->title = $form_state['values']['title'];
  drupal_write_record('felix_block_set', $set, empty($form['#felix_set']) ? array() : array('name'));

  if (!empty($form['#felix_set'])) {
    db_delete('felix_block_set_block')->condition('block_set', $form_state['values']['name'])->execute();
    db_delete('felix_block_set_nodetype')->condition('block_set', $form_state['values']['name'])->execute();
    db_delete('felix_block_set_viewmode')->condition('block_set', $form_state['values']['name'])->execute();
  }

  foreach ($form_state['values'] as $name => $value) {
    if (!preg_match('/^module_(.+)$/s', $name, $match)) {
      continue;
    }
    foreach ($value as $delta => $checked) {
      if (!$checked) {
        continue;
      }
      $block = new stdClass();
      $block->block_set = $form_state['values']['name'];
      $block->module = $match[1];
      $block->delta = $delta;
      drupal_write_record('felix_block_set_block', $block);
    }
  }

  foreach ($form_state['values']['nodetypes'] as $name => $value) {
    if (!$value) {
      continue;
    }
    $nodetype = new stdClass();
    $nodetype->block_set = $form_state['values']['name'];
    $nodetype->nodetype = $name;
    drupal_write_record('felix_block_set_nodetype', $nodetype);
  }

  foreach ($form_state['values']['viewmodes'] as $name => $value) {
    if (!$value) {
      continue;
    }
    $viewmode = new stdClass();
    $viewmode->block_set = $form_state['values']['name'];
    $viewmode->viewmode = $name;
    drupal_write_record('felix_block_set_viewmode', $viewmode);
  }

  drupal_set_message(t('The block set was saved succesfully.'));
  $form_state['redirect'] = 'admin/structure/felix/sets';
}

/**
 * Delete block set form callback.
 */
function felix_ui_delete_set_form($form, &$form_state, $set) {
  $set = db_select('felix_block_set', 'cbs')
    ->fields('cbs', array('name', 'title'))
    ->condition('cbs.name', $set)
    ->execute()
    ->fetchObject();
  if (!$set) {
    drupal_not_found();
    module_invoke_all('exit');
    exit;
  }

  $form['#felix_set'] = $set;

  $regions = db_select('felix_region', 'cr')
    ->fields('cr', array('title'))
    ->condition('cr.block_set', $set->name)
    ->orderBy('title')
    ->execute()
    ->fetchCol();

  if ($regions) {
    $info = '<p>' . t('You cannot delete this set because it is in use for the following set(s):') . '</p>';
    $info .= theme('item_list', $regions);
  }
  else {
    $info = '<p>' . t('Are you sure you want to delete the block set %title?', array('%title' => $set->title)) . '</p>';
  }

  $form['info'] = array(
    '#type' => 'markup',
    '#markup' => $info,
  );

  if (!$regions) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
    );
  }

  $form['cancel'] = array(
    '#type' => 'markup',
    '#markup' => l(t('cancel'), 'admin/structure/felix/sets'),
  );

  return $form;
}

/**
 * Form submit handler for the delete block set form.
 */
function felix_ui_delete_set_form_submit($form, &$form_state) {
  db_delete('felix_block_set')->condition('name', $form['#felix_set']->name)->execute();
  db_delete('felix_block_set_block')->condition('block_set', $form['#felix_set']->name)->execute();
  db_delete('felix_block_set_nodetype')->condition('block_set', $form['#felix_set']->name)->execute();
  db_delete('felix_block_set_viewmode')->condition('block_set', $form['#felix_set']->name)->execute();

  drupal_set_message(t('The block set %title has been deleted.', array('%title' => $form['#felix_set']->title)));
  $form_state['redirect'] = 'admin/structure/felix/sets';
}

/**
 * Helper function for the machine name field.
 */
function felix_ui_set_name_exists($name) {
  $title = db_select('felix_block_set', 'cbs')
    ->fields('cbs', array('title'))
    ->condition('cbs.name', $name)
    ->execute()
    ->fetchField();
  return (bool) $title;
}

/**
 * Helper function for the machine name field.
 */
function felix_ui_region_name_exists($name) {
  $title = db_select('felix_region', 'cbr')
    ->fields('cbr', array('title'))
    ->condition('cbr.name', $name)
    ->execute()
    ->fetchField();
  return (bool) $title;
}
