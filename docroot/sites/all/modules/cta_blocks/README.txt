CTA Blocks Module
=================

This module provides a simple custom block type, that accepts a title, optional subtitle, and a link.

Some benefits and features of CTA Blocks:
- CTA Blocks are managed on their own form at: admin/structure/block/cta-blocks
- CTA Blocks are fully import/exportable via the ctools export api.
- CTA Blocks use machine names instead of serialized IDs.
- CTA Blocks can be fully themed via template.
  - Default template is cta-block.tpl.php
  - Also available out-of-the-box is cta-block--MACHINE-NAME.tpl.php
    So, if you have a cta block with a machine name of 'foo_bar_baz' it has a template option
    of 'cta-block--foo-bar-baz.tpl.php' by default.
    

See cta_blocks.api.php for information on how to provide default cta blocks programatically.
