# Entity Formatter

Entity Formatter can generate *custom formatter classes* for your
entities. The classes are generated via a simple drush command and provide
convenient FieldFormatter objects for each field attached to the entity.

For example you could use this code in your node.tpl.php file:

    <?php
      // Create the entity formatter object.
      $page = new PageNodeFormatter($node);
    
      // Access each field formatter via a get method,
      // and use convenient formatting methods depending on the field type.
      print $page->getBody()->summary(250);
    
      // You can easily iterate over multi value fields.
      foreach ($page->getImages() as $image) {
        print $image->setImageStyle('thumbnail')->img();
      }
    ?>
    
## Language
By default the current content language is used for printing the fields.

## Supported fields
Currently all core fields are supported plus some special fields:

- <a href="https://www.drupal.org/project/link">Link</a>
- <a href="https://www.drupal.org/project/date">Date</a> (Enddate support still missing)
- <a href="https://www.drupal.org/project/entityreference">Entity Reference</a>
- <a href="https://drupal.org/project/addressfield">Addressfield</a>


## Drush

The module provides a single drush command "entity-formatter-make" or short "efm".

<code>
drush efm node --bundles=page,article --module=entity_formatter_custom
</code>

This example generates formatter classes for page and article node types and
place them in the entity_formatter_custom module (generating module if not exists).

## Why should I use this module?

The main benefits are:

 - Access single and multi value fields the same way.</li>
 - Ease language handling for printing fields.</li>
 - Make use of code completion tools in your IDE for printing fields.</li>


## Related
The code to generate the classes is taken from
<a href="https://www.drupal.org/project/wrappers_delight">Wrappers Delight module</a>,
which was also a great source of inspiration for this module.