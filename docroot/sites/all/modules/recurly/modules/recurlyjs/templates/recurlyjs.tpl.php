<?php
/**
 * @file
 * Prints the Recurly.js form elements. This template is responsible for all
 * three different form types:
 *  - recurlyjs_subscribe
 *  - recurlyjs_billing
 *  - recurlyjs_transaction
 */
?>
<div class="recurly-form-wrapper">
  <div id="<?php print $element['#id']; ?>">
    <script type="text/javascript">document.write('<?php print t('Please wait while loading...') ?>');</script>
    <noscript><?php print t('This order form requires JavaScript and Cookies to be enabled in your browser.'); ?></noscript>
  </div>
  <?php print drupal_render_children($element); ?>
</div>
