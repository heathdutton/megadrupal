; WetKit Deployment Makefile

api = 2
core = 7.x

; Modules for WetKit Deployment

projects[deploy][version] = 2.0-alpha3
projects[deploy][subdir] = contrib
projects[deploy][patch][1604938] = http://drupal.org/files/deploy-1604938_1.patch
projects[deploy][patch][2052767] = http://drupal.org/files/deploy-undefined_variable_user-2052767-3.patch
projects[deploy][patch][2092335] = http://drupal.org/files/deploy_new_alter_hook-2092335-4.patch
projects[deploy][patch][2136595] = http://drupal.org/files/issues/helpful_helper_text-2136595-1.patch

projects[deploy_services_client][version] = 1.0-beta2
projects[deploy_services_client][subdir] = contrib
projects[deploy_services_client][type] = module

projects[entity_dependency][version] = 1.0-alpha2
projects[entity_dependency][subdir] = contrib
projects[entity_dependency][patch][1589794] = http://drupal.org/files/entity_dependency_iterator_documentation-1589794-1.patch
projects[entity_dependency][patch][1590280] = http://drupal.org/files/entity_dependency_comment_typos-1590280-1.patch
projects[entity_dependency][patch][1772372] = http://drupal.org/files/documentation-1772372.patch
projects[entity_dependency][patch][2051797] = http://drupal.org/files/2051797-file-bug-1.patch

projects[entity_menu_links][version] = 1.0-alpha2
projects[entity_menu_links][subdir] = contrib
projects[entity_menu_links][patch][2089133] = http://drupal.org/files/issues/wetkit_deployment_entity_menu_links_create_new_menu-2089133-12.patch

projects[environment_indicator][version] = 2.5
projects[environment_indicator][subdir] = contrib

projects[quicktabs][version] = 3.6
projects[quicktabs][subdir] = contrib

projects[services][version] = 3.11
projects[services][subdir] = contrib

projects[services][version] = 3.12
projects[services][subdir] = contrib
projects[services][patch][2475119] = http://drupal.org/files/issues/services_wrong_function_call_in_node_resource-2475119-99.patch
projects[services][patch][2480763] = http://drupal.org/files/issues/services_resource_admin_ui_styles_inheriting_from_theme-2480763-99.patch

projects[shared_content][version] = 1.0-beta2
projects[shared_content][subdir] = contrib

projects[uuid_redirect][version] = 1.x-dev
projects[uuid_redirect][subdir] = contrib
projects[uuid_redirect][type] = module
projects[uuid_redirect][download][type] = git
projects[uuid_redirect][download][revision] = ce1ab849
projects[uuid_redirect][download][branch] = 7.x-1.x
