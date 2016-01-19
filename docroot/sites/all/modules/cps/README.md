# Content Preview System

The Content Preview System, or CPS, takes total control over revision management
for all supported entities. For CPS-enabled entities, the revision tab is completely
replaced and users are not allowed to manually create or move around revisions.

In order to manage revisions, CPS introduces the concept of a *site version* which
will be considered to own all entity revisions. The default site version, known
as the *Live* site version, is the only one accessible to non-privileged users,
and it is considered to be the publically available state of the site.

Privileged users are not allowed to create, edit or delete content while viewing
the Live changeset. Instead, users must create a site version and select it in the
site version tool bar (which appears in the IIB bar just below the admin toolbar).

Once a site version is selected, all entity changes made will be stored in revisions
that are attached to that changeset. The Live version of the site will not be
affected. No matter how many changes are made to a single entity in a site version,
only one revision for that site version will be stored.

Once content administrators are satisfied with a site version, administrators have
the option of publishing the changeset. In that operation, all revisions in that
site version are made live and the changeset is moved into an archived state for
later use.

## Basic internal CPS workflow

### Load
* entity_load() gets called for an CPS-enabled entity.
* If no changeset is selected in the widget, or the user does not have access
  to view changesets, the live revision of the entity is loaded. No other
  changes are made.
* CPS, via hook_entity_load checks to see if there is a different revision for
  the current changeset.
* If so, CPS loads that entity instead and swaps it out. While doing this, it
  sets a flag on CPS to ensure that loads during this process do not try to
  swap again.

### Save
* entity_save() gets called for a CPS-enabled entity.
* If the entity does not yet exist, and the entity has a published (status) flag,
  a special 'live' revision is created that is unpublished; this live revision
  is never used, it is simply a placeholder to ensure that unpublished content
  does not appear. If it did not exist, the system would be unable to tell
  that 'published' content that appears only within a changeset is not visible
  to users.
* If a revision does not already exist for the changeset, a revision is created
  and all data is written to it.
* If a revision already exists for that changeset, that revision is updated with
  changes.
* Live data is not affected.

### Views queries
Views queries are altered so that data is retrieved only from revisions, and
the correct changeset revision is JOINed in to the query.

### Entity field queries
Entity field queries are altered so that data is retrieved only from revisions, and
the correct changeset revision is JOINed in to the query.

## Implementing CPS for a custom entity
In order for your custom entity to be visible to CPS, it must implement a plugin.

First, to implement a plugin your module must create an API hook:
```php
/**
 * Implements hook_ctools_plugin_directory().
 *
 * @todo CPS support will require this.
 */
function MYMODULE_plugin_directory($module, $type) {
  if ($module == 'cps') {
    return 'plugins/' . $type;
  }
}
```

Second, in plugins/entity you must have a file whose name corresponds to the
key of the entity used in hook_entity_info. This file tells CPS what plugin
class to use:

```php
$plugin = array(
  'handler' => array(
    'class' => 'CPSHANDLERCLASS',
    // This lets us put the class somewhere else if we would like; having several
    // entity modules share a single class is perfectly possible.
    'path' => drupal_get_path('module', 'MYMODULE') . '/includes'
  ),
);
```

### Things you have to customize

Note: Both cps_node and cps_file_entity modules give good examples on things that might
need to be customized. field_collection_item.inc in plugins/entity shows how the
very simplest entities need very little, if they are Entity API based.

There are several tasks that the base entity plugin isn't able to do for you. If you
happen to be using Entity module to implement your entity, CPS can do quite a lot
for you using the CPSEntityAPIBase class. But there are still a few very important
points:

* You will need to implement restoreEntityData() to tell CPS how to restore the
  live data when CPS writes a revision. This is needed because Drupal's entity
  system assumes that entities only write to the 'current' revision, but CPS wants
  to write to non-current revisions. Luckily, this is usually a very simple
  bit of code, and generally you just have to write to the entity's base table.
* You will need to extend access control for your entity using
  cps_can_edit_entities(). The node CPS module is a good example to see what
  must be done.
* You will need to set up the revisions tab to display CPS' site version information
  instead of the default revisions.
* If your entity can be published, you will need to extend the handler class from
  CPSEntityPublishableBase and implement publish(), unpublish(), isPublished()
  and createUnpublishedRevision(). CPSEntityAPIBase can help with this, but the
  actual fields used to control published/unpublished are not easily known.
* Watch out for loading additional entities in the controller's attachLoad().
  attachLoad() is what fires of hook_entity_load() and that is where CPS does
  its thing. If additional entities are loaded after this hook is fired, CPS
  might think it is in the process of swapping a revision out, and not swap
  loaded entities. The short version of the story is: Load any additional entities
  prior to calling parent::attachLoad().
* You will need to make certain that users do not have access to the Revision
  checkbox for any CPSable entities, just to ensure that CPS's revision control
  cannot be subverted.

Note: Internally site versions are referred to as changesets.

## Extending CPS for workflow

There are several hooks that will allow a module to add workflow states to CPS
and act upon them. See cps_workflow_simple as a good example and cps.api.php for
some documentation on the hooks.
