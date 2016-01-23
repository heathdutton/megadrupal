<?php
/**
 * @file
 * Storify template.
 * Display the story as a link to the story on Storify.com.
 *
 */
?>
<p><?php print $data['link'];?></a></p>
<?php if ($data['settings']['noscript']): ?>
<noscript><?php print $data['noscript']; ?></noscript>
<?php endif; ?>
