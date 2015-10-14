<?php
/**
 * @file page--external.tpl.php
 * Starter template for external iframe
 *
 * - $url: external link url
 */
 ?>
<div id="external">
<?php if ($messages): ?>
  <?php print render($messages); ?>
<?php endif; ?>
<div id="external-container">
  <div class="hide-toolbar">
    <a href="<?php print $url;?>">CLOSE &nbsp;&nbsp;X</a></span>
  </div>
  <div class="logo" id="community-nav">
    <a href="<?php print $front_page; ?>" title="Return home"><img src="<?php print $logo; ?>"></a>
  </div>
  <?php if (isset($external_iframe_share_button)): ?>
    <div id="external-iframe-share-button"><?php print $external_iframe_share_button; ?></div>
    <!-- @todo figure out how to add script ... doesn't work using drupal_add_js -->
    <script type="text/javascript" src="http://static.addtoany.com/menu/page.js"></script>
  <?php endif; ?>
</div>
<div id="external-site-container" height="100%">
  <iframe id="external-site" src="<?php print $url; ?>" scrolling="auto" frameBorder="0" height="100%" />
    <h3>Your Browser Does Not Support Iframes. <a href="<?php print $url; ?>" title="<?php print $url; ?>">Click here to view the page you selected</a></h3>
  </iframe>
</div>
</div>
