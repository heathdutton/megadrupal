The Render As module adds the ability for elements to be rendered as a specific
user and/or role(s), which can be used for the purposes of testing how content
will be seen by another user or a user or a particular role without having to
change users.

Render As was written and is maintained by Stuart Clark (deciphered).
- http://stuar.tc/lark
- http://twitter.com/Decipher



How to use
--------------------------------------------------------------------------------

The Render As API module is intended for developers, and can be used in two
different ways:

1. #renderas parameter.

  Any element being passed through the render() or drupal_render() function that
  has a registered #type (markup, page, value, etc) will trigger the Render As
  behaviour if the #renderas parameter is present and keyed appropriated (see
  below).

  If the element is not a registered #type or has not #type you can still use
  the Render As beahviour by attaching the Render As #pre_render and
  #post_render callbacks along with the #renderas parameter:


    $element['#pre_render'][] = 'renderas_api_element_pre_render';
    $element['#post_render'][] = 'renderas_api_element_post_render';



2. renderas()/drupal_renderas() function.

  Render As also comes with some wrapper functions for the standard render() and
  drupal_render() functions, being renderas() and drupal_renderas(). These
  functions take an additional argument, an array which contains the Render As
  parameter settings to be bound to the supplied element, keyed as below.


Render As parameter format:

  The Render As parameter expects an array with one or both of the following
  keys:

    - uid: The user ID of the user to be used to render the element.

    - roles: A keyed array of roles to be used to render the element, where the
             key is the role ID.


  Example:

    The following will render the supplied element with user 0 (anonymous user)
    with the 'authenticated user' role applied to it.


    array(
      'uid' => 0,
      'roles' => array(
        2 => 'authenticated user',
      ),
    );
