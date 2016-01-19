# Drush Migrate Inspect

Tools for inspecting records created by [Drupal Migrate](https://drupal.org/project/migrate).

## Installation

### Drush

Via drush:

    drush dl drush_migrate_inspect

### Github

Installation from Github:

    pushd ~/.drush
    git clone git@github.com:robballou/drush-migrate-inspect.git migrate_inspect
    popd
    drush cc drush

## Usage

### migrate-inspect-last

Open the last migrated item in a browser:

    drush migrate-inspect-last MyMigration
    drush mil MyMigration

### migrate-inspect-random

Open a random migrated item in a browser:

    drush migrate-inspect-random MyMigration
    drush mir MyMigration

### migrate-inspect-list

List the last 10 migrated items (source and destination ids):

    drush migrate-inspect-list MyMigration
    drush mils MyMigration

### migrate-inspect-source

List the source ID for a migrated item (101 is the destination id):

    drush migrate-inspect-source MyMigration 101
    drush mis MyMigration 101

### migrate-inspect-destination

List the destination ID for a migrated item (101 is the source id):

    drush migrate-inspect-destination MyMigration 101
    drush mid MyMigration 101

## Why?

Drupal's migrate module has Drush integration that gives you access to several pieces of a migrate workflow, but it doesn't give you an easy way to inspect things as you go. This extension is meant to make it easier to view migrated records/nodes/entities/whatnot as you move through the process. For example, currently if you are working on a migration you may:

1. Write your migration code
1. Run you migration, limiting to a single item
1. View that item to see if your migration is working

Currently, to view that item you'd need to check out the migration map table yourself. Migrate Inspect can help with that by giving commands to view the last node, a random node (handy for spot checking when migrating lots of nodes), or listing/finding source or destination ids for specific cases you want to verify.
