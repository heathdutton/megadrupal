Good Relations Plugins.

Good Relations is the most powerful Web vocabulary for e-commerce.

Good Relation
- classes:gr: BusinessEntity (for the company)
          gr: Offering (for an offer)
          gr: ProductOrService (for the object or service)
          gr: Location (for a store)

Momentarily the information is basic and can/will be extended for more complex
structures. :)


I will firstly explain the good_relations_test_node.
-> good_relations_test_node.info
The purpose of this module is to create a test node in the node_type table.
The newly created node_type (Test_CAR) in our case will be directly mapped
according to a certain (defined by us) structure.
http://www.ebusiness-unibw.org/tools/grsnippetgen/ (you can generate an XML
using good relations snippet generator, to see how it will look like).
http://www.heppnetz.de/ontologies/goodrelations/goodrelations-UML.png (all
implementations are based on the this big picture diagram).

-> good_relations_test_node.install
The content type is created, the field and instances are set.
In order to set the field_collection_item elements, you need to change the
field Widget from Hidden -> Embedded.
Create a new content of type Test_CAR and start populating the example as presented
in the descriptions (not complicated).
View the newly created content's page.
Inspect the element, select the HTML (from the beginning, in order to select
the entire HTML) -> copy HTML.
Go to -> https://www.google.com/webmasters/tools/richsnippets?q=uploaded:8004f223b2b6d5b86e01b72dc85222b9
Copy the HTML and test to see what Google sees.
As you will notice every completed field is identified correctly.

This is the basic example of what this module is set to achieve.

-> good_relations_test_node.module
Here will be the hardcoded mapping and the namespace setting for our
newly created content type.
Some namespaces are already included in Drupal.
https://drupal.org/node/2154821


Now moving on to the Main Module.

Steps:
1. download and enable the module and its dependencies.
2. in admin/config/user-interface/ use -> mapping or view/add tags links
(or click the configure link on the modules page).
3. configure as you please (take in mind the correctness)
4. verify your information in google webmasteri for conclusions on functionality.

All modules will be a part of the good_relations module (submodules).
1) Good Relations main module.
-> good_relations.info file
The main module will of course have as dependencies the submodules
(good_relations_products, good_relations_commerce_product, good_relations_tags),
I will explain each one separately. Also, Drupal core comes with the RDF module
(which is not enabled), you'll have to enable it.

RDF (Resource Description Framework) is a W3C standard for modeling and sharing
distributed knowledge based on a decentralized open-world assumption. Any
knowledge about anything can be decomposed into triples (3-tuples) consisting of
subject, predicate, and object; essentially, RDF is the lowest common denominator
for exchanging data between systems.
https://drupal.org/project/rdf

In Drupal 7 we have RDFX (extensions)
Drupal 7 core includes an RDF module which outputs RDFa.

Also, we have the ctools dependency due to the fact that we create and work with
plugins based on previously developed implementations and methods which come in
very handy.
https://drupal.org/project/ctools

We include only 1 file.
The good_relations_interface.inc. (excluded the abstract class for now).
The current complexity doesn't require both of them, but for further
implementations they will be very useful (I will explain soon).

-> good_relations_interface.inc
Anyone who used the .alpha version can see that the abstract class is missing.
The reason => it is not needed.
The classes that implement the interface have 2 methods that need to/can differ.
Namespace and Map.
An example would be that the mappping is done differently depending on multiple
factors as you will notice when following code.
The other methods implemented in the classes are unique, thus not requiring an
abstract class to extend.
The abstract class will be added when same methods are needed for multiple
classes (if someday necessary) or else we will just stick to the interface.
An interface implies declaring all of the methods will which be used by the
classes implementing this method.

-> good_relations.module
The steps are well commented and structured properly for a better understanding,
and include the following methods, in order (general description):
Create the menu items.
Create the form properly (using easily understandable names).
(modified in Beta version, options and structure declarations improved)
Ajax callback.
Validate form. (still needs work)
Submit form.
Set namespace (good relations).
Instance methodology (node and entity).
Get the available options for autocomplete from the database.
Get the entity reference fields (commerce product reference or field collection items).

2. Good_relations_commerce_product
(This module basically allows you to map the commerce_product entities without
having to configure the product variations for each node separately, which can
be useful if you simply want to map the variation types, easier usage for expert
users).
-> good_relations_commerce_product.info
We include 2 files. The good_relations_commerce_product.inc and the
good_relations_commerce_product.module.

-> good_relations_commerce_product.inc
The whole idea is that the functionality of this class is exactly the same
as for the good_relations_entity_reference.inc (later presented).

-> good_relations_commerce_product.module
The modules handles things a little differently, requesting only the
commerce_product_entities and not the whole structure (node + entity references).
For a better testing of the application you can verify the modifications as
they happen, when firstly modifying only the commerce_product reference =>
modifications in the products (module), and vice-versa.

This of course can be improved.

3. good_relations_products module (dependency of Good Relations)
-> good_relations_products.info
We include two different files (beta version), the good_relations_content_types.inc
and the good_relations_entity_reference.inc.
The reason this was modified was because the two classes only need to implement
the interface, they have their own methods, the only difference being the
mapping which is done differently in each of them.

-> good_relations_content_types.inc
Will not be extending the abstract class anymore (beta version).
It will implement the interface (personal mapping) and also have its own
methods.
The content_types now will include every possible 'node' found in the system.
The basic content types from the system and the added nodes for the commerce
kickstart except for the entity references (commerce_product (reference) and
field_collection_items).
This has been done for an easier observation and understanding of functionality.
(ex. you wish to map the fields (excepting the inclusions of product variations
field), for bags_cases. This class handles just that. The mapping will be stored
in the database as type : node (because everything content type is basically
a node) and as bundle : the machine name given for the product).

-> good_relations_entity_reference.inc
Same as previously presented.
The only difference is that the entity references will have a different mapping
system in the database. Depending on which entity reference you choose to map.
(ex. you wish you map the product variations from the bags_cases, which in the
field settings is commerce product (reference). The mapping for this will be
stored in the database separately as type : commerce_product and as bundle :
the machine name given for the variation type to which you link).

-> good_relations_products.module
It will generate the plugins accordingly.
Works with the database using predefined methods.
There will be two types of plugins which will be generated arrays, following a
well-defined and easily understandable structure, which can later be relatively
easy to use when retrieving the elements.'
You set the two plugins which basically have the same handler (content_types).
One of the plugins refers to the basic content types, and the other to the
new content types added by the commerce kickstart profile. You set the same
handler due to the fact that, being nodes, they are handled the same way.
You need to make some modification for the structure to be similar, in order
to be used more efficiently (composing usable and easily understandable arrays).

IMPORTANT! The logical part: if you consider to map same entity reference
fields in both places -> the priority will be from the 'node' itself and not
from the separate mapping system.

4. good_relations_tags module (dependency of Good Relations)
-> good_relations_tags.info
We include a .install file which will generate the schema.

-> good_relations_tags.install
We create our schema following the ID, tag_name and tag_description structure.
Some predefined tags will be saved in the database, to make it more user friendly.
(gr:name, gr:description...). Will be extended for more
complex functionality.

-> good_relations_tags.module
We will have a new menu item (separate for the mapping one).
This will allow users to view the existing tags in the database, and also allow
them to save new ones for later usage.
These tags can be viewed in the autocomplete stage when mapping (just some extra
information).


Thank you for downloading the module.
For a better understanding or if you have more questions, please contact us
on Drupal.org :)


