/**
 * Capture JavaScript errors, then post
 * to /jserror/ and record them
 */
(function (window) {
  var data = [],
  when = 0,
  serializer = {
      escapable: /[\\\"\x00-\x1f\x7f-\x9f\u00ad\u0600-\u0604\u070f\u17b4\u17b5\u200c-\u200f\u2028-\u202f\u2060-\u206f\ufeff\ufff0-\uffff]/g,
      // Table of character substitutions
      meta: {
        '\b': '\\b',
        '\t': '\\t',
        '\n': '\\n',
        '\f': '\\f',
        '\r': '\\r',
        '"': '\\"',
        '\\': '\\\\'
      },
      quote: function (string) {
        // If the string contains no control characters, no quote characters, and no
        // backslash characters, then we can safely slap some quotes around it.
        // Otherwise we must also replace the offending characters with safe escape
        // sequences.
        this.escapable.lastIndex = 0;
        return this.escapable.test(string) ? '"' + string.replace(this.escapable, function (a) {
          var c = this.meta[a];
          return typeof c === 'string' ? c : '\\u' + ('0000' + a.charCodeAt(0).toString(16)).slice(-4);
        }) + '"' : '"' + string + '"';
      },
      str: function (key, holder) {
        // Produce a string from holder[key].
        var i,
        k,
        v,
        length,
        partial,
        value = holder[key];
        // What happens next depends on the value's type.
        switch (typeof value) {
          case 'string':
            return this.quote(value);

          case 'number':
            // JSON numbers must be finite. Encode non-finite numbers as null.
            return isFinite(value) ? String(value) : 'null';

          case 'boolean':
          case 'null':
            // If the value is a boolean or null, convert it to a string. Note:
            // typeof null does not produce 'null'. The case is included here in
            // the remote chance that this gets fixed someday.
            return String(value);

            // If the type is 'object', we might be dealing with an object or an array or
            // null.
          case 'object':
            // Due to a specification blunder in ECMAScript, typeof null is 'object',
            // so watch out for that case.
            if (!value) {
              return 'null';
            }
            partial = [];
            // Is the value an array?
            if (Object.prototype.toString.apply(value) === '[object Array]') {
              // The value is an array. Stringify every element. Use null as a placeholder
              // for non-JSON values.
              length = value.length;
              for (i = 0; i < length; i += 1) {
                partial[i] = this.str(i, value) || 'null';
              }
              // Join all of the elements together, separated with commas, and wrap them in
              // brackets.
              v = partial.length === 0 ? '[]' : '[' + partial.join(',') + ']';
              return v;

            }
            // Otherwise, iterate through all of the keys in the object.
            for (k in value) {
              if (Object.prototype.hasOwnProperty.call(value, k)) {
                v = this.str(k, value);
                if (v) {
                  partial.push(this.quote(k) + ':' + v);
                }
              }
            }
        }
        // Join all of the member texts together, separated with commas,
        // and wrap them in braces.
        v = (!partial || partial.length === 0) ? '{}' : '{' + partial.join(',') + '}';
        return v;
      },
      stringify: function (value) {
        // The stringify method takes a value and returns a JSON text.
        // Make a fake root object containing our value under the key of ''.
        // Return the result of stringifying the value.
        return this.str('', {'': value});
      },
      unquote: function (value){
        return value.replace (/(^")|("$)/g, '');
      }

  },

  // Send data to the server.
  send = function (d) {
    var xmlhttp;
    if (window.XMLHttpRequest) {
      xmlhttp = new XMLHttpRequest();
    }
    else {
      xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    }
    // Act on the response from the server.
    xmlhttp.onreadystatechange = function () {
      if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
        var resp = ("undefined" !== typeof JSON && "function" === typeof JSON.parse) ? JSON.parse(xmlhttp.responseText) : serializer.unquote(xmlhttp.responseText);
        if(resp == "LIMIT_OVERFLOW") {
          // Set client limit to zero if server is not accepting further requests.
          Drupal.settings.jserrorLimit = 0;
        }
        if(resp == "SUCCESS") {
          if(0 < Drupal.settings.jserrorLimit) {
            Drupal.settings.jserrorLimit--;
          }
        }
      }
    }
    var payload = ("undefined" !== typeof JSON && "function" === typeof JSON.stringify) ? JSON.stringify(d) : serializer.stringify(d);
    xmlhttp.open("POST", Drupal.settings.basePath + 'jserror/', true);
    xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    xmlhttp.send("payload=" + payload);
    // Reset data
    data.length = 0;
  },
  // Check if data any error exists, prepare data format and store it in a variable.
  track = function () {
    var a = window.jserror.data;
    while (0 < a.length && a.shift) {
      var b = a.shift();
      var errObj = new Object();
      b[0] && (errObj.m = b[0]);
      b[1] && (errObj.f = b[1]);
      b[2] && (errObj.l = b[2]);
      b[3] && (errObj.c = b[3]);
      b[4] && (errObj.s = b[4].stack ? b[4].stack : b.stacktrace || b.stack);
      errObj.w = when;
      data.push(errObj);
    }
    if(!when) {
      when = 1;
    }
    if (0 < data.length && (0 < Drupal.settings.jserrorLimit || - 1 == Drupal.settings.jserrorLimit)) send({
      client: {
        cookie: navigator.cookieEnabled,
        lang: navigator.language || navigator.userLanguage,
        page: location.href,
        userAgent: navigator.userAgent
      },
      data: data
    });
  };
  track();
  // Call track function every second.
  window.setInterval(function () {
    track()
  }, 1E3);
})(window);
