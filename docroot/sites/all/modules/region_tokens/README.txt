                          Drupal Module: Region Tokens

   Author: Aaron Klump [1]sourcecode@intheloftstudios.com

Summary

   The aim of this module is to provide Content Managers with a means to
   place a region (one or more blocks) anywhere within the content of a
   node (or entity).

   This is achieved by using tokens that represent block regions, which
   are inserted into the node content. These tokens are rendered into
   their respective regions and the blocks assigned to the regions thus
   appear inside the node content.

   Since we're using a token, the placement of the region can and will
   vary from node to node. This can be thought of as an "inline" region.

   The block visibility settings (Pages, Content Types, Roles, Users) work
   just as you would expect under the normal circumstances of a standard
   region.

   This module is one answer to the question, "How do I place blocks
   inside of nodes?" There are other modules that can provide this
   solution (read more below).

   You may also visit the [2]project page on Drupal.org.

Requirements

   You must have the [3]Token module enabled. While not technically
   required, for foreseen use cases you need to also install/enable
   [4]Token Filter.

Installation

    1. Install as usual, see [5]http://drupal.org/node/70151 for further
       information.
    2. Unless you deem unneccessary, make sure you have [6]Token Filter
       installed and enabled.

Configuration

    1. Visit /admin/config/content/region-tokens and define one or more
       regions to be available as tokens. These will now be visible in the
       token tree under the group Regions.
    2. Now make sure you have at least one block assigned to the region
       you just enabled.
    3. Inside the node edit form wherein you wish to place the region,
       insert the appropriate region token. (For a list of tokens visit:
       /admin/help/token and refer to the token group Regions.)
    4. After saving the node, you should see the region rendered in place
       of the token, inside the node content!
    5. If you do not see what you expected, take a look at the Recent log
       messages /admin/reports/dblog.

Suggested Use

    1. In practice, we create one or more extra regions in my theme
       devoted only to this type of thing, something like:
regions[inline] = Inline
regions[inline2] = Inline 2

    2. Be aware that if you set your visibility settings incorrectly, the
       block token will return an empty string, rather than the region
       you're expecting.
    3. In the fringe case where you find a need to place two blocks on the
       same page, in separate locations, you will need to utilize two
       separate regions, as this is the only way to create multiple
       tokens, thus achieving the multiple insertion points.

  Adding a Region to a Form

// Add the output of a region to my form
$form['region_content'] = array(
  '#markup' => token_replace('[region:my_theme:inline]'),
);

Performance Note

    1. It is critical for performance sake that you set the block
       visibility to ONLY those node pages where you will be implementing
       the tokens. For more information read [7]this post by Angie Byron.

Design Decisions/Rationale

   Having blocks that are being used via insert of a token, but which
   appear as disabled on the Block admin page was confusing to our Content
   Manager(s). This sometimes led to blocks getting deleted, which should
   not have been deleted, only to find that out later. We were looking for
   an approach that was more intuitive in terms of the core Block system.

Contact

     * In the Loft Studios
     * Aaron Klump - Developer
     * PO Box 29294 Bellingham, WA 98228-1294
     * aim: theloft101
     * skype: intheloftstudios
     * d.o: aklump
     * [8]http://www.InTheLoftStudios.com

References

   1. mailto:sourcecode@intheloftstudios.com
   2. http://www.drupal.org/project/region_tokens
   3. https://drupal.org/project/token
   4. https://drupal.org/project/token_filter
   5. http://drupal.org/node/70151
   6. https://drupal.org/project/token_filter
   7. http://www.lullabot.com/blog/article/drupal-performance-tip-block-visibility
   8. http://www.InTheLoftStudios.com/
