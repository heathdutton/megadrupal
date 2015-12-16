/**
 * @file
 * The textcomplete js.
 *
 * Js to instantiate the textcomplete functionality on a textarea
 */

(function($) {
  var htmlElementAttributes = {
    a: {href: ''},
    iframe: {src: '', width: '', height: ''},
    img: {src: ''}
  };

  var textcompleteInit = function(o, settings) {
    var strategies = [];
    if (settings.emoji_status) {
      var matchRegex = new RegExp("(^|\\s)" + settings.emoji.keycode + "(\\w*)$");
      strategies.push({
        // Emoji strategy
        match: matchRegex,
        search: function(term, callback) {
          var regexp = new RegExp('^' + term);
          callback($.grep(emojies, function(emoji) {
            return regexp.test(emoji);
          }));
        },
        template: function(data) {
          return '<img src="' + Drupal.settings.textcomplete.library_path + '/media/images/emoji/' + data + '.png"></img>' + data;
        },
        replace: function(data) {
          return '$1[textcomplete:emoji:' + data + '] ';
        }
      });
    }

    if (settings.html_status) {
      var htmlElements = settings.html.tags.split(" ");
      strategies.push({
        // HTML strategy
        match: /<(\w*)$/,
        search: function(term, callback) {
          callback($.map(htmlElements, function(element) {
            return element.indexOf(term) === 0 ? element : null;
          }));
        },
        index: 1,
        replace: function(element) {
          var attributes = [];
          if (typeof htmlElementAttributes[element] != 'undefined') {
            for (var key in htmlElementAttributes[element]) {
              attributes.push(key + '="' + htmlElementAttributes[element][key] + '"');
            }
          }
          var htmlOpenTag = '<' + element;
          htmlOpenTag += attributes.length ? ' ' + attributes.join(' ') + '>' : '>';
          return [htmlOpenTag, '</' + element + '>'];
        }
      });
    }

    if (settings.entityreference_status) {
      var pattern = "",
              matchRegex;
      if (settings.entityreference.keycode) {
        pattern = "(^|\\s)" + settings.entityreference.keycode + "(\\w*)$";
      }
      else {
        pattern = "(\\b)(\\w{2,})$";
      }
      matchRegex = new RegExp(pattern);
      strategies.push({
        // Entityreference strategy
        match: matchRegex,
        search: function(term, callback) {
          term = term.replace(/\_/g, " ");
          $.getJSON(settings.autocomplete_path + encodeURIComponent(term), function(data) {
            var result = [];
            /*
             * Turn the json data back into an array, as this is expected by
             * the textcomplete plugin.
             */
            for (var i in data) {
              result.push([i, data[i]]);
            }
            callback(result);
          }).fail(function() {
            callback([]);
          });
        },
        template: function(data) {
          return data[1];
        },
        replace: function(data) {
          var key = data[0],
                  id = key.match(/\(([0-9]+)\)$/)[1],
                  output = '$1<a href="[aet:' + settings.entityreference.settings.target_type + ':' + id + ':url:unaliased]">';
          /*
           * If the settings use a view then we need to keep the title returned
           * from that view, rather than replacing it with the original name.
           */
          if (settings.entityreference.settings.handler === 'views') {
            output += $(data[1]).text().trim();
          }
          else if (settings.entityreference.settings.target_type === 'node') {
            output += '[aet:' + settings.entityreference.settings.target_type + ':' + id + ':title]';
          }
          else if (settings.entityreference.settings.target_type === 'comment') {
            output += '[aet:' + settings.entityreference.settings.target_type + ':' + id + ':subject]';
          }
          else {
            output += '[aet:' + settings.entityreference.settings.target_type + ':' + id + ':name]';
          }

          return output += '</a> ';
        },
        cache: true
      });
    }

    o.textcomplete(strategies);

  };

  Drupal.behaviors.textcomplete = {
    attach: function(context, settings) {
      for (var key in Drupal.settings.textcomplete.fields) {
        var field = Drupal.settings.textcomplete.fields[key];
        $('textarea[name^="' + key + '["]', context).once('textcomplete', function() {
          textcompleteInit($(this), field);
        });
      }
    }
  };

})(jQuery);
