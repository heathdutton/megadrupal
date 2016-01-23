<?php
/**
 * @file
 * Template file for a page rebdered in Modal Frame based on jQuery UI dialog.
 *
 * This template provides the same exact variables provided to page.tpl.php,
 * and serves the same purpose, with the exeption that this template does not
 * render regions such as head, left and right because the main purpose of this
 * template is to render a frame that is displayed on a modal jQuery UI dialog.
 *
 * @see modalframe_theme_registry_alter()
 * @see modalframe_preprocess_page()
 * @see template_preprocess_page()
 * @see template_preprocess()
 * @see theme()
 */
?>
<div class="modalframe-page-wrapper">
  <div class="modalframe-page-container clear-block">
    <div class="modalframe-page-content">
      <?php print $messages; ?>
      <?php print render($page['help']); ?>
      <div class="clear-block">
        <?php print render($page['content']); ?>
      </div>
    </div>
  </div>
</div>
