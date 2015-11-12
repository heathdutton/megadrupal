<?php
/**
 * @file
 * Admin help for FPP Bundles.
 */
?>
<section class="assets" role="region">
  <h2 role="heading">
    <?php print t('Automatically include the CSS & JS'); ?>
  </h2>
  <p>
    <?php
      print t('If panel has the "!name" machine name, then the module will try to find CSS & JS in following directories:', array(
        '!name' => '<b>media_bundle</b>',
      ));
    ?>
  </p>
  <ul>
    <li>PATH/TO/THEME/css/fieldable-panels-panes/media-bundle.css</li>
    <li>PATH/TO/THEME/js/fieldable-panels-panes/media-bundle.js</li>
  </ul>
  <p>
    <?php print t('Also, for <b>administrative users</b> can be added the additional assets. The files should be placed and named the same as in the next example:'); ?>
  </p>
  <ul>
    <li>PATH/TO/THEME/css/fieldable-panels-panes/admin/media-bundle-admin.css</li>
    <li>PATH/TO/THEME/js/fieldable-panels-panes/admin/media-bundle-admin.js</li>
  </ul>
  <h4 role="heading">
    <?php print t('Templates'); ?>
  </h4>
  <p>
    <?php print t('For each panel can be created the template file in "templates" directory of current Drupal theme. But strongly recommended to store the templates in:'); ?>
  </p>
  <ul>
    <li>PATH/TO/THEME/templates/fieldable-panels-panes/fieldable-panels-pane--media-bundle.tpl.php</li>
  </ul>
</section>

<br />
<hr />
<br />

<section class="api" role="region">
  <h2 role="heading">
    <?php print t('Application programming interface'); ?>
  </h2>
  <p><?php print t('The module has the next hooks:'); ?></p>
  <ul>
    <li>
      <b>hook_fpp_bundles_bundle_insert</b>
    </li>
    <li>
      <b>hook_fpp_bundles_bundle_update</b>
    </li>
    <li>
      <b>hook_fpp_bundles_bundle_delete</b>
    </li>
  </ul>
  <p><?php print t('Each of this hooks occurs after successfully operation with DB and called only when event happened and cannot affect on the data structure.'); ?></p>
  <h6 role="heading">
    <?php print t('Also, the next functions can help for developers:'); ?>
  </h6>
  <ul>
    <li>
      <b>fpp_bundles_save</b> - <?php print t('Programmatically save the new bundle;'); ?>
    </li>
    <li>
      <b>fpp_bundles_remove</b> - <?php print t('Programmatically update an existing bundle.'); ?>
    </li>
  </ul>
  <br />
  <div>
    <strong>
      <?php print t('Creation example:'); ?>
    </strong>
    <pre>
// If the value of "$status" variable will be TRUE, then bundle was
// created successfully, in another case the error message will be
// stored in the Dblog and the value will be FALSE.
$status = fpp_bundles_save(array(
  // The "name" is required.
  'name' => 'Video',
  // By default it set to "0",
  'level' => 1,
  // By default it set to "1".
  'assets' => 0,
  // By default it is empty.
  'category' => 'Media',
));
    </pre>
  </div>
  <div>
    <strong>
      <?php print t('Update example:'); ?>
    </strong>
    <pre>
// Change only one parameter of the bundle.
$status = fpp_bundles_save(array(
  // The ID of created bundle.
  'bid' => 1,
  // Change "assets" to "1".
  'assets' => 1,
));
    </pre>
  </div>
  <div>
    <strong>
      <?php print t('Remove example:'); ?>
    </strong>
    <pre>
// Remove the bundle by ID. If bundle with such ID does not exist,
// then error message will be stored in the Dblog and a value
// of "$status" variable will be FALSE.
$status = fpp_bundles_remove(1);
    </pre>
  </div>
  <div>
    <strong>
      <?php print t('Append your own assets in <b>preprocess</b> hooks:'); ?>
    </strong>
    <pre>
/**
 * Implements hook_preprocess_HOOK().
 */
function hook_preprocess_fieldable_panels_pane(&$variables) {
  $entity = $variables['elements']['#element'];

  // The "assets" property provided by "FPP Bundles" module
  // and allowed only for panels, created from UI.
  if (isset($entity->assets)) {
    $entity->assets['css'][] = 'path/to/your/own/file.css';
  }
}
    </pre>
  </div>
  <p>
    <?php print t('Highly recomended to create/update/delete the bundles !link and export them with !features.', array(
      '!link' => l(t('via UI'), 'admin/structure/fieldable-panels-panes'),
      '!features' => l(t('Features'), 'https://www.drupal.org/project/features'),
    )); ?>
  </p>
</section>
