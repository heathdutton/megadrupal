<?php
/**
 * @file
 * Default theme implementation to wrap the page_top region.
 *
 * Available variables:
 * - $menu: The renderable array containing the menu.
 *
 * The closing div is rendered in responsive-menu-page-bottom-wrapper.tpl.php.
 *
 * @see responsive_menu_preprocess_responsive_menu_page_top_wrapper()
 */
?>
<?php print render($menu) ?>
<?php print $content; ?>
<div class="l-responsive-page-container">