<?php

/**
 * @file
 * This is the tempate for the JNLP file to output.
 */

// We print the XML header as otherwise the PHP parser might choke on "<?xml".
print '<?xml version="1.0" encoding="UTF-8"?>';
?>
<jnlp spec="6.0+">
  <information>
    <title><?php print check_plain($title); ?></title>
    <vendor><?php print check_plain($vendor); ?></vendor>
    <?php if ($offline) print '<offline-allowed/>'; ?>
  </information>
  <resources>
    <java version="1.6+"/>
    <?php
    $first = TRUE;
    foreach(explode("\n", $jars) as $jar) {
      $jar = trim($jar);
      if (empty($jar)) {
        continue;
      }
      $jar = check_plain($jar);
      print "<jar href='$jar' ";
      if ($first) {
        $first = FALSE;
        print 'main="true"';
      }
      print "/>\n";
    }

    if ($pack200) {
      print '<property name="jnlp.packEnabled" value="true"/>';
    }
    ?>
  </resources>
  <application-desc main-class="<?php print check_plain($mainclass); ?>">
    <?php 
    foreach(explode("\n", $arguments) as $argument) {
      $argument = trim($argument);
      if (empty($argument)) {
        continue;
      }
      $argument = token_replace($argument);
      $argument = check_plain($argument);
      print "<argument>$argument</argument>\n";
    }
    ?>
  </application-desc>
</jnlp>
