<?php
/**
 * @file views-view-atom.tpl.php
 * Default template for feed displays that use the ATOM style.
 *
 * @ingroup views_templates
 */
?>
<?php print "<?xml"; ?> version="1.0" encoding="utf-8" <?php print "?>"; ?>
<feed<?php print $namespaces; ?>>
  <title><?php print $title; ?></title>
  <id><?php print $id; ?></id>
  <subtitle><?php print $subtitle; ?></subtitle>
  <link href="<?php print $link; ?>" rel="self" />
  <updated><?php print $updated; ?></updated>
  <author>
    <name><?php print $author_name; ?></name>
    <uri><?php print $author_uri; ?></uri>
  </author>
  <?php if ($license_uri) : ?>
    <link rel="license" type="application/rdf+xml" href="<?php print $license_uri; ?>" />
  <?php endif; ?>
  <generator uri="http://drupal.org">Drupal</generator>
  <?php print $channel_elements; ?>
  <?php print $items; ?>
</feed>
