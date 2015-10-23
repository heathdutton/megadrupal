Dummy Permissions
=================
This module allows you to create permissions that do absolutely nothing by
default. Useful for:

- Views Permission-Based access control
- As one of many conditions (using `user_access()`) in Views PHP access control
- In Panels custom PHP selection rules and Access Checks

Installation
------------
Nothing special.

Use
---
To create some Dummy Permissions, first log in as a user who has the
"administer dummy permissions" role. Then head to the People > Permissions >
Dummy Permissions > Add New Dummy Permission. The text boxes here are:

- **Name:** This is the name of your permission that shows up on the
permissions page.
- **Description:** This is the text that appears under the name on the
permissions page.
- **Machine name:** This is the what you pass as an argument to
`user_access()`. It should basically be the name with everything lowercased and
punctuation removed.
- **Trusted Users Only** Whether or not to show the "This permission has
security implications" message.
- **Trusted Users Only Message** The message to display instead of the normal
"this permission has security implications". To be used sparingly.
