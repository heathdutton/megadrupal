Many times you want to execute an operation over a list of entities. Sometimes you will want to retroactively apply a
default value to a certain field, other times you will want to export the articles tagged as Drupal to a CSV file, etc.

This drush command simplifies that process for you. You only need to write the callback function that does your
operation, given an entity. All valid callbacks will receive the entity type as the first argument and the loaded entity
as the second parameter.

The drush command will let you select the entities on which you want to apply the command and do it in batches for you
while showing the progress in the command line screen.

### Selection rules
*Entity Process Callback* always receives an entity type as the primary filter. If you need to apply the same command to
multiple entity types you will need to run it several times, once for every entity type.

Beyond the entity type filter you can narrow your selection by using the following filters:

  1. By ID. Provide a list of IDs to process.
  1. By bundle. You can select the bundles that you want to limit the entities that will be affected.
  1. By property and field values. Only process the entities that match a certain value in a given field.

## Help
```
/var/www/docroot $ drush epc --help
This command will process the selected entities with the provided callback.

Examples:
 drush epc node                            Will process all nodes.
 \MyClass::addDefaultValueForNewField
 --size=25
 drush epc node my_module_custom_callback  Will process nodes with nid 12 and 56.
 --ids=12,56
 drush epc node entity_save                This will re-save all the articles.
 --bundles=article
 drush epc node entity_save                This will re-save all the articles asynchronously
 --bundles=article --queue                 (putting the save operations into a queue).

Arguments:
 entity-type                               Process all the entities of this type.
 callback                                  Callable name that will be executed for every selected
                                           entity. It takes the entity type as the first argument
                                           and the entity as the second. Return TRUE for success or
                                           FALSE for error.

Options:
 --bundles=<article,page>                  Limit the entities to the list of bundles, separated by
                                           commas. Ignored when ids is populated.
 --fields=<field_category|tid|555,field_n  Limit the entities to field conditions, pipe-delimted,
 ame|value|%sam%|like>                     separated by commas. Ignored when ids is populated.
 --ids=<1,4,66>                            Supply a comma-separated list of entity ids to process.
                                           If empty all entities are assumed.
 --queue                                   Put the items in a queue to be processed asynchronously.
 --size=<10>                               Number of entities to populate per run. Defaults to 10.

Aliases: epc, ep-callback
```

