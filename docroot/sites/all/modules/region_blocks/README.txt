REGION BLOCKS
-------------

This module allows for site builders to configure theme regions as blocks,
that can then be placed in other regions.

INSTALLATION
------------

- Download module files to your site's /sites/all/modules directory.
- Enable Region Blocks at /admin/modules when logged into your site.
- Enable which region(s) you want available as a block at
  /admin/structure/region-blocks.
- Set what you want the cache set as for all blocks by default. (You
  will also be able to override this on a per block level).
- In the blocks admin page (/admin/structure/block), place blocks in the
  region(s) you configured in the previous step.
- Place the new "Region Block: <region_name>" block on your site.
