<?php

/**
 * @file
 * Migration code for migrate_aggregator.
 */

/**
 * Implements hook_migrate_api.
 */
function migrate_aggregator_migrate_api() {
  /*
   * DB schema (each migration class matches a table):
   *
   *            -----cid.{aggregator_category}.cid---
   *            |                                   |
   *            |                                   |
   *           cid                                 cid
   * {aggregator_category_item}          {aggregator_category_feed}
   *           iid                                 fid
   *            |                                   |
   *            |                                   |
   *           iid                                 fid
   *     {aggregator_item}.fid-------------fid.{aggregator_feed}
   *
   */
  $api = array(
    'api' => 2,
    'groups' => array(
      'aggregator' => array(
        'title' => t('Aggregator'),
      ),
    ),
    'migrations' => array(
      'AggregatorCategoryMigration' => array(
        'class_name' => 'AggregatorCategoryMigration',
        'description' => t('Import Aggregator categories from Drupal 6'),
        'group_name' => 'aggregator',
        'machine_name' => 'AggregatorCategoryMigration',
        'source_connection' => 'd6',
        'source_version' => 6,
      ),
      'AggregatorCategoryFeedMigration' => array(
        'class_name' => 'AggregatorCategoryFeedMigration',
        'dependencies' => array(
          'AggregatorCategoryMigration',
          'AggregatorFeedMigration',
        ),
        'description' => t('Import Aggregator category/feed joins from Drupal 6'),
        'group_name' => 'aggregator',
        'machine_name' => 'AggregatorCategoryFeedMigration',
        'source_connection' => 'd6',
        'source_version' => 6,
      ),
      'AggregatorCategoryItemMigration' => array(
        'class_name' => 'AggregatorCategoryItemMigration',
        'dependencies' => array(
          'AggregatorCategoryMigration',
          'AggregatorItemMigration',
        ),
        'description' => t('Import Aggregator category/item joins from Drupal 6'),
        'group_name' => 'aggregator',
        'machine_name' => 'AggregatorCategoryItemMigration',
        'source_connection' => 'd6',
        'source_version' => 6,
      ),
      'AggregatorFeedMigration' => array(
        'class_name' => 'AggregatorFeedMigration',
        'dependencies' => array(
          'AggregatorCategoryMigration'
        ),
        'description' => t('Import Aggregator feeds from Drupal 6'),
        'group_name' => 'aggregator',
        'machine_name' => 'AggregatorFeedMigration',
        'source_connection' => 'd6',
        'source_version' => 6,
      ),
      'AggregatorItemMigration' => array(
        'class_name' => 'AggregatorItemMigration',
        'dependencies' => array(
          'AggregatorCategoryMigration',
          'AggregatorFeedMigration',
        ),
        'description' => t('Import Aggregator items from Drupal 6'),
        'group_name' => 'aggregator',
        'machine_name' => 'AggregatorItemMigration',
        'source_connection' => 'd6',
        'source_version' => 6,
      ),
    ),
  );
  return $api;
}
