# Views Random

Enables you to show cached views results randomly.

## How it works

It makes the views to render all items, and on the client side via JavaScript mixes them.

## Requirements

- View and row templates should have $classes printed.
- All items should be printed one by one, without columns.

## Supported styles

The following styles have been tested and appear to work correctly:

- Unformatted
- Table
- HTML List

Other styles may work; if they do not & you need it, please create an issue; support for it might be added in.

## Usage

1. Create a view
2. Select 'Display randomly' pager
3. Specify number of items to display

## Execute custom callback for showed items

In case you need to execute some custom JS code for showed items, there is callback system.

Create the callback like following:


    (function ($) {
      Drupal.viewsRandom = Drupal.viewsRandom || {};
      Drupal.viewsRandom.callbacks = Drupal.viewsRandom.callbacks || {};

      Drupal.viewsRandom.callbacks.customCallback = function(item) {
        // Do something.
      };
    })(jQuery);

Then specify the callback into JS settings:


     drupal_add_js(array('views_random' => array(
        $view_name => array(
          $display_id => array(
            'callbacks' => array('customCallback'),
          ),
        )
      )), 'setting');


## Credits

Module maintained by:

- [Dmitry Demenchuk](https://www.drupal.org/u/mrded)

[Full list of contributors](https://www.drupal.org/node/1983206/committers)
