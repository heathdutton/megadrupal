Front Themer helps you to map your Drupal theme implementations to very simple
Javascript implementations. This hopefully helps you to offload some of the
theming cost to the user's browser by allowing you to maintain only one set
of theme implementations (namely, your own Drupal theme). With Front Themer,
it's a bit easier and more organized to implement architectures where your
site has AJAX backends that serve e.g. JSON data instead of HTML.

Front Themer is NOT a templating engine. It is NOT capable of any kind of logic
or looping or anything complex, for that matter. Its sole purpose is to help
you to avoid end up writing HTML or duplicating theme implementations in JS.
It's meant to be used with very simple theme bits (such as boxes and list
items). You can work around some of its limitations by mapping one Drupal
theme implementation into several JS implementations (there's an example
available on this). If you are in need of a JS templating engine for Drupal,
please refer to e.g. jtemplate <http://drupal.org/project/jtemplate>.

Front Themer defines a new hook, hook_front_themer_theme(), which you can use
to map Drupal themes into JS implementations. The hook is well-documented in
the module code.

The module comes with a submodule called front_themer_example, which provides
a straightforward hands-on example on how to use the module, both in Drupal and
in your Javascript code.
