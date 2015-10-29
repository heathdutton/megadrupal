<?php
// $Id: page.tpl.php,v 1.15 2010/11/28 21:38:23 shannonlucas Exp $
/**
 * @file
 * The variable-layout page structure for Nitobe.
 *
 * In addition to the standard variables Drupal makes available to page.tpl.php,
 * these variables are made available by the theme:
 * - $nitobe_footers_empty: TRUE if all of the regions in the bottom block area
 *   are empty.
 * - $nitobe_content_width: The CSS class providing the full width of the
 *   content region without any push/pull classes.
 * - $nitobe_node_timestamp The node timestamp, if applicable.
 * - $nitobe_page_title: The pre rendered page title element with the
 *   appropriate CSS classes assigned.
 * - $nitobe_tabs_primary: The array of primary tabs for this page.
 * - $nitobe_tabs_secondary: The array of secondary tabs for this page.
 */
?>
<div class="page-wrapper clearfix">
  <div class="container-16 header-area">
    <?php echo render($page["title_group"]); ?>
    <?php if ($page["header"]) echo render($page["header"]); ?>
    <hr class="break"/>
    <?php echo render($page["menu_bar"]); ?>
    <?php echo render($page["masthead"]); ?>
  </div>
  <hr class="break" />
  <?php if (isset($page['help'])) echo render($page['help']); ?>
  <div id="content-area" class="container-16 content-area">
    <div class="nitobe-main-column <?php echo $nitobe_content_width; ?>">
      <?php if (!empty($breadcrumb)) { echo $breadcrumb; } ?>
      <a id="main-content"></a>
  	  <div class="main-content">
        <?php echo render($title_prefix); ?>
        <?php if ($title): ?>
          <div class="page-headline clearfix">
            <?php echo $nitobe_page_title; ?>
            <?php if (isset($nitobe_node_timestamp)): ?>
              <span class="timestamp"><?php echo $nitobe_node_timestamp; ?></span>
            <?php endif; ?>
          </div>
  		<?php endif; ?>
        <?php echo render($title_suffix); ?>
        <?php if ($tabs || $tabs2): ?>
          <div class="tabs-wrapper <?php echo $nitobe_content_width; ?> alpha omega clearfix">
            <?php if ($nitobe_tabs_primary): ?>
              <h2 class="element-invisible"><?php echo t("Primary tabs"); ?></h2>
              <ul class="tabs primary clearfix<?php if ($nitobe_tabs_secondary) { echo ' has-secondary'; } ?>">
                <?php echo render($nitobe_tabs_primary); ?>
              </ul>
            <?php endif; ?>
            <?php if ($nitobe_tabs_secondary): ?>
              <h2 class="element-invisible"><?php echo t("Secondary tabs"); ?></h2>
              <ul class="tabs secondary"><?php echo render($nitobe_tabs_secondary) ?></ul>
            <?php endif; ?>
          </div>
          <br class="break"/>
        <?php endif; ?>
        <?php if ($show_messages && !empty($messages)): ?>
          <div id="drupal-status-messages">
            <?php echo $messages; ?>
          </div>
        <?php endif; ?>
        <?php if ($page['highlighted']) echo render($page['highlighted']); ?>
        <?php if ($action_links): ?>
          <ul class="action-links">
            <?php echo render($action_links); ?>
          </ul>
        <?php endif; ?>
  		<?php echo render($page["content"]); ?>
  	  </div>
    </div>
  	<?php if ($page["sidebar_first"]) echo render($page["sidebar_first"]); ?>
    <?php if ($page["sidebar_second"]) echo render($page["sidebar_second"]); ?>
  </div>
  <hr class="break"/>
  <div class="footer-columns container-16">
    <?php if (!$nitobe_footers_empty): ?>
      <hr class="rule-bottom grid-16"/>
    <?php endif; ?>
    <?php echo render($page['footer_firstcolumn']); ?>
    <?php echo render($page['footer_secondcolumn']); ?>
    <?php echo render($page['footer_thirdcolumn']); ?>
    <?php echo render($page['footer_fourthcolumn']); ?>
  </div>
  <hr class="break"/>
  <?php echo render($page["footer"]); ?>
</div>
