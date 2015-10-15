<?php

/**
 * @file
 * Template for single occurence
 *
 * Available variables:
 * - $type: Occurance type
 * - $data: String PCDATA.
 * - $dataref: An url to occurrences resource
 * - $scope: Scope of occurence, usually used for language. Optional
 */
?>
<occurrence>
  <type>
    <topicRef href="#<?php echo $type ?>" />
  </type>
  <?php if (!empty($data)): ?>
    <resourceData><?php echo $data; ?></resourceData>
  <?php endif; ?>
  <?php if (!empty($dataref)): ?>
    <resourceRef  href="<?php echo $dataref; ?>" />
  <?php endif; ?>
  <?php if (!empty($scope)): ?>
  <scope>
    <topicRef href="#<?php echo $scope; ?>" />
  </scope>
  <?php endif; ?>
</occurrence>
