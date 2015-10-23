<?php
/**
 * @file
 * Template to render the export HTML string.
 */

/**
 * Template variables.
 *
 * @var $job_id
 *   The Job ID.
 * @var $job_item_id
 *   The JobItem ID.
 * @var $source_language
 *   The translation source language.
 * @var $target_language
 *   The translation target language.
 * @var $data
 *   The data that needs to be translated.
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xml:lang="en" lang="en" xmlns="http://www.w3.org/1999/xhtml">

  <head>
    <meta http-equiv="content-type" content="text/html; charset=utf-8" />

    <meta name="JobID" content="<?php print $job_id; ?>" />
    <?php if (!empty($job_item_id)): ?><meta name="JobItemID" content="<?php print $job_item_id; ?>" /><?php endif; ?>
    <meta name="languageSource" content="<?php print $source_language; ?>" />
    <meta name="languageTarget" content="<?php print $target_language; ?>" />

    <title>Job ID <?php print $job_id; ?><?php if (!empty($job_item_id)): ?>:<?php print $job_item_id; ?><?php endif; ?></title>
  </head>

  <body>
    <?php foreach ($data as $item_key => $item): ?><div class="asset" id="<?php print $item_key; ?>">
      <?php foreach ($item as $field_key => $field): ?><div class="atom" id="<?php print $field_key; ?>">
        <?php print $field['#text']; ?>

      </div>
      <?php endforeach; ?>

    </div>
    <?php endforeach; ?>

  </body>

</html>
