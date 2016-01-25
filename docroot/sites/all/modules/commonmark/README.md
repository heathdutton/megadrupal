# CommonMark

This module provides a text format filter that converts Markdown to HTML based
on the [CommonMark] spec via [thephpleague/commonmark] PHP library, created and
maintained by [The League of Extraordinary Packages].

## Try out a demonstration!
http://commonmark.unicorn.fail

To see a full list of "long tips" provided by this filter, visit:
http://commonmark.unicorn.fail/filter/tips

## Requirements
- **PHP >= 5.4.8**  
  This is a hard requirement due to [thephpleague/commonmark].

## Soft Requirements
This modules supports the following methods for installing the necessary PHP
libraries and autoloading the module and library PSR-4 classes. You must choose
one of the following methods:

- **Composer**  
  Composer is the preferred method for installing PHP libraries and autoloading
  the module and library PSR-4 classes. Works with [Composer Manager] and
  [Drupal Composer Packagist].
- **Registry Autoload or X Autoload**  
  This module supports using the [Registry Autoload] module or the [X Autoload]
  module. If you use either of these modules, you must also install the
  necessary PHP libraries. You can do this automatically using the following
  Drush command or by manually placing the necessary PHP libraries in
  `sites/*/libraries`:  
    
  ```sh
  drush commonmark-download
  ```

## CommonMark Extensions
- **Enhanced Links** - _Built in, enabled by default_  
  Extends CommonMark to provide additional enhancements when rendering links.
- **@ Autolinker** - _Built in, disabled by default_  
  Automatically link commonly used references that come after an at character
  (@) without having to use the link syntax.
- **# Autolinker** - _Built in, disabled by default_  
  Automatically link commonly used references that come after a hash character
  (#) without having to use the link syntax.
- **[CommonMark Attributes Extension]**
  Adds syntax to define attributes on various HTML elements inside a CommonMark
  markdown document. To install, enable the `commonmark_attributes` sub-module.
- **[CommonMark Table Extension]**  
  Adds syntax to create tables in a CommonMark markdown document.  To install,
  enable the `commonmark_table` sub-module.

## Features Support
There is a sub-module named `commonmark_feature` that you can enable to get up
and going. Or you can simply use it as a base/reference for your own feature.

## APIs
There are extensive APIs for providing additional extensions, settings and tips.
You can the primary documentation in `commonmark.api.php`. This module also
provides a group for its hooks. This means you can place procedural code for
CommonMark hooks in a file named `MODULE_NAME.commonmark.inc`. This module
implements these hooks for its built in extensions and can be found in
`commonmark.commonmark.inc`.

## Programmatic Conversion
In some cases you may need to programmatically convert CommonMark Markdown to
HTML. You can accomplish this in the following manner:
```php
  $markdown = '# Hello World!';
  $html = commonmark_convert_to_html($markdown);
  print $html; // <h1>Hello World!</h1>
```

If you have multiple Markdown strings to convert, in a linear fashion, it may
be easier to load the converter instance yourself and avoid additional calls to
`commonmark_convert_to_html`:
```php
  $output = '';
  $converter = commonmark_get_converter();
  foreach ($items as $markdown) {
    $output .= $converter->convertToHtml($markdown);
  }
  print $output; // The concatenated output.
```

[CommonMark]: http://commonmark.org/
[CommonMark Attributes Extension]: https://github.com/webuni/commonmark-attributes-extension
[CommonMark Table Extension]: https://github.com/webuni/commonmark-table-extension
[Composer Manager]: https://www.drupal.org/project/composer_manager
[Drupal Composer Packagist]: https://packagist.drupal-composer.org/packages/drupal/commonmark
[thephpleague/commonmark]: https://github.com/thephpleague/commonmark
[Registry Autoload]: https://www.drupal.org/project/registry_autoload
[The League of Extraordinary Packages]: http://commonmark.thephpleague.com/
[X Autoload]: https://www.drupal.org/project/xautoload
