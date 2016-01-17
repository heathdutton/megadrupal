<?php
/**
 * @file
 * Storify template.
 * Display the story as you would see it on Storify.com.
 *
 */
?>
<p><?php print $data['metadata']['description'];?></p>
<script src='<?php print $data['embed_url']; ?>' type='text/javascript' language="JavaScript"></script>
<?php if ($data['settings']['noscript']): ?>
<noscript><?php print $data['noscript']; ?></noscript>
<?php endif;?>
