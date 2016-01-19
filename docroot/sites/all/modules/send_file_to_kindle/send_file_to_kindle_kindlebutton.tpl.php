<?php
/**
 * @file
 * Send_file_to_kindle's template for kindle file link.
 *
 * Available variables:
 *
 * - $fileobject: The complete file object, in case you want to build the
 *     link yourself.
 * - $linkmarkup: The HTML markup for the file link.
 * - $kindlefile. Boolean literal. True if the file type is for Kindle.
 * - $buttonform. A form array that is rendered to produce the button.
 * -   If you want to change anything about the email, modify $kindlebuttform.
 */
?>
<span class="file">
    <?php print $linkmarkup; ?>
    <?php if (($kindlefile) && ($logged_in)):?>
        <?php print render($buttonform); ?>
    <?php endif; ?>
</span>
