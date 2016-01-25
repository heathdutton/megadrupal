The Mirrors module automagically creates content exporters and importers for
active bundles.

Exports are Views Data Exports in CSV format.
Imports are Feeds.

Content import and export takes a lot of time.
Although there is excellent tooling to do this (Views, Feeds, Migrate) it stays
difficult and error-prone to setup these modules to function appropriately. Its
a lot of clicking around, testing and retesting. And when you want to
change/add/remove a field, you have to do this at least 3 times: field_ui,
views_ui and your importer. Thats where this module kicks in.
There are a couple of use-case for this module:
1. While developing; you export/import your users, content, products and don't
like to fallback on your database backups but instead want a litte more
regression-testability.
2. As a tool to get your content in version control. As we have features for 
configration, you can use this for content.
3. As a lazy way to create complex Views and Feeds. Let Mirrors create them, and
afterwards you clone and adjust them. See mirrors_example for entities with a
lot of fields.

This module supports specific core entities and fields.

Supported Entities:
- node
- taxonomy_term
- user

Supported Fields:
- boolean
- date
- decimal
- integer
- list
- taxonomy_term
- text
- text_formatted

Supported Properties:
- boolean
- date
- integer
- text


Installation:
1. Enable this module.
2. Enable 'Views UI' and 'Feeds UI'
3. Visit 'admin/structure/mirrors' to enable Mirrors per bundle.
