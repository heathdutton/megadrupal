<?php
/**
 * @file brb-widget.tpl.php
 * Main widget template
 *
 * Variables available:
 * - $body: The main content.
 * - $browsers: Array with all optional browsers offered to the user.
 *     ['id']: The id attribute used for the item list
 *     ['name']: The browser name and text to be enclosed with the anchor tag
 *     ['url']: The download URL for the browser
 *     ['attributes']: An associative array of HTML attributes to apply to the anchor tag
 */
?>

<div id="brb-wrap">
  <div id="brb-wrap-inner">
    <?php // Message ?>
    <div id="brb-message">
      <div id="brb-message-inner">
        <?php print $body; ?>
      </div>
    </div>
    
    <?php // Browsers list ?>
    <?php if (isset($browsers) && sizeof($browsers) > 0) : ?>
      <div id="brb-browsers">
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


