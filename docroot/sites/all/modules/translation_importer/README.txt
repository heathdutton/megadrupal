OVERVIEW
--------

Translation importer is a really simple module built to automatically import
translation catalogs (.po files) whenever these are updated. Original idea and
code are from dcycleproject[1]. This approach required an update hook to import
translations. That works perfectly but that's also unnecessary extra work for
translators who normally do not need to touch module .install files.

With this module the translations workflow is reduced to minimal amount of
steps:

 1. Checkout translation catalogs from Git.
 2. Push changes back to Git.

Translation catalogs are full catalogs and these catalogs should be the only
source for translations as translation import will always overwrite existing
translations. Catalog extracts, translations updates, merges and whatnot are
tasks that require other modules. These modules are related but quite different
because with a dev - staging - production workflow translation modules are not
required in staging or production environment at all. Translation importer is
all you need on these environments to keep translations up to date.


INSTALLATION & CONFIGURATION
----------------------------

This module requires no configuration but the source directory for translation
catalogs can be configured with "file_translations_path" -variable. This module
does not have any UI.


USAGE
-----

Translation importer module uses catalogs:// file stream to get the
translation catalogs. Files are automatically detected from that stream as long
as they are named correctly in form of "langcode-textgroup.po".

Examples:

 - fi-default.po
 - en-gb-field.po
 - sv-webform.po
 - etc...

Translations directory defaults to "conf_dir()"/translations, so usually that
is sites/default/translations. If these defaults are followed, this module
requires no configuration whatsoever.

This module will not add new languages or enable any localization modules. It
is recommended to use this module with a deploy module[2] or similar deployment
strategy. Catalog changes are detected whenever hook_flush_caches is ran. By
default that only happens when `drush updb` is invoked. Catalog updates are
checked even if there is no other pending updates.

[1] http://dcycleproject.org/blog/28/approach-deploying-translations-multilingual-site
[2] http://dcycleproject.org/blog/44/what-site-deployment-module
