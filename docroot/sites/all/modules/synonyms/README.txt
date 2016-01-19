
-- SUMMARY --

The Synonyms module extends the Drupal core Taxonomy features. Currently
the module provides this additional functionality:
* support of synonyms through Field API. Any field, for which synonyms behavior
   exists, can be enabled as source of synonyms.
* synonyms-friendly autocomplete and select widgets for taxonomy_term_reference
   fields
* integration with Drupal search functionality through Synonyms Search
   submodule. It enables searching content by synonyms of the terms that the
   content references. Synonyms Search submodule also integrates with Term
   Search contributed module in a fashion that allows your terms to be found by
   their synonyms.
* integration with Search API. If you include synonyms of a term into your
   Search API search index, your clients will be able to find content with
   search keywords that contain synonyms and not actual names of terms.

-- REQUIREMENTS --

The Synonyms module requires the following modules:
* Taxonomy module
* CTools module

-- SYNONYMS BEHAVIOR, SUPPORTED FIELD TYPES --

Module ships with ability to use the following field types as synonyms:
* Text
* Taxonomy Term Reference
* Entity Reference
* Number
* Float
* Decimal

If you want to implement your own synonyms behavior that would enable support
for any other field type, refer to synonyms.api.php file for instructions on how
to do it, or file an issue against Synonyms module. We will try to implement
support for your field type too. If you have written your synonyms behavior
implementation, please share by opening an issue, and it will be included into
this module.

-- GRANULATION WITHIN SYNONYMS BEHAVIOR --

In order to achieve greater flexibility, this module introduced additional
granularity into what "synonyms" mean. Then you can enable different synonyms
behaviors for different fields. For example, field "Typos" can be part of
autocomplete behavior, while field "Other spellings" can be part of search
integration behavior. Currently the following synonym behaviors are recognized
(other modules actually can extend this list):
* General synonym - normally we suggest to enable this behavior for all fields
   that have enabled at least one another behavior. In technical words, this
   behavior is responsible for including content of the field into term synonyms
   and also enables ability to add entities as synonym into this field.
* Autocomplete - whether content of this field should participate in
   autocomplete suggestions. This module ships an autocomplete synonyms friendly
   widget and its autocomplete suggestions will be filled in with the content of
   the fields that have enabled this behavior.
* Select - whether content of this field should be included in the synonyms
   friendly select widget.
* Search integration (requires Synonyms Search enabled) - allows your content to
   be found by synonyms of the terms it references. Your nodes  will be found by
   all synonyms that have this behavior enabled.

Therefore, on the vocabulary edit page you will see a table, where rows are
fields that can become synonyms and columns are these "synonym behaviors" and
you decide what synonym behaviors to activate on what fields.

-- INSTALLATION --

* Install as usual

-- CONFIGURATION --

* The module itself does not provide any configuration as of the moment.
Although during editing of a Taxonomy vocabulary you will be specify for that
particular vocabulary the additional functionality this module provides, you
will find additional fieldset at the bottom of the vocabulary edit page.

-- FUTURE DEVELOPMENT --

* If you are interested into converting this module from synonyms for Taxonomy
terms into synonyms for any entity type, please go to this issue
http://drupal.org/node/1194802 and leave a comment. Once we see some demand for
this great feature and the Synonyms module gets a little more mature, we will
try to make it happen.
