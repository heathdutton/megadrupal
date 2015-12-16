<?php
/**
 * @file
 * Default theme implementation for the administrative toolbar.
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
 * @see template_preprocess_toolbar()
 *
 * @ingroup themeable
 */
?>
<script type="text/javascript">
  document.body.className = document.body.className.replace('no-js','js');
</script>

<div id="adminmenumain" role="navigation" aria-label="Main menu">
  <a href="#wpbody-content" class="screen-reader-shortcut">Skip to main content</a>
  <a href="#wp-toolbar" class="screen-reader-shortcut">Skip to toolbar</a>
  <div id="adminmenuback"></div>
  <div id="adminmenuwrap">
    <ul id="adminmenu">
      <?php $count = 0; ?>
      <?php foreach ($menu as $link): ?>
        <?php $count++; ?>
        <?php if ($link['separator']): ?>
          <li class="wp-not-current-submenu wp-menu-separator" aria-hidden="true"><div class="separator"></div></li>
        <?php endif; ?>
        <?php if (strstr($current_path, $link['url']) && $count > 1): ?>
          <?php $current_class = 'wp-has-current-submenu wp-menu-open'; ?>
        <?php elseif ($base_path . $link['url'] == $current_path || $base_path . $link['url'] . '/index' == $current_path): ?>
          <?php $current_class = 'wp-has-current-submenu wp-menu-open'; ?>
        <?php else: ?>
          <?php $current_class = 'wp-not-current-submenu'; ?>
        <?php endif; ?>

        <li class="wp-has-submenu <?php print $current_class; ?> menu-top menu-icon-<?php print $link['icon']; ?> menu-top-first" id="menu-<?php print $link['icon']; ?>">
          <a href="<?php print url($link['url']); ?>" class="wp-has-submenu <?php print $current_class; ?> menu-top menu-icon-appearance menu-top-first" aria-haspopup="true">
            <div class="wp-menu-arrow"><div></div></div>
            <div class="wp-menu-image dashicons-before dashicons-<?php print $link['icon']; ?>"><br /></div>
            <div class="wp-menu-name"><?php print $link['text']; ?></div>
          </a>
          <?php if (count($link['children']) > 0): ?>
            <ul class="wp-submenu wp-submenu-wrap">
              <li class="wp-submenu-head"><?php print $link['text']; ?></li>
              <?php foreach ($link['children'] as $sublink): ?>
                <li><a href="<?php print url($sublink['url']); ?>"><?php print $sublink['text']; ?></a></li>
              <?php endforeach ?>
            </ul>
          <?php endif; ?>
        </li>
      <?php endforeach; ?>


      <li id="collapse-menu" class="hide-if-no-js"><div id="collapse-button"><div></div></div><span>Collapse menu</span></li>

    </ul>
  </div>
</div>
