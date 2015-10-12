Giveaways
=========

## Purpose
To manage and distribute Game keys to users while ensuring fair distribution.
  
## Implementation
This module exposes a content type `Key Giveaway` which allows the user to specify
  various settings related to the particular giveaway. The actual key import is 
  managed by feeds.module.
  
  The keys are imported into a custom db table that is exposed as an entity to drupal
  by schema.module & data.module
  
  The module exposed a few blocks and ctools access plugins to help admins setup the key
  giveaway page.

* Giveaway Claim Block - The claim button users press to get their key.
* Giveaway Keys Left Block - A block to display the number of keys left.
* Giveaway User Claimed Key Block - after claiming, the details of the key for later reference.
* Giveaway: keys are available - plugin to control access depending on whether keys are available.
* Giveaway: user has claimed this giveaway - plugin to control access depending on whether a user 
has already claimed a key.