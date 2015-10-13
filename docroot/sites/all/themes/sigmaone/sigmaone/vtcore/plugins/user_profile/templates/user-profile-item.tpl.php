<?php
// $Id: user-profile-item.tpl.php,v 1.3 2008/10/13 12:31:43 dries Exp $

/**
 * @file
 * Default theme implementation to present profile items (values from user
 * account profile fields or modules).
 *
 * This template is used to loop through and render each field configured
 * for the user's account. It can also be the data from modules. The output is
 * grouped by categories.
 *
 * @see user-profile-category.tpl.php
 *      for the parent markup. Implemented as a definition list by default.
 * @see user-profile.tpl.php
 *      where all items and categories are collected and printed out.
 *
 * Available variables:
 * - $title: Field title for the profile item.
 * - $value: User defined value for the profile item or data from a module.
 * - $attributes: HTML attributes. Usually renders classes.
 *
 * @see template_preprocess_user_profile_item()
 */
?>
<div<?php if (isset($data['#item_attributes'])) : ?><?php print drupal_attributes($data['#item_attributes']); ?><?php endif;?>>
  <h4<?php if (isset($data['#title_attributes'])) : ?><?php print drupal_attributes($data['#title_attributes']); ?><?php endif;?>>
    <?php print $title; ?>
  </h4>
  <p<?php if (isset($data['#content_attributes'])) : ?><?php print drupal_attributes($data['#content_attributes']); ?><?php endif;?>>
    <?php print $value; ?>
  </p>
</div>