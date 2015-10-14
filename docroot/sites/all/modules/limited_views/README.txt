## LIMITED VIEWS ##

This module allows you to limit your users to viewing only a certain number
of nodes within a certain time period. It has a configurable combination of
content types, maximum views and an expiry period.

## USE CASES ##
A common use-case for this module would be a 'Pay Wall' (http://en.wikipedia.org/wiki/Pay_wall)


## CONFIGURATION ##
1. Enable the module at admin/build/modules
2. Go to admin/user/permission#module-limited_views and grant permissions to the
   relevent roles.
3. Go to admin/settings/limited-views
4. Choose the content types you wish to restrict
5. Choose how many views users should have access to (threshold)
6. Choose the expiry time limit
7. Enter a message to present to user's when they exceed the threshold.
8. Click 'Save settings', and then go to admin/content/node-settings
9. Click 'Rebuild Permissions'

## CAVEATS / CONSIDERATIONS ##
- The count is global for all of the restricted content types. At the moment you
  can't limit views on a per-content-type basis.
- Viewing the same node multiple times does not increase the count. It does
  however reset the expiry time, keeping that view up to date.
- Once the limit has been exceeded, users can not view previously viewed nodes
  until the expiration time.
- When the user has no cookie support, content views are always permissable
- Google First Click Free support 
  http://support.google.com/webmasters/bin/answer.py?hl=en&answer=74536
  http://support.google.com/news/publisher/bin/answer.py?hl=en&answer=40543

  Limited_views now supports Google's policy on First Click Free (when enabled) by ;
  - allowing all GoogleBot user-agent to crawl the content (Overrides premium_content
    decisions when enabled)
  - a page load where the referrer is from Google (or other engines) is recorded but 
    the page view is always allowed, this also overrides any choice made if you have
    premium_content module enabled

- Under Varnished/other, limited_views automatically sets the NO_CACHE cookie
  when the limit is reached, the presence of this cookie means that your
  proxy should skip caching and pass the request onto Drupal so the site
  can do any further processing.
  
  

