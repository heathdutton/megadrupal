<?php
/**
 * @file
 * Project Source's implementation to display project update release XML.
 *
 * Available variables:
 * - $name: The unique name for this extension release.
 * - $version: The version identifier for this release.
 * - $tag: The tag associated with this release.
 * - $version_major: The major version of this release (e.g., for a release
 *   version 7.x-2.4, this would be "2").
 * - $version_patch: The minor version of this release (using the above example,
 *   this would be "4").
 * - $version_extra: Any extra version indicators for this release (e.g. "dev"
 *   or "alpha" or "beta",
 * - $status: The status of this release (e.g. "published").
 * - $release_link: A URL point to a page representing this release.
 * - $download_link: A URL pointing to a packaged archive for this release.
 * - $date: The UNIX timestamp representing when this release was created.
 * - $mdhash: The MD5 checksum representing the packaged archive.
 * - $filesize: The size of the packaged archive in bytes.
 * - $archive_type: The file type of the archive (e.g. "tar.gz").
 * - $terms: Any terms associated with this release.
 *
 * @see template_preprocess_project_src_xml()
 * @see template_preprocess_project_src_release()
 * @see template_preprocess_project_src_term()
 */
?>
<release>
  <name><?php echo $name; ?></name>
  <version><?php echo $version; ?></version>
  <tag><?php echo $tag; ?></tag>
  <version_major><?php echo $version_major; ?></version_major>
  <?php if ($version_patch || $version_patch === 0): ?>
    <version_patch><?php echo $version_patch; ?></version_patch>
  <?php endif; ?>
  <?php if ($version_extra): ?>
    <version_extra><?php echo $version_extra; ?></version_extra>
  <?php endif; ?>
  <status><?php echo $status; ?></status>
  <release_link><?php echo $release_link; ?></release_link>
  <download_link><?php echo $download_link; ?></download_link>
  <date><?php echo $date; ?></date>
  <mdhash><?php echo $mdhash; ?></mdhash>
  <filesize><?php echo $filesize; ?></filesize>
  <files>
    <file>
      <url><?php echo $download_link; ?></url>
      <archive_type><?php echo $archive_type; ?></archive_type>
      <md5><?php echo $mdhash; ?></md5>
      <size><?php echo $filesize; ?></size>
      <filedate><?php echo $date; ?></filedate>
    </file>
  </files>
  <?php if ($terms): ?>
    <terms>
      <?php echo $terms; ?>
    </terms>
  <?php endif; ?>
</release>
