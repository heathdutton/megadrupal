##Installing the PMPAPI Module(s)
1. Download modules as you would normally.
2. Get a copy of the PHP SDK via git:

    `git clone -b v0.1.2 --depth 1 https://github.com/publicmediaplatform/phpsdk.git`
    
3. Or get the zip at

    `https://github.com/publicmediaplatform/phpsdk/archive/v0.1.2.zip`

4. **Make sure you do not use a newer version of the SDK.** It can and will break the modules.
5. The PHP SDK can be placed inside the PMPAPI module directory or, if using version 7-1.2 or above, you can place it in `sites/all/libraries` (recommended).
6. Enable the pmpapi module (or more)
7. Make sure you have the proper PMP credentials. https://support.pmp.io/register
8. Configure your PMP settings at: admin/config/services/pmp

##Architecture
- This package is composed of 8 distinct modules.
- pmpapi provides basic, minimal PMP API functionality.
- pmpapi_push (push) allows one to turn local entities into PMP docs, and then write them to the API.
- pmpapi_pull (pull) allows one to pull down various API docs, and then them turn the into (local, Drupal) entities.
- pmpapi_query_tools allows the user to build, save and edit complicated pull queries that are run on cron.
- pmpapi_groups allows the user to create/edit/delete PMP user groups. There is no strict dependence between the groups and permissions module.
- pmpapi_permissions (perms) allows one to manage permissions on API content. Do not confuse these with native Drupal permissions.
- pmpapi_updates (updates) allows one to have pulled stories automatically updated, with a minimal amount of effort.
- pmpapi_remote_files extends functionality of pull, via the [Remote stream wrapper module](https://drupal.org/project/remote_stream_wrapper). It allows certain file-based profiles to be pulled, without the file being copied locally.
- pmpapi, pmpapi_push, pmpapi_pull, and pmpapi_update rely heavily on classes that, in theory, could be extended or replaced with custom classes.
- These modules try to be very particular about vocabulary, in order to reduce confusion. When referring to "low-level" functionality of the base module (pmpapi.module), the verbs are: fetch (a single doc), query (make a general query), send (data to the API), and remove (doc(s) from the API). When referring to functionality that maps drupal entities to PMP docs (and vice versa), the verbs are push and pull. 

##Push
- In general, the push module maps an entity on your site to a PMP profile. In addition, fields attached to that entity can be mapped to attributes and items of that profile.
- Push is dependent upon pmpapi and, for the sake of much more readable code, [Entity API](https://drupal.org/project/entity)
- Entities will be re-pushed any time they are edited.
- Deleted entities will also be deleted from the PMP API. (There is a failure queue as part of the PMPAPI base module, in an attempt to ensure their deletion.)
- Entities that are unpublished (i.e., $entity->status = 0) will also be removed from the API.
- Some entities, such as file entities, have no status property (or a different notion of status) and, thus, can only be removed from the API by deleting the corresponding entity.
- If you have a node (or other entity) with attached images or other entities, those images will be linked to your doc in the PMP if and only if they are first pushed to the API as separate docs (with a completely separate mapping of entity<-->profile and field<-->attribute).
- In other words, if you'd like to push images and other files, the [file_entity module](https://drupal.org/project/file_entity) is highly recommended, but, strictly speaking, not a dependency.
- This granular, non-recursive process of pushing also favors new sites -- or pre-existing sites interested in pushing only new content. Re-stated: pushing random, old pieces of content with deep entity associations could be problematic.
- Entities -- if they are deemed "OK to push" and do not already have a GUID -- are assigned a GUID on presave. That GUID is then saved to a pmpapi-specfic lookup table.
- See `pmpapi_push_entity_ok_to_push()` for more info on deeming an entity "OK to push."

## Pull
- Pull works in a very similar fashion to push, except, of course, in reverse. Incoming docs and the attributes and items are mapped to Drupal entities and their fields.
- Unilke push, pulled entities will *not* be re-pulled on entity save.
- Also unlike push, attached entities are also pulled. Though there are some rules: the attached entity much be file-based (i.e. - has an enclosure link). For now, the allowed profiles are hard-coded to just audio, image, and video. In addition, the pull will only recurse to a depth of 1. That is, if a story is linked to an image, and that image is linked to, say, another image, only the first image will be pulled.
- Pull has hard dependencies on the pmpapi module and the Entity API module. As with push, if you'd like to push images and other files, the [file_entity module](https://drupal.org/project/file_entity) is highly recommended, but, strictly speaking, not a dependency.

## Query Tools
- Go to `admin/config/services/pmp/queries` to start creating pull queries.
- Queries will automatically add content at cron.

## Groups
- PMP user groups are simply collections of PMP user/org docs. By themselves, they have no notion of blacklist/whitelist, or even any explicit relation to PMP doc permissions.
- This module is a very simple UI that allows one to add, edit, and delete PMP groups.
- For users planning on creating involved whitelist/blacklist permissions, this module is highly recommended.

## Permissions
- This module allows the user to set explicit read permissions around a doc, with an entity-level granularity.
- Default permissions can be set at a entity-type level (e.g., on the /admin/structure/types/manage/<NODE TYPE> config page).
- The module is actually a simplified version of the PMP permission system. Users can modify access to a doc by adding a *single* **whitelist** permission to a doc. Still, this should address 90+% of permission use-cases.
- There are three basic permissions provided by this module:
  - Grant access to everyone (no actual permissions on the doc).
  - Make private (doc is whitelisted to only the current user).
  - Doc is whitelisted to a selected group.
- Dealing with blacklists is much more complicated, as they usually require a complementary whitelist and, as of this writing, they don't really work as you might expect (e.g., there is no conecpt of whitelisting all users).
- If there is enough demand for more complicated permissioning, we might write a pmpapi_advanced_permissions module that allows one to add multiple, additive, whitelist *or* blacklist permissions to a given entity.
- Currently, permissions can only be added to nodes and file entities (if the site is using the file_entity module).
- For much more about PMP permission, see: [http://cdoc.io/spec.html#content-rights](http://cdoc.io/spec.html#content-rights)

## Updates
- This module allows one to have pulled stories automatically updated, with a minimal amount of effort.

## Remotes Files
- This module extends functionality of pull, via the [Remote stream wrapper module](https://drupal.org/project/remote_stream_wrapper).
- It allows certain file-based profiles to be pulled, without the file being copied locally.
- When enabled, the user will see an extra fieldset on the pull configuration page: a list of all file entities.
- Checked file entities will still be pulled, but Drupal will *not* make a local copy of the actual accompanying file (mp3, jpg, etc.)
- This is particularly useful for, say, large video and audio files that are already hosted on a CDN.