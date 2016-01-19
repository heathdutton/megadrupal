Description
-----------
This module provides api that should help with integrating your custom block
configuration with i18n, so it will be translatable on block "translate" page
just like the block title.

This module is for developers, you need to register your block property with
hook_block_i18n_config_info(), to see how to do this, see the file:
block_i18n_config.api.inc .

Dependencies:
-----------
* Block languages (i18n_block) (part of "i18n" module)