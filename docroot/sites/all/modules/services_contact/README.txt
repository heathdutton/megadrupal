Enable the module as usual, then...

1. Go to: Admin -> Structure -> Services
2. Click "Edit Resources" next to your endpoint
3. Enable your desired resources under the "contact" service
4. Click "Save"
5. Flush all of Drupal's caches

Site Wide Contact Form
======================

To use the side wide contact form, for example, do a POST to this URL:

http://www.example.com/?q=rest/contact/site.json

The arguments expected are:

name
mail
subject
cid (the contact category id - optional)
message
copy (optional)

Personal Contact Form
=====================

To use the personal contact form, for example, do a POST to this URL:

http://www.example.com/?q=rest/contact/personal.json

The arguments expected are:

name
mail
to (the recipient Drupal user name)
subject
message
copy (optional)

Contact Categories
==================

To retrieve the contact categories, for example, do a GET on this URL:

http://www.example.com/?q=rest/contact.json

