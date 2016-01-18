<?php

/**
 * @file
 * Template for single variance
 *
 * Available variables:
 * - $scopes: Scopes of variant, used for language.
 * - $data: String PCDATA.
 * - $dataref: An url to variant resource
 */
?>
<variant>
  <scope>
    <?php foreach ($scopes as $key => $id): ?>
      <topicRef href="#<?php echo $id; ?>" />
    <?php endforeach; ?>
  </scope>
  <?php if (!empty($data)): ?>
    <resourceData><?php echo $data; ?></resourceData>
  <?php endif; ?>
  <?php if (!empty($dataref)): ?>
    <resourceRef  href="<?php echo $dataref; ?>" />
  <?php endif; ?>
</variant>
