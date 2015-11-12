Block Title Tooltip

-----
This module allows you to show your block description or about block in tooltip
style with block title.

Block tooltip module create a field in block admin configuration page
to add tooltip.

It can be used to make additional information (eg. instructions, help) available
to the end user in a way that does not use too much space on the screen.

After finish configuration you can get tooltip when you hover on block title.

It works by creating new template variables in the $block object. then uses
hook_preprocess_block to wrap a tooltip around the block->subject variable.
There is a also a configuration parameter on a per block basis that disables
the tooltip from rendering in the title.
