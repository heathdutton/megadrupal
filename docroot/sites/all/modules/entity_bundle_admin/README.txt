Entity Bundle Admin
======================

This is a small module that provides the functions and core interaction necessary to provide a simple read-only admin UI for a custom entity type's bundles.

The use case for this is that your entity type's different bundles are only defined in code, possibly across several modules, but you need an admin UI for Field UI's 'Manage fields' tabs to hang off.

If you want your bundles themselves to be editable in the UI, you should use Entity API to create bundles as entities (in the manner of profile2_type entities being the bundles of the profile2 entity).
