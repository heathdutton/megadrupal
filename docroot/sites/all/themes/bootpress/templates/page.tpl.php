<?php
/**
 * Page template
 * @theme Boot Press
 * @author Pitabas Behera
 *
 **/
?>

<?php require_once _boot_press_include_template('header.tpl.php'); ?>
  <div id="page" class="clearfix">
    <?php if ($page['highlighted']):?>
    <section id="content-header" class="clearfix">
      <div class="container">
        <div class="content-header-inner" class="clearfix">
          <div class="row">
            <div class="col-md-12">
            <?php print render($page['highlighted']); ?>
            </div>
          </div>
        </div>
      </div>
    </section>
    <?php endif; ?>

    <div id="main-content">
      <div class="container">
        <?php if ($messages):?>
        <div id="messages-section" class="clearfix">
          <div class="row">
            <div class="col-md-12">
            <?php print $messages; ?>
            </div>
          </div>
        </div>
        <?php endif; ?>
        
        <div class="row">
          <?php if ($page['sidebar_first']):?>
          <aside class="<?php print $sidebar_grid_class; ?>">  
            <section id="sidebar-first" class="sidebar animated fadeInLeft clearfix">
            <?php print render($page['sidebar_first']); ?>
            </section>
          </aside>
          <?php endif; ?>

          <section class="<?php print $main_grid_class; ?>">
            <div id="main" class="clearfix">
              <div id="content-wrapper">
                <?php print render($title_prefix); ?>
                <?php if ($title):?>
                <h1 class="page-title"><?php print $title; ?></h1>
                <?php endif; ?>
                <?php print render($title_suffix); ?>
                
                <?php print render($page['help']); ?>
                
                <?php if ($tabs):?>
                  <div class="tabs">
                  <?php print render($tabs); ?>
                  </div>
                <?php endif; ?>

                <?php if ($action_links):?>
                  <ul class="action-links">
                  <?php print render($action_links); ?>
                  </ul>
                <?php endif; ?>

                <?php print render($page['content']); ?>
                <?php print $feed_icons; ?>
              </div>
            </div>
          </section>

          <?php if ($page['sidebar_second']):?>
          <aside class="<?php print $sidebar_grid_class; ?>">
            <section id="sidebar-second" class="sidebar animated fadeInRight clearfix">
            <?php print render($page['sidebar_second']); ?>
            </section>
          </aside>
          <?php endif; ?>
          
        </div>
      </div>
    </div>

    <?php if ($page['content_footer']):?>
    <section id="content-footer" class="clearfix">
      <div class="container">
        <div class="content-footer-inner" class="clearfix">
          <div class="row">
            <div class="col-md-12">
            <?php print render($page['content_footer']); ?>
            </div>
          </div>
        </div>
      </div>
    </section>
    <?php endif; ?>
  </div>
<?php require_once _boot_press_include_template('footer.tpl.php'); ?>
