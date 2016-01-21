<?php
/**
 * @file
 *
 * Page display
 */
// additional classes 
$content_classes = '';
?>

<div id="page-top"></div>
<div id="page-link"><a class="link-top" href="#page-top">Top</a><a class="link-bottom" href="#page-bottom">Bottom</a></div>
<div class="wrapper-outer">
  <div id="container" class="wrapper">
    <?php print $breadcrumb; ?>
    <div id="container-inner" class="wrapper-inner">
        <?php print theme('content_elements', $content_elements_vars); ?>
        <?php print render($page['help']); ?>
        <?php print $messages; ?>
        <?php print $content_region; ?>
    </div> <!-- /#container-inner -->
  </div> <!-- /#container -->
</div> <!-- /#wrapper-outer -->
<div id="page-bottom"></div>
