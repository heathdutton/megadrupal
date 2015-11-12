Module: 'urlrouter'
kris@o3world.com
mar 20 '13

Maintain a list of source and destination URL's
for which redirects (301 or 302) will be performed.
Source URL is absolute, destination URL
may be a path, or another absolute URL.

Source-destination URL pairs are stored in the
variable 'urlrouter_array' as an associative array.
The actual redirect takes place in hook_boot(), so
that URL routing takes place as quickly as possible.
