<?php
/**
 * @file views-view.tpl.php
 * Main view template
 *
 * Variables available:
 * - $classes_array: An array of classes determined in
 *   template_preprocess_views_view(). Default classes are:
 *     .view
 *     .view-[css_name]
 *     .view-id-[view_name]
 *     .view-display-id-[display_name]
 *     .view-dom-id-[dom_id]
 * - $classes: A string version of $classes_array for use in the class attribute
 * - $css_name: A css-safe version of the view name.
 * - $css_class: The user-specified classes names, if any
 * - $header: The view header
 * - $footer: The view footer
 * - $rows: The results of the view query, if any
 * - $empty: The empty text to display if the view is empty
 * - $pager: The pager next/prev links to display, if any
 * - $exposed: Exposed widget form/info to display
 * - $feed_icon: Feed icon to display, if any
 * - $more: A link to view more, if any
 * - $admin_links: A rendered list of administrative links
 * - $admin_links_raw: A list of administrative links suitable for theme('links')
 * - $forum_description: Forum description
 *
 * - $forum_tools: Drop down list of forum tools
 * - $forum_jump: Drop down list of forum hierarchy
 *
 * @ingroup views_templates
 */
?>

<div class="<?php print $classes; ?>">
  <?php if (!empty($admin_links)): ?>
    <div class="views-admin-links views-hide">
      <?php print $admin_links; ?>
    </div>
  <?php endif; ?>

  <?php if ($header) print $header ?>
  <?php if ($exposed) print $exposed ?>
  <?php if ($attachment_before) print $attachment_before ?>

  <div class='row'>
    <div class='col-sm-6'>
      <?php if ($node_create_list) print $node_create_list ?>
    </div>

    <div class='col-sm-6'>
      <?php if ($pager): ?>
        <div class="pull-right"><?php print $pager; ?></div>
      <?php endif; ?>
    </div>
  </div>

  <?php if ($rows): ?>
    <div class="view-content">
      <?php print $rows; ?>
    </div>
  <?php elseif ($empty): ?>
    <div class="view-empty">
      <?php print $empty; ?>
    </div>
  <?php endif; ?>

  <div class='row'>
    <div class='col-sm-6'>
      <div class="forum-node-create-links"><?php print $node_create_list ?></div>
    </div>

    <div class='col-sm-6'>
      <?php if ($pager): ?>
        <div class="pull-right"><?php print $pager; ?></div>
      <?php endif; ?>
    </div>
  </div>

  <div class='row'>
    <div class='col-sm-6'>
      <?php if (!empty($view->sort_form)) print advanced_forum_forum_topic_list_sort() ?>
    </div>

    <div class='col-sm-6'>
      <?php if (!empty($forum_tools)) print $forum_tools ?>
    </div>
  </div>

  <?php if ($attachment_after) print $attachment_after ?>
  <?php if ($more) print $more ?>
  <?php if ($footer) print $footer ?>
  <?php if ($feed_icon) print $feed_icon ?>
</div>
