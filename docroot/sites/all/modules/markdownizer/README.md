[Markdown](https://en.wikipedia.org/wiki/Markdown) syntax parser.

## Usage

All that you need - is call the `markdownizer()` function and use `text()`
method with string in markdown syntax as an argument.

```php
var_dump(
  // <h1>Headline 1</h1>
  markdownizer()->text('# Headline 1')
);
```

## Dependencies

- [Libraries API](https://www.drupal.org/project/libraries)
- [Parsedown](https://github.com/BR0kEN-/parsedown)
