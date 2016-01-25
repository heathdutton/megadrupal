<?php
/**
 * @file
 * Template to generate a drush alias file for a project.
 *
 * Variables:
 * - $project_path: The filesystem path to the project's directory
 * - $vms: An array of formatted aliases for he project's VMs
 */
?>
<?php print "<?php\n\n" ?>
$options['project-path'] = '<?php print $project_path; ?>';

$aliases['project'] = array(
  'type' => 'project',
);


<?php foreach ($vms as $key => $alias) { print $alias; } ?>
