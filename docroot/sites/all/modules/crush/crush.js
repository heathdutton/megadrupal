(function($) {

Drupal.crush = Drupal.crush || {};
Drupal.crush.commands = Drupal.crush.commands || {};
Drupal.crush.history = Drupal.crush.history || [];
Drupal.crush.promptQueue = Drupal.crush.promptQueue || [];

Drupal.behaviors.crush = {
  attach: function(context, settings) {
    var currentIndex = -1;
    Drupal.crush.history = [];
    $('#crush-form', context).submit(function() {
      if (Drupal.crush.promptQueue.length) {
        var fn = Drupal.crush.promptQueue.shift();
        fn($('#crush').val());
      }
      else {
        Drupal.crush.process($('#crush').val());
      }
      Drupal.crush.clear();
      currentIndex = -1;
      return false;
    }).keydown(function(e) {
      if (!e) {
        e = window.event;
      }
      var prevIndex = currentIndex;
      switch (e.keyCode) {
        case 38: // Up arrow.
          if (currentIndex < Drupal.crush.history.length - 1) {
            currentIndex++;
          }
          else {
            return false;
          }
          break;
        case 40: // Down arrow.
          if (currentIndex > -1) {
            currentIndex--;
          }
          else {
            return false;
          }
          break;
        default:
          return true;
      }
      Drupal.crush.history.reverse();
      Drupal.crush.history[prevIndex] = $('#crush').val();
      $('#crush').val(Drupal.crush.history[currentIndex]);
      Drupal.crush.history.reverse();
      return false;
    });
    $('#crush').focus();
  }
};

/**
 * Process a command.
 */
Drupal.crush.process = function(str) {
  parsed = Drupal.crush.parse(str);
  Drupal.crush.lineOut('>>> ' + str);
  if (str === '') {
    return;
  }
  Drupal.crush.history.push(str);
  if (Drupal.settings.crushCommands[parsed.command] === undefined) {
    Drupal.crush.lineOut(Drupal.t('!command: command not found', {'!command': parsed.command}));
  }
  else {
    if (typeof Drupal.crush.commands[parsed.command] == 'function') {
      Drupal.crush.commands[parsed.command](parsed);
    }
    else {
      Drupal.crush.processAjax(parsed);
    }
  }
};

Drupal.crush.processAjax = function(parsed, fn) {
  results = $.post(Drupal.settings.crushAjax, {'parsed_command': parsed, 'token': Drupal.settings.crushToken}, function(data, textStatus, jqXHR) {
    if (typeof data != 'string' && data.length) {
      for (index in data) {
        Drupal.crush.lineOut(data[index]);
      }
    }
    if (fn !== undefined) {
      fn(data);
    }
  });
}

/**
 * Clear the input box.
 */
Drupal.crush.clear = function() {
  $('#crush').val('');
};

/**
 * Add a line to the messages.
 */
Drupal.crush.lineOut = function(msg, id) {
  if (id == undefined) {
    id = $('.crush-line').length;
    line = $('<div class="crush-line" id="crush-line-' + id + '"></div>').appendTo('#crush-log');
    if ($('.crush-line:visible').length > 10) {
      $('.crush-line:visible:first').hide();
    }
  }
  else {
    line = $('#crush-line-' + id);
  }
  line.append(Drupal.checkPlain(msg));
  return id;
};

/**
 * Display a prompt.
 */
Drupal.crush.prompt = function(fn) {
  Drupal.crush.promptQueue.push(fn);
}

/**
 * Parses a string into a logical command object.
 */
Drupal.crush.parse = function(str) {
  nonquotes = str.split(/".*?[^\\]"/) || [];
  quotes = str.match(/".*?[^\\]"/g) || [];
  parts = [];
  parsed = {};
  for (index in nonquotes) {
    if (index !== undefined) {
      subparts = nonquotes[index].split(/ +/);
      for (subindex in subparts) {
        if (subindex !== undefined) {
          parts.push(subparts[subindex]);
        }
      }
      if (quotes[index] !== undefined) {
        parts.push(quotes[index]);
      }
    }
  }
  parsed.command = parts.shift();
  parsed.options = {};
  parsed.args = [];
  for (index in parts) {
    part = parts[index];
    if (part[0] == '-') {
      // Option.
      opts = part.split('');
      opts.shift();
      for (key in opts) {
        parsed.options[opts[key]] = true;
      }
    }
    else {
      // Arg.
      if (part[0] == '"') {
        part = part.substr(1, part.length - 2);
      }
      parsed.args.push(part);
    }
  }
  return parsed;
};

/**
 * Helper function: go to a page.
 */
Drupal.crush.go = function(destination, parsed) {
  fullUrl = Drupal.settings.basePath + '?q=' + destination;
  if (parsed.options['n'] || parsed.options['N']) {
    // Open in a new window.
    window.open(fullUrl, '_blank');
  }
  else {
    $('iframe').attr('src', fullUrl);
  }
}

})(jQuery);
