<?php

/**
 * @file
 * Search and explore related admin pages.
 */

define('HOOK_WILDCARD', '*');

/**
 * Menu autocomplete callback for hook search.
 */
function devcontrol_admin_hook_search_autocomplete($string) {

  $ret = array();
  $starts_with = false;

  if (0 === strpos($string, 'hook_')) {
    $string = substr($string, 5);
    // If the user typed 'hook_' this means that he knows what he's looking
    // for, case in which we definitely need to restrict the search.
    $starts_with = true;
  }

  // Clone, and not reference (we'll sort it).
  $cache = drupal_static('module_implements');
  ksort($cache);

  foreach ($cache as $name => $modules) {

    $pos = strpos($name, $string);

    if ($string === $name || ($starts_with && $pos === 0) || (!$starts_with && false !== $pos)) {
      $ret[$name] = $name . ' (' . count($modules) .')';
    }
  }

  drupal_json_output($ret);
  exit; // We don't care about cleanup tasks on autocomplete.
}

/**
 * Hook search form.
 */
function devcontrol_admin_hook_search_form($form, &$form_state, $hook = null) {

  $form['#tree'] = true;

  $form['header'] = array(
    '#type' => 'fieldset',
    '#title' => t("Hook name"),
    '#collapsible' => false, 
  );
  $form['header']['hook'] = array(
    '#type' => 'textfield',
    '#autocomplete_path' => 'developer/search/hook-search/autocomplete',
    '#required' => true,
    '#description' => t("You can use the * wildcard and hit search for multiple results."),
    '#default_value' => $hook,
  );
  $form['header']['search'] = array(
    '#type' => 'submit',
    '#value' => t("Search"),
  );

  // If we have a hook name let's display everything we can about it.
  if (isset($hook)) {

    // First cleanup the hook variable in order to remove the leading hook_ if
    // present, that will normalize the search.
    if (0 === strpos($hook, 'hook_')) {
      $hook = substr($hook, 5);
    }

    // Calling a first time module_implements() will force it to rebuild its
    // cache. If user hit the page without using the autocomplete, it allows
    // him to see data that the core hasn't cached yet.
    // Do this only if the hook name is valid.
    if (preg_match('/^[a-zA-Z_]+$/', $hook)) {
      module_implements($hook);
    }
    $cache = drupal_static('module_implements');
    $list = system_list('module_enabled');
    $candidates = array();

    // Find candidates (if many).
    if (false === strpos($hook, HOOK_WILDCARD)) {
      $candidates[$hook] = (isset($cache[$hook]) && !empty($cache[$hook]));
    } else {
      $pattern = '/^' . str_replace(HOOK_WILDCARD, '.*', $hook) . '$/';
      foreach ($cache as $hook => $modules) {
        if (preg_match($pattern, $hook)) {
          $candidates[$hook] = !empty($cache[$hook]);
        }
      }
    }

    foreach ($candidates as $hook => $found) {
      $options = array();

      $form['implements'][$hook] = array(
        '#type' => 'fieldset',
        '#title' => $hook . ' (' . count($cache[$hook]) . ')',
        '#collapsible' => true,
      );

      if ($found) {
        foreach ($cache[$hook] as $module => $group) {
          $options[$module]['name']['data']['#markup'] = $module;
          $options[$module]['group']['data']['#markup'] = $group ? $group : '';
          $options[$module]['weight']['data']['#markup'] = $list[$module]->weight;
        }

        $form['implements'][$hook]['data'] = array(
          '#type' => 'tableselect',
          '#options' => $options,
          '#header' => array(
            'name' => t("Module"),
            'group' => t("Group"),
            'weight' => t("Weight"),
          ),
        );
      } else {
        $form['implements'][$hook]['text']['#markup'] = t("No modules implement <strong>hook_@hook()</strong>", array(
          '@hook' => $hook,
        ));
        $form['implements'][$hook]['#collapsed'] = true;
      }
    }
  }

  return $form;
}

/**
 * Hook search form search action submit handler.
 */
function devcontrol_admin_hook_search_form_submit($form, &$form_state) {
  $hook = $form_state['values']['header']['hook'];
  $form_state['redirect'] = 'developer/search/hook-search/' . $hook;
}

/**
 * Menu autocomplete callback for hook search.
 */
function devcontrol_admin_entity_search_autocomplete($string) {

  $ret = array();
  $entity_info = entity_get_info();

  ksort($entity_info);

  foreach ($entity_info as $entity_type => $info) {

    if (false !== strpos($entity_type, $string)) {
      $ret[$entity_type] = $entity_type . ' - ' . $info['label'];
    } else if (false !== strpos($info['label'], $string)) {
      $ret[$entity_type] = $entity_type . ' - ' . $info['label'];
    } else {
      foreach ($info['bundles'] as $bundle => $bundle_info) {
        if ((isset($info['bundle']) && false !== strpos($info['bundle'], $string)) ||
            false !== strpos($bundle_info['label'], $string))
        {
          $ret[$entity_type] = $entity_type . ' - ' . $info['label'];
          break;
        }
      }
    }
  }

  drupal_json_output($ret);
  exit; // We don't care about cleanup tasks on autocomplete.
}

/**
 * Explore fields form.
 */
function devcontrol_admin_field_explore_form($form, &$form_state) {

  $types  = field_info_field_types();
  $etypes = entity_get_info();
  $yes    = t("Yes");
  $no     = "-";
  $fields = field_info_fields();

  $filter_fn = null;
  $filter_ft = null;
  $filter_et = null;
  $filter_eb = null;

  $filter_options_fn  = array();
  foreach ($fields as $field_name => $field) {
    $filter_options_fn[$field_name] = $field_name;
  }
  asort($filter_options_fn);

  $filter_options_ft = array();
  foreach ($types as $field_type => $field_type_info) {
    $filter_options_ft[$field_type] = t($field_type_info['label']) . ' (' . $field_type . ')';
  }
  asort($filter_options_ft);

  $filter_options_et = array();
  foreach ($etypes as $entity_type => $entity_info) {
    if ($entity_info['fieldable']) {
      $filter_options_et[$entity_type] =  t($entity_info['label']) . ' (' . $entity_type . ')';
    }
  }
  asort($filter_options_et);

  $form['#tree'] = true;

  $form['filter'] = array(
    '#type'        => 'fieldset',
    '#title'       => t("Filter"),
    '#collapsible' => false,
  );
  $form['filter']['fn'] = array(
    '#title'   => t("Field"),
    '#type'    => 'select',
    '#options' => array(null => '-- ' . t("None") . ' --') + $filter_options_fn,
  );
  $form['filter']['ft'] = array(
    '#title'   => t("Field type"),
    '#type'    => 'select',
    '#options' => array(null => '-- ' . t("None") . ' --') + $filter_options_ft,
  );
  $form['filter']['et'] = array(
    '#title'   => t("Entity type"),
    '#type'    => 'select',
    '#options' => array(null => '-- ' . t("None") . ' --') + $filter_options_et,
  );
  $form['filter']['eb'] = array(
    '#title'    => t("Entity bundle"),
    '#type'     => 'select',
    '#options'  => array(null => '-- ' . t("None") . ' --'),
    '#disabled' => true,
  );
  $form['filter']['actions'] = array(
    '#type'  => 'actions',
    'submit' => array(
      '#type'   => 'submit',
      '#value'  => t("Filter"),
      '#submit' => array('devcontrol_admin_field_explore_form_filter_submit'),
    ),
    'cancel' => array(
      '#markup' => l(t("Reset"), current_path()),
    ),
  );

  if (isset($_GET['fn']) && isset($filter_options_fn[$_GET['fn']])) {
    $filter_fn = $form['filter']['fn']['#default_value'] = $_GET['fn'];
  }
  if (isset($_GET['et']) && isset($filter_options_et[$_GET['et']])) {
    $filter_et = $form['filter']['et']['#default_value'] = $_GET['et'];

    // Add possible bundles to bundle field.
    $filter_options_eb = array();
    foreach ($etypes[$_GET['et']]['bundles'] as $bundle => $bundle_info) {
      $filter_options_eb[$bundle] = t($bundle_info['label']) . ' (' . $bundle . ')';
    }
    asort($filter_options_eb);

    // Undisabled it.
    $form['filter']['eb']['#options'] += $filter_options_eb;
    $form['filter']['eb']['#disabled'] = false;

    if (isset($_GET['eb']) && isset($filter_options_et[$_GET['et']])) {
      $filter_eb = $form['filter']['eb']['#default_value'] = $_GET['eb'];
    }
  }
  if (isset($_GET['ft']) && isset($filter_options_ft[$_GET['ft']])) {
    $filter_ft = $form['filter']['ft']['#default_value'] = $_GET['ft'];
  }

  // FIXME: For later, provide a filter and sorter.
  // uasort($array, $cmp_function);

  $options = array();
  foreach ($fields as $field_name => $field) {

    // Apply field name filter.
    if (isset($filter_fn) && $field_name !== $filter_fn) {
      continue;
    }
      // Apply field type filter.
    if (isset($filter_ft) && $filter_ft !== $field['type']) {
      continue;
    }
    // Apply entity type and bundle filter.
    if (isset($filter_et) && (
            empty($field['bundles'][$filter_et]) || (
                isset($filter_eb) &&
                !in_array($filter_eb, $field['bundles'][$filter_et])
            )
        )
    ){
      continue;
    }

    $add  = null;
    $type = $types[$field['type']];

    $current = array(
      'id'     => $field['id'],
      'name'   => $field_name,
      'type'   => $type['label'],
      'typeid' => $field['type'],
      'locked' => $field['locked'] ? $yes : $no,
      'module' => $field['module'],
    );

    $used_list = array();

    if (!empty($field['bundles'])) {
      foreach ($field['bundles'] as $entity_type => $bundle_list) {

        $entity_info = entity_get_info($entity_type);

        foreach ($bundle_list as $bundle) {

          if (isset($entity_info['bundles'][$bundle])) {
            $used_list[] = sprintf(
              "%s - %s",
              $entity_info['label'],
              $entity_info['bundles'][$bundle]['label']);  
          } else {
            $used_list[] = t("Unknow bundle %bundle of %entitytype", array(
              '%bundle' => $bundle,
              '%entitytype' => $entity_info['label'],
            ));
          }
        }
      }
    }

    if (isset($used_list) && !empty($used_list)) {
      $current['used'] = theme('item_list', array('items' => $used_list));
    } else {
      $current['used'] = $no;
    }

    if (null === $add || $add) {
      $options[$field_name] = $current;
    }
  }

  $form['fields']['fields'] = array(
    '#type' => 'tableselect',
    '#options' => $options,
    '#header' => array(
      'id'     => t("Id"),
      'name'   => t("Name"),
      'type'   => t("Type"),
      'typeid' => t("Type Id"),
      'locked' => t("Locked"),
      'module' => t("Module"),
      'used'   => t("Used by"),
    ),
  );

  return $form;
}

/**
 * Explore fields form filter submit.
 */
function devcontrol_admin_field_explore_form_filter_submit($form, &$form_state) {

  $query     = drupal_get_query_parameters();
  $filters   = $form_state['values']['filter'];
  $additions = array();

  foreach (array('ft', 'et', 'fn', 'eb') as $key) {
    unset($query[$key]);
    if (!empty($filters[$key])) {
      $query[$key] = $filters[$key];
    }
  }

  ksort($additions);
  $query += $additions;

  $form_state['redirect'] = array(current_path(), array('query' => $query));
}

/**
 * Hook search form.
 */
function devcontrol_admin_entity_search_form($form, &$form_state, $entity_type = null) {

  $form['#tree'] = true;

  $form['header'] = array(
    '#type' => 'fieldset',
    '#title' => t("Entity type name"),
    '#collapsible' => false, 
  );
  $form['header']['entity'] = array(
    '#type' => 'textfield',
    '#autocomplete_path' => 'developer/search/entity-search/autocomplete',
    '#required' => true,
    '#default_value' => $entity_type,
  );
  $form['header']['search'] = array(
    '#type' => 'submit',
    '#value' => t("Search"),
  );

  // If we have a hook name let's display everything we can about it.
  if (isset($entity_type)) {
    if ($entity_info = entity_get_info($entity_type)) {
      $form['matches']['#markup'] = '<small><pre>' . print_r($entity_info, true) . '</pre></small>';
    } else {
      $form['matches']['#markup'] = '<p>' . t("No match found.") . '</p>';
    }
  }

  return $form;
}

/**
 * Hook search form search action submit handler.
 */
function devcontrol_admin_entity_search_form_submit($form, &$form_state) {
  $entity_type = $form_state['values']['header']['entity'];
  $form_state['redirect'] = 'developer/search/entity-search/' . $entity_type;
}

/**
 * Cache explore.
 */
function devcontrol_admin_cache_explore_form($form, &$form_state) {

  $form['#id'] = 'bin-select';

  $options = array();
  foreach (devcontrol_cache_bins_list_get() as $bin) {
    $options[$bin] = $bin;
  }

  $form['bin'] = array(
    '#title' => t("Select cache bin"),
    '#type' => 'select',
    '#options' => $options,
  );

  $form['change-bin'] = array(
    '#type' => 'submit',
    '#value' => t("Select bin"),
    '#submit' => array('devcontrol_admin_cache_explore_form_bin_submit'),
    '#ajax' => array(
      'callback' => 'devcontrol_admin_cache_explore_form_js',
      'wrapper' => 'bin-select',
    ),
    '#limit_validation_errors' => array(array('bin')),
  );

  if (isset($form_state['bin'])) {

    $cache = _cache_get_object($form_state['bin']);

    if ($cache->isEmpty()) {
      $form['cid']['#markup'] = t("This cache bin is empty.");
    } else {
      if ($cache instanceof DrupalDatabaseCache) {
  
        $cids = db_select($form_state['bin'], 'b')
          ->fields('b', array('cid', 'cid'))
          ->execute()
          ->fetchAllKeyed();
  
        $form['cid'] = array(
          '#title' => t("Select cache identifier"),
          '#type' => 'select',
          '#options' => $cids,
          '#required' => true,
        );
      } else {
        $form['cid'] = array(
          '#title' => t("Type cache identifier"),
          '#type' => 'textfield',
          '#required' => true,
        );
      }

      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t("Explore"),
        '#submit' => array('devcontrol_admin_cache_explore_form_cid_submit'), 
        '#ajax' => array(
          'callback' => 'devcontrol_admin_cache_explore_form_js',
          'wrapper' => 'bin-select',
        ),
      );

      if (isset($form_state['cid'])) {

        $cache = cache_get($form_state['cid'], $form_state['bin']);

        if (false === $data) {
          // FIXME
        } else {
          /*
          if (module_exists('plumber')) {
            $form['output'] = array(
              '#theme' => 'plumber_tree',
              '#nodes' => array(new \Plumber\Tree\ReadOnlyArrayTree($form_state['cid'], $cache->data, false)),
              '#identifier' => 'root',
              '#context' => new \Plumber\Context\TreeContext(),
            );
          } else {
           */
            $form['output'] = array(
              '#prefix' => '<pre class="smallcode">',
              '#suffix' => '</pre>',
              '#markup' => print_r($cache->data, true),
            );
          /*
          }
           */
        }
      }
    }
  }

  return $form;
}

function devcontrol_admin_cache_explore_form_bin_submit($form, &$form_state) {
  $form_state['bin'] = $form_state['values']['bin'];
  unset($form_state['cid']);
  $form_state['rebuild'] = true;
}

function devcontrol_admin_cache_explore_form_cid_submit($form, &$form_state) {
  $form_state['cid'] = $form_state['values']['cid'];
  $form_state['rebuild'] = true;
}

function devcontrol_admin_cache_explore_form_js($form, &$form_state) {
  return $form;
}

/**
 * Display all available tokens.
 */
function devcontrol_admin_tokens_form($formn, &$form_state) {

  $form['tokens']['token_tree'] = array(
    '#theme' => 'token_tree',
    '#token_types' => 'all',
    '#global_types' => true,
  );

  return $form;
}
