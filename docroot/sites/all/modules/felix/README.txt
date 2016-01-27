
PREFACE
-------
The Felix module provides a block interface for editors on top of the Context
module. Multiple regions can be created to allow editors to add specific blocks
to that region without using the block of context admin interface. Only blocks
from a predefined block set can be used.

CONTENTS
--------
1. Installation
2. Configuration
2.1. Block sets
2.2. Regions
3. Blocks
4. Views integration

1. Installation
--------------------------------------------------------------------------------
Before installing Rate, you need Context. If not already installed, download
the latest stable version at http://drupal.org/project/context
Please follow the readme file provided by Context on how to install.

Copy Felix into your modules directory (i.e. sites/all/modules) and enable Felix
(on admin/modules).

2. Configuration
--------------------------------------------------------------------------------
After installation, the configuration page will be available at
admin/structure/felix. This page shows a list with Felix regions (see ¤2.2).
A list of block sets is available under the tab "Block sets" (see ¤2.1).

2.1. Block sets
---------------
Before you can do anything with Felix, you have to create a block set. A block
set is a definition of which blocks can be added to a Felix region. You can
create multiple block sets, but a single block set for all regions may be fine
in many cases. Click the "Block sets" tab en and then "Add block set". Check
all the blocks that may be added to regions with this block set. Blocks from
modules are grouped by the module package. Nodetypes are below the packages.

2.2. Regions
------------
Regions are locations were users can add blocks. These are added from the Felix
admin under the "Add region" tab. This form has the following fields:

- Title
  Name of the region (visible for editors).
- Region
  Theme region. The Felix region will be visible in this theme region.
- Context
  The Felix region is only visible when this context is active.
- Weight
  Blok weight of the Felix region.
- Block set
  Set of blocks available in this Felix region.
- Differentiate content per
  Blocks added to this Felix region will be visible on all pages by default.
  Checking "path" for example will show different blocks per page. "Nodetype"
  will show the same blocks on all Article nodes, but different blocks on other
  nodetypes.

3. Blocks
--------------------------------------------------------------------------------
A new block will be visible for each Felix region. This blocks has a link
"Add block". This can be used to add new blocks in this region.

Felix blocks have new links added to it:

- Move up
- Move down
- Edit attributes
- Remove

The edit attributes link leads to a new page where the user is able to edit
the block title and block configuration.

4. Views integration
--------------------------------------------------------------------------------
Felix has an integration with views in the felix_views submodule. This module
enables you to set values for contextual filters (arguments) configured in the
views block. This configuration can be found on the "Edit attributes" page
or on the "Add block" page.

5. Hooks
--------------------------------------------------------------------------------
Felix is extendible via hooks. Hook documentation is available in felix.api.php.
