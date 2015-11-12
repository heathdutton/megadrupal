<?php
/**
 * Default template for rendering embedded Socrata dataset views via Socrata Filter module.  Stock variables available are:
 *
 * dataset:       Socrata dataset ID
 * embed_url:     Constructed src URL for IFRAME src
 * width:         Width, in pixels
 * height:        Height, in pixels
 * source:        Socrata source settings as defined in Drupal
 *   endpoint:    Landing page for dataset
 *   description: Descriptive text defined when dataset was added to Drupal
 *   app_token:   Socrata application token
 */
?>
<div>
  <iframe width="<?php print $width; ?>px" height="<?php print $height; ?>px" src="<?php print $embed_url; ?>" frameborder="0" scrolling="no">
    <a href="<?php print $source['endpoint']; ?>" title="<?php print $source['description']; ?>" target="_blank"><?php print $source['description']; ?></a>
  </iframe>

  <p>
    <a href="http://www.socrata.com/" target="_blank">Powered by Socrata</a>
  </p>
</div>
