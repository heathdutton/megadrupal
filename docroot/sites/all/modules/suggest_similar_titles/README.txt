
== Installation ==

Just enable the module using admin interface. This is plug and play module,
and enables for all the content types by default.

== What this module does ==

This module provides setting page where following settings can be made:
- Enable this feature for any content type(s).
- Select maximum number of titles to show up in suggestion.
- Select whether this module should consider node permissions
before displaying node title as suggestion.
- Enter ignore keywords in title comparison.
- Enter percentage how exact do you want to compare the title. For example,
if you enter 75, then at-least 75% matching title will be considered duplicate.
- Display settings for suggested similar node title at node add page.
It can be above title or below title field.
- This module has template file to theme the suggested content as per
 requirement/design.

There is another module uniqueness 
which can also be used to avoid duplicate titles.
But following are points uniqueness is not offering but this module:
- Suggested content is being displayed using template file
which can be themed as per page design.
- Title comparison percentage can be specified.
- Ignore keywords can be specified.
- Maximum number of titles to display as suggestion can be specified 

== Admin setting page ==

After login as admin goto admin > config > system > Similar titles suggestion.

== Instrucitons ==
One should have permission "view similar titles suggestion" 
enabled to view similar title suggestions.
Drupal node permission settings can be found on setting page of this module 
which is after this master permission.

== Author ==
Tanzeel-ur-Rehman Ghumman (http://drupal.org/user/320511)
Email: tanzeelghuman@gmail.com
