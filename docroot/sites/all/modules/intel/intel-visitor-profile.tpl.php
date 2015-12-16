<?php

/**
 * @file
 * Default theme implementation to present all user profile data.
 *
 * This template is used when viewing a registered member's profile page,
 * e.g., example.com/user/123. 123 being the users ID.
 *
 * Use render($user_profile) to print all profile items, or print a subset
 * such as render($user_profile['user_picture']). Always call
 * render($user_profile) at the end in order to print all remaining items. If
 * the item is a category, it will contain all its profile items. By default,
 * $user_profile['summary'] is provided, which contains data on the user's
 * history. Other data can be included by modules. $user_profile['user_picture']
 * is available for showing the account picture.
 *
 * Available variables:
 *   - $user_profile: An array of profile items. Use render() to print them.
 *   - Field variables: for each field instance attached to the user a
 *     corresponding variable is defined; e.g., $account->field_example has a
 *     variable $field_example defined. When needing to access a field's raw
 *     values, developers/themers are strongly encouraged to use these
 *     variables. Otherwise they will have to explicitly specify the desired
 *     field language, e.g. $account->field_example['en'], thus overriding any
 *     language negotiation rule that was previously applied.
 *
 * @see user-profile-category.tpl.php
 *   Where the html is handled for the group.
 * @see user-profile-item.tpl.php
 *   Where the html is handled for each item in the group.
 * @see template_preprocess_user_profile()
 *
 * @ingroup themeable
 */
?>
<div id="intel-profile" class="profile"<?php print $attributes; ?>>
    <?php if (isset($header)): ?>
      <div id="header" class="header clearfix">
        <div id="header-sidebar" class="sidebar">
        <?php if ($picture): ?>
          <div id="picture" class="picture">
            <?php print render($picture) ?>
          </div>
        <?php endif; ?>
        </div>
        <div id="header-main" class="main">
          <?php if ($title): ?>
            <h1 class="title">
              <?php print render($title) ?>
            </h1>
          <?php endif; ?>
          <?php if ($subtitle): ?>
            <h2 class="subtitle">
              <?php print render($subtitle) ?>
            </h2>
          <?php endif; ?>
          <?php if ($header_content): ?>
            <div class="content">
              <?php print render($header_content) ?>
            </div>
          <?php endif; ?>
        </div>
      </div>
    <?php endif; ?>
   
    <?php if ($summary): ?>
      <div id="summary" class="summary clearfix">
        <?php print render($summary); ?>
      </div>
    <?php endif; ?> 
    
    <div id="profile-body" class="profile-body clearfix">
      <?php if ($sidebar): ?>
        <div id="sidebar" class="sidebar">
          <?php print render($sidebar); ?>
        </div>
      <?php endif; ?>
  
      <div id="main" class="clearfix">
        <?php print render($main); ?>
      </div>
    </div>
</div>