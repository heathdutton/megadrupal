<?php

/**
 * @file
 * Default view template to display a item in an RSS feed.
 *
 * @ingroup views_templates
 */
?>
    <item>
      <title><?php print $title; ?></title>
      <link><?php print $link; ?></link>
      <?php print $item_elements; ?>
      <description><?php print $description; ?></description>
      <content:encoded><?php print $content_encoded; ?></content:encoded>
    </item>
