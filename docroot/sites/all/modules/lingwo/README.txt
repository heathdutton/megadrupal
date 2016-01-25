Lingwo - Online Dictionary modules
==================================

Lingwo is a small family of modules for creating an online dictionary, which try to support
modern, professional lexicographical practices as much as possible.  It focuses on translating
dictionaries (ex. Spanish-English dictionary) which are maintained collaboratively by a team,
like a wiki.

This is the code that powers the dictionary part of [BiblioBird.com](http://www.bibliobird.com).

Installation
------------

 * First, enable the "lingwo_entry" module.
 * Next, if you want to implement a translating dictionary:
   - Enable the "locale" and "translation" modules.
   - Go to "Admin >> Site configuration >> Languages" and add any languages you need.
   - Next, we need to make the "Dictionary entry" content type translatable:
     * Go to "Admin >> Content management >> Content types"
	 * Click "edit" by the "Dictionary entry" type
	 * Expand "Workflow settings" and change "Multilingual support" to "Enabled, with translation"
	 * Click "Save content type"
   - If you are using the views module, you may want to consider disabling the "lingwo_dictionary"
     view and enabling the "lingwo_dictionary_multiple" view.
 * If you want to using Lingwo to manage multiple senses on each entry, enable the "lingwo_senses"
   module.

Now, you can create content of the "Entry" type and translate it using Drupal's built-in translation
mechanisms.

Configuration
-------------

You can find configuration for all the Lingwo modules at:

>  Admin >> Site configuration >> Lingwo dictionary settings

I recommend taking a look at what is configurable right away.

Modules
-------

### lingwo_entry

 * Core module on which all the other modules build
 * Defines a "Dictionary entry" content type
 * Adds a "Part of speech" field to all entries and makes sure that all entries are unique according
   to language, part of speech and headword.
 * Makes dictionary entries accesible through a lookup path.  For example:

   >  http://example.com/lookup/es-en/coche(noun)

   (Links to the Spanish-English dictionary translation of the noun "coche".  This example uses
    the default lookup path "lookup", which can be altered in admin)
 * Provides a search block for dictionary entries

### lingwo_senses

 * Adds a list of multiple "senses" to each entry
 * On source entries (ex. the Spanish entry in a Spanish-English dictionary) each sense gets:
   - "Difference" - Describes how this sense is different than the others (can be configured to
      be a "Definition" instead)
   - "Example" - A single or multi-line example (configured in admin) of the word in use
   - "Relationship" - The relationship between this sense and another entry.
 * On translation entries (ex. the English translation of a Spanish entry) each sense gets:
   - "Translation of Example" - Allows you to translate the example into the translation language.
   - "Translations" - A list of translations for the entry
   - "Same As" - Lets you mark this sense as having the same translation as another sense
   - "No equivalent" - Let you mark this senses as having no equivalent in this language (normally
     only necessary for grammar words in unrelated languages)

### lingwo_fields

 * Adds arbitrary "fields" to each entry.
 * Standard fields can be configured for each part of speech and language (when used with the
   lingwo_language module).
 * Its main purpose is to store additional forms of the word, for example: plural of nouns or
   conjugation of verbs.
 * Can be given JavaScript to calculate the value of standard forms based on rules.  This is
   helpful for languages with a lot of forms.  The JavaScript can calculate the forms based on
   the rules so that users only have to enter the exceptions.
 * Allows entries to be found by the dictionary search block via their other forms

### lingwo_pron

 * Allows entries to attach one or more pronunciation elements
 * Includes support for audio (and an ogg vorbis player in Firefox and Chrome)
 * Includes an IPA field for transcriptions in the International Phonetic Alphabet
 * Includes a configurable "Tag" field to mark pronunciations as belonging to a particular
   dialect or accent (ex. British vs. American)

### lingwo_language

 * Allows languages to have seperate definitions of configurable values inside of Lingwo
 * Adds a new "Language definition" content type to facilitate creating definitions for each language
 * Used by the following modules:
   - lingwo_entry: To allow different part of speech values per language.
   - lingwo_senses: To allow different sense relationship values per language.
   - lingwo_fields: To allow different standard fields per language.
   - lingwo_pron: To allow different "Tags" per language.

Security note
-------------

The lingwo_fields module allows users to with the appropriate permission
(*edit Lingwo fields JavaScript*) to enter JavaScript code which will be loaded by other users.
**Grant this permission with care!**  Untrusted users could use this for all kinds of nasty
things.  This is similar in nature to the "php" module which lets certain users enter PHP code.
It should probably only be granted to site administrators.

In the future, I want to integrate some JavaScript sandboxing technology (like
[Caja](http://code.google.com/p/google-caja/) or [AdSafe](http://www.adsafe.org/)), so that normal
users can more safely use this feature.

But for now, you have been warned!

Integration with other modules
------------------------------

Lingwo has no dependencies outside of core Drupal modules.  But it does integrate with a number
of other modules if your site happens to have them enabled.

 * [views] [1]: Provides integration with the Lingwo database tables and two default views
   for browsing dictionary entries.
 * [i18nstrings] [2]: Allows translation of part of speech values and sense relationship values.
 * [context] [3]: Allows the search block to maintain its context after a search.
 * [languageicons] [4]: Adds flags to entry titles.
 * [cck] [5]: Besides allowing you to add interesting new fields to your dictionary entries, when
   the "content" module is enabled, you can rearrange all our custom fields on the node edit form.
 * [node_export] [6]: Useful for importing/exporting languages defintions.  The language definitions
   from BiblioBird.com for English and Polish are included with this module.  You can use them
   as examples, or to help you jumpstart creation of your own English or Polish dictionary.

[1]: http://www.drupal.org/project/views
[2]: http://www.drupal.org/project/i18nstrings
[3]: http://www.drupal.org/project/context
[4]: http://www.drupal.org/project/languageicons
[5]: http://www.drupal.org/project/cck
[6]: http://www.drupal.org/project/node_export

Copyright
---------

Copyright (C) 2010, 2011 David Snopek

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

