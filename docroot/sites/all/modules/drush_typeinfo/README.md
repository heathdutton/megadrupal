# Drush Typeinfo

Get information about entity bundles and their fields.

## Installation

### Drush

    drush dl drush_typeinfo

### Git

    pushd ~/.drush
    git clone git@github.com:robballou/drush_typeinfo.git

## Usage

List all entity types and their bundles using `typeinfo-list` (`til`):

    drush typeinfo-list
    drush typeinfo-list --type=node

List all fields for the node/article bundle with the `typeinfo` (`ti`):

    drush typeinfo article

List all the fields for the taxonomy_term/location bundle:

    drush typeinfo location taxonomy_term

Get info about a particular field with `typeinfo-field` (`tif`):

    drush typeinfo-field field_location

To get some instance information when field_location is used in the page content type:

    drush typeinfo-field field_location page

To view raw information about a field:

    drush typeinfo-field field_location page --field-info
    drush typeinfo-field field_location page --display-info
    drush typeinfo-field field_location page --widget-info

Get a "report" about an entity type, useful for adding to a spreadsheet when auditing a site:

    drush typeinfo-report article
