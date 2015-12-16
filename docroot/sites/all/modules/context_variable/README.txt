The Contextual variables module allows for Variables to be temporarily modified
using the Context module, for example, you could turn the Environment Indicator
module on or off depending on what the URL is or which user is currently
logged in.

Variables for use with this module are provided by hook_variable_info() and the
Variable module, please take into account that not every module currently
supports this hook but the number is growing.

Contextual variables was written and is maintained by Stuart Clark (deciphered).
- http://stuar.tc/lark
- http://twitter.com/Decipher



Required modules
--------------------------------------------------------------------------------

* Context  - http://drupal.org/project/context
* Variable - http://drupal.org/project/variable
