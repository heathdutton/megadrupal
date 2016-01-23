AT Config
=======

[![Build Status](https://secure.travis-ci.org/andytruong/at_route.png?branch=7.x-1.x)](http://travis-ci.org/andytruong/at_route)

Faster way to define menu item in Drupal 7:

```yaml
# %atest_route/config/route.yml
routes:
  atest_route/drupal:
    title: Hello Drupal
    page callback: atest_route_page_callback
    page arguments: ['Andy Truong']
    access arguments: ['access content']
  atest_route/controller:
    title: Hello
    access arguments: ['access content']
    controller: [\Drupal\atest_route\Controller\HelloController, helloAction, {name: 'Andy Truong'}]
  atest_route/string_template:
    title: String Template
    access arguments: ['access content']
    page callback: at_theming_render_string_template
    page arguments: ['Hello {{ name }}', {name: Andy Truong}]
```

### Other ideas to be implemented in spare time

````
GET|POST|DELELETE /at-lazy/module_x/contact-us.[json]

  /**
   * @cache($bin = 'cache_at_route', ttl = '+ 30 minutes')
   * @permission('access content')
   * @role('role_x AND role_y OR !role_z')
   * @termimate('self::postTerminate', '\Other\Controller::doTerminate')
   */
  \Drupal\module_x\Controller\ContactUs::get|post|delete()
````

- Return JSON format if user append .json
- Support annotations: @cache, @permission, @roles, @terminate
- Think more about arguments converting
