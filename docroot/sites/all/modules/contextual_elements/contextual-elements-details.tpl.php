<?php
/**
 * @file
 * Print out the details of the tags.
 */
?>
<div id="contextual-elements-details">
  <h2 class="title"><?php print $contextual_element->name; ?></h2>
  <em><?php print $contextual_element->description; ?></em>

  <div class="element">Status: <span class="value"><?php print $status; ?></span></div>
  <div class="element">Debug Filters: <span class="value"><?php print $debug; ?></span></div>

  <div class="element code variables">
    Javascript Variables:
    <div class="value"><?php print $var_data; ?></div>
  </div>
  
  <div class="element code variables">
    Google Analytics Variables:
    <div class="value"><?php print $ga_var_data; ?></div>
  </div>
  
  <div class="element code header">
    Header Elements:
    <div class="value"><?php print $header; ?></div>
  </div>

  <div class="element code footer">
    Footer Elements:
    <div class="value"><?php print $footer; ?></div>
  </div>

  <div class="element">
    <?php print $domain_inc; ?>:
    <div class="value"><?php print $domains; ?></div>
  </div>

  <div class="element">
    <?php print $page_inc; ?>:
    <div class="value"><?php print $pages; ?></div>
  </div>

  <div class="element">
    <?php print $role_inc; ?>:
    <div class="value"><?php print (!empty($roles) ? implode("<br />", $roles) : t('None')); ?></div>
  </div>

  <div class="element">
    <?php print $users; ?>
  </div>
</div>
