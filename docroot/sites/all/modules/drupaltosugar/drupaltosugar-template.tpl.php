<?php

/**
 * @file
 * Display front page of drupaltosugar module.
 */
?>
<div id="drupaltosugar_integration">
  <div class="drupal-blck">
    <h2><a href="#" class ="drupal-sso">Drupal SugarCRM SSO</a></h2>
    <p class="cntent" style="height:200px">
      This module will enable 'single sign on' between Drupal website and SugarCRM system that share common users.  Using Drupal user credentials, you will be able to log-in into your SugarCRM system with a single step. While signing in from Drupal, if a user exists in SugarCRM, it will create the user here in Drupal. This module is coming soon.
    </p>
    <p class="buttn">
      <a href="#" class ="drupal-sso">Launch Drupal SugarCRM SSO</a>
    </p>
    <p class="view-hlp"><a href="#">View Help</a></p>
    <p class="watc-vdeo"><a href="#">Watch video</a></p>
  </div>

  <div class="drupal-blck">
    <h2> <?php print l(t('Webform To SugarCRM Integration'), 'admin/drupaltosugar/system_configuration', array('attributes' => array('class' => 'drupal-to-sugar'))); ?></h2>
    <p class="cntent" style="height:200px">
      This module enables you to create integration between your Drupal website and SugarCRM system. Drupal SugarCRM Integration gives multiple capabilities that are not available in earlier modules, such as creating entries in related modules of sugarCRM. This makes integration faster, more comprehensive and better aligned with your business needs.
    </p>
    <p class="buttn">
      <?php print l(t('Launch Webform To SugarCRM'), 'admin/drupaltosugar/system_configuration', array('attributes' => array('class' => 'drupal-to-sugar'))); ?>
    </p>
    <p class="view-hlp"><a href="#">View Help</a></p>
    <p class="watc-vdeo"><a href="#">Watch video</a></p>
  </div>

  <div class="drupal-blck">
    <h2><a href="#" class ="drupal-content-sugar">SugarCRM To Content Type Integration</a></h2>
    <p class="cntent" style="height:200px">
      This module will give you flexibility of automatic Node creation in your Drupal website according to related content types vis-a-vis a new entry in your SugarCRM system.  When you create new content in SugarCRM, the related node will get created in Drupal. If the node already exists, it will be updated by new content. This module is coming soon.
    </p>
    <p class="buttn">
      <a href="#" class ="drupal-content-sugar">Launch SugarCRM To Content Type</a>
    </p>
    <p class="view-hlp"><a href="#">View Help</a></p>
    <p class="watc-vdeo"><a href="#">Watch video</a></p>
  </div>
</div>
