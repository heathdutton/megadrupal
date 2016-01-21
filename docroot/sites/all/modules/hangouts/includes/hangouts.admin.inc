<?php

function hangouts_add_block_form($form, &$form_state) {
  module_load_include('inc', 'block', 'block.admin');
  module_load_include('inc', 'hangouts', 'includes/hangouts');
  $form = block_admin_configure($form, $form_state, 'hangouts', NULL);
  $form['hangouts_block_gid'] = array(
    '#type' => 'textfield',
    '#title' => t('GID'),
    '#required' => TRUE,
    '#weight' => -2,
  );
  $form['hangouts_block_size'] = array(
    '#type' => 'radios',
    '#title' => t('Size of the button'),
    '#default_value' => variable_get('hangouts_block_size', '79x15'),
    '#options' => hangouts_get_icon_images(),
    '#weight' => -1,
  );
  $form_state['build_info']['base_form_id'] = 'block_add_block_form';
  $form['#validate'] = array();
  $form['#submit'] = array('hangouts_add_block_form_submit');
  return $form;
}

function hangouts_add_block_form_submit($form, &$form_state) {
  $block_ids = variable_get('hangouts_ids', array());
  $delta = empty($block_ids) ? 1 : max($block_ids) + 1;
  $form_state['values']['delta'] = $delta;
  $block_ids[] = $delta;
  variable_set('hangouts_ids', $block_ids);
  hangouts_block_save($delta, $form_state['values']);
  $query = db_insert('block')->fields(array(
    'visibility',
    'pages',
    'custom',
    'title',
    'module',
    'theme',
    'region',
    'status',
    'weight',
    'delta',
    'cache')
  );
  foreach (list_themes() as $key => $theme) {
    if ($theme->status) {
      $region = !empty($form_state['values']['regions'][$theme->name]) ? $form_state['values']['regions'][$theme->name] : BLOCK_REGION_NONE;
      $query->values(array(
        'visibility' => (int) $form_state['values']['visibility'],
        'pages' => trim($form_state['values']['pages']),
        'custom' => (int) $form_state['values']['custom'],
        'title' => $form_state['values']['title'],
        'module' => $form_state['values']['module'],
        'theme' => $theme->name,
        'region' => ($region == BLOCK_REGION_NONE ? '' : $region),
        'status' => 0,
        'status' => (int) ($region != BLOCK_REGION_NONE),
        'weight' => 0,
        'delta' => $delta,
        'cache' => DRUPAL_NO_CACHE,
      ));
    }
  }
  $query->execute();
  $query = db_insert('block_role')->fields(array('rid', 'module', 'delta'));
  foreach (array_filter($form_state['values']['roles']) as $rid) {
    $query->values(array(
      'rid' => $rid,
      'module' => $form_state['values']['module'],
      'delta' => $delta,
    ));
  }
  $query->execute();
  drupal_set_message(t('The block has been created.'));
  cache_clear_all();
  $form_state['redirect'] = 'admin/structure/block';
}

function hangouts_delete($form, &$form_state, $delta = 0) {
  $title = "Hangout-" . $delta;
  $form['block_title'] = array('#type' => 'hidden', '#value' => $title);
  $form['delta'] = array('#type' => 'hidden', '#value' => $delta);
  $question = t('Are you sure you want to delete the "%name" block?', array('%name' => $title));
  $path = 'admin/structure/block';
  return confirm_form($form, $question, $path, NULL, t('Delete'), t('Cancel'));
}

function hangouts_delete_submit($form, &$form_state) {
  $delta = $form_state['values']['delta'];
  $block_ids = variable_get('hangouts_ids', array());
  unset($block_ids[array_search($delta, $block_ids)]);
  sort($block_ids);
  variable_set('hangouts_ids', $block_ids);
  variable_del("hangouts_{$delta}_gid");
  variable_del("hangouts_{$delta}_size");
  db_delete('block')
    ->condition('module', 'hangouts')
    ->condition('delta', $delta)
    ->execute();
  db_delete('block_role')
    ->condition('module', 'hangouts')
    ->condition('delta', $delta)
    ->execute();
  drupal_set_message(t('The block "%name" has been removed.', array('%name' => $form_state['values']['block_title'])));
  cache_clear_all();
  $form_state['redirect'] = 'admin/structure/block';
}
