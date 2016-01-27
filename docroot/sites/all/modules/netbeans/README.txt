This is not a Drupal module but an exported set of language-aware code templates
to enhance Drupal development with the NetBeans IDE versions 7.0 and above.

Installation
============

1.  Download and extract the file
2.  Open the Netbeans preferences dialog
3.  Click "Import" in the lower left-hand corner
4.  Select the "drupal-netbeans.zip" file from the downloaded package and click "OK"
5.  Select "OK" in the confirmation prompt. Don't worry, your existing config will
    remain intact, only code templates with the same names will be overwritten.

Usage
=====

Type the template name and press tab to insert it into the current file.


Templates
=========

All templates are for Drupal 7 unless otherwise noted.

Are you missing a frequently used hook implementation?
Have any other useful templates you'd like to add?

Create an issue here: http://drupal.org/node/add/project-issue/netbeans

This is the full list of bundled templates:

PHP
---

  - behavior
      Insert a Drupal.behaviors code block, for inserting javascript inline
  - block (Drupal 6)
      Inserts a hook_block() skeleton
  - block_configure
      Inserts a hook_block_configure() skeleton
  - block_info
      Inserts a hook_block_info() skeleton
  - block_save
      Inserts a hook_block_save() skeleton
  - block_view
      Inserts a hook_block_view() skeleton
  - callback (Drupal 6 and 7)
      Inserts a MENU_CALLBACK menu array item, for use in hook_menu()
  - cdata
      Insert a CDATA code block
  - closure
      Insert a closure to wrap your JavaScript code in, for inserting javascript inline
  - cron
      Inserts a hook_cron() skeleton
  - cron_queue_info
      Inserts a hook_cron_queue_info() skeleton
  - dac
      Inserts a drupal_add_css() skeleton
  - dai
      Inserts a drupal_add_js() inline code skeleton
  - daj
      Inserts a drupal_add_js() skeleton
  - das
      Inserts a drupal_add_js() settings skeleton
  - disable
      Inserts a hook_disable() skeleton
  - dump
      Inserts a var_dump() code skeleton
  - enable
      Inserts a hook_enable() skeleton
  - entity
      Inserts an entity and entity type schema definition for use with
      hook_schema(), use it with the entity_info template to quickly
      create a custom, exportable entity and type
  - entity_info
      Inserts a hook_entity_info() skeleton which defines an Entity and an
      Entity Type
  - exit
      Inserts a hook_exit() skeleton
  - field_formatter_info
      Inserts a hook_field_formatter_info() skeleton
  - field_formatter_view
      Inserts a hook_field_formatter_view() skeleton
  - fieldset
      Inserts a fieldset Form API array item
  - float
      Inserts a float column database schema item, for use with hook_schema()
  - form_alter
      Inserts a hook_form_alter() skeleton
  - fk
      Inserts a foreign keys database schema item, for use with hook_schema()
  - help
      Inserts a hook_help() skeleton
  - init
      Inserts a hook_init() skeleton
  - install
      Inserts a hook_install() skeleton
  - int
      Inserts an integer column database schema item, for use with hook_schema()
  - kr
      Shortcut for krumo() which manually includes the Krumo class, useful for
      krumoing when testing users that don't have permission to use krumo
  - load
      Inserts a window.load() code block, for inserting javascript inline
  - menu
      Inserts a hook_menu() skeleton
  - node_delete
      Inserts a hook_node_delete() skeleton
  - node_insert
      Inserts a hook_node_insert() skeleton
  - node_prepare
      Inserts a hook_node_prepare() skeleton
  - node_presave
      Inserts a hook_node_presave() skeleton
  - node_submit
      Inserts a hook_node_submit() skeleton
  - node_update
      Inserts a hook_node_update() skeleton
  - node_validate
      Inserts a hook_node_validate() skeleton
  - node_view
      Inserts a hook_node_view() skeleton
  - node_view_alter
      Inserts a hook_node_view_alter() skeleton
  - nodeapi (Drupal 6)
      Inserts a hook_nodeapi() skeleton
  - perm (Drupal 6)
      Inserts a hook_perm() skeleton
  - permission
      Inserts a hook_permission() skeleton
  - preprocess_page
      Inserts a hook_preprocess_page() skeleton
  - ready
      Inserts a jQuery ready() code block, for inserting javascript inline
  - rules_access
      Inserts a rules access callback function for use with rules actions and events
  - rules_action_info
      Inserts a hook_rules_action_info() skeleton
  - rules_condition_info
      Inserts a hook_rules_condition_info() skeleton
  - rules_event_info
      Inserts a hook_rules_event_info() skeleton
  - rules_invoke
      Inserts skeleton code to manually invoke a rules event
  - schema
      Inserts a hook_schema() skeleton
  - settings
      Inserts a Form callback to create settings forms, for use with drupal_get_form()
  - table
      Inserts a database schema table array item, for use with hook_schema()
  - text
      Inserts a text column database schema item, for use with hook_schema()
  - theme
      Inserts a hook_theme() skeleton
  - tpl
      Inserts a template array item, for use with hook_theme()
  - uninstall
      Inserts a hook_uninstall() skeleton
  - unique
      Inserts a unique keys database schema item, for use with hook_schema()
  - update_n
      Inserts a hook_update_N() skeleton
  - user (Drupal 6)
      Inserts a hook_user() skeleton
  - user_delete
      Inserts a hook_user_delete() skeleton
  - user_insert
      Inserts a hook_user_insert() skeleton
  - user_login
      Inserts a hook_user_login() skeleton
  - user_presave
      Inserts a hook_user_presave() skeleton
  - user_update
      Inserts a hook_user_update() skeleton
  - varchar
      Inserts a varchar column database schema item, for use with hook_schema()
  - variable
      Inserts a variable array item, for use with hook_variable_info()
  - variable_info
      Inserts a hook_variable_info() skeleton
  - views_api
      Inserts a hook_views_api() skeleton
  - views_data
      Inserts a hook_views_data() skeleton
  - views_data_alter
      Inserts a hook_views_data_alter() skeleton

JavaScript
----------

  - behavior
      Insert a Drupal.behaviors code block
  - closure
      Insert a closure to wrap your JavaScript code in
  - clog
      Inserts a console.log() code block
  - load
      Inserts a window.load() code block
  - ready
      Inserts a jQuery ready() code block

HTML
----

  - cdata
      Insert a CDATA code block
