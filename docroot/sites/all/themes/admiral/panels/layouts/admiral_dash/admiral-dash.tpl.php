<?php
/**
 * @file
 * Template for Admiral Dashboard.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display admiral-dashboard clearfix <?php if (!empty($classes)) { print $classes; } ?><?php if (!empty($class)) { print $class; } ?>" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>
  <div class="container-fluid">
    <div class="row-fluid">

      <!-- Content Header -->
      <div class="span12 content-header panel-panel">
        <div class="panel-panel-inner">
          <?php print $content['contentheader']; ?>
        </div>
      </div>
    </div>

    <div class="row-fluid">
      <div class="span12 main-content panel-panel">
        <div class="row-fluid">

          <!-- Main Content -->
          <div class="span8 main-content panel-panel">
            <div class="panel-panel-inner">
              <?php print $content['content']; ?>
            </div>
          </div>

          <!-- Sidebar -->
          <div class="span4 sidebar panel-panel">
            <div class="panel-panel-inner">
              <?php print $content['sidebar']; ?>
            </div>
          </div>
          
        </div>
      </div>
    </div>
  </div>  
</div>
