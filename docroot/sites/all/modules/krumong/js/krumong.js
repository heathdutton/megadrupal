(function($){

  var krumong = window.krumong = {};

  function render(data, key, $container) {
    var type = (data === null) ? 'nullValue' : (typeof data);
    if (render[type]) {
      render[type](data, key, $container);
    }
    else {
      render.other(data, key, $container);
    }
  }

  render.object = function(data, key, $container) {

    var $ul = $('<ul>').addClass('krumo-node');
    if (typeof data.recursion == 'object') {
      // Recursion warning, show the trail of keys.
      for (var i = 0; i < data.recursion.length; ++i) {
        var $li = $('<li>').addClass('krumo-child').appendTo($ul);
        render(data.recursion[i], i, $li);
      }
    }
    else {
      for (var k in data) {
        // Keys have a '.' appended to avoid nameclashes with native keys.
        if (k[0] === '.') {
          var $li = $('<li>').addClass('krumo-child').appendTo($ul);
          render(data[k], k.substr(1), $li);
        }
      }
    }

    var n = $ul[0].childNodes.length;
    var $element, nString;
    if (data.class) {
      $element = renderElement('Object', null, key, data.class, $container);
    }
    else if (typeof data.recursion == 'object') {
      nString = (n == 0) ? '= root item' : ('= level ' + n + ' item');
      $element = renderElement('Recursion', nString, key, null, $container);
    }
    else {
      nString = (n == 1) ? '1 element' : (n + ' elements');
      $element = renderElement('Array', nString, key, null, $container);
    }

    if (n > 0) {
      // Not empty.
      $element.addClass('krumo-expand');
      var $nest = $('<div>').addClass('krumo-nest');
      $nest.appendTo($container);
      $nest.append($ul);
    }
  }

  render.nullValue = function(data, key, $container) {
    renderElement('NULL', null, key, null, $container);
  }

  render.number = function(data, key, $container) {
    renderElement('Number', null, key, data, $container);
  }

  render.string = function(data, key, $container) {
    var n = data.length;
    var nString = (n == 1) ? '1 character' :  (n + ' characters');
    renderElement('String', nString, key, data, $container);
  }

  render.boolean = function(data, key, $container) {
    renderElement('Boolean', null, key, data ? 'TRUE' : 'FALSE', $container);
  }

  render.other = function(data, key, $container) {
    renderElement('Other', null, key, data, $container);
  }

  function renderElement(typestring, n, key, strong, $container) {
    var $element = $('<div>').addClass('krumo-element').appendTo($container);
    $('<a>').addClass('krumo-name').html(key).appendTo($element);
    $element.append(' (');
    if (n !== null) {
      typestring += ', ';
    }
    var $em = $('<em>').addClass('krumo-type').html(typestring).appendTo($element);
    $('<strong>').addClass('krumo-string-length').html(n).appendTo($em);
    $element.append(') ');
    if (strong !== null) {
      $('<strong>').addClass('krumo-string').text(strong).appendTo($element);
    }
    return $element;
  }

  function animate($root) {
    $('.krumo-expand', $root).click(function(){
      var $li = $(this).parent();
      if ($li.is('.expanded')) {
        $li.removeClass('expanded');
      }
      else {
        $li.addClass('expanded');
      }
    });
  }

  krumong.krumong = function($root, data, called_from) {
    $root = $($root);
    var $ul = $('<ul>').addClass('krumo-node').addClass('krumo-first');
    $ul.appendTo($root);
    var $li = $('<li>').addClass('krumo-child').appendTo($ul);
    var $li_footnote = $('<li>').addClass('krumo-footnote').appendTo($ul);
    var $krumo_version = $('<div>').addClass('krumo-version').appendTo($li_footnote);
    $('<h6>').html('Krumo NG').appendTo($krumo_version);
    if (called_from) {
      var $krumo_call = $('<span>').addClass('krumo-call').html('Called from line ').appendTo($li_footnote);
      $('<code>').html(called_from.line).appendTo($krumo_call);
      $krumo_call.append(' of ');
      $('<code>').html(called_from.file).appendTo($krumo_call);
    }
    $li_footnote.append('&nbsp;');
    $root.addClass('krumo-root');
    render(data, null, $li);
    animate($root);
  }

})(jQuery);
