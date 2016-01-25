# Getting started

## Description

This module offers a lightweight JavaScript librairy for frontend templating.


## Installation

Install as usual.


## Usage

Inlude the librairy in your module (e.g. in a hook_init) :
```php
drupal_add_library('microtemplating', 'microtemplating');
```

Define a JavaScript template in your hook_theme :
```php
/**
 * Implements hook_theme().
 */
function my_module_theme() {
    return array(
        'my_module_js_template' => array(
            'template'  => 'templates/my_module_js_template',
            'variables' => array(),
        ),
    );
}
```

Wrap your template with the built-in wrapper and render it on your page :
```php
$my_template = array(
    '#theme_wrappers' => array('js_template'),
    '#theme'          => 'my_module_js_template',
    '#id'             => 'js__my_module_js_template',
);
render($my_template);
```

Example of the template file templates/my_module_js_template.tpl.php :
```html
<div class="<%= htmlClass %>">
    <%= title %>
</div>
```

Example of JavaScript using this template :
```javasript
// Get template.
var template = tmpl('js__my_module_js_template');

// Process template with input data.
var content = template({
    htmlClass: 'hello-world',
    title: 'Hello world !'
});

// Append content to body.
jQuery('body').append(content);
```

More information about the usage of the JavaScript librairy can be found
[here][ejohn].

For a complete example please take a look at the microtemplating_example module.

After enabling the module go to the [Todo application page](/todos) for demo.

## Credits

This librairy was design by [John Resig][ejohn].

[ejohn]: http://ejohn.org/blog/javascript-micro-templating/
