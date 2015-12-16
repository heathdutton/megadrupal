<?php

/**
 * @file spritz.tpl.php
 * Returns HTML for embedding a Spritz widget
 */

?>

<div id="<?php print $variables['id']; ?>" data-role="spritzer" data-selector=".spritz-text"></div>
<p class="spritz-text"><?php print $variables['content']; ?></p>