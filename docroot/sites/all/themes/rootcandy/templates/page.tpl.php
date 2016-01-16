<!-- Layout -->
<?php if (!$hide_panel) { ?>
<div id="top-panel">
  <div id="panel" class="clearfix">
    <div class="panel-opacity"></div>
  </div>
  <div id="top-panel-head">
    <div id="go-home">
      <a href="<?php print base_path() ?>" title="<?php print t('Go Back to Homepage') ?>"><?php print t('Go Back to Homepage') ?></a>
    </div>
    <div id="admin-links">
      <?php print $welcome_user ?>
    </div>
    <div id="header-title" class="clearfix">
    </div>
  </div>
</div>
<?php } ?>

<div id="page-wrapper"><div id="page-wrapper-content">
  <?php print render($page['header']); ?>
  <?php if (!$hide_header) { ?>
    <div id="navigation" class="clearfix<?php print $rootcandy_navigation_class; ?>">
      <?php print $rootcandy_navigation; ?>
      <?php if ($logo): ?>
        <img src="<?php print $logo; ?>" alt="<?php print t('Logo'); ?>" id="logo" />
      <?php endif; ?>
    </div>
  <?php } ?>

  <div id="breadcrumb-title-wrapper">
    <?php print render($title_prefix); ?>
    <?php if ($title): ?>
      <h2 id="title"><?php print $title; ?></h2>
    <?php endif; ?>
    <?php print render($title_suffix); ?>
    <?php if ($breadcrumb): ?>
      <div id="breadcrumb"><?php print $breadcrumb; ?></div>
    <?php endif; ?>
  </div>

  <div id="content-wrap">
    <div id="inside">
      <?php if (isset($page['sidebar_first'])) { ?>
        <div id ="sidebar-left">
          <?php print render($page['sidebar_first']); ?>
        </div>
      <?php } ?>
      <?php if (isset($page['sidebar_second'])) { ?>
        <div id ="sidebar-right">
          <?php print render($page['sidebar_second']); ?>
        </div>
      <?php } ?>
      <div id="content">
        <div class="t"><div class="b"><div class="l"><div class="r"><div class="bl"><div class="br"><div class="tl"><div class="tr"><div class="content-in">
          <?php if (isset($tabs) && $tabs): print '<div id="tabs-primary"><ul class="tabs primary">'. render($tabs) .'</ul></div><div class="level-1 clearfix">'; endif; ?>
          <?php if (isset($tabs2) && $tabs2): print '<div id="tabs-secondary"><ul class="tabs secondary">'. render($tabs2) .'</ul></div><div class="level-2 clearfix">'; endif; ?>
          <?php if ($messages): ?>
            <div id="messages"><div class="section clearfix"><?php print $messages; ?></div></div> 
          <?php endif; ?>
          <?php print render($page['help']); ?>
          <?php if ($action_links): ?>
            <ul class="action-links">
              <?php print render($action_links); ?>
            </ul>
          <?php endif; ?>
          <?php print render($page['content']); ?>
          <?php if (isset($tabs2) && $tabs2): print '</div>'; endif; ?>
          <?php if (isset($tabs) && $tabs): print '</div>'; endif; ?>
          </div><br class="clear" /></div></div></div></div></div></div></div></div>
        </div>
      </div>
    </div>

    <?php if (isset($page['footer'])) { ?>
      <div id="footer">
        <?php print $legal; ?>
        <?php print render($page['footer']); ?>
      </div>
    <?php } ?>
  </div>
</div>
<!-- /layout -->
