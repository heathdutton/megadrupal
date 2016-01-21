-- SUMMARY --

This module introduces a Context condition that allows you to trigger a context
based on query parameters on a given request.

This module requires the Context module.


-- FEATURES --

* Trigger contexts based on query parameters, irrespective of parameter order.
* Add as many parameter rulesets to a context as you want
* Each parameter ruleset can use one of a number of operators.
* You can use "AND" logic to require all rulesets, or "OR" logic to have the
  context triggered when any one ruleset matches.


-- INSTALLATION --

* Ensure that Context module is already enabled.
* Install and enable this module.
* You may need to clear your site's cache for the condition to appear.
* In order to successfully configure multiple rulesets, JavaScript must be
  enabled in your browser.


-- USAGE --

A query parameter is an individual item found in an HTTP query string. For
example, consider the following URL:

http://example.com?foo=bar&baz

The query string for this URL is "foo=bar&baz". You could say that there are two
query parameters "foo" and "baz". In this case, the "foo" parameter has a value
of "bar" and the baz parameter has no value.

This module allows you to add any number of parameter "rulesets." The simplest
example would be to set a Context condition to fire when foo=bar.

  ------------------------------------------------------
  | Parameter name | Operator        | Parameter value |
  ------------------------------------------------------
  | foo            | must equal      | bar             |
  ------------------------------------------------------
  [X] Require all rulesets

In other cirumstances, you may want your Context to be triggered when a query
parameter is set, but you don't care what value it has. In that case, configure:

  ------------------------------------------------------
  | Parameter name | Operator        | Parameter value |
  ------------------------------------------------------
  | baz            | must be set     |                 |
  ------------------------------------------------------
  [X] Require all rulesets

In another cirumstance, you may want your Context to be triggered when a query
parameter is not set. In that case, configure:

  ------------------------------------------------------
  | Parameter name | Operator        | Parameter value |
  ------------------------------------------------------
  | baz            | must not be set |                 |
  ------------------------------------------------------
  [X] Require all rulesets

Note in the above two examples, the "parameter value" field will be disabled
because there is no need to collect that information to evaluate the operation.

You can also click the "add parameter" button to add as many of these rulesets
as you want. For instance, to match the above example URL exactly, configure:

  ------------------------------------------------------
  | Parameter name | Operator        | Parameter value |
  ------------------------------------------------------
  | foo            | must equal      | bar             |
  | baz            | must be set     |                 |
  ------------------------------------------------------
  [X] Require all rulesets

The order in which you create the rulesets does not matter.

Also note that you can uncheck the "require all rulesets" checkbox below the
table of rulesets to use "OR" logic (rather than the default "AND" logic) when
evaluating the condition.

  ------------------------------------------------------
  | Parameter name | Operator        | Parameter value |
  ------------------------------------------------------
  | foo            | must equal      | bar             |
  | baz            | must be set     |                 |
  ------------------------------------------------------
  [ ] Require all rulesets

In this case, the Context would be triggered at all four of the following URLs:
1. http://example.com?foo=bar
2. http://example.com?baz
3. http://example.com?foo=bar&baz
4. http://example.com?baz&foo=bar
