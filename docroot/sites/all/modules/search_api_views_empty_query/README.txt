When defining a search view in Search Views module and exposing the
"Fulltext search" filter, the view will display all results if the
search text is empty. This module changes this behavior to display
either the view's No Results content or a custom text.

To use this module, create a search view:

- Add new view
- Show YOUR_INDEX sorted by Unsorted;
- Make sure "Create a page" is checked; configure the page settings
  and press "Continue & edit"
- Set the view's Format to your liking
- In the "Filter Criteria" section, add "Search: Fulltext search"
  and expose it. Make sure it is not marked as required. You can
  add other filters, but do not expose them.
- Set the sort criteria to your liking (you probably want to sort
  by "Search: Relevance (desc)")
- Add a "No Result Behavior", either "Global: Text area" or
  "Global: Unfiltered text". Enter some text to display when the
  search returns no results.
- Tweak the view's other settings to your liking.

Now, if you go on the search page without enabling this module, you
will notice that it displays all the site content if you leave the
text box empty. To fix this annoying behavior, enable this module. Go
back to the search page: the No Result text is now displayed when the
text box is empty.

If you would like to display a different message for this particular
case, go to admin/config/search/search_api_views_empty_query, check
the "Display custom message" checkbox and enter a message inviting the
user to enter some text in the search form.

If you have any other exposed filter other than "Search: Fulltext
search", the module will ignore your view and the deafult behavior of
showing all content will apply.
