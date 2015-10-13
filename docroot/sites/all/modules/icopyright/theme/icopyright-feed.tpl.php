<?php
/**
 * @file
 * Theme file that renders the components of a node (title, body, created) in a format for iCopyright to read
 */

/**
 * Displays the node as an iCopyright feed. This is parsed by the iCopyright server to
 * register the content and make it available for use by licensees. (Note that the iCopyright
 * servers sanitize the content that you send.)
 *
 * This default implementation makes some assumptions about how your content is structured. It assumes
 * that the author of the piece is the person who posted it, that its publication date is
 * the date the node was created, and so on. To make changes to what you send, override this theme,
 * implement template_preprocess_icopyright_feed, or both.
 *
 * Valid fields are as follows. You should include at least the headline, copyright notice, and body.
 *
 * icx_authors                 A human-readable name of the author or authors of the piece
 * icx_byline                  A byline, like "Staff Writer" or "New York, NY"
 * icx_copyright               Your copyright notice for the piece
 * icx_deckheader              A deckheader -- like a teaser or sidebar that describes the article
 * icx_headline                The headline of the piece
 * icx_pubdate                 Publication date (Month Day, Year is best)
 * icx_section                 The section of the article (News, Rumors, Finance, Forum, etc.)
 * icx_story                   The body of the story
 * icx_url                     The canonical URL of the story
 *
 * You can turn on and off categories of service on this particular node by adding these to the feed:
 *
 * icx_disallow_search         Do not use this story in web clipping or search services
 * icx_disallow_free           Do not allow free print, email, or post services
 * icx_disallow_instant        Do not permit instant paid licensing services
 * icx_disallow_custom         Do not allow custom reprints or posts
 * icx_disallow_syndication    Do not allow other publishers to syndicate this story from you
 *
 * If you have questions contact iCopyright support at drupal@icopyright.com
 */
?>
<icx>
  <icx_authors><?php print $icx_authors ?></icx_authors>
  <icx_copyright><?php print $icx_copyright ?></icx_copyright>
  <icx_headline><?php print $icx_headline ?></icx_headline>
  <icx_pubdate><?php print $icx_pubdate ?></icx_pubdate>
  <icx_story><?php print $icx_story ?></icx_story>
  <icx_url><?php print $icx_url ?></icx_url>
</icx>
