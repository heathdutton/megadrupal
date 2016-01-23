<?php
/**
 * @file noie6-widget.tpl.php
 * Main widget template
 *
 * Variables available:
 * - $overlay: Boolean to determine if an overlay is applyed or not.
 * - $body: The main content.
 * - $browsers: Array with all optional browsers offered to the user.
 *     ['id']: The id attribute used for the item list
 *     ['name']: The browser name and text to be enclosed with the anchor tag
 *     ['url']: The download URL for the browser
 *     ['attributes']: An associative array of HTML attributes to apply to the anchor tag
 */
?>

<?php if ($overlay) { ?>
<div id="noie6-overlay"></div>
<?php } ?>

<div id="noie6-wrap">
  <div id="noie6-wrap-inner">
    <?php // Close button ?>
    <a href="#" id="noie6-close" title="close"><?php print t('close'); ?></a>
    
    <?php // Message ?>
    <div id="noie6-message">
      <div id="noie6-message-inner">
        <?php print $body; ?>
      </div>
    </div>
    
    <?php // Browsers list ?>
    <?php if (isset($browsers) && sizeof($browsers) > 0) : ?>
      <div id="noie6-browsers">
        <ul>
          <?php $last = sizeof($browsers) - 1; ?>
          <?php foreach ($browsers as $key => $browser) : ?>
            <?php // mark first and last
              $edge = ($key == 0) ? 'first' : ($key == $last) ? 'last' : '';
            ?>
            <li id="<?php print $browser["id"]; ?>" class="<?php print $edge; ?>">
              <?php print l($browser["name"], $browser["url"], array('attributes' => $browser["attributes"])); ?>
            </li>
          <?php endforeach; ?>
        </ul>
      </div>
    <?php endif; ?>
  </div>
</div>'


