<?php
/**
 * @file
 * Project Source's implementation to display project update XML.
 *
 * Available variables:
 * - $title: The human-readable name for this Drupal extension.
 * - $short_name: The shortname for this Drupal extension.
 * - $creator: The original author of this Drupal extension.
 * - $api_version: The Drupal API version with which this extension is
 *   compatible (e.g. 7.x).
 * - $recommended_major: The recommended major release of this Drupal extension.
 * - $supported_majors: A comma seperated list of supported major release
 *   versions for this Drupal extension.
 * - $default_major: The default major version of this Drupal extension.
 * - $project_status: The status of this Drupal extension (e.g. "published").
 * - $terms: Any terms associated with this extension as a whole.
 * - $releases: Any releases available for this extension and version.
 *
 * @see template_preprocess_project_src_xml()
 * @see template_preprocess_project_src_release()
 * @see template_preprocess_project_src_term()
 */
?>
<?php print '<?xml version="1.0" encoding="utf-8"?>'; ?>
<project xmlns:dc="http://purl.org/dc/elements/1.1/">
  <title><?php echo $title; ?></title>
  <short_name><?php echo $short_name; ?></short_name>
  <dc:creator><?php echo $creator; ?></dc:creator>
  <type><?php echo $type; ?></type>
  <api_version><?php echo $api_version; ?></api_version>
  <recommended_major><?php echo $recommended_major; ?></recommended_major>
  <supported_majors><?php echo $supported_majors; ?></supported_majors>
  <default_major><?php echo $default_major; ?></default_major>
  <project_status><?php echo $project_status; ?></project_status>
  <link><?php echo $link; ?></link>
  <?php if ($terms): ?>
    <terms>
      <?php echo $terms; ?>
    </terms>
  <?php endif; ?>
  <releases>
    <?php echo $releases; ?>
  </releases>
</project>
