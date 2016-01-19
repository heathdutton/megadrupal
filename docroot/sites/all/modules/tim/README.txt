Token i18n Macrons
------------------

The Token i18n Macron Module is a flexible translation module whose primary goal
is to translate given macrons.

The module is able to hook changes to tokens to provide translations to
individual characters to entire words.

More translatable macrons can be added through the hook system.

The module will come installed with simple Maori Translation. This can be
overwritten or extended with another module.

Translations may covert the result to only ASCII characters or allow unicode
too.

The module will come with a hook to extend path_auto to allow URLs to only
contain ASCII characters.

Suggested usage is to remove Unicode characters from URLs to make them
cross-browser compliant.

Similar Modules
---------------
There are no modules that implement this exactly. Below are two similar:
1. Entity Translation can be used to translate fields. This however does not
   include the URL.
2. Advanced Entity Tokens can use tokens to take data from the database directly.
   This does not include a translation or normalisation means.

Recommended Modules
-------------------
- PathAuto

Installation
------------
1) Copy the Token i18n Macron module directory to the modules folder in your
  installation.

2) Enable the module using Administer -> Modules (/admin/modules)

Configuration
-------------
The configuration for this module is all hook based. To modify any settings
first extend hook_token_i18n_macrons_dictionary_options. The variables available
to modify are listed below:

$options['settings']['ascii only'] - If true then all non-ASCII characters will
 be replaced with default (defined below).
$options['settings']['default'] - Default to empty string, suggested values
 include '_' and '-'. Usage defined above.
$options['settings']['pathauto'] - If true then it will apply this to all
 path auto generated URLs.
