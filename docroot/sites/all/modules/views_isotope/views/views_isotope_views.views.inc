<?php
/**
 * @file
 * Define the Views plugins.
 */

/**
 * Implements hook_views_plugins().
 */
function views_isotope_views_views_plugins() {
  return array(
    'style' => array(
      'isotope_grid' => array(
        'title' => t('Isotope Grid'),
        'help' => t('Display the results using the jQuery Isotope plugin'),
        'handler' => 'ViewsIsotopeViewsPluginStyleIsotopeGrid',
        'theme' => 'views_isotope_views_grid',
        'uses options' => TRUE,
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'uses grouping' => FALSE,
        'type' => 'normal',
        'theme file' => 'views_isotope_views.theme.inc',
      ),
      'isotope_filter' => array(
        'title' => t('Isotope Filter'),
        'help' => t('Display the results formatted to filter an Isotope View'),
        'handler' => 'ViewsIsotopeViewsPluginStyleIsotopeFilter',
        'theme' => 'views_isotope_views_filter',
        'uses options' => TRUE,
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'uses grouping' => FALSE,
        'type' => 'normal',
        'theme file' => 'views_isotope_views.theme.inc',
      ),
      'isotope_sorter' => array(
        'title' => t('Isotope Sort Block'),
        'help' => t('Display the results formatted to sort an Isotope View'),
        'handler' => 'ViewsIsotopeViewsPluginStyleIsotopeSorter',
        'theme' => 'views_isotope_views_sorter',
        'uses options' => TRUE,
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'uses grouping' => FALSE,
        'type' => 'normal',
        'theme file' => 'views_isotope_views.theme.inc',
      ),
    ),
  );
}
