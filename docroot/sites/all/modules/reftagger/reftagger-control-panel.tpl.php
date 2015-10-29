<?php

/**
 * @file reftagger-control-panel.tpl.php
 * Default theme implementation for the Reftagger Control Panel
 *
 * Available variables:
 *
 * - $version_options: An options array of the Bible version available
 * - $current_version: The current version selected
 *
 * @see template_preprocess_reftagger_control_panel()
 * @see theme_reftagger_control_panel()
 */
?>

<!-- RefTagger Control Panel -->
  <div id="lbsRefTaggerCP">
    <a href="http://www.logos.com/reftagger"><div id="lbsHeader">Bible Options</div></a>
    <div id="lbsVersionContainer">
      <select id="lbsVersion">
        <?php print $version_options; ?>
      </select>
    </div>
    <div id="lbsLibronixContainer">
      <input id="lbsUseLibronixLinks" type="checkbox">
      <label for="lbsUseLibronixLinks">Libronix</label>
    </div>
    <div id="lbsSaveContainer">
      <input value="Save" id="lbsSave" onclick="javascript:Logos.ReferenceTagging.lbsSavePrefs()" type="button">
      </div>
    <div id="lbsFooter">
      <div id="lbsLogo"><a href="http://www.logos.com/"><img src="http://www.logos.com/images/Reftagger/transparent.gif" alt="Logos Bible Software" title="Logos Bible Software" border="0" height="19" width="64"></a></div>
      <a href="http://www.logos.com/demo">Bible Study Software</a></div>
    </div>
<!-- End RefTagger Control Panel. For more info visit http://www.logos.com/reftagger. -->
