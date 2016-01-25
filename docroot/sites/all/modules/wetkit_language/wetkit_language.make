; WetKit Language Makefile

api = 2
core = 7.x

; Contrib

projects[entity_translation][version] = 1.0-beta4
projects[entity_translation][subdir] = contrib
projects[entity_translation][patch][1989084] = http://drupal.org/files/issues/entity_translation-fix-i18n-menu-item-validate-1989084-8.patch
projects[entity_translation][patch][2166157] = http://drupal.org/files/issues/edit_draft-2557443-18.patch
projects[entity_translation][patch][2203801] = http://drupal.org/files/issues/i18n_taxonomy_integration-2203801-5.patch
projects[entity_translation][patch][2305547] = http://drupal.org/files/issues/entity_translation-more-defensive-code.patch
projects[entity_translation][patch][2415189] = http://drupal.org/files/issues/entity_translation_2415189_0.patch
projects[entity_translation][patch][2452279] = http://drupal.org/files/issues/entity_translation-duplicated-langcode-variable-edit-access.patch

projects[features_translations][version] = 2.0
projects[features_translations][subdir] = contrib

projects[i18n][version] = 1.13
projects[i18n][subdir] = contrib

projects[i18nviews][version] = 3.x-dev
projects[i18nviews][type] = module
projects[i18nviews][subdir] = contrib
projects[i18nviews][download][type] = git
projects[i18nviews][download][revision] = fdc8c33
projects[i18nviews][download][branch] = 7.x-3.x
projects[i18nviews][patch][1788832] = http://drupal.org/files/issues/transformed-contextual-filter-fix-178832-10.patch

projects[language_switch][version] = 1.0-alpha2
projects[language_switch][subdir] = contrib

projects[l10n_client][version] = 1.3
projects[l10n_client][subdir] = contrib
projects[l10n_client][patch][2191771] = http://drupal.org/files/issues/l10n_client-browser_is-2191771-17.patch

projects[l10n_update][version] = 1.1
projects[l10n_update][subdir] = contrib

projects[potx][version] = 1.0
projects[potx][subdir] = contrib

projects[stringoverrides][version] = 1.8
projects[stringoverrides][subdir] = contrib

projects[title][version] = 1.x-dev
projects[title][type] = module
projects[title][subdir] = contrib
projects[title][download][type] = git
projects[title][download][revision] = 32e8016
projects[title][download][branch] = 7.x-1.x
projects[title][patch][2605040] = http://drupal.org/files/issues/title-removed-recursive-call-to-entity-get-info-2605040-5-d7.patch

projects[variable][version] = 2.5
projects[variable][subdir] = contrib

projects[webform_localization][version] = 4.0-alpha14
projects[webform_localization][subdir] = contrib
projects[webform_localization][patch][2627812] = http://drupal.org/files/issues/undefined_index_nid_in-2627812-2.patch
projects[webform_localization][patch][2627813] = http://drupal.org/files/issues/undefined_index_nid-2627812-5_1.patch
