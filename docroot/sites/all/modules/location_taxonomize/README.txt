/////////////////////////////////////////////
//
// Location Taxonomize
//
/////////////////////////////////////////////

Location Taxonomize is a Drupal contrib module.

Location Taxonomize allows for the creation and automatic synchronization of a
hierarchical Taxonomy based on data collected by the Location module.

Check it out at http://drupal.org/sandbox/goron/1234754

-------------------
Usage Instructions
-------------------

Location taxonomize will "associate" itself with one vocabulary you choose, and fill
that vocabulary with data from the Location module. To set up:

1. Install the module as you would any Drupal module. Install also at least one of the source
   modules to enable integration with either Location or Address Field
2. Navigate to the configuration page (admin/config/content/location_taxonomize)
3. You will first need to initialize your Location Vocabulary:
4. Choose a source module: either Location or Address Field.
5. Choose an initialization method. You have two choices:
    A)  Create a new Location Vocabulary. If you choose this, a new vocabulary will be
        created for you, with the name 'location_taxonomize', and this vocabulary will
        be associated with Location taxonomize.
    B)  Use an existing vocabulary. To enable this option, you must change the name of
        your existing vocabulary to 'location taxonomize'. This vocabulary will then
        appear as an option on the initialization page. If you choose this, the vocabulary
        will be associated with Location taxonomize, and new data will be added to the existing
        terms. (NOTE that the hierarchy structure of your vocabulary should match the one you
        choose in the next step)
6. Choose a hierarchy structure. This will determine how terms are saved in your Location
   vocabulary. A few fields are available for you to choose from, and only the ones you choose
   will be saved as terms. The terms will be saved in the order shown. The first field you choose in
   the list will describe the top-level terms of your vocabulary, and the next terms will be children
   and grandchildren, with a hierarchy level for each field. For a Country->State->City hierarchy,
   simply select those fields.
   NOTE that you cannot change this after you initialize. However, you can always disassociate the
   vocabulary from Location taxonomize, and re-initialize it with different settings.
7. Initialize your Location vocabulary.
8. If you chose to integrate with Address Field, note that you must enable the "Taxonomize locations
   from this field using Location Taxonomize." option for each field you want to taxonomize in that
   field's settings.
9. Your vocabulary is now set up and active. Now you should see more options:

OPERATIONS
Gives you tools to:
- Empty Vocab: Remove all terms from the location vocabulary (All terms will be removed IMMEDIATELY.
  Mostly here for testing purposes)
- Disassociate Vocab: Remove the association with the current Location Vocabulary. The vocabulary
  will remain with all its terms, but it will not be connected to this module. You can then change
  basic settings and re-associate it.
- Bulk Taxonomize Locations: This will process all locations that have been saved on this site by
  the Location module and add them to the Location Vocabulary

SETTINGS
- Turn on or off synchronizing with Location data
- Change the way terms are named when they are saved
- Attach terms: if you select this and provide a term reference field on nodes you use with this module,
  the terms created for the locations will then be attached back to the node.
- Add a Long Name field to the term. This is used to save a field with a value like "San Francisco, CA, USA"
