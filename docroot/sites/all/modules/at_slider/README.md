AT Slider
===

This is a very simple module that provides slider blocks to your Drupal site.

Built on:

- AT Base — For config, helper functions, …
- Bean module — Block as Entity
- Field Collection — Each slide (bean entity) has image or more information, like
    text, links, … FieldCollection module allow Drupal admin add fields to entity
    field.
- Image — Provide image field for slider item.
- Features — Used for preconfig things.
- jQuery Cycle 2

### Install

This is just a simple module, we can install it as same as other modules.

After install we need download jquery cycle library too. This can be simply done
with Drush — run `drush atr` after enabled this module. If we do not have Drush
available, we can download http://malsup.github.io/min/jquery.cycle.all.min.js to
./sites/all/libraries/jquery.cycle/jquery.cycle.all.min.js

### Place the slider

A slider is a block, we can use block management UI to place the slider to place
we want.

For more flexibility, we can use [context.module](https://drupal.org/project/context)

### Customize

We can provide options for each slider bean rendered on page.

1. In my_module.info, add `dependencies[] = at_slider`
1. Create `/path/to/my_module/config/slider_options.yml
1. Define slider_options.yml like this:

```yaml
options:
    default:
        speed: 600
        manualSpeed: 100
        # If you want customize HTML structure of slider-item, you can define
        # this option
        item-template: '@crom_andy/templates/slider/item-image.html.twig'
```

jCycle options reference can be found at http://jquery.malsup.com/cycle2/api/#options

### Example for slider with pager:

```yaml
options:
    home-slider:
        pager: '#slider-home-pager'
        wrapper-after: '<div id="slider-home-pager"></div>'
```

### Credits

Started by Andy Truong for GO1.
