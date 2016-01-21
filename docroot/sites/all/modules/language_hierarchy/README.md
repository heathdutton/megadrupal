
This module provides a gui to change the language fallback system through hook_language_fallback_candidates_alter(). This
will allow you to setup language hierarchies where content can fall back to parent language content, without falling back
to English. 


Common Modules
==============

This module changes the language fallback system you will need to enable other modules to use this module effectively.

entity_translation   - Enable fields, nodes, taxonomies to be translated. If an entity block replacement (like BEAN) is
                       used, then block content will also respect the fallback content.

i18n_node_hierarchy  - Support for node translations.

i18n_menu_hierarchy  - Menus.


An Example
==========

We assume drupal's default language is English.

1. Enable language detection based on URL at admin/config/regional/language/configure .

2. Goto admin/config/regional/language and;
    Create Spanish language, with es prefix.
    Create Spanish (Mexico) language, with es-MX prefix.
    Create Spanish (Spain) language, with es-ES prefix.

    Order the languages to;

    English
    Spanish \
            \ - Spanish (Mexico)
            \ - Spanish (Spain)


3. If you have a default drupal install goto admin/structure/types/manage/page and in the 
   Publishing options tab select 'Enabled, with field translation' under 'Multilingual support'.

   Goto the body field edit page at admin/structure/types/manage/page/fields/body and
   select 'Enable translations'.


4. Add some page content in English. 
   Click the translate tab and add some content in Spanish.

5. If node created was node 10 then you should see the following;

http://localhost/node/10        - English content
http://localhost/es/node/10     - Spanish content
http://localhost/es-ES/node/10  - There is no Spain content, so it will display the Spanish content.
http://localhost/es-MX/node/10  - There is no Mexico content, so it will display the Spanish content.


Advanced
========

You can set up far more advanced fallback structures. e.g. Mexico, with Spanish fallback then English fallback.
 
  en -> es -> es-MX


Views support
=============

Filtering of entities can be done by adding language filter for field and then
selecting "Language fallback" option. Selecting "Distinct" in query settings is
also recommended.
