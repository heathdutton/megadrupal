<?php

/**
 * Menu callback: confirm rebuilding of relations.
 */
function og_menu_link_admin_rebuild_confirm() {
  return confirm_form(array(), t('Are you sure you want to rebuild the relations between menu links and groups?'),
                  'admin/reports/status', t('This action rebuilds all relations between menu links and groups, and may be a lengthy process. This action cannot be undone.'), t('Rebuild relations'), t('Cancel'));
}

/**
 * Handler for rebuild confirmation.
 *
 * @see og_menu_link_admin_rebuild_confirm()
 */
function og_menu_link_admin_rebuild_confirm_submit($form, &$form_state) {
  db_truncate('og_menu_link')->execute();
  $batch = array(
    'title' => t('Determining relations between menu links and groups.'),
    'file' => drupal_get_path('module', 'og_menu_link') . '/og_menu_link.admin.inc',
    'operations' => array(
      array('og_menu_link_admin_rebuild_batch_operation', array())
    ),
  );
  batch_set($batch);
  $form_state['redirect'] = 'admin/reports/status';
}

/**
 * Batch operation: determines and stores relations between menu links and groups.
 *
 * This is a multistep operation, iterating all nodes with a menu link by
 * packs of 20.
 *
 * @see og_menu_link_admin_rebuild_confirm_submit()
 */
function og_menu_link_admin_rebuild_batch_operation(&$context) {
  $query = db_select('menu_links', 'ml', array('fetch' => PDO::FETCH_ASSOC));
  $query->leftJoin('menu_router', 'm', 'm.path = ml.router_path');
  $query->fields('ml');
  // Weight should be taken from {menu_links}, not {menu_router}.
  $query->addField('ml', 'weight', 'link_weight');
  $query->fields('m');
  // Order by depth so parents are processed first.
  $query->orderBy('depth');

  if (empty($context['sandbox'])) {
    // Initiate multistep processing.
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = $query->countQuery()->execute()->fetchField();
  }

  // Process the next 20 menu links.
  $limit = 20;
  $menu_links = $query
    ->range($context['sandbox']['progress'], $limit)
    ->execute();

  foreach ($menu_links as $item) {
    $item['weight'] = $item['link_weight'];
    _menu_link_translate($item);

    og_menu_link_relations_determine($item);
  }
  $context['sandbox']['progress'] += $limit;

  // Multistep processing: report progress.
  if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}
