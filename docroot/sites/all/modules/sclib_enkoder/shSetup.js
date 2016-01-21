(function($, Drupal, window, document, undefined) {
  function path() {
    var args = arguments,
      result = [];

    for (var i = 0; i < args.length; i++) {
      result.push(args[i].replace('@', 'http://alexgorbatchev.com/pub/sh/current/scripts/'));
    }

    return result;
  }

  $(document).ready(function() {
    if (typeof SyntaxHighlighter == "undefined") {
      console && console.log("No SyntaxHighlighter found");
    }
    else {
      SyntaxHighlighter.autoloader.apply(null, path(
        'js jscript javascript  @shBrushJScript.js',
        'php                    @shBrushPhp.js'
      ));
      SyntaxHighlighter.defaults['toolbar'] = false;
      SyntaxHighlighter.all();
    }
  });
})(jQuery, Drupal, window, document);
