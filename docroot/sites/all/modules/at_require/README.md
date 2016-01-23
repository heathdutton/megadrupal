at_require
==========

Module provides drush command to download all dependencies for your modules.

Easier for modules/themes to require external libraries.

To list dependencies, modules/themes need defined ./config/at_require.yml

```yml
projects:
  ctools:
    type: module
    version: 1.3
  mustangostang/spyc
    type: composer
    version: 0.5.*
  twig:
    type: library
    download:
      type: git
      url: https://github.com/fabpot/Twig.git
      revision: v1.14.2
  jquery.cycle:
    type: library
    download:
      type: file
      url: http://malsup.github.io/jquery.cycle.all.js
```

Drush commands:

```bash
$ # Download dependencies for all modules those depend on at_base module
$ drush at_require
$ # Download dependencies for specific module
$ drush at_require module_name
$ # or just enable the module
$ #   twig library will be auto downloaded
$ drush en at_theming
```
