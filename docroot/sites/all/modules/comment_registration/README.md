
Comment Registration enables inline registration of new accounts while commenting.

## Features

- Comment registration for anonymous users
- Assign roles during comment registration
- Hide undeletable core comment fields
- [Profile2](http://drupal.org/project/profile2) support
- [Email Registration](http://drupal.org/project/email_registration) support

## Configuration

Comment Registration is enabled and configured per content type via the "Comment Registration" fieldset below "Comment settings".

Ensure that users can register accounts. In admin/config/people/accounts select "Visitors" or "Visitors, but administrator approval is required" under "Who can register accounts?".

## Issues

- **Currently incompatible with Comment Access**
- http://pareview.sh/pareview/httpgitdrupalorgprojectcommentregistrationgit

## TODO:

- User is able to register again with the same email, likely because email_registration creates unique username for the same email.
  "New user: email_registration_SFkk6NRbbs (maediprichard+tester2@gmail.com)."

DONE.

- <strike>Notice: Array to string conversion in drupal_write_record() (line 7170 of /Applications/MAMP/htdocs/includes/common.inc).
- <strike>Notice: Undefined offset: 0 in comment_submit() (line 2197 of /Applications/MAMP/htdocs/modules/comment/comment.module).
