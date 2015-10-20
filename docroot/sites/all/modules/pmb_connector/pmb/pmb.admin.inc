<?php

/**
 * @file
 * PMB administration form.
 */

/**
 * Base PMB admin form.
 *
 * @ingroup forms
 * @see pmb_admin_form_submit()
 * @see system_settings_form()
 */
function pmb_admin_form() {
  $form = array();

  $form['tab'] = array(
    '#type' => 'vertical_tabs',
    '#default_tab' => 'gateway',
  );

  $form['tab']['gateway'] = array(
    '#type' => 'fieldset',
    '#title' => t('PMB gateway'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 1,
  );
  $form['tab']['gateway']['pmb_link_type'] = array(
    '#type' => 'select',
    '#title' => t('PMB service link type'),
    '#default_value' => pmb_variable_get('pmb_link_type'),
    '#description' => t('The type of the link to PMB'),
    '#options' => array(
      'json-rpc' => t('JSON-RPC'),
    ),
    '#required' => TRUE,
  );
  $form['tab']['gateway']['pmb_link_serverurl'] = array(
    '#type' => 'textfield',
    '#title' => t('PMB service full URL'),
    '#default_value' => pmb_variable_get('pmb_link_serverurl'),
    '#size' => 50,
    '#maxlength' => 256,
    '#description' => t('The full url of the PMB service'),
    '#required' => TRUE,
  );
  $form['tab']['gateway']['pmb_link_serveruser'] = array(
    '#type' => 'textfield',
    '#title' => t('PMB service login'),
    '#default_value' => pmb_variable_get('pmb_link_serveruser'),
    '#size' => 50,
    '#maxlength' => 256,
    '#description' => t('The login of the PMB service. Let empty in case of an anonymous use.'),
    '#required' => FALSE,
  );
  $form['tab']['gateway']['pmb_link_serverpassword'] = array(
    '#type' => 'textfield',
    '#title' => t('PMB service password'),
    '#default_value' => pmb_variable_get('pmb_link_serverpassword'),
    '#size' => 50,
    '#maxlength' => 256,
    '#description' => t('PMB service password. Let empty in case of an anonymous use.'),
    '#required' => FALSE,
  );

  $form['tab']['alias'] = array(
    '#type' => 'fieldset',
    '#title' => t('URL and paths aliases'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 2,
  );
  if (module_exists('path')) {
    $form['tab']['alias']['pmb_alias_custom'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable custom paths'),
      '#default_value' => pmb_variable_get('pmb_alias_custom'),
      '#description' => t('Check this box if you want to customize paths. Uncheck it if you use i18n module or another module to customize paths.') . '<br />'
        . t('This option allows to set custom core paths without another module.') . ' ' . t('Default customization is a full French translation without accented characters.'),
    );
    $form['tab']['alias']['pmb_alias_info'] = array(
      '#type' => 'item',
      '#description' => t('These fields allow to set custom paths for catalog access.') . '<br />'
        . '<strong>' . t('Examples') . ':</strong> '
        . '<ul><li>' . t('"<strong>catalogue</strong>", "<strong>library</strong>" or "<strong>bibliothèque</strong>" for www.example.com/<em>catalog</em>;') . '</li>'
        . '<li>' . t('"<strong>bibliothèque/notice/%</strong>" for www.example.com/<em>catalog/record/%</em>;') . '</li>'
        . '<li>' . t('"<strong>lecteur/%/emprunt</strong>" for www.example.com/<em>reader/%/loan</em>.') . '</li></ul>'
        . '<strong>' . t('Notes') . ':</strong> '
        . '<ul><li>' . t('This form helps to set unique customized paths. If you want more paths or aliases, especially in case of a multilanguage site, you should use modules as !link_1, !link_2/!link_3 and !link_4. PMB Connector is compatible with any other Drupal module.', array(
          '!link_1' => l(t('i18n'), 'http://drupal.org/project/i18n'),
          '!link_2' => l(t('pathauto'), 'http://drupal.org/project/pathauto'),
          '!link_3' => l(t('subpathauto'), 'http://drupal.org/project/subpathauto'),
          '!link_4' => l(t('extended path'), 'http://drupal.org/project/path_alias_xt'),
        )) . '</li>'
        . '<li>' . t('These paths are not aliases, but core links. You can add aliases directly in !link, but be careful if you change items with alias.', array(
          '!link' => l(t('Admin > Config > Search and metadata > URL aliases'), 'admin/config/search/path'),
        )) . '</li>'
        . '<li>' . t('Items should be unique and without "/".') . '</li></ul>'
        . '<strong>' . t('Warning') . ':</strong> '
        . '<ul><li>' . t('If you use "URL aliases" page or "i18n" module or any other path module, avoid to change these settings.') . '</li>'
        . '<li>' . t('Once paths are set, avoid to change them, especially "catalog" and "record", so links will be durable and permanent for users and referencing.') . '</li></ul>',
    );
    foreach (pmb_variable_get('pmb_alias') as $key => $path) {
      $form['tab']['alias'][$key] = array(
        '#type' => 'textfield',
        '#title' => check_plain($key),
        '#default_value' => $path,
        '#size' => 60,
        '#maxlength' => 256,
        '#states' => array(
          'enabled' => array(
            ':input[name=pmb_alias_custom]' => array('checked' => TRUE),
          ),
        ),
      );
    }
  }
  else {
    $form['tab']['alias']['pmb_alias_custom'] = array(
      '#type' => 'value',
      '#value' => FALSE,
    );
    $form['tab']['alias']['pmb_alias_info'] = array(
      '#type' => 'item',
      '#description' => t('Drupal core "Path" module is not enabled.'),
    );
  }

  // Items are filled by each module hook_pmb_admin_form_alter().
  $form['tab']['user_interface'] = array(
    '#type' => 'fieldset',
    '#title' => t('User interface'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 6,
  );
  $form['tab']['user_interface']['pmb_notice_sine'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display <em>sine</em> when needed'),
    '#default_value' => pmb_variable_get('pmb_notice_sine'),
    '#description' => t("Check this box if you want to display <em>sine loco (s.l.), sine nomine (s.n.), sine datum (s.d.)</em>, 'No title' and 'No author' when they aren't set in a record.") . '<br />'
      . t('Note: Some templates may bypass this setting.'),
  );
  if (module_exists('pmb_search')
      || module_exists('pmb_catalog')
      || module_exists('pmb_reader')
    ) {
    $form['tab']['user_interface']['info'] = array(
      '#type'        => 'item',
      '#description' => t('Note: Menus and links can always be modified directly in <a href="!link">Drupal admin menu</a>.', array(
        '!link' => url('admin/structure/menu'),
      )) . '<br />'
        . t('These settings modify user interface only. You need to set permissions in accordance in <a href="!link">Drupal permission menu</a>.', array(
          '!link' => url('admin/people/permissions'),
        )),
    );
  }
  if (module_exists('pmb_search') && module_exists('pmb_catalog')) {
    $form['tab']['user_interface']['pmb_menu_default'] = array(
      '#type' => 'radios',
      '#title' => t('Default page to use when "Search catalog" and "Browse catalog" menus are merged'),
      '#options' => array(
        'search' => t('Search the catalog'),
        'browse' => t('Browse the catalog'),
      ),
      '#default_value' => pmb_variable_get('pmb_menu_default'),
      '#description' => t('This page will be used with "www.example.com/catalog".') . '<br />'
        . t('"Search catalog" and "Browse catalog" menus are merged when they are in the same menu.'),
    );
  }

  if (module_exists('pmb_view')
      || module_exists('pmb_catalog')
      || module_exists('pmb_search')
      || module_exists('pmb_reader')
    ) {
    $form['tab']['display'] = array(
      '#type' => 'fieldset',
      '#title' => t('Display lists'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#description' => t('Set number of records to display per page for each type of list'),
      '#weight' => 7,
    );
    $items = array();
    if (module_exists('pmb_view')) {
      $items['PMB View'] = array(
        'pmb_noticeperpage_author' => t('Author'),
        'pmb_noticeperpage_publisher' => t('Publisher'),
        'pmb_noticeperpage_serie' => t('Serie'),
        'pmb_noticeperpage_subserie' => t('Sub-serie'),
      );
    }
    if (module_exists('pmb_catalog')) {
      $items['PMB Catalog'] = array(
        'pmb_noticeperpage_section' => t('Section'),
        'pmb_noticeperpage_shelf' => t('Shelf'),
        'pmb_noticeperpage_block_shelf' => t('Shelf block'),
        'pmb_bulletinsperpage_serials' => t('Serial issues'),
        'pmb_noticeperpage_categories' => t('Category'),
      );
    }
    if (module_exists('pmb_search')) {
      $items['PMB Search'] = array(
        'pmb_noticeperpage_search' => t('Results of a search'),
      );
    }
    if (module_exists('pmb_reader')) {
      $items['PMB Reader'] = array(
      );
    }
    foreach ($items as $module => $value) {
      $form['tab']['display'][$module] = array(
        '#type' => 'fieldset',
        '#title' => check_plain($module),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );
      foreach ($value as $key => $item) {
        $form['tab']['display'][$module][$key] = array(
          '#type' => 'textfield',
          '#title' => check_plain($item),
          '#default_value' => pmb_variable_get($key),
          '#size' => 5,
          '#maxlength' => 10,
          '#required' => TRUE,
        );
      }
    }
  }

  $form['tab']['cache'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cache'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 10,
  );
  $form['tab']['cache']['pmb_user_cache_profile'] = array(
    '#type' => 'radios',
    '#title' => t('Cache profile'),
    '#default_value' => pmb_variable_get('pmb_user_cache_profile'),
    '#description' => t("The profile of the cache. Determine how to cache the notices. This will have an impact on performances."),
    '#required' => TRUE,
    '#options' => array(
      'cache_anonymous_all' => t('Anonymous and readers share the same cache.'),
      'cache_anonymous_reader_all' => t('Anonymous readers have their own cache. All readers share the same cache.'),
      'cache_anonymous_reader_separate' => t('Anonymous readers have their own cache. Each reader has its own cache.'),
    ),
  );
  if (module_exists('pmb_view')
      || module_exists('pmb_catalog')
      || module_exists('pmb_search')
      || module_exists('pmb_reader')
    ) {
    $form['tab']['cache']['cache_info'] = array(
      '#type'        => 'item',
      '#title' => t('Set duration of cache for each item, in seconds'),
    );
  }
  $items = array();
  if (module_exists('pmb_view')) {
    $items['PMB View'] = array(
      'pmb_cache_duration_notice' => t('Record'),
      'pmb_cache_duration_notice_external' => t('External record'),
      'pmb_cache_duration_serials' => t('Serial'),
      'pmb_cache_duration_author' => t('Author'),
      'pmb_cache_duration_publisher' => t('Publisher'),
      'pmb_cache_duration_serie' => t('Serie'),
      'pmb_cache_duration_subserie' => t('Sub-serie'),
    );
  }
  if (module_exists('pmb_catalog')) {
    $items['PMB Catalog'] = array(
      'pmb_cache_duration_locations_and_sections' => t('Locations and sections'),
      'pmb_cache_duration_shelves' => t('Shelves'),
      'pmb_cache_duration_thesauri' => t('Thesaurus'),
      'pmb_cache_duration_category' => t('Categories'),
    );
  }
  if (module_exists('pmb_search')) {
    $items['PMB Search'] = array(
      'pmb_cache_duration_search' => t('Search'),
      'pmb_cache_duration_search_external' => t('External search'),
      'pmb_cache_duration_search_external_sources' => t('External search sources'),
      'pmb_cache_duration_search_fields' => t('Search fields'),
    );
  }
  if (module_exists('pmb_reader')) {
    $items['PMB Reader'] = array(
      'pmb_cache_duration_reader_reservations' => t('Reader reservation'),
      'pmb_cache_duration_reader_loans' => t('Reader loans'),
      'pmb_cache_duration_suggs_items' => t('Sources and categories for suggestions of readers'),
    );
  }
  foreach ($items as $module => $value) {
    $form['tab']['cache'][$module] = array(
      '#type' => 'fieldset',
      '#title' => check_plain($module),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    foreach ($value as $key => $item) {
      $form['tab']['cache'][$module][$key] = array(
        '#type' => 'textfield',
        '#title' => check_plain($item),
        '#default_value' => pmb_variable_get($key),
        '#size' => 10,
        '#maxlength' => 10,
        '#required' => TRUE,
      );
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  $form['actions']['pmb_default_values'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#submit' => array('pmb_default_values'),
  );

  return $form;
}

/**
 * Implements hook_submit().
 *
 * This submit function is needed only because it's impossible with a normal
 * hook_menu() to set a child as a main menu with its parent ('catalog/search'
 * or 'catalog/browse' on the same level than 'catalog').
 *
 * @see pmb_admin_form()
 */
function pmb_admin_form_submit($form, &$form_state) {
  $values = &$form_state['values'];

  // Keep aliases in one variable array and remove single values.
  $alias = pmb_variable_get('pmb_alias');
  $values['pmb_alias'] = array();
  foreach ($alias as $key => $value) {
    $values['pmb_alias'][$key] = $values[$key];
    unset($values[$key]);
  }

  // Check if alias changed in order to rebuild menus after saving items.
  $alias_changed = (($alias !== $values['pmb_alias'])
      || ($values['pmb_alias_custom'] != pmb_variable_get('pmb_alias_custom'))
    ) ?
    TRUE :
    FALSE;

  // Check if menu options changed in order to rebuild menus after saving items.
  $previous = &$form_state['complete form']['tab']['user_interface'];
  $menu_changed = (isset($values['pmb_menu_default'])
      && (($previous['pmb_menu_default']['#default_value'] != $values['pmb_menu_default'])
        || (isset($values['pmb_tabs_reader_default']) && ($previous['pmb_tabs_reader_default']['#default_value'] !== $values['pmb_tabs_reader_default']))
        || (isset($values['pmb_menu_attach_reader']) && ($previous['pmb_menu_attach_reader']['#default_value'] !== $values['pmb_menu_attach_reader']))
    )) ?
    TRUE :
    FALSE;

  system_settings_form_submit($form, $form_state);

  if ($alias_changed || $menu_changed) {
    menu_rebuild();
    cache_clear_all(NULL, 'cache_pmb_connector');
  }
}

/**
 * Restore recommended default values.
 *
 * @see pmb_admin_form()
 */
function pmb_default_values($form, &$form_state) {
  foreach (pmb_default_variables() as $key => $value) {
    variable_set($key, $value);
  }
  unset($form_state['values']);
  unset($form_state['storage']);

  menu_rebuild();
  cache_clear_all(NULL, 'cache_pmb_connector');

  drupal_set_message(t('Options have been reset to default.'));
}
