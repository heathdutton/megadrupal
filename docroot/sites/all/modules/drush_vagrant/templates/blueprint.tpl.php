<?php
/**
 * @file
 * Template to generate .config/blueprint.inc, a file to record the blueprint
 * used for a project, and possibly other arbitrary data.
 *
 * Variables:
 * - $extension: The Drush extension that provides the blueprint.
 * - $blueprint: The blueprint used to build a project.
 * - $data: An array of arbitrary data provided by the blueprint extension.
 */
?>
<?php print "<?php\n\n" ?>
$options['blueprint'] = array(
  'extension' => '<?php print $extension; ?>',
  'blueprint' => '<?php print $blueprint; ?>',
<?php
  if (isset($data)) {
    print "  'data' => array(\n";
    foreach ($data as $key => $value) {
      sprintf("    '%s' => '%s',\n", $key, $value);
    }
    print "  );\n";
  }
?>
);
