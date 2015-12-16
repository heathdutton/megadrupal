<?php

function google_sitelinks_search_box_group_info() {
  $groups = array();

  $groups['google_sitelinks_search_box_main_settings'] = array(
    'title' => t('Main google sitelinks search box settings'),
    'access' => 'administer search',
  );

  return $groups;
}

function google_sitelinks_search_box_variable_info() {
  $variables = array();

  $variables['google_sitelinks_search_box_default_search_page'] = array(
    'title' => t('Default search page'),
    'description' => t("Default search page url eg. 'my_search_url'. No domain name required."),
    'group' => 'google_sitelinks_search_box_main_settings',
    'type' => 'string',
    'element' => array(
      '#type' => 'textfield',
    ),
    'default' => 'search',
  );

  return $variables;
}
