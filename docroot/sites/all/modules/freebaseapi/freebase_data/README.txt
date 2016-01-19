Enhance Drupal data entry fields with Google Freebase suggestions.

THIS DIFFERS FROM FREEBASE_SUGGEST in that:
* It does not use the published javascript library
* it makes the lookups at the back-end from server-to-server
* it uses the Drupal autocomplete plugin

This means 
* it could be a little slower than pure client-side
* you don't get the little info-box
* we can programatically configure the requests on our side and change its behavior.

We add further contextual behavior, such as getting OTHER fields on the
form to fill in if you select a recognised topic identifier in an edit form.

