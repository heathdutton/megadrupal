<?php

/**
 * @file
 * Default simple view template to all the fields as a row.
 *
 * - $view: The view in use.
 * - $fields: an array of $field objects. Each one contains:
 *   - $field->content: The output of the field.
 *   - $field->raw: The raw data for the field, if it exists. This is NOT output safe.
 *   - $field->class: The safe class id to use.
 *   - $field->handler: The Views field handler object controlling this field. Do not use
 *     var_export to dump this object, as it can't handle the recursion.
 *   - $field->inline: Whether or not the field should be inline.
 *   - $field->inline_html: either div or span based on the above flag.
 *   - $field->wrapper_prefix: A complete wrapper containing the inline_html to use.
 *   - $field->wrapper_suffix: The closing tag for the wrapper.
 *   - $field->separator: an optional separator that may appear before a field.
 *   - $field->label: The wrap label text to use.
 *   - $field->label_html: The full HTML of the label to use including
 *     configured element type.
 * - $row: The raw result object from the query, with all data it fetched.
 *
 * @ingroup views_templates
 */
?>
<div
  class='oa-list oa-discussion-topic well clearfix'>
  <div class='oa-pull-left'>
  <?php print $field_user_picture; ?>
  </div>
  <div class='oa-list-header oa-description oa-list-category oa-pull-right'>
  <?php print $cid_1 . " " . format_plural($cid_1, t('reply'), t('replies')); ?>
    <?php if (!empty($timestamp_new_comments)): print $timestamp_new_comments . t(' new'); endif; ?>
    <?php if (!empty($timestamp_updated_comments)): print $timestamp_updated_comments . t(' updated'); endif; ?>
    <?php
    $last_post = (!empty($nid_3)) ? $nid_3 : $nid;
    print t(" Last update: ") . l($last_date, 'node/' . $last_post);
    ?>
      <br \><?php print $og_group_ref; ?> <?php print $oa_section_ref; ?>
  </div>
  <div class='oa-list-header'>
    <?php print $name . t(' posted '); ?>
    <h5>
      <?php print $title; ?>
    </h5>
    <?php
    if (!empty($row->history_timestamp_new_comments)) {
      print theme('mark', array('type' => MARK_NEW));
    }
    elseif (!empty($timestamp_updated_comments) || !empty($timestamp_updated_comments)) {
      print theme('mark', array('type' => MARK_UPDATED));
    }
    ?>
  </div>
</div>
