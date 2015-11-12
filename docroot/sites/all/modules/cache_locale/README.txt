CONTENTS OF THIS FILE
---------------------
   
 * Introduction
 * Installation
 * Configuration
 * Development

INTRODUCTION
------------
Provides a framework overriding the locale() function thereby enabling various ways of caching translations.

Requires a core patch, which is bundled with the module. The patch allows for override of the locale() function.

A couple of translation cache handlers are bundled with the module.

INSTALLATION
------------
Apply the core patch, and enable the module.

CONFIGURATION
-------------
Example:
@code
  // Use the Cache Locale as cache handler for translations.
  $conf['custom_locale'] = 'CacheLocale';

  // Module's default settings.
  $conf['cache_locale_class'] = 'CacheLocaleCore';
  $conf['cache_locale_bin'] = 'cache_locale';
  $conf['cache_locale_expire'] = 0;
  $conf['locale_cache_length'] = 75;
@endcode

Use the custom cache class that comes with the Cache Locale module.

@code
  // Use the Cache Locale as cache handler for translations.
  $conf['custom_locale'] = 'CacheLocale';
@endcode

The CacheLocale class uses a DrupalCacheArray based class for
handling cache hits/misses. There a four handlers bundled with the Cache Locale
module.

| Class                  | Description                                        |
|------------------------|----------------------------------------------------|
| CacheLocaleCore        | Caches (almost) like core. One cache entry for all |
|                        | translations having source less than 75 characters |
|                        | long AND one cache entry per translation having    |
|                        | source that is 75 or more characters long. The     |
|                        | latter is what differs from core.                  |
| CacheLocaleSingle      | Use a single cache entry per translation.          |
| CacheLocalePath        | Use a cache entry per path.                        |
| CacheLocaleContextRoles| Use a cache entry per context/roles.               |


@code
  // Use the CacheLocaleCore.
  $conf['cache_locale_class'] = 'CacheLocaleCore';
@endcode

@code
  // Use the bin: cache_locale.
  $conf['cache_locale_bin'] = 'cache_locale';
@endcode

@code
  // Expire translations after 24 hours.
  $conf['cache_locale_expire'] = 3600;
@endcode

@code
  // Cache all translations having source text less the 50 characters long into 
  // one cache entry.
  $conf['locale_cache_length'] = 50;
@endcode

DEVELOPMENT
-----------
Create either your own locale class that implements LocaleInterface and set
$conf['custom_locale'] to this class, or create a new class for the CacheLocale
class to handle cache translations (see the source code for the bundled cache 
handlers under handlers/*.inc).
