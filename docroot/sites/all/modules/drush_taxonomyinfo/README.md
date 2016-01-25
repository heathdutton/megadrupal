# Drush Taxonomyinfo

Get information about taxonomies and their terms.

## Installation

    pushd ~/.drush
    git clone git@github.com:robballou/drush_taxonomyinfo.git

## Usage

List all taxonomy vocabs with `taxonomy-vocab-list` (`tvl`):

    drush taxonomyinfo-vocab-list

List all terms in a vocab with `taxonomy-term-list` (`ttl`):

    drush taxonomy-term-list article_types
