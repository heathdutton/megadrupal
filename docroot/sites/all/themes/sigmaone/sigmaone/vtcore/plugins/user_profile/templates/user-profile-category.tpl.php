<?php
// $Id: user-profile-category.tpl.php,v 1.3 2008/10/13 12:31:43 dries Exp $

/**
 * @file
 * Default theme implementation to present profile categories (groups of
 * profile items).
 *
 * Categories are defined when configuring user profile fields for the site.
 * It can also be defined by modules. All profile items for a category will be
 * output through the $profile_items variable.
 *
 * @see user-profile-item.tpl.php
 *      where each profile item is rendered. It is implemented as a definition
 *      list by default.
 * @see user-profile.tpl.php
 *      where all items and categories are collected and printed out.
 *
 * Available variables:
 * - $title: Category title for the group of items.
 * - $profile_items: All the items for the group rendered through
 *   user-profile-item.tpl.php.
 * - $attributes: HTML attributes. Usually renders classes.
 *
 * @see template_preprocess_user_profile_category()
 */
?>
<div<?php if (isset($data['#category_attributes'])) : ?><?php print drupal_attributes($data['#category_attributes']); ?><?php endif; ?>>
  <?php if ($title) : ?>
    <h3<?php if (isset($data['#title_attributes'])) : ?><?php print drupal_attributes($data['#title_attributes']); ?><?php endif; ?>>
      <?php print $title; ?>
    </h3>
  <?php endif; ?>

  <div<?php if (isset($data['#content_attributes'])) : ?><?php print drupal_attributes($data['#content_attributes']); ?><?php endif; ?>">
    <?php print $profile_items; ?>
  </div>
</div>
