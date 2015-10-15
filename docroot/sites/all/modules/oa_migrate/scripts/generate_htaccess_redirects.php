<?php
//
// Generate code for .htaccess to redirect from content on the OA1 site to
// the OA2 site once it's fully migrated.
//
//
// USAGE:
//
//   drush @oa2alias scr generate_htaccess_redirect.php http://oa1.example.com http://oa2.example.com
//
// Optionally, you can pass a --parent-space=X option, which will only generate
// redirects for nodes which are children (or grandchildren and onward) of a
// particular Space. It's basically just like the 'parent_space_default' config
// value that you give in settings.php.

$parent = drush_get_option('parent-space');
$single = drush_get_option('single');
$leading_slash = drush_get_option('leading-slash') ? '/' : '';

if (!$single) {
  $origin = drush_shift();
  if (empty($origin)) {
    drush_log('You must pass the domain name of the legacy site.', 'error');
    exit(1);
  }
}
else {
  $origin = '';
}

$destination = drush_shift();
if (empty($destination)) {
  drush_log('You must pass the domain name of the new site.', 'error');
  exit(1);
}

// Remove trailing slashes from both $origin and $destination.
$origin = rtrim($origin, '/');
$destination = rtrim($destination, '/');

// Remove the http:// or https:// from the beginning of $origin, if given.
$origin = preg_replace('/^https?:\/\//', '', $origin);

// Convert the $origin into a perl-compatible regex for RewriteCond.
$origin_pattern = '^' . str_replace('.', '\.', $origin) . '$';

// Query the data from the 'field_oa_migrate_legacy_id' field.
$query = db_select('field_data_field_oa_migrate_legacy_id', 'f')
  ->fields('f', array('entity_type', 'bundle', 'entity_id', 'field_oa_migrate_legacy_id_value'))
  ->condition('f.entity_type', 'node')
  ->orderBy('f.entity_id');
if (!empty($parent)) {
  // Generate a list of all the Spaces underneath the parent.
  $parents = array();
  $stack = array($parent);
  while ($current = array_shift($stack)) {
    if (!isset($parents[$current])) {
      $children = $parents[$current] = db_select('og_membership', 'm')
        ->fields('m', array('etid'))
        ->condition('m.entity_type', 'node')
        ->condition('m.group_type', 'node')
        ->condition('m.gid', $current)
        ->execute()->fetchCol();
      $stack = array_merge($stack, $children);
    }
  }

  // Alter our query to only include content on any of our parent spaces.
  $query->join('og_membership', 'm', 'm.etid = f.entity_id');
  $query
    ->condition('m.entity_type', 'node')
    ->condition('m.group_type', 'node')
    ->condition('m.gid', array_keys($parents), 'IN');
}
$result = $query->execute();

//
// Start output the mod_rewrite rules.
//

print "\n";
print "# Place this in the .htaccess file for the legacy site, above the\n";
print "# rewrite rules from Drupal.\n";
print "<IfModule mod_rewrite.c>\n";
print "  RewriteEngine on\n";

foreach ($result as $row) {
  // We can't redirect anything whose legacy_id included a fragment.
  if (strpos($row->field_oa_migrate_legacy_id_value, '#') === FALSE) {
    if (!$single) {
      print "\n";
      print "  RewriteCond %{HTTP_HOST} $origin_pattern [NC]\n";
    }
    print "  RewriteRule ^$leading_slash(?:[a-zA-Z1-9_-]+/)?node/{$row->field_oa_migrate_legacy_id_value}(/.*)?\$ $destination/node/{$row->entity_id}$1 [L,R=301]\n";
  }
}

print "</IfModule>\n";
