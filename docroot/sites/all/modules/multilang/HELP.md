### _Multilang_ 

This module is intended to allow entering all desired translations of a given
content inside of a unique node, instead of having to create a separate node
(with _Multilingual Content_) or a distinct field version (with _Entity
Translation_) for each translation. 

The benefits of using this method are mainly: 

*   when entering new content or updating an existing one, authors keep a total
visibility upon all the translations at the same time
*   a direct consequence is to reduce the risk of omitting to update the
translations when an original content has been modified
*   when a content includes a lot of lang-insensitive data (like images, links,
or simply verbous HTML with a number of attributes), these parts don't have to
be duplicated: only the textual parts must be entered as "multi" and rewritten
in the different languages
*   since it emulates the Spip <multi> syntax, this method is nicely suitable to
allow the direct migration of contents from this CMS, without the need of a
painful data restructuration
*   moreover, in the latter case authors can maintain their previous work habits
unchanged

### Installation/Configuration 

_Multilang_ works without the need of other translation module, assumed you have
set the "Internationalization" option of _i18n_, and configured the desired
languages. Nevertheless you can simultaneously use alternative translation
methods, like _Multilingual Content_, _Menu translation_ etc. 

By itself, the module contains no configuration option. By cons, to use the
"multi" syntax you must: 

*   In "Administer > Configuration > Regional and language > Languages", enable
the "URL" detection method, and configure it to "Path prefix"
*   For each enabled language ensure to provide the corresponding standard "Path
prefix language code" (don't leave it empty)
*   Position the _Language switcher (User interface text)_ block in a region,
anywhere you like

If you're also using other options of the _i18n_ module, there are some
additional configuration options you must pay attention to: 

*   for each node where the "multi" syntax is used you MUST select the **Neutral
language** in the "Language" field (or the lang switcher would deactivate any
lang other than the specified one)
*   for each block where the the "multi" syntax is used, you MUST **keep
unchecked all the proposed languages** in the "Languages" part of "Visibility
settings"

### Syntax 

NOTE: this is the raw internal structure which makes the _Multilang_ mechanism
to work, and was initially designed to easily migrate data from the _SPIP_ CMS.  
As of 7.x-2.0 you can get totally rid of directly managing the "multi" syntax
(look at **User interface** below). However you can continue to use whenever
you want.

When entering data in a text area, you may insert multilingual pieces of text by
enclosing them between `[multi]` and `[/multi]` tags.  
Such a piece of text is a "multi" _segment_, which may contain a number of
"multi" _blocks_, each representing the desired content translated in a given
language, like `[language-mark]…content…`, where: 

*   `language-mark` is the involved language code, such as `en`, `fr`…
*   `…content…` is the translated content

Example:  

>
>     [multi]  
>     [en]This is an english text  
>     [fr]Ceci est un texte en français  
>     [/multi]
>

Any spaces, newlines, line-breaks or paragraph-breaks are ignored when they are
located just after the opening tags, juste before the closing tags, and just
around the language-marks. In other words, each text block is rendered trimmed. 

### Userinterface

While _Multilang_ fundamental work is to interpret the "multi" parts
of text found to render it in the current language when displaying public
pages, it also manages how these parts are presented to the user when it is
creating or updating nodes or blocks contents.  
Obviously at this moment the user must be able to look at all the available
different translations of the same text: this is primarily achieved by
merely displaying the raw "multi" syntax, as explained above, but it often
may be a painful job to navigate through a lot of [multi] and [lang] tags.

To allow a more friendly way to manage this, _Multilang_ includes
different UI facilities, depending on which input type is involved:
*   **"long-text" fields** (such as body), which represent quantitatively the
largest part of text and are input through <textarea> elements, may contain a
lot of (even complex) HTML structures: it's why they're usually processed
through a wysiwyg editor.  
So for them _Multilang_ includes a _CKEditor_ plugin that can be activated for
any text format (see **Using _CKEditor_** below).
*   **all other text fields** (including "text", "link" and so on) are 
presented by Drupal through simple <input type="text"> elements.  
As of 7.x-2.0, _Multilang_ works as follows:
    1.  when the element content is already using "multi" syntax, with
        language blocks for each of the currently defined languages,
        attaches a widget (activated on focus) which displays each language
        part in a dedicated cell.
    2.  when the element contains only raw text, or is already using "multi"
        syntax but with a limited set of languages, offers a "Use Multilang
        template" link, which on click:
        -   automatically normalizes the element content, i.e. formats it 
            with the complete set of currently defined languages (if only raw
            text was present, it is affected to the default site language)
        -   attaches widget as explained above
        -   offers a "Back" link to restaure the original content
This way the user remains free to normalize input or not (may be keeping only a
limited set of defined languages), while he has an easy way to update when a
new language is added to the site.
*    by extension this behaviour is also available for texts which are not 
really fields, such as **field labels** and **node or block title**.

### Using with _CKEditor_ 

With the _CKEditor_ module you can benefit from improved input method which gets
you rid of the above syntax and automatically offers input areas dedicated to
each of the languages defined for the site.  
For this to work: 

*   your version of _CKEditor_ must include the _Widget_ plugin (you may install
it from the [ CKEditor builder](http://ckeditor.com/builder))
*   in the _CKEditor_ configuration, for each profile where you want to allow
it, in the "EDITOR'S APPEARANCE" group: 
    1.  in the section "Tools bar", add the "Multilang" button to the tools bar
    2.  in the section "Plugins", check "Multilang" in the list of plugins to be
    activated
*   then with this profile any text field part where the "multi" syntax is used
automatically displays a "MULTILANG" group, with a subgroup inside for each
defined language
*   at any time you may click the "Multilang" button in the tools bar to create
a new empty "MULTILANG" group
*   note that only text fields (or blocks text) can be entered through
_CKEDITOR_: in views, and in nodes or blocks titles, you still must use the
"multi" syntax

### Using with _Pathauto_ 

With the _Pathauto_ module, if you have introduced "multi" syntax in the node
titles, you may use the `[node:multilang-native-title]` token to generate URL
aliases, which will be localized using the **site default language**.   
CAUTION: using the `[node:title]` token would generate aliases from the **raw**
title, resulting in something like `multienmy-titlefrmon-titremulti`! 

### Using with _Views_ 

With the _Views_ module, you can use the "multi" syntax inside of the texts you
enter in the definition forms of a view.  
They will be rendered like explained above, in the views summary, in the
previews, and of course in the pages where they are included. 

### Notes 

1.  Which language code is used to render depends on the lang part of the
current URL (such as "en" in `http://example.com/en/...`), which is generally
defined by how the lang switcher is currently set. If no language is currently
defined (so Drupal language is empty), the site lang is used.  
As a fortunate side effect, at any moment you may deactivate the lang switcher,
and all contents including the "multi" syntax are simply rendered in the site
lang.  
CAUTION: at the time this document is written (Drupal 7.34), deactivate the lang
switcher seems to cause vocabulary terms translations to be lost!
2.  If a "multi" block does not contain translation for the current lang, the
available text in the site lang will be rendered instead.
3.  You may use the "multi" syntax not only in any text field or block body, but
also in the node or block title.
4.  In order to allow a simple migration of contents from the Spip CMS, an
alternative syntax is also accepted, using HTML-fashion tags like `<multi>`,
rather than `[multi]`.  
You may also use this syntax when manually entering text (using plain text
editor). 

### Localization 

Because of its mission to integrate multiple languages in the same container,
the _Multilang_ module does not conform to the standard Drupal localization
system: on this level, it is self-sufficient and embeds all of its own
translations, written with the "multi" syntax.  
All these translations (including the text you are reading now) are gathered in
the file `multilang.data.inc`. 

To add a language, you just have to extend this file, in which the various
"multi" blocks required are divided into a few number of functions, standardized
according to the following scheme: 

>     function _multilang_FUNCTION() {  
>       module_load_include('inc', 'multilang', 'multilang.core');  
>       ob_start();  
>     ?>  
>  **[multi]  
>  [en]This is an english text  
>  [fr]Ceci est un texte en français  
>  [/multi]  
>  **
>     <?php
>       return _multilang_process(ob_get_clean());  
>     }
>   `

In the example above, to add a Spanish translation will require you insert it in
the "multi" block, which will become: 

>   **[multi]  
>  [en]This is an english text  
>  [fr]Ceci est un texte en français  
>  [es]Este es un texto en español  
>  [/multi]  
>  **

The __multilang_token_info()_ function is an exception to the above diagram,
because the text it contains is distributed in a two-dimensional array, where
each element is a "multi" block. It is within these that you will insert the new
translations. 

_NOTE FOR ENGLISH SPEAKERS: the creator of this module is not fluent in English,
so it is unsure of the quality of writing. **All corrections and improvements
are welcome.**_ 
