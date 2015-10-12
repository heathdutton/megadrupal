# Secure Data Cookie

## Introduction

**Secure Data Cookie** is a module that implements the
[Secure Cookie Protocol](http://www.cs.utexas.edu/~gouda/papers/conference/cookie.pdf). This
protocol garantuees that cookies cannot be tampered.

Tamper resistance is achieved through the use of an HMAC that
validates the data stored in the cookie each time the data is
retrieved. If the validation fails an empty object is returned.

## Design goals

The goals we aim for are two:

 1. Have a way to store data in a cookie so that this data cannot be
    tampered with.

 2. Have data confidentiality. Note that at the moment that's not
    yet implemented.

 3. Implement an expirable token so that cookies are regenerated
    periodically and the token contains the time to live so that
    replay attacks can be better diffused. 

## Remarks on the Implementation

The above cited paper prescribes SSL for the solution to be considered
really secure.

Otherwise you're exposed to
[MitM](https://en.wikipedia.org/wiki/Man-in-the-middle_attack)
attacks. Note also that there's not any protection against
[replay attacks](https://en.wikipedia.org/wiki/Replay_attack). The
only existing functionality to address that type of attack in a small
way is the cookie expiration time. When closing the browser the cookie
is destroyed.

Currently the use of the cookie generated by the module is advisable
only for non critical things. One example is the order id in
[Drupal Commerce](https://drupal.org/project/commerce), allowing you
to implement a cart that doesn't generate a session whenever you
create a cart.

Another example is storing data about some sort of dynamic page that
is unique to each client/user.

## Target Audience

This is a module aimed at **developers** or to site builders that
*dabble* in development to put it bluntly.

## Use Cases

Use cases for the module are whenever **security** and/or
**performance** are at play.

It's common for module developers to use session variables to store
some particular settings that are specific to a given user. This
practices raises two issues:

 1. As soon as you store anything in a session, i.e., *write* to
    `$_SESSION`, Drupal starts a session, sending a cookie with a
    session token. This invalidates the vast majority of caching
    strategies.
    

 2. When setting a session variable you hit the persistence
    layer. Meaning the database in most cases. Therefore hurting
    performance.

This module addresses these issues by maintaining whatever it is you
need to store between the client and the server. The overhead in
terms of HTTP payload size is negligable relative to the common Drupal
size page.

## How to Use

The module provides two functions to be used in your module to store
and retrieve data from the cookie.

### Setting/modifying a cookie

Invoke `secure_data_cookie_put(<data>)` where `<data>` is the data you
want to store in the cookie. By default the cookie is called
`SecureCookieData`. The name can be altered through the
`SecureCookieBasic` class `$__cookie_name` attribute.

Usually you have the data in a serialized format. JSON is particularly
useful. Here's a complete example:

    secure_cookie_data_put(json_encode(array('arg' => 'property_one')));

this creates a cookie (named `SecureDataCookie` by default) with the
given name containing a base64 encoding of the data:

    eyJhcmciOiJ0ZXN0WVVZZ2hnaFVtZTM0NTUiLCJobWFjIjoiZThhNmVmNmNmNjFiMTMxZGZmMDcxMzhiZDcyYTdmNTkwM2I1YzY5NiJ9

Now to recover the value you do:

    secure_cookie_data_get();

that prints the initial array.

## Cookie life time

By default the cookie is removed once you close the browser, i.e., the
life time is 0. To set it to a different value through the class
attribute `secureCookieBasic::$__cookie_duration`.

See the remarks above replay attacks on the consequences of messing
with cookie duration.

## HMAC key

There's a method `get_secret` that uses
[`drupal_get_hash_salt()`](https://api.drupal.org/api/drupal/includes%21bootstrap.inc/function/drupal_get_hash_salt/7)
to generate the key. This means that each Drupal has its **unique
key**. This avoids the cookie being vulnerable to client side
modification by recomputation of the HMAC.

This method can be overloaded by extending the class. So you can
implement your own HMAC key management scheme.

## Complex data structures in cookies: storing unbalacend binary trees 

Besides the basic flat data structure that you can use non flat data
structures like trees. There's a submodule called
`secure_cookie_data_tree_two`.

This module allows you to store a list of unbalanaced binary trees. It
was developed for a client project that needs to store a product
configuration that is specific to an user. Before it used a session
variable that stored that information. The result being that it
created a session voiding page caching.

Here's an example storing the configuration data:

      array('label_level0' => 'nid', 'nid' => 123,
            'label_level1' => 'support', 'value' = 'xxx')
 
 
                          nid => 123
                                 / \
                                /   \
                          support  format
                              /       \
                             /         \
                  (array) [xxx]    [big, small]

Let's got through it carefully:

 1. The tree has two levels.

 2. The first level has the value of `label_level1`.

 3. The second level has only values. The values can be whatever you
    want.

 4. All the trees are stored in a list. The list is implemented as an
    associative array with a key. The key is defined by `label_level0`.

The implementation of this data structure is done through the
extension of the `secureCookieBasic` class. It can be used as an
example for defining your own data structure to be stored in the cookie.

## Final Remarks

Note that the secret generation could be done automatically upon
install. Nevertheless that would void one of the design goals which is
to avoid hitting the database at all. Since setting the value in
install would envolve using a variable, hence the database.

Even if it's a little less convenient, the gains in terms of
simplicity and performance justify it largely.


## TODO

 * Implement cookie data confidentiality using crypto.

 * Implement a token that defines an expiration so that
   the cookies are regenerated periodically, to void
   replay attacks.

## Acknowledgements

The development and maintenance of this module is sponsored by
[Commerce Guys](http://commerceguys.com).
