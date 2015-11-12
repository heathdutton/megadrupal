<?php
/**
 * @file
 * Default theme implementation for the top wpmenu.
 *
 * Available variables:
 * - attributes: HTML attributes for the wrapper.
 * - toolbar_attributes: HTML attributes to apply to the toolbar.
 * - toolbar_heading: The heading or label for the toolbar.
 * - tabs: List of tabs for the toolbar.
 *   - attributes: HTML attributes for the tab container.
 *   - link: Link or button for the menu tab.
 * - trays: Toolbar tray list, each associated with a tab. Each tray in trays
 *   contains:
 *   - attributes: HTML attributes to apply to the tray.
 *   - label: The tray's label.
 *   - links: The tray menu links.
 * - remainder: Any non-tray, non-tab elements left to be rendered.
 *
 * @see template_preprocess_wpmenu_top()
 *
 * @ingroup themeable
 */
?>
<div id="wpadminbar" class="nojq nojs">
  <div class="quicklinks" id="wp-toolbar" role="navigation" aria-label="Toolbar" tabindex="0">
    <ul id="wp-admin-bar-root-default" class="ab-top-menu">
      <li id="wp-admin-bar-menu-toggle"><a class="ab-item"  href="#">
          <span class="ab-icon"></span><span class="screen-reader-text">Menu</span></a>
      </li>
      <li id="wp-admin-bar-wp-logo" class="menupop"><a class="ab-item"  aria-haspopup="true" href="https://ndevr.io/"><img src="<?php print $base_path . $module_path; ?>/img/ndevr.svg" height="32" /><span class="screen-reader-text">About Ndevr</span></a>
        <div class="ab-sub-wrapper">
          <ul id="wp-admin-bar-wp-logo-default" class="ab-submenu">
            <li id="wp-admin-bar-about"><a class="ab-item"  href="https://ndevr.io/">Ndevr</a></li>
          </ul>
          <ul id="wp-admin-bar-wp-logo-external" class="ab-sub-secondary ab-submenu">
            <li id="wp-admin-bar-wporg"><a class="ab-item"  href="https://ndevr.io/what-we-do/">About Ndevr</a></li>
            <li id="wp-admin-bar-documentation"><a class="ab-item"  href="https://github.com/ndevrinc/wpmenu">Documentation</a></li>
            <li id="wp-admin-bar-feedback"><a class="ab-item"  href="https://github.com/ndevrinc/wpmenu">Feedback</a></li>
          </ul>
        </div>
      </li>
      <li id="wp-admin-bar-site-name" class="menupop">
        <a class="ab-item"  aria-haspopup="true" href="<?php print $base_path; ?>"><?php print $site_name; ?></a>
        <div class="ab-sub-wrapper">
          <ul id="wp-admin-bar-site-name-default" class="ab-submenu">
            <?php if ($is_admin): ?>
              <li id="wp-admin-bar-view-site"><a class="ab-item"  href="<?php print $base_path; ?>">Visit Site</a></li>
            <?php else: ?>
            <li id="wp-admin-bar-dashboard" class=""><a class="ab-item" href="<?php print url('admin/index'); ?>">Dashboard</a> </li></ul><ul id="wp-admin-bar-appearance" class="ab-submenu">
            <li id="wp-admin-bar-themes" class=""><a class="ab-item" href="<?php print url('admin/appearance'); ?>">Themes</a> </li>
            <li id="wp-admin-bar-widgets" class=""><a class="ab-item" href="<?php print url('admin/structure/block'); ?>">Blocks</a> </li>
            <?php endif; ?>
          </ul>
        </div>
      </li>
      <li id="wp-admin-bar-new-content" class="menupop">
        <a class="ab-item"  aria-haspopup="true" href="<?php print url('admin/content/add'); ?>"><span class="ab-icon"></span><span class="ab-label">New</span></a>
        <div class="ab-sub-wrapper">
          <ul id="wp-admin-bar-new-content-default" class="ab-submenu">
            <?php foreach ($content_types as $content_type): ?>
              <li><a class="ab-item" href="<?php print url('node/add/' . $content_type); ?>"><?php print $content_type; ?></a></li>
            <?php endforeach ?>
            <li id="wp-admin-bar-new-user"><a class="ab-item"  href="<?php print url('admin/people/create'); ?>">user</a>		</li>
          </ul>
        </div>
      </li>
    </ul>
    <ul id="wp-admin-bar-top-secondary" class="ab-top-secondary ab-top-menu">

      <li id="wp-admin-bar-search" class="admin-bar-search       ">
        <div class="ab-item ab-empty-item" tabindex="-1">
          <?php print $form; ?>
        </div>
      </li>

      <li id="wp-admin-bar-my-account" class="menupop with-avatar">
        <a class="ab-item"  aria-haspopup="true" href="<?php print url('user/' . $uid); ?>">Howdy, <?php print $username; ?><img alt='' src='<?php print $gravatar; ?>?s=26&#038;d=mm&#038;r=g' srcset='<?php print $gravatar; ?>?s=52&amp;d=mm&amp;r=g 2x' class='avatar avatar-26 photo' height='26' width='26' /></a>
        <div class="ab-sub-wrapper">
          <ul id="wp-admin-bar-user-actions" class="ab-submenu">
            <li id="wp-admin-bar-user-info"><a class="ab-item" tabindex="-1" href="<?php print url('user/' . $uid . '/edit'); ?>"><img alt='' src='<?php print $gravatar; ?>?s=64&#038;d=mm&#038;r=g' srcset='<?php print $gravatar; ?>?s=128&amp;d=mm&amp;r=g 2x' class='avatar avatar-64 photo' height='64' width='64' /><span class='display-name'><?php print $username; ?></span><span class='username'><?php print $username; ?></span></a></li>
            <li id="wp-admin-bar-edit-profile"><a class="ab-item"  href="<?php print url('user/' . $uid . '/edit'); ?>">Edit My Profile</a></li>
            <li id="wp-admin-bar-logout"><a class="ab-item"  href="<?php print url('user/logout'); ?>">Log Out</a></li>
          </ul>
        </div>
      </li>
    </ul>
  </div>
  <a class="screen-reader-shortcut" href="<?php print url('user/logout'); ?>">Log Out</a>
</div>
