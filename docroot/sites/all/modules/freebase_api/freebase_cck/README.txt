Freebase CCK
============

A plugin to cck_importer that provides an interface to load Freebase schemas
as CCK content types.

RECOMMENDED:
Get the rdf_mapping module also!

When used in association with rdf_mapping, the imported types and each of their
fields are persistently tagged with a UIR/CURI reference to the data source and
data type of the mapping. This enables CCK-managed content to be labelled
identically to that in the shared open data version as found on Freebase, and 
can provide hints for autocompletion.
The autocompletion etc is handled by Freebase API and jquery_freebase (suggest)
and is not a requirement for one-off CCK schema imports.


@author 'dman' Dan Morrison http://coders.co.nz/
@version 2010