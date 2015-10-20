# Drush integration for [Libraries API](https://drupal.org/project/libraries)

- Implement [hook_libraries_info()](http://www.drupalcontrib.org/api/drupal/contributions!libraries!libraries.api.php/function/hook_libraries_info/7).
- Define couple libraries inside with `download url` pointing to file that could be downloaded.
- Execute `drush libget` and follow the instructions.

## Installation

```
drush dl libapi
```

## Usage

```
drush help liblist
drush help libget
```
