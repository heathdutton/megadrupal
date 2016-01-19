# MIME Info

Developers know that the Drupal determines MIME-type of file by extension, but this is not quite the right way. If I'll change an extension of `*.php` file to `*.jpg`, then MIME-type will be determined as `image/jpeg` but, in fact, it will be the `application/x-httpd-php`. The **MIME Info** module solves this problem.

## Features

The module provides an overrides for three basic stream wrappers - `public`, `private` and `temporary`. If you allow the overriding, the MIME-type of files will be determined with help of [FileInfo](http://php.net/manual/en/book.fileinfo.php) extension, or, if it does not exist, - by binary data.

Also, if you have non-standard stream wrapper, you can use the `mimeinfo($uri)` function in the `getMimeType()` method or easily override an object by following the instruction:

- Select available wrapper in `Override the stream wrappers` section on `/admin/config/media/file-system` page.
- Add `mimeinfo` as the dependency to a module that will override the wrapper.
- Create the class by following the naming rules: Create the class by following the naming rule: `MimeInfo<WRAPPER>StreamWrapper`. If the wrapper name is `test-public_fb@files`, then class should be named `MimeInfoTestPublicFbFilesStreamWrapper`.

**Note**: if you use the extended wrapper and trying to get the MIME-type of non-existent file, then will be used the method of parent object. Due to that, the MIME-type of non-existent file will be determined by file extension.

## Usage

If the module was enabled, then you'll able to use the `mimeinfo($uri)` function wherever you want. Just do not forget to add it as the dependency into `*.info` file of your project.

**Example of overriding the "Remote" stream wrapper**

```php
class MimeInfoRemoteStreamWrapper extends DrupalPublicStreamWrapper {
  /**
   * {@inheritdoc}
   */
  public static function getMimeType($uri, $mapping = NULL) {
    return mimeinfo($uri) ?: parent::getMimeType($uri, $mapping);
  }
}
```

## Known issues

- If version of PHP is greater than 5.4 then you'll get a warning message that the `stream_cast` method [is not implemented by Drupal](https://www.drupal.org/node/2107287).
