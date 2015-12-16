; WetKit Search Makefile

api = 2
core = 7.x

; Apache Solr

projects[apachesolr][version] = 1.x-dev
projects[apachesolr][type] = module
projects[apachesolr][subdir] = contrib
projects[apachesolr][download][type] = git
projects[apachesolr][download][revision] = 1668889
projects[apachesolr][download][branch] = 7.x-1.x
projects[apachesolr][patch][1707404] = http://drupal.org/files/apachesolr-field-name-truncates-1707404-9.patch
projects[apachesolr][patch][1930686] = http://drupal.org/files/duplicate-paths-1930686-D7.patch
projects[apachesolr][patch][1977832] = http://drupal.org/files/apachesolr-hidden_display_indexed-1977832-5.patch
projects[apachesolr][patch][1974124] = http://drupal.org/files/1974124-2.patch
projects[apachesolr][patch][1852088] = http://drupal.org/files/1852088-13.patch
projects[apachesolr][patch][1852088] = http://drupal.org/files/apachesolr-facet-custom-filter-1916558-1.patch
projects[apachesolr][patch][1852088] = http://drupal.org/files/apachesolr-file-entity-bundles-2014067-1.patch
projects[apachesolr][patch][1852088] = http://drupal.org/files/apachesolr-teaser-snippet-fix-2025465-1.patch
projects[apachesolr][patch][1852088] = http://drupal.org/files/2017593-apache_solr_node_status_callback-3_0.patch

projects[apachesolr_autocomplete][version] = 1.x-dev
projects[apachesolr_autocomplete][type] = module
projects[apachesolr_autocomplete][subdir] = contrib
projects[apachesolr_autocomplete][download][type] = git
projects[apachesolr_autocomplete][download][revision] = dbd4c36
projects[apachesolr_autocomplete][download][branch] = 7.x-1.x
projects[apachesolr_autocomplete][patch][719382] = http://drupal.org/files/apachesolr_autocomplete-719382-38.patch
projects[apachesolr_autocomplete][patch][1884134] = http://drupal.org/files/apachesolr_autocomplete-allow_facet_field_alteration-1884134-1.patch
projects[apachesolr_autocomplete][patch][1882050] = http://drupal.org/files/apachesolr_autocomplete-move_alter_query-1882050-1.patch
projects[apachesolr_autocomplete][patch][1828930] = http://drupal.org/files/order_of_spellcheck_suggestions-1828930-2.patch

projects[apachesolr_multilingual][version] = 1.x-dev
projects[apachesolr_multilingual][type] = module
projects[apachesolr_multilingual][subdir] = contrib
projects[apachesolr_multilingual][download][type] = git
projects[apachesolr_multilingual][download][revision] = ff47c1d
projects[apachesolr_multilingual][download][branch] = 7.x-1.x

projects[apachesolr_sort][version] = 1.x-dev
projects[apachesolr_sort][type] = module
projects[apachesolr_sort][subdir] = contrib
projects[apachesolr_sort][download][type] = git
projects[apachesolr_sort][download][revision] = 4dd78b6
projects[apachesolr_sort][download][branch] = 7.x-1.x
projects[apachesolr_sort][patch][1765678] = http://drupal.org/files/apachesolr_sort-1765678-6.patch
projects[apachesolr_sort][patch][1923818] = http://drupal.org/files/apachesolr_sort-1923818-score-loses-parameters-1.patch
;projects[apachesolr_sort][patch][1722114] = http://drupal.org/files/apachesolr_sort-undefined_index_groupValue-1722114.patch

projects[apachesolr_views][version] = 1.x-dev
projects[apachesolr_views][type] = module
projects[apachesolr_views][subdir] = contrib
projects[apachesolr_views][download][type] = git
projects[apachesolr_views][download][revision] = 36f14d1
projects[apachesolr_views][download][branch] = 7.x-1.x
projects[apachesolr_views][patch][1791908] = http://drupal.org/files/1791908.patch
projects[apachesolr_views][patch][1651386] = http://drupal.org/files/apachesolr_views_decode_entities.patch
projects[apachesolr_views][patch][1823230] = http://drupal.org/files/check_for_empty_value_in_add_where_function-1823230-1.patch
projects[apachesolr_views][patch][1969268] = http://drupal.org/files/1969268_ensure_valid_context.patch
projects[apachesolr_views][patch][1761432] = http://drupal.org/files/set_group_operator-1761432-1.patch
projects[apachesolr_views][patch][1999986] = http://drupal.org/files/show_errors_only_if_required_0.patch
projects[apachesolr_views][patch][1750952] = http://drupal.org/files/use_arguments-1750952-13.patch
