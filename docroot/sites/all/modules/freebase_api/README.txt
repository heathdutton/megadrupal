Freebase API
============

Utility to make requests from the Freebase API.

This utility also supports a 'mapping' between CCK+Fields and Freebase
Schemas and attributes. In a similar way to how RDF (should) do it, with
reference to D6/D7 RDF hook_rdf_mapping.

Other modules
-------------------------------------------------------------------------------
This is similar to, and possibly complementary to, but currently different from 
http://drupal.org/project/freebase
The freebase.module project however is dead in 2010.


To use this functionality fully, you are expected to install & enable rdf_mapper.
And to do anything useful, try the cck_importer.

@author 'dman' Dan Morrison http://coders.co.nz/
@version 2010

Content types
-------------------------------------------------------------------------------

A 'Book' CCK Content type is equivalent to Freebase '/book/book/ type schema,

http://www.freebase.com/type/schema/book/book

and has an rdf mapping of 'freebase:book.book' 

http://rdf.freebase.com/ns/book.book

Freebase 'book' is a compound type, inheriting properties of 'Topic' and
'Written Work', a combination of this defines the fields that the CCK may
use.


Fields
-------------------------------------------------------------------------------

Those type schemas have properties, which also have URIs, expressible in RDF,
eg

FREEBASE http://www.freebase.com/type/schema/book/author

FREEBASE ID /book/author

URI http://rdf.freebase.com/ns/book.author

CURIE freebase:book.author


We add these fields to our CCK type, named by default 'field_book_author'
(replacing odd characters with _)

Mappings
-------------------------------------------------------------------------------

To allow better configurable control, we can use other field names, and the
mapping between our CCK field names is stored somewhere, and then exposed the
way RDF (D7) expects it.
This is managed by rdf_mapping module - a backport-of-sorts from Drupal 7
entity info API

