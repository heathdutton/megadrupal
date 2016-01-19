This module provides a Feeds Fetcher that consumes data from Freebase
and a mapper+processor that helps translate the responses into Feeds-compatible
Drupal data.

Usage

The Freebase query that fetches data can either be
 * simple (using basic form parameters)
 * complex (your own query using full Freebase MQL syntax)

For the manual MQL setup, see the separate README.

For automatic setup, you must be using content types that already have
semantic annotations on their fields, such as those auto-generated using the
schema importer.

EG, use the schema importer and import a content type definition for "Film".
Once that content type becomes part of your site, you can map data values to it
directly.

You can also set these up manually, by entering predicate namespaces directly.
