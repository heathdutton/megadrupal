<?php

/**
 * Form builder function for the admin settings form.
 */
function amazon_store_admin_form($form, &$form_state) {

  $form['amazon_store_store_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Store name'),
    '#description' => t('The descriptive name of the store to be be shown in the menu, title, etc.'),
    '#size' => 25,
    '#default_value' => variable_get('amazon_store_store_name', 'Amazon Store'),
  );
  $form['amazon_store_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to store'),
    '#description' => t('The Drupal path to the store. If you change this, you will have to rebuild the menu cache for it to take effect.'),
    '#size' => 25,
    '#default_value' => variable_get('amazon_store_path', 'amazon_store'),
  );
  $form['amazon_store_show_searchform'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display search form'),
    '#default_value' => variable_get('amazon_store_show_searchform', 1),
  );
  $form['amazon_store_show_narrowby_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display "narrow by" form'),
    '#default_value' => variable_get('amazon_store_show_narrowby_form', 1),
  );
  $form['amazon_store_show_sort_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display sort form'),
    '#default_value' => variable_get('amazon_store_show_sort_form', 1),
  );
  $form['amazon_store_show_category_select'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display category selection pulldown'),
    '#default_value' => variable_get('amazon_store_show_category_select', 1),
    '#description' => t("Deselect if you want to hide the category selection pulldown.")
  );

  $form['amazon_store_merchant_id'] = array(
    '#type' => 'radios',
    '#title' => t('Search Amazon or other merchants'),
    '#default_value' => variable_get('amazon_store_merchant_id', 'Amazon'),
    '#options' => array('Amazon' => 'Amazon', 'Other' => 'All'),
    '#description' => t('You may search for items sold by Amazon or all Amazon Marketplace sellers (including Amazon).'),
  );
  $form['amazon_store_search_index_choice'] = array(
    '#type' => 'select',
    '#title' => t('Default search index selection on amazon_store page'),
    '#description' => t("The default search index on the amazon_store page. If you turn off the category pulldown above, this will be used as the search index."),
    '#options' => $GLOBALS['amazon_store_search_indexes']->getSearchIndexPulldown(FALSE),
    '#default_value' => variable_get('amazon_store_search_index_choice', "Books"),
  );
  $form['amazon_store_default_items'] = array(
    '#type' => 'radios',
    '#title' => t('Default items for amazon_store page if no search has been done (no keywords present)'),
    '#description' => t("You may choose either a simple search index (and its default browsenode), a specific browsenode, or a list of items to display on the default amazon_store page"),
    '#options' => array(
      'searchindex' => t("SearchIndex Only - Use Amazon's default browsenode for the selected search index"),
      'browsenode' => t('A browsenode specified below'),
      'itemlist' => t("A list of Amazon ASINs specified below"),
    ),
    '#default_value' => variable_get('amazon_store_default_items', 'searchindex'),
  );
  $form['amazon_store_default_search_index'] = array(
    '#type' => 'select',
    '#title' => t('Search index used to populate items on page with no keywords'),
    '#description' => t("If you selected 'SearchIndex Only' above, this is the search index that will be used to select items"),
    '#options' => $GLOBALS['amazon_store_search_indexes']->getSearchIndexPulldown(TRUE),
    '#default_value' => variable_get('amazon_store_default_search_index', "Books"),
  );

  $form['amazon_store_default_browsenode_id'] = array(
    '#type' => 'textfield',
    '#title' => t("Default Browsenode ID"),
    '#description' => t("Browsenode ID to use as default search. This browsenode MUST be valid with the Search Index you have chosen. You can find browsenodes at <a href='http://browsenodes.com'>browsenodes.com</a>"),
    '#size' => 12,
    '#default_value' => variable_get('amazon_store_default_browsenode_id', ""),
  );

  $form['amazon_store_default_item_list'] = array(
    '#type' => 'textfield',
    '#title' => t("Default Item List"),
    '#description' => t("Comma-separated list of up to 10 Amazon ASINs that should be displayed by default on the amazon_store page"),
    '#size' => 80,
    '#default_value' => variable_get('amazon_store_default_item_list', ""),
  );

  $period = drupal_map_assoc(array(3600, 7200, 43200, 86400), 'format_interval');
  $form['amazon_store_refresh_schedule'] = array(
    '#type' => 'select',
    '#title' => t('Refresh schedule'),
    '#description' => t("Cached information must be cleared regularly to keep informaton current and to comply with Amazon's requirements. Cron must be enabled for this function to work properly."),
    '#default_value' => variable_get('amazon_store_refresh_schedule', 43200),
    '#options' => $period
  );

  $form['amazon_store_include_categories'] = array(
      '#type' => 'checkboxes',
      '#title' => t("Categories to include in search box, category search block, etc. It's often preferable to exclude some categories"),
      '#description' => t("Note that this list changes if you change the locale, so you will have to revisit this page after changing the locale on the <a href='!amazon_settings_url'>Amazon Module Settings</a> page.", array('!amazon_settings_url' => url('admin/config/services/amazon'))),
      '#options' => $GLOBALS['amazon_store_search_indexes']->getAllCategories(),
      '#default_value' => variable_get('amazon_store_include_categories', $GLOBALS['amazon_store_search_indexes']->getRecommendedCategories()),
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'amazon_store_admin_form_submit';
  return $form;
}

/**
 * Rebuild menu when settings submitted, as they might have changed the menu.
 */
function amazon_store_admin_form_submit($form, &$form_state) {
  menu_rebuild();
}
