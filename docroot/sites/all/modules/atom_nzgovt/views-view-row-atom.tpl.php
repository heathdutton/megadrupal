<?php
/**
 * @file views-view-row-rss.tpl.php
 * Default view template to display a item in an ATOM feed.
 *
 * @ingroup views_templates
 */
?>
  <entry>
    <title><?php print $title; ?></title>
    <link rel="alternate" type="text/html" href="<?php print $link; ?>" />
    <content type="xhtml"><div xmlns="http://www.w3.org/1999/xhtml"><?php print $content; ?></div></content>
    <?php foreach($attachments as $index => $attachment) : ?>
      <link rel="enclosure" title="<?php print _atom_nzgovt_html_to_xml_entities(check_plain($attachment['description'])); ?>" type="<?php print $attachment['filemime']; ?>" href="<?php print file_create_url($attachment['uri']); ?>" length="<?php print $attachment['filesize']; ?>" />
    <?php endforeach; ?>
    <?php foreach($category as $index => $cat) : ?>
      <category term="<?php print _atom_nzgovt_html_to_xml_entities(check_plain($cat)); ?>" scheme="http://www.e.govt.nz/standards/nz/2009-03-01#information-type" />
    <?php endforeach; ?>
    <?php foreach($terms as $index => $term) : ?>
      <category term="<?php print _atom_nzgovt_html_to_xml_entities(check_plain($term)); ?>" />
    <?php endforeach; ?>
    <?php print $item_elements; ?>
  </entry>
