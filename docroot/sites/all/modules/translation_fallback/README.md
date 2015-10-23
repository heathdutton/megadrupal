# Introduction

Translation contexts is a great thing, but way too specific in `i18n`'s
implementation: each single string has a context and you need to translate
the same string for each of its contexts.

This module enables fallback to the default context when a string has no
translation in its own context.


## Supported objects

At present the module is able to fall back to core translation for the
following objects:

 * Blocks (needs `i18n_block`, part of ``i18n`` project)
 * Fields and instances (needs `i18n_field`, part of `i18n` project)
 * Menu items (needs `i18n_menu`, part of `i18n` project)
 * Panels (needs `i18n_panels`, part of panels project)
 * Taxonomy terms (needs `i18n_taxonomy, part of `i18n` project)
 * Views (needs `i18nviews` project)


# Install instructions

This module works out of the box. Just enable it. There're no configuration
options.
If you're enabling it on a website where you've already added translations, see
the [Prepare translation tables](#prepare-translation-tables) section below.

Note: Due to the nature of some objects, you need to flush caches for its
strings to start translating.


## Prepare translation tables

Here's a set of SQL queries to sanitize the translation tables and also prepare
them for translation_fallback integration.

Sanitization consists on removing empty translations introduced by `i18n` module.

Preparation consists on "flatten" the translation contexts by moving all
translations to the default context and removing duplicates.

These queries are not reversible. Be sure you know what your doing. In any case,
don't forget to perform a backup of the tables first.


## Remove empty translations

When you use `i18n`'s form to translate and save it with no translations, an
empty row is added to the translations table.

This query is safe: it won't destroy valuable date.

`DELETE FROM locales_target WHERE translation = '';`


# Move all strings to the default context

This query will move all context-specific translations to the core default
context. This way `translation_fallback` module can find them.

If you want to preserve all context-specific translations in place, don't
perform do it!.
If you just want to move some context to the default context, tweak the `WHERE`
clause to fit your needs.

`UPDATE locales_source SET context='' WHERE context!='';`


## Remove duplicate strings without translations, not in the default textgroup

Once we've moved all translations to the default context. There may be a lot of
duplicates coming from different contexts (menu links, taxonomy terms, views and
panel titles...). Remove all of them that have no translation.

`DELETE ls FROM locales_source ls LEFT JOIN locales_target lt ON ls.lid=lt.lid WHERE lt.lid IS NULL AND textgroup != 'default';`


## Remove all duplicate strings

This is the final set: the remaining duplicate strings all have translations.
Note that translations may be different. This query removes all strings but
the one with lower lid.

`DELETE FROM locales_source WHERE lid NOT IN (SELECT lid FROM (SELECT MIN(lid) as lid FROM locales_source WHERE textgroup='default' GROUP BY source) AS tmp);`


## Delete translations that don't have a source

`DELETE lt FROM locales_target lt LEFT JOIN locales_source ls ON ls.lid=lt.lid WHERE ls.lid IS NULL;`


