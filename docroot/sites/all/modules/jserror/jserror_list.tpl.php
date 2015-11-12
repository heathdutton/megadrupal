<?php

/**
 * @file
 * Default theme implementation to display the list of error message.
 *
 * Available variables:
 * - $errors: An array of errors.
 * - $empty: empty string to display when no errors are found.
 *
 * @see template_preprocess()
 * @see template_process()
 *
 * @ingroup themeable
 */
?>
<div id="js-error">
  <?php if(!empty($errors)):?>
  <ul id="error-list">
    <?php foreach($errors as $error):?>
    <li class="js-error">
      <div class="row-count">
        <?php echo $error['row_count'];?>
        occurrences
      </div>
      <h3>
        <?php echo l($error['message'], 'admin/reports/jserror/' . $error['mid'], array('attributes' => array('title' => $error['message'])));?>
      </h3>
      <p>
        On line
        <?php echo $error['line'] . ':' . $error['col']?>
        of <span class="url"><?php echo l(truncate_utf8($error['file'], 100, FALSE, TRUE), $error['file'], array('attributes' => array('target' => 'blank', 'title' => $error['file'])))?> </span>
      </p>
      <p class="details">
        <span class="browser"></span> Last seen on
        <?php echo $error['browser'] . " " . $error['browser_version'] ?>
        for
        <?php echo $error['platform'] ?>
        ,
        <?php echo format_interval(time() - $error['timestamp'], 1); ?>
        ago. Occurred
        <?php echo $error['row_count']; ?>
        times so far. Occurred
        <?php echo $error['pageload'] ? t("after page load") : t("before page load") ?>
        .
      </p>
    </li>
    <?php endforeach; ?>
  </ul>
  <?php else:?>
  <tr>
    <p>
      <?php echo $empty ?>
    </p>
  </tr>
  <?php endif;?>
</div>
