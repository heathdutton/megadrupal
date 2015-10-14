Cacheable CSRF protection
=========================

The Cacheable CSRF protection module allows forms to be cached in HTML for
authenticated users, for example in reverse proxies or CDNs.

Drupal's built in CSRF protection for forms embeds a user/session-specific
token in the HTML returned from form rendering. The result is that a form
generated for one user cannot be submitted by another, since their CSRF token
will differ. However in many cases the only difference between the forms will
be the CSRF token. For example the core search form does not store any
information nor have any user-specific settings by default.

Cacheable CSRF disables core's CSRF handling and adds its own, in a way which
is compatible with reverse proxies and CDNs.

The form token is saved to a cookie.

When the cookie is missing, it is updated via an AJAX request that sets the
cookie to the correct value.

The cookie uses drupal_get_token() to ensure it's unique.

When forms are rendered, JavaScript adds the token to a hidden field, the value
of the token is then submitted along with the form.

A validation handler then checks the user's cookie, the form token, and
drupal_get_token() to ensure all three match.

This mechanism relies on the same origin policy to prevent the value of the
token cookie from being leaked. It is vulnerable where the user is already the
victim of an attack such as XSS or sidejacking, but in both cases CSRF
protection will be useless anyway.

To use the module, enable it then implement hook_cacheable_csrf_form_ids()
to specify forms to swap the CSRF protection for.

The module does not cache the form HTML, it's up to you to do this via
render caching or another mechanism. In all cases whether the user is anonymous
or not should be included in cache IDs, since anonymous users do not need CSRF
protection the JavaScript is not added for those users, nor the cookie set.
