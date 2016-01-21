# FPP Bundles (Fieldable Panels Panes UI)

**Fieldable Panel Panes Bundles (Fieldable Panel Panes UI)** - module that helps to create bundles of the [Fieldable Panels Panes](https://www.drupal.org/project/fieldable_panels_panes) entity. The project aims to simplify developers life by way of refraining of writing the code for panels creation.

The module has an integration with [Features](https://www.drupal.org/project/features) and allows to export an existing bundles with automatically adding their dependencies (fields, field groups, etc).

## Installation

**Drush:**
```
drush en fpp_bundles -y
```

**Manual:**
- Download the release from [drupal.org](https://www.drupal.org/project/fpp_bundles);
- Put the unpacked folder into your Drupal installation "modules" directory;
- Go to `/admin/modules` and enable the module.

## Documentation

### API hooks

See all hooks and their description in [fpp_bundles.api.php](fpp_bundles.api.php).

### Helper functions

Programmatically creation of a bundle:

```php
// If the value of "$status" variable will be TRUE, then bundle was
// created successfully. In another case, when machine name is
// already exist, the error message will be stored in the Dblog 
// and the value will be FALSE.
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
```

Programmatically update the bundle:

```php
// Change only one parameter of the bundle.
$status = fpp_bundles_save(array(
  // The ID of created bundle.
  'bid' => 1,
  // Change "assets" to "1".
  'assets' => 1,
));
```

Programmatically remove the bundle:

```php
// Remove the bundle by ID. If bundle with such ID does not exist,
// then error message will be stored in the Dblog and a value
// of "$status" variable will be FALSE.
$status = fpp_bundles_remove(1);
```

Append your own assets in `preprocess` hooks:

```php
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
```

### Best practices

Creating the new bundle with frontend and backend parts.

- Create the bundle from UI with name `Media`;
  - Correct the machine name of bundle by adding prefix `fpp_` (after changes it should be `fpp_media`);
  - If necessary, type the name of category and check/uncheck the box that allow you to see the bundle in general list;
  - Do not uncheck the box that indicate that CSS & JS for panel will be included automatically if they are exists;
  - Create a bundle and add necessary fields and field groups, configure the displays;
- Create the template, CSS and JS if needed;
  - Put your template file in: `path/to/theme/templates/fieldable-panels-panes/fieldable-panels-pane--fpp-media.tpl.php`;
  - Put your CSS file in: `path/to/theme/css/fieldable-panels-panes/fpp-media.css`;
  - Put your JS file in: `path/to/theme/js/fieldable-panels-panes/fpp-media.js`;
  - If needed, put the CSS or/and JS in `path/to/theme/[js/css]/fieldable-panels-panes/admin/fpp-media-admin.[js/css]`;
- Export the bundle via Features;
- Commit created feature and theming files using your VCS system.

## Changelog

**Version [1.0](https://github.com/BR0kEN-/fpp_bundles/tree/7.x-1.0)**, December 18, 2014:
- UI CRUD interface for bundles of Fieldable Panels Panes entity.
- Integration with Features.

**Version [1.1](https://github.com/BR0kEN-/fpp_bundles/tree/7.x-1.1)**, December 23, 2014:
- Fixed an [issue](https://www.drupal.org/node/2397973) with Features integration.

**Version [1.2](https://github.com/BR0kEN-/fpp_bundles/tree/7.x-1.2)**, December 24, 2014:
- Improved integration with Features. From now, all fields, groups and widgets will be added into the Feature automatically.

**Version [1.3](https://github.com/BR0kEN-/fpp_bundles/tree/7.x-1.3)**, January 29, 2015:
- Added the API hooks which occurs after successful insert/update/delete operation.
- Added the functions for programmatically creation, validation and deletion the bundles.
- Added an option to automatically load CSS & JS for a panel.
- Added the help section at `/admin/help/fpp_bundles`.
- Improved SimpleTest.

**Version [1.4](https://github.com/BR0kEN-/fpp_bundles/tree/7.x-1.4)**, February 11, 2015:
- Added the possibility to include CSS & JS for administrative users.
- Updated the documentation.

## Authors

- [Sergey Bondarenko (BR0kEN)](https://github.com/BR0kEN-)
