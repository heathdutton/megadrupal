Freebase CCK
============

A plugin to field_schema_importer that provides a parser to transform Freebase 
schemas into Drupal content types.
The admin UI that does the import is field_schema_importer

When used in association with rdf_mapping, the imported types and each of their
fields are persistently tagged with a URI/CURI reference to the data source and
data type of the mapping. This enables CCK-managed content to be labelled
identically to that in the shared open data version as found on Freebase, and 
can provide hints for autocompletion.
The autocompletion etc is handled by Freebase API and freebase_data (suggest)
and is not a requirement for one-off CCK schema imports.


@author 'dman' Dan Morrison http://coders.co.nz/
