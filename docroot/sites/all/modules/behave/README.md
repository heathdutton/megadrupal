# Drupal.behave ![build status](https://travis-ci.org/tableau-mkt/behave.svg?branch=7.x-1.x)

Super sexy Drupal JS behaviors.

## API in a nutshell

### Drupal.behave(...)

It's a super simple, jQuery like, chainable API.

### Before: The Conventional Drupal 7 Way

```
(function ($) {
  Drupal.behaviors.exampleModule = {
    attach: function (context, settings) {
      $('.myDOM', context).text('Who throws a shoe?!?');
    }
  };
})(jQuery);
```

### After: Yeah, Baby Way!

`jQuery` is also passed in as the third argument. Yeah, baby!

```
Drupal.behave('exampleModule').attach(function (context, settings, $) {
  $('.myDOM', context).text('Who throws a shoe?!?');
});
```

Even easier, you can use `.ready` without the context and settings arguments — the function context (`this`) will provide `context`, `settings`<sup>1</sup>, and `behavior`<sup>2</sup>. Easier to read and write.

```
Drupal.behave('exampleModule').ready(function ($) {
  $('.myDOM', this.context).text('Who throws a shoe?!?');
});
```
<sup>1</sup>The `settings` property on the function context refers to `Drupal.settings`.

<sup>2</sup>The `behavior` property on the function context refers to `Drupal.behaviors.exampleModule` in this case.

### Detach, if you want.

```
Drupal.behave('exampleModule')
  .attach(function (context, settings, $) {
    $('.myDOM', context).text('Who throws a shoe?!?');
  });
  .detach(function (context, settings, trigger, $) {
    $('.myDOM', context).text('Oh, behave!'); 
  })
```

### Extending the Drupal behavior

You can extend the Drupal behavior, e.g., with your own functions, like this:

```
Drupal.behave('exampleModule')
  .extend({
    myFunction: function myFunction() {

    }
  });
```

### Low-level access to the behave object

If you really want to, you can grab the behave object.

```
var behave = Drupal.behave('exampleModule').behave();
```
