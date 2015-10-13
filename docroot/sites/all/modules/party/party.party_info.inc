<?php
/**
 * @file
 * Party info hook include.
 */

/**
 * Implements hook_party_party_pieces().
 *
 * Defines:
 *  - the basic piece for showing a party itself
 *  - pieces that data sets provide
 *  - pieces provided by Views as display plugins.
 */
function party_party_party_pieces() {
  $pieces = array(
    // The main party piece.
    // By default this is the default tab, but the MENU_DEFAULT_LOCAL_TASK
    // is supplied in hook_menu() to allow the piece order setting to change
    // the default tab accordingly.
    'party' => array(
      'title' => 'Party',
      'page callback' => 'party_page_view',
      'page arguments' => array(1),
      'file' => 'party.pages.inc',
      'access arguments' => array('view contacts'),
      'weight' => -10,
    ),
  );

  // Party data sets may provide a piece each. Here we handle those of type
  // 'callback'; type 'views' is handled by our party_views_default_views()
  // and our Views display plugin.
  $sets = party_get_data_set_info();
  //dsm($sets);
  foreach ($sets as $set_name => $set) {
    if (isset($set['piece']) && in_array($set['piece']['maker'], array('callback', 'core'))) {
      if ($set['piece']['maker'] == 'core') {
        // 'core' is a special case of 'callback' where we supply the details.
        $set['piece'] += array(
          'title' => $set['label'],
          'page callback' => 'party_view_data_set',
          'page arguments' => array(1, $set_name),
          'access callback' => 'party_access',
          'access arguments' => array('view', 1, $set_name),
          'file' => 'party.pages.inc',
        );
      }

      // The path becomes the piece's key.
      $path = $set['piece']['path'];
      unset($set['piece']['path']);
      // Add a key to say we came from a dataset.
      $set['piece']['data_set'] = $set_name;

      $pieces[$path] = $set['piece'];
    }
  }

  // Custom pieces can be created as Views plugins.
  if (module_exists('views')) {
    // Get all views displays that implement our hook.
    // There's no need to cache: views_menu() doesn't cache for page displays.
    $views = views_get_applicable_views('uses hook party_party_pieces');
    foreach ($views as $data) {
      list($view, $display_id) = $data;
      $result = $view->display[$display_id]->handler->execute_hook_party_party_pieces();
      if (is_array($result)) {
        // Add in access properties.
        foreach ($result as $path => $piece) {
          $result[$path] += array(
            'access callback' => 'party_access',
            'access arguments' => array('view', 1, $piece['data_set']),
          );
        }

        $pieces = array_merge($pieces, $result);
      }
    }
  }

  return $pieces;
}

/**
 * Implements hook_party_primary_fields_sources_alter().
 */
function party_party_primary_fields_sources_alter(&$sources) {
  // Allow pid to be converted to 'Party {pid}'.
  if (isset($sources['party']['sources']['party:pid'])) {
    $sources['party']['sources']['party:pid']['callbacks']['party_label_default'] = array(
      'label' => t('Party default label'),
      'callback' => 'party_primary_fields_callback_party_label_default',
      'type' => 'text',
      'file' => array(
        'type' => 'inc',
        'module' => 'party',
        'name' => 'party.party',
      ),
    );
  }

  // Find any name fields.
  foreach ($sources as &$data_set) {
    foreach ($data_set['sources'] as &$source_info) {
      if ($source_info['field_type'] == 'name') {
        $source_info['callbacks']['party_name_field_label'] = array(
          'label' => t('Convert Name Field to Label'),
          'callback' => 'party_primary_fields_callback_party_name_field_label',
          'type' => 'text',
          'file' => array(
            'type' => 'inc',
            'module' => 'party',
            'name' => 'party.party',
          ),
        );
      }
    }
  }
}
