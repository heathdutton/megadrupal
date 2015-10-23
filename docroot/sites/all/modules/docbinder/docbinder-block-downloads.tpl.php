<?php
/**
 * @file
 * Template to display DocBinder Downloads block content.
 *
 * $count - number of files in binder currently
 * $files - the files
 */
 ?>
<div class="docbinder-block-downloads">
  <?php print l(t('View docbinder'), 'docbinder', array('html' => TRUE)); ?><br>
  <?php print l(t('Download docbinder (<span class="docbinder-download-count">!count</span> files)', array('!count' => $count)), 'docbinder/get', array('html' => TRUE, 'query' => _docbinder_get_destination_alias())); ?>
</div>
